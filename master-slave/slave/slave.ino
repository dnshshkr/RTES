#include<Arduino_JSON.h>
#include<SoftwareSerial.h>
const uint8_t rx = A1, tx = A0;
SoftwareSerial master(rx, tx);
void setup()
{
  master.begin(38400);
  Serial.begin(9600);
  Serial.println("waiting for request");
}
void loop()
{
  if (master.available())
  {
    Serial.println(master.read());
  }
}
