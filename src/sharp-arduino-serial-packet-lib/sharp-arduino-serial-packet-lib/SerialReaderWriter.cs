using System;
using System.Diagnostics;

namespace sharp_arduino_serial_packet_lib
{
    public class SerialReaderWriter : IDisposable
    {
        //statistics
        public int ReceivedPackets { get; set; }
        public int CorruptPackets { get; set; }

        private readonly SerialPortManager spManager;
        public SerialReaderWriter(int baudrate = 115200, string comport = "COM2")
        {
            spManager = new SerialPortManager();
            var settings = spManager.CurrentSerialSettings;
            settings.BaudRate = baudrate;
            settings.PortName = comport;
            spManager.NewSerialDataReceived += spManager_NewSerialDataReceived;
        }

        public event EventHandler<SerialArduinoMessageEventArgs> SerialMessageReceived;

        void spManager_NewSerialDataReceived(object sender, SerialDataEventArgs e)
        {
            ReceivedPackets++;

            try
            {
                ParseData(e.Data);
            }
            catch (Exception)
            {
                Debug.WriteLine("Corrupt packet: dropped + (" + (e.Data) + ")");
                CorruptPackets++;
            }

        }

        private Packet incomingPacket = new Packet();
        private PacketFields currentField;


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
                spManager.StartListening();
        }

        public void StopListening()
        {
            if (spManager != null)
                spManager.StopListening();
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


    static class ExtensionsMethods
    {

        public static int FromHexStringToInt(this  string hexstr)
        {
            if (hexstr.Length == 2)
            {
                return Convert.ToInt32(hexstr, 16);
            }
            throw new IndexOutOfRangeException("Can only create int from 2-length hex arrays. String received: " + hexstr);
        }

        public static T[] SubArray<T>(this T[] data, int index, int length)
        {
            var result = new T[length];
            Array.Copy(data, index, result, 0, length);
            return result;
        }
    }
}
