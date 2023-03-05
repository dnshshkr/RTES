#include<SoftwareSerial.h>
const int state=A5,tx=A4,rx=A3,gnd=A2,vcc=A1,en=A0;
SoftwareSerial bt(rx,tx);
void setup()
{
  Serial.begin(38400);
  bt.begin(38400);
  pinMode(state,INPUT);
  pinMode(gnd,OUTPUT);
  pinMode(vcc,OUTPUT);
  pinMode(en,OUTPUT);
  digitalWrite(gnd,LOW);
  digitalWrite(vcc,HIGH);
  digitalWrite(en,HIGH); 
}
void loop()
{
  if(bt.available())
    Serial.write(bt.read());
  if(Serial.available())
    bt.write(Serial.read());
}
