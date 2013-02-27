namespace sharp_arduino_serial_packet_lib
{
     
    public class Packet
    {
        public Defines PacketType { get; set; }
        public Defines NodeID { get; set; }
        public Defines SensorID { get; set; }
        public Defines CommandID { get; set; }
        public Defines Payload { get; set; }
        public Defines Parity { get; set; }

        public override string ToString()
        {
            return string.Format("PacketType:{0} " +
                                 "NodeID:{1} " +
                                 "SensorID:{2} CommandID: {3}"+
                                 "Payload:{4} Parity:{5}",PacketType,NodeID,SensorID,CommandID,Payload, Parity);

        }
    } 
}
