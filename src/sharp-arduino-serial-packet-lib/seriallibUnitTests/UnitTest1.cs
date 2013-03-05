using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using sharp_arduino_serial_packet_lib;
using sharp_arduino_serial_packet_lib.SerialPortReaderWriter;

namespace seriallibUnitTests
{
    [TestClass]
    public class PacketParsingTest
    {
        [TestMethod]
        public void SendPacketTest()
        {
            SerialReaderWriter rw = new SerialReaderWriter(new SerialSettings());
            rw.ParseData("T01N01I00P32Q00");

            Packet p = new Packet();
            p.PacketType= PacketTypes.Command;
            
            Assert.AreEqual(rw.incomingPacket.PacketType,p.PacketType);

            //TODO: expand test (test other fields, test with more strings)
            //TODO: override Equals method in Packet class
        }
        //TODO: test other methods (if we are bored some day :)   )
    }
}
