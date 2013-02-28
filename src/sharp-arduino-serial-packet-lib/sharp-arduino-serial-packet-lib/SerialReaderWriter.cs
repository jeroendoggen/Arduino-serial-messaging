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
            spManager.NewSerialDataRecieved += spManager_NewSerialDataRecieved;
        }

        public event EventHandler<SerialArduinoMessageEventArgs> SerialMessageReceived;
        public event EventHandler<string> RawDataAsStringReceived;
        void spManager_NewSerialDataRecieved(object sender, SerialDataEventArgs e)
        {

            ParseData(e.Data);

            if (RawDataAsStringReceived != null)
            {
                //Just send as as string
                RawDataAsStringReceived(this, Encoding.UTF8.GetString(e.Data));
            }
        }

        private Packet incomingPacket = new Packet();
        private PacketFields currentField = PacketFields.Unknown;


        private void ParseData(byte[] p)
        {


            var packetStr = Encoding.UTF8.GetString(p).Replace(Environment.NewLine, null);
            Debug.WriteLine("New packet:" + packetStr);
            var packetByteArray = Encoding.ASCII.GetBytes(packetStr);
            for (int i = 0; i < packetStr.Length; i++)
            {
                //Simple state-machine
                if (packetStr[i] == 'T')
                {
                    incomingPacket = new Packet();
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
                            //if (packetStr[i] == '0' && packetStr[i + 1] == '1')
                            //    incomingPacket.PacketType = PacketTypes.Command;
                            //else if (packetStr[i] == '0' && packetStr[i + 1] == '2')
                            //    incomingPacket.PacketType = PacketTypes.Command_Reply;
                            //else if (packetStr[i] == '1' && packetStr[i + 1] == '1')
                            //    incomingPacket.PacketType = PacketTypes.Data_Request;
                            //else if (packetStr[i] == '1' && packetStr[i + 1] == '2')
                            //    incomingPacket.PacketType = PacketTypes.Data_Byte;
                            //else if (packetStr[i] == '1' && packetStr[i + 1] == '3')
                            //    incomingPacket.PacketType = PacketTypes.Data_Int;
                            //else if (packetStr[i] == '2' && packetStr[i + 1] == '1')
                            //    incomingPacket.PacketType = PacketTypes.Data_Array_Request;
                            //else if (packetStr[i] == '2' && packetStr[i + 1] == '2')
                            //    incomingPacket.PacketType = PacketTypes.Data_Array;
                            //else
                            //    incomingPacket.PacketType = PacketTypes.Unknown;
                            var psub = packetStr.Substring(i, 2);
                            incomingPacket.PacketType=(PacketTypes)psub.GetIntegerFromBinaryString(2);
                            i++;
                            break;
                        case PacketFields.NodeID:
                            var sub = packetStr.Substring(i, 2);
                            incomingPacket.NodeID = sub.GetIntegerFromBinaryString(2);
                            i++;
                            break;
                        case PacketFields.SensorID:
                            var subsens = packetStr.Substring(i, 2);
                            incomingPacket.NodeID = subsens.GetIntegerFromBinaryString(2);
                            i++;
                            break;
                        case PacketFields.CommandID:
                            var subcom = packetByteArray.SubArray(i, 2);
                            incomingPacket.CommandID = BitConverter.ToInt16(subcom, 0);
                            i++;
                            break;
                        case PacketFields.Payload:
                            break;
                        case PacketFields.Parity:
                            if(SerialMessageReceived !=null)
                                SerialMessageReceived(this, new SerialArduinoMessageEventArgs(incomingPacket));
                            break;
                        default:
                            throw new ArgumentOutOfRangeException();
                    }
                }

            }




            //http://stackoverflow.com/questions/623104/byte-to-hex-string
            
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

    static class ExtensionArray
    {
        public static T[] SubArray<T>(this T[] data, int index, int length)
        {
            T[] result = new T[length];
            Array.Copy(data, index, result, 0, length);
            return result;
        }
    }

    static class ExtensionBitConvertor
    {
        public static int GetIntegerFromBinaryString(this string binary, int bitCount)
        {
            if (binary.Length == bitCount && binary[0] == '1')
                return Convert.ToInt32(binary.PadLeft(32, '1'), 2);
            else
                return Convert.ToInt32(binary, 2);
        }
    }
}
