// SendPacketsOnRequest.ino - Demo application to send packet data when requested by the host
// Copyright 2012 Jeroen Doggen (jeroendoggen@gmail.com)
// 
// Program flow:
//    - ...
// Options:
//    - select binary or ASCII mode by changing the #define values in 'includes.h'

#include "SerialPacket.h"
#include "defines.h"

#define SERIAL_ASCII
//#define SERIAL_BINARY

SerialPacket Packet;

uint8_t sensorValue=0;
uint8_t commandValue=0;
int counter=0;

uint8_t dataArray1[5]={0,1,2,3,4};
uint8_t dataArray2[10]={0,1,2,3,4,5,6,7,8,9};

void setup()
{
  Packet.begin(115200,0,2,4);     //begin(speed,type,nodeID,sensorID);
  Packet.setPacketType(DATAPACKET);     //data packets
}

void loop()
{
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == '1'){
      Packet.sendPacket(sensorValue);
      Packet.sendPacket(sensorValue*2);
      }
     if (inByte == '1'){
      Packet.sendPacket(sensorValue);
      Packet.sendPacket(sensorValue*2);
      sensorValue++;
  }
} 
