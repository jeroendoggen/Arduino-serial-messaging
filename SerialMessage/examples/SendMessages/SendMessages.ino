// SendMessages.ino - Demo application to send message data
// Copyright 2012 Jeroen Doggen (jeroendoggen@gmail.com)
//
// Program flow:
//    - send a data message every second: x10
//    - send a command every second: x5
//    - send a 5  field data message every second: x5
//    - send a 10 field data message every second: x5
//    - no messages for 5 seconds
//    - restart
// Options:
//    - select binary or ASCII mode by changing the #define values in 'includes.h'

#include "SerialMessage.h"
#include "defines.h"

SerialMessage Message;

uint8_t sensorID=0;
uint8_t commandID=0;

uint8_t payload=0;

uint8_t counter=0;

uint8_t dataArray1[5]={0,1,2,3,4};
uint8_t dataArray2[10]={0,1,2,3,4,5,6,7,8,9};

void setup()
{
  Message.begin(115200,0);                         //begin(speed,nodeID);
}

void loop()
{
  //   if(counter < 1)
  //   {
  //     Message.sendCommand(10, payload);
  //     payload++;
  //   }

  //   if(counter < 3)
  //   {
  //     Message.sendCommand(payload);
  //     payload++;
  //   }

  //   if(counter > 4 && counter < 10)
  //   {
  //     Message.sendCommandReply(commandID, payload);
  //     payload++;
  //   }
  //
  //   if(counter > 9 && counter < 15)
  //   {
  //     Message.sendDataRequest(sensorID, payload);
  //     payload++;
  //   }
  //
  //   if(counter > 14 && counter < 20)
  //   {
  //     Message.sendData(sensorID, payload);
  //     payload++;
  //   }
  //
  if(counter > 20 && counter < 25) {
    Message.sendData(sensorID, payload);
    payload++;
  }

  //   if(counter > 19 && counter < 25){
  //     Message.setMessageType(AGGREGATEDDATA);   //data messages
  //     Message.sendMessage(dataArray1,sizeof(dataArray1));
  //     for(int i=0;i<sizeof(dataArray1);i++){
  //       dataArray1[i]++;
  //     }
  //   }
  //
  //   if(counter > 24 && counter < 30){
  //     Message.setMessageType(AGGREGATEDDATA);   //data messages
  //     Message.sendMessage(dataArray2,sizeof(dataArray2));
  //     for(int i=0;i<sizeof(dataArray2);i++){
  //       dataArray2[i]++;
  //     }
  //   }

  if(counter == 30) {
    counter = -1;                                 //reset counter
  }

  counter++;
  delay(100);                                     // wait for 100 milliseconds
}
