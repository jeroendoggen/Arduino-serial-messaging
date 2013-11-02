// ReceiveMessages.ino - Demo application to receive data and command messages
// Copyright 2012 Jeroen Doggen (jeroendoggen@gmail.com)
//
// Warning: this code is untested (not sure if it compiles...)

#include "SerialMessage.h"
#include "defines.h"

SerialMessage Message;

void setup()
{
  Message.begin(115200,0);                         //begin(speed,nodeID);
}

void loop()
{
  delay(500);
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
time loop() runs, so using delay inside loop can delay
response.  Multiple bytes of data may be available.
*/

void serialEvent()
{
  Message.readSerialData();
  serialActions();
}

/// Take Actions based on incoming commands:
void serialActions(){
  Serial.println("Starting serialActions");

  if (_Message.getCommandID() == SET_SPEED_MOTOR_A) {
    int motorSpeed = _Message.getPayload();
    Serial.println("Set the motor speed to value 'motorSpeed'");
    _Message.sendData(COMMAND_REPLY,uint8_t(SET_SPEED_MOTOR_A));
    }
  }