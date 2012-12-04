// Select either ASCII or the normal binary serial packet type

#define SERIAL_ASCII
//#define SERIAL_BINARY

#define COMMAND            0x01
#define COMMAND_REPLY      0x02

#define DATA_REQUEST       0x11
#define DATA_BYTE          0x12
#define DATA_INT           0x13

#define DATA_ARRAY_REQUEST 0x21
#define DATA_ARRAY         0x22

/// Sensor Types:

#define TEMPERATURE 0x10
#define HUMIDITY    0x11

#define DISTANCE    0x30