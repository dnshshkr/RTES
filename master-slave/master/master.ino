/*
   0x01 - load params
   0x02 -
*/
#include<Arduino_JSON.h>
#include<SoftwareSerial.h>
const uint8_t gnd = 13, vin = 12, rx = 27, tx = 26;
SoftwareSerial slave(rx, tx);
void setup()
{
  slave.begin(38400);
  Serial.begin(9600);
  pinMode(gnd, OUTPUT);
  pinMode(vin, OUTPUT);
  digitalWrite(gnd, LOW);
  digitalWrite(vin, HIGH);
}

void loop()
{
  slave.write(0x80);
}
