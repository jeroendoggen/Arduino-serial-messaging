using System;
using sharp_arduino_serial_packet_lib;

namespace seriallibtester
{
    class Program
    {
        private static SerialReaderWriter r;
        static void Main(string[] args)
        {
            try
            {
                r = new SerialReaderWriter(comport: "COM4");
                r.SerialMessageReceived += r_SerialDataReceived;
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

        static void r_SerialDataReceived(object sender, SerialArduinoMessageEventArgs e)
        {

            Console.WriteLine(e.Packet.ToString());
            Console.WriteLine("Received:{0} Corrupt: {1}",r.Statistics.ReceivedPackets, r.Statistics.CorruptPackets);

        }
    }
}
