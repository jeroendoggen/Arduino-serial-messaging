#include "SerialPacket.h"
#include "defines.h"

SerialPacket Packet;

#define SENSORID 2

void setup()
{
  Packet.begin(115200,1);     //begin(speed,nodeID);
}

void loop()
{
  int a= analogRead(A0);
  Serial.println(a);
  Packet.sendData(SENSORID, a >> 2); 
  delay(1000);         // wait for 100 milliseconds
}
