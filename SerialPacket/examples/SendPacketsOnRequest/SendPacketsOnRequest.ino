// SendPacketsOnRequest.ino - Demo application to send packet data when requested by the host
// Copyright 2012 Jeroen Doggen (jeroendoggen@gmail.com)
//
// Program flow:
//    - Program waits for input from the user
//    - When the user sends: (over the serial port)
//       -- the number '1' --> reply= data packet with sensorvalue=1
//       -- the number '2' --> reply= data packet with sensorvalue=2
//       -- the letter 'a' --> reply= command packet with commandValue=1
//       -- the letter 'b' --> reply= command packet with commandValue=2

#include "SerialPacket.h"
#include "defines.h"

SerialPacket Packet;

uint8_t sensorValue=1;
uint8_t commandValue=1;

#define MYCOMMAND1 1
#define MYCOMMAND2 1

#define MYDATA1 1
#define MYDATA2 1

void setup()
{
  Packet.begin(115200,0);                         //begin(speed,nodeID);
}

void loop()
{
  if (Serial.available() > 0) {
    char inByte = Serial.read();

    if (inByte == '1') {
      Packet.sendData(sensorValue);
    }

    if (inByte == '2') {
      Packet.sendData(sensorValue*2);
    }

    if (inByte == 'a') {
      Packet.sendCommand(commandValue);
    }

    if (inByte == 'b') {
      Packet.sendCommand(commandValue*2);
    }
  }
}
