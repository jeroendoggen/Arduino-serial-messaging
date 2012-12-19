// ReceivePackets.ino - Demo application to receive data and command messages
// Copyright 2012 Jeroen Doggen (jeroendoggen@gmail.com)
//
// Program flow:
//    -
// Options:
//    -

#include "SerialPacket.h"
#include "defines.h"

SerialPacket Packet;

void setup()
{
  Packet.begin(115200,0);                         //begin(speed,nodeID);
}

void loop()
{
  delay(500);
  //    Packet.sendData(1,uint8_t(6));
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
time loop() runs, so using delay inside loop can delay
response.  Multiple bytes of data may be available.
*/

void serialEvent()
{
  Packet.readSerialData();
}
