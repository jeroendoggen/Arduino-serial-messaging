namespace sharp_arduino_serial_packet_lib
{
    //http://www.codeproject.com/Articles/75770/Basic-serial-port-listening-application 
    public class Defaults
    {
        public const int DEFAULT_BAUDRATE = 115200;
    }
    public enum SensorTypes
    {

        TEMPERATURE = 0x10,
        HUMIDITY = 0x11,
        DISTANCE = 0x30,
        MOTORSTATUS = 0x50
    }
    public enum Commands
    {
        STOP_MOTOR_A = 0x10,
        START_MOTOR_A = 0x11,
        SET_SPEED_MOTOR_A = 0x12,
        BRAKE_MOTOR_A = 0x13,
        STOP_MOTOR_B = 0x15,
        START_MOTOR_B = 0x16,
        SET_SPEED_MOTOR_B = 0x17,
        BRAKE_MOTOR_B = 0x18
    }
    public enum PacketFields { Type, NodeID, SensorID, CommandID, Payload, Parity }
    public enum PacketTypes { Command = 0x01, Command_Reply = 0x02, Data_Request = 0x11, Data_Byte = 0x12, Data_Int = 0x13, Data_Array_Request = 0x21, Data_Array = 0x22 }

}
