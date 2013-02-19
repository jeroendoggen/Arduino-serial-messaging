using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sharp_arduino_serial_packet_lib
{
    class Defines
    {
        public const int DEFAULT_BAUDRATE = 115200;

        public const int COMMAND = 0x01;
        public const int COMMAND_REPLY = 0x02;

        public const int DATA_REQUEST = 0x11;
        public const int DATA_BYTE = 0x12;
        public const int DATA_INT = 0x13;

        public const int DATA_ARRAY_REQUEST = 0x21;
        public const int DATA_ARRAY = 0x22;

        //Sensor types:

        public const int TEMPERATURE = 0x10;
        public const int HUMIDITY = 0x11;

        public const int DISTANCE = 0x30;
        public const int MOTORSTATUS = 0x50;

        //Command IDs

        public const int STOP_MOTOR_A = 0x10;
        public const int START_MOTOR_A = 0x11;
        public const int SET_SPEED_MOTOR_A = 0x12;
        public const int BRAKE_MOTOR_A = 0x13;

        public const int STOP_MOTOR_B = 0x15;
        public const int START_MOTOR_B = 0x16;
        public const int SET_SPEED_MOTOR_B = 0x17;
        public const int BRAKE_MOTOR_B = 0x18;


    }
}
