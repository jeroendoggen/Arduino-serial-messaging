using System;
using System.Diagnostics;
using sharp_arduino_serial_packet_lib.SerialPortReaderWriter;

namespace sharp_arduino_serial_packet_lib
{
    public class SerialReaderWriter : IDisposable
    {

        public SerialSettings SerialSettings
        {
            get { return spManager.CurrentSerialSettings; }
            set { spManager.CurrentSerialSettings = value; }
        }

        public Statistics Statistics
        {
            get { return _statistics; }
        }


        private readonly SerialPortManager spManager = new SerialPortManager();

        public SerialReaderWriter(int baudrate = 115200, string comport = "COM2")
        {
            _statistics = new Statistics();
            spManager.CurrentSerialSettings.BaudRate = baudrate;
            spManager.CurrentSerialSettings.PortName = comport;
        }

        public SerialReaderWriter(SerialSettings settings)
        {
            _statistics = new Statistics();
            spManager.CurrentSerialSettings = settings;
        }


        public event EventHandler<SerialArduinoMessageEventArgs> SerialMessageReceived;

        void spManager_NewSerialDataReceived(object sender, SerialDataEventArgs e)
        {
            Statistics.ReceivedPackets++;

            try
            {
                ParseData(e.Data);
            }
            catch (Exception)
            {
                Debug.WriteLine("Corrupt packet: dropped + (" + (e.Data) + ")");
                Statistics.CorruptPackets++;
            }

        }

        private Packet incomingPacket = new Packet();
        private PacketFields currentField;
        private readonly Statistics _statistics;


        private void ParseData(string packetStr)
        {


            // var packetStr = Encoding.ASCII.GetString(p);//.Replace(Environment.NewLine, null);

            Debug.WriteLine("New packet:\t string:" + packetStr);

            for (int i = 0; i < packetStr.Length; i++)
            {
                //Simple state-machine
                if (packetStr[i] == 'T')
                {
                    incomingPacket = new Packet { RawString = packetStr };
                    currentField = PacketFields.Type;
                }

                else if (packetStr[i] == 'N')
                    currentField = PacketFields.NodeID;
                else if (packetStr[i] == 'I')
                {
                    if (incomingPacket.PacketType == PacketTypes.Command || incomingPacket.PacketType == PacketTypes.Command_Reply)
                        currentField = PacketFields.CommandID;
                    else if (incomingPacket.PacketType == PacketTypes.Data_Array_Request || incomingPacket.PacketType == PacketTypes.Data_Int || incomingPacket.PacketType == PacketTypes.Data_Request)
                    {
                        currentField = PacketFields.SensorID;
                    }
                }
                else if (packetStr[i] == 'P')
                    currentField = PacketFields.Payload;
                else if (packetStr[i] == 'Q')
                    currentField = PacketFields.Parity;
                else
                {
                    switch (currentField)
                    {
                        case PacketFields.Type:
                            incomingPacket.PacketType = (PacketTypes)packetStr.Substring(i, 2).FromHexStringToInt();
                            i++;
                            break;
                        case PacketFields.NodeID:
                            incomingPacket.NodeID = packetStr.Substring(i, 2).FromHexStringToInt();
                            i++;
                            break;
                        case PacketFields.SensorID:
                            incomingPacket.SensorID = packetStr.Substring(i, 2).FromHexStringToInt();
                            i++;
                            break;
                        case PacketFields.CommandID:
                            incomingPacket.CommandID = (Commands)packetStr.Substring(i, 2).FromHexStringToInt();
                            i++;
                            break;
                        case PacketFields.Payload:
                            incomingPacket.Payload = packetStr.Substring(i, 2).FromHexStringToInt();
                            i++;
                            break;
                        case PacketFields.Parity:
                            incomingPacket.Parity = packetStr.Substring(i, 2).FromHexStringToInt();
                            i = packetStr.Length; //we're done with this packet

                            if (SerialMessageReceived != null && ComputeParity() == incomingPacket.Parity) //&& parity klopt
                                SerialMessageReceived(this, new SerialArduinoMessageEventArgs(incomingPacket));
                            else
                            {
                                Debug.WriteLine("Parity failed");
                            }
                            break;
                        default:
                            throw new ArgumentOutOfRangeException();
                    }
                }

            }

        }

        private int ComputeParity()
        {
            //TODO: compute parity (ask Jeroen)
            return incomingPacket.Parity;
        }


        public void StartListening()
        {
            if (spManager != null)
            {
                spManager.NewSerialDataReceived += spManager_NewSerialDataReceived;
                spManager.StartListening();
            }
           
        }
    
        public void StopListening()
        {
            if (spManager != null)
            {
                spManager.NewSerialDataReceived -= spManager_NewSerialDataReceived;
                spManager.StopListening();
            }
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                spManager.StopListening();
                spManager.NewSerialDataReceived -= spManager_NewSerialDataReceived;
                spManager.Dispose();
            }

        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }


    }
    public class SerialArduinoMessageEventArgs : EventArgs
    {
        public Packet Packet { get; set; }

        public SerialArduinoMessageEventArgs(Packet pckt)
        {
            Packet = pckt;
        }
    }
}
