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

        public string RawString { get; set; }
        public override string ToString()
        {
            return string.Format("Raw string:{6} \nType:{0} " +
                                 "Node:{1} " +
                                 "Sensor:{2} Command: {3}"+
                                 "Payload:{4} Parity:{5}\n",PacketType,NodeID,SensorID,CommandID,Payload, Parity, RawString.Trim());

        }

        internal byte[] ToByteArray()
        {
            throw new System.NotImplementedException();
        }
    } 
}
