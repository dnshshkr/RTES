/*
   0x80 - request params
   0x81 - reset params
   0x82 - send new params
   0x83 - run RTES
   0x84 - stop RTES
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
  slave.write(0x80);
}
void loop()
{
}
