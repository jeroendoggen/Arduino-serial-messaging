namespace sharp_arduino_serial_packet_lib
{
     
    public class Packet
    {
        public PacketTypes PacketType { get; set; }
        public int NodeID { get; set; }
        public int SensorID { get; set; }
        public Commands CommandID { get; set; }
        public int Payload { get; set; }
        public int Parity { get; set; }

        public override string ToString()
        {
            return string.Format("PacketType:{0} " +
                                 "NodeID:{1} " +
                                 "SensorID:{2} CommandID: {3}"+
                                 "Payload:{4} Parity:{5}",PacketType,NodeID,SensorID,CommandID,Payload, Parity);

        }
    } 
}
