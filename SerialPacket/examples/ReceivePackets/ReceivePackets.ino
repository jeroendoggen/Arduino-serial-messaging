#include <DistanceSRF04.h>

DistanceSRF04 Dist;
int distance;

void setup()
{
  Serial.begin(9600);
  Dist.begin(2,3);
}

void loop()
{
  distance = Dist.getDistanceTime();
  Serial.print("\nRound trip time (microseconds)): ");
  Serial.print(distance);  
  delay(500); //make it readable
}