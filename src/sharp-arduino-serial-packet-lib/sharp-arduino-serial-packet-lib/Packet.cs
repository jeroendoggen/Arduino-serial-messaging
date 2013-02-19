using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sharp_arduino_serial_packet_lib
{
     
    public struct Packet
    {
        public byte PacketType;
        public byte NodeID;
        public byte SensorID;
        public byte CommandID;
        public byte Payload;
        public byte Parity;
    } ;             
}
