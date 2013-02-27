using System;
using System.Collections.Generic;
using System.Diagnostics;
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
        public event EventHandler<string> SerialMessageReceivedAsString;
        void spManager_NewSerialDataRecieved(object sender, SerialDataEventArgs e)
        {
            //Parse message 
            if (SerialMessageReceived != null)
            {
                Packet packet = ParsePacket(e.Data);
                SerialMessageReceived(this, new SerialArduinoMessageEventArgs(packet));

            }
            if (SerialMessageReceivedAsString != null)
            {
                //Just send as as string
                SerialMessageReceivedAsString(this, Encoding.UTF8.GetString(e.Data));
            }
        }

        private Packet ParsePacket(byte[] p)
        {
           Packet result= new Packet();
            string packasStr = Encoding.UTF8.GetString(p);
            result.PacketType = GetPacketType(packasStr[1]);
            //http://stackoverflow.com/questions/623104/byte-to-hex-string
            return result;
        }

        private Defines GetPacketType(char p)
        {
            int ptype = (int) p;
            switch (ptype)
            {
                case Defines.COMMAND: 

            }

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
}
