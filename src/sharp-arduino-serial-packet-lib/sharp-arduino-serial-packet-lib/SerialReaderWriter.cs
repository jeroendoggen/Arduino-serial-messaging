using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sharp_arduino_serial_packet_lib
{
    public class SerialReaderWriter
    {
        private SerialPortManager spManager;
        public SerialReaderWriter(int baudrate = 115200, string comport = "COM2")
        {
            spManager = new SerialPortManager();
            var settings = spManager.CurrentSerialSettings;
            settings.BaudRate = baudrate;
            settings.PortName = comport;
            spManager.NewSerialDataRecieved += spManager_NewSerialDataReceived;
        }

        public event EventHandler<SerialArduinoMessageEventArgs> SerialMessageReceived;
        public event EventHandler<string> RawDataAsStringReceived;
        void spManager_NewSerialDataReceived(object sender, SerialDataEventArgs e)
        {
            if (RawDataAsStringReceived != null)
            {
                //Just send as as string
                RawDataAsStringReceived(this, Encoding.UTF8.GetString(e.Data));
            }

            try
            {
                ParseData(e.Data);
            }
            catch (Exception ex)
            {

                Debug.WriteLine("Corrupt packet: dropped + (" + Encoding.UTF8.GetString(e.Data) + ")");
            }

        }

        private Packet incomingPacket = new Packet();
        private PacketFields currentField = PacketFields.Unknown;


        private void ParseData(byte[] p)
        {


            var packetStr = Encoding.ASCII.GetString(p);//.Replace(Environment.NewLine, null);

            Debug.WriteLine("New packet:\t string:" + packetStr);

            for (int i = 0; i < packetStr.Length; i++)
            {
                //Simple state-machine
                if (packetStr[i] == 'T')
                {
                    incomingPacket = new Packet() {RawString = packetStr};
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
                    else
                        currentField = PacketFields.Unknown;

                }
                else if (packetStr[i] == 'P')
                    currentField = PacketFields.Payload;
                else if (packetStr[i] == 'Q')
                    currentField = PacketFields.Parity;
                else
                {
                    switch (currentField)
                    {
                        case PacketFields.Unknown:
                            break;
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
                            i++;
                            currentField = PacketFields.Unknown;
                            if (SerialMessageReceived != null && ComputeParity()== incomingPacket.Parity) //&& parity klopt
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
            T[] result = new T[length];
            Array.Copy(data, index, result, 0, length);
            return result;
        }
    }
}
