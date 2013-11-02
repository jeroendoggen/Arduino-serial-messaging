// SendMessagesOnRequest.ino - Demo application to send message data when requested by the host
// Copyright 2012 Jeroen Doggen (jeroendoggen@gmail.com)
//
// Program flow:
//    - Program waits for input from the user
//    - When the user sends: (over the serial port)
//       -- the number '1' --> reply= data message with sensorvalue=1
//       -- the number '2' --> reply= data message with sensorvalue=2
//       -- the letter 'a' --> reply= command message with commandValue=1
//       -- the letter 'b' --> reply= command message with commandValue=2

#include "SerialMessage.h"
#include "defines.h"

SerialMessage Message;

uint8_t sensorValue=1;
uint8_t commandValue=1;

#define MYCOMMAND1 1
#define MYCOMMAND2 1

#define MYDATA1 1
#define MYDATA2 1

void setup()
{
  Message.begin(115200,0);                         //begin(speed,nodeID);
}

void loop()
{
  if (Serial.available() > 0) {
    char inByte = Serial.read();

    if (inByte == '1') {
      Message.sendData(sensorValue);
    }

    if (inByte == '2') {
      Message.sendData(sensorValue*2);
    }

    if (inByte == 'a') {
      Message.sendCommand(commandValue);
    }

    if (inByte == 'b') {
      Message.sendCommand(commandValue*2);
    }
  }
}
