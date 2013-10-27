using System.Text;

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

        public override bool Equals(object obj)
        {
            Packet inco = obj as Packet;
            if (inco != null)
            {
                if (inco.Parity == this.Parity)
                {
                    if (inco.NodeID == this.NodeID && inco.PacketType == this.PacketType && inco.Payload == this.Payload &&
                        inco.SensorID == this.SensorID)
                        return true;
                }
            }
            return false;
        }

        public string ToStringMessageArray()
        {
            StringBuilder res = new StringBuilder();
            res.Append('T');
            int type = (int) this.PacketType;
            res.Append(type.FromIntToHexString()); //TODO: make this more straightforward/faster
            res.Append('N');
            res.Append(NodeID.FromIntToHexString());
            res.Append('I');
            if (this.PacketType == PacketTypes.Command || this.PacketType == PacketTypes.Command_Reply)
            {
                int com = (int) this.CommandID;
                res.Append(com.FromIntToHexString());
            }
            else res.Append(this.SensorID.FromIntToHexString());
            res.Append('P');
            res.Append(this.Payload.FromIntToHexString());
            res.Append('Q');
            res.Append(this.Parity);
            return res.ToString();

        }
    } 
}
