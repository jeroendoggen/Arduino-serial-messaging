#include "SerialPacket.h"
#include "defines.h"

SerialPacket Packet;

#define SENSORID 2
int a=32760 ;
uint8_t b=0;

void setup()
{
  Packet.begin(115200,1);                         //begin(speed,nodeID);
}

void loop()
{

  /*Serial.println(a);
  Serial.println(a,HEX);
  Packet.sendData(SENSORID, a);

  Serial.println(b);
  Serial.println(b,HEX);
  Packet.sendData(SENSORID, b)*/
  ;

  Packet.sendDataArrayRequest(10,20);
  delay(1000);                                    // wait for 100 milliseconds
  a++;
  b++;
}
