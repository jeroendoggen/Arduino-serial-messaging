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

        

        #region Constructors
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

        private readonly SerialPortManager spManager = new SerialPortManager();
        #endregion

        private int ComputeParity()
        {
            //TODO: compute parity (ask Jeroen)
            return incomingPacket.Parity;
        }

        #region Receiving methods

        public void StartListening()
        {
            if (spManager != null)
            {
                spManager.NewSerialDataReceived += OnNewSerialDataReceived;
                spManager.StartUsingPort();
            }

        }
        public void StopListening()
        {
            if (spManager != null)
            {
                spManager.NewSerialDataReceived -= OnNewSerialDataReceived;
                spManager.StopUsingPort();
            }
        }

        private readonly Statistics _statistics;
        void OnNewSerialDataReceived(object sender, SerialDataEventArgs e)
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
        public event EventHandler<SerialArduinoMessageEventArgs> SerialMessageReceived;

       

        public Packet incomingPacket = new Packet();
        private PacketFields currentField;
        public void ParseData(string packetStr)
        {

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
        #endregion

        #region Send methods

        public void SendPacket(PacketTypes ptype, int nodeId, int sensorID, Commands command, int payload)
        {
            Packet res= new Packet();
            res.PacketType = ptype;
            res.NodeID = nodeId;
            res.SensorID = sensorID;
            res.CommandID = command;
            res.Payload = payload;
            res.Parity = ComputeParity(); //TODO

            spManager.SendSerialData(res.ToStringMessageArray());
            
        }
        #endregion

        #region Dispose
        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                spManager.StopUsingPort();
                spManager.NewSerialDataReceived -= OnNewSerialDataReceived;
                spManager.Dispose();
            }

        }
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
        #endregion


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
