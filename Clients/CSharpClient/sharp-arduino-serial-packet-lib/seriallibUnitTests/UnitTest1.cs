using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using sharp_arduino_serial_packet_lib;
using sharp_arduino_serial_packet_lib.SerialPortReaderWriter;

namespace seriallibUnitTests
{
    //More info: http://msdn.microsoft.com/en-us/library/hh694602.aspx 
    [TestClass]
    public class PacketParsingTest
    {
        [TestMethod]
        public void SendPacketTest()
        {
            SerialReaderWriter rw = new SerialReaderWriter(new SerialSettings());
            rw.ParseData("T01N01I00P32Q00");

            Packet p = new Packet();
            p.PacketType = (PacketTypes)"01".FromHexStringToInt();

            p.NodeID = "01".FromHexStringToInt();
            p.CommandID = (Commands)"40".FromHexStringToInt();
            p.Payload = "32".FromHexStringToInt();
            p.Parity = "00".FromHexStringToInt();


            Assert.AreEqual(rw.incomingPacket, p);

            //TODO: expand test (test other fields, test with more strings)
            
        }
        //TODO: test other methods (if we are bored some day :)   )
    }
}
