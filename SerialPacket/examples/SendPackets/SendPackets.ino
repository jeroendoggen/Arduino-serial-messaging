// SendPackets.ino - Demo application to send packet data
// Copyright 2012 Jeroen Doggen (jeroendoggen@gmail.com)
// 
// Program flow:
//    - send a data packet every second: x10
//    - send a command every second: x5
//    - send a 5  field data packet every second: x5
//    - send a 10 field data packet every second: x5
//    - no packets for 5 seconds
//    - restart
// Options:
//    - select binary or ASCII mode by changing the #define values in 'includes.h'

#include "SerialPacket.h"
#include "defines.h"

#define SERIAL_ASCII
//#define SERIAL_BINARY

SerialPacket Packet;

uint8_t sensorValue=0;
uint8_t commandValue=0;
uint8_t counter=0;

uint8_t dataArray1[5]={0,1,2,3,4};
uint8_t dataArray2[10]={0,1,2,3,4,5,6,7,8,9};

void setup()
{
  Packet.begin(115200,0,2,4,6);     //begin(speed,type,nodeID,sensorID,commandID);
  Packet.setPacketType(DATAPACKET);     //data packets
}

#define COMMANDPACKET     1
#define COMMANDREPLY      2
#define DATAREQUESTPACKET 3
#define DATAPACKET        4
#define AGGREGATEDDATA    5

void loop()
{
  if(counter < 1){
    Packet.setPacketType(COMMANDPACKET);
    Packet.sendPacket(commandValue); 
    commandValue++;
  }
//   
//   if(counter > 4 && counter < 10){
//     Packet.setPacketType(COMMANDREPLY);
//     Packet.sendPacket(commandValue); 
//     commandValue++;
//   }
//   
//   if(counter > 9 && counter < 15){
//    Packet.setPacketType(DATAREQUESTPACKET);
//    Packet.sendPacket(sensorValue); 
//    sensorValue++;
//  }
  
//   if(counter > 14 && counter < 20){
//     Packet.setPacketType(DATAPACKET);   //data packets
//     Packet.sendPacket(sensorValue); ; 
//   }
  
  if(counter > 19 && counter < 25){
    Packet.setPacketType(AGGREGATEDDATA);   //data packets
    Packet.sendPacket(dataArray1,sizeof(dataArray1)); 
    for(int i=0;i<sizeof(dataArray1);i++){
      dataArray1[i]++;
    }
  }
  
  if(counter > 24 && counter < 30){
    Packet.setPacketType(AGGREGATEDDATA);   //data packets
    Packet.sendPacket(dataArray2,sizeof(dataArray2)); 
    for(int i=0;i<sizeof(dataArray2);i++){
      dataArray2[i]++;
    }
  }

  if(counter == 30){
    counter = -1;          //reset counter
  }

  counter++;
  delay(100);            // wait for 1 second
}
