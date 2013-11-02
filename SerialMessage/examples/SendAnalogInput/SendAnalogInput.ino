#include "SerialMessage.h"
#include "defines.h"

SerialMessage Message;

#define SENSORID 2
int a=32760 ;
uint8_t b=0;

void setup()
{
  Message.begin(115200,1);                         //begin(speed,nodeID);
}

void loop()
{

  /*Serial.println(a);
  Serial.println(a,HEX);
  Message.sendData(SENSORID, a);

  Serial.println(b);
  Serial.println(b,HEX);
  Message.sendData(SENSORID, b)*/
  ;

  Message.sendDataArrayRequest(10,20);
  delay(1000);                                    // wait for 100 milliseconds
  a++;
  b++;
}
