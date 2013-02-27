using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using sharp_arduino_serial_packet_lib;

namespace seriallibtester
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {

                SerialReaderWriter r = new SerialReaderWriter(comport:"COM4");
                r.SerialMessageReceived += r_SerialDataReceived;
                r.SerialMessageReceivedAsString += r_SerialMessageReceivedAsString;
                r.StartListening();
                while (true)
                {

                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }


        }

        static void r_SerialMessageReceivedAsString(object sender, string e)
        {
            Console.WriteLine(e);
        }

        static void r_SerialDataReceived(object sender, SerialArduinoMessageEventArgs e)
        {

            Console.WriteLine(e.Packet.ToString());
        }
    }
}
