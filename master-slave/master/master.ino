/*
   request codes
   0x80 - request params
   0x81 - reset params
   0x82 - send new params
   0x83 - toggle RTES
   0x84 - exclusive stop RTES
   0x85 - reset total pulses
   0x86 - reset cycleCount

   response codes
   0xf7 - new params received
   0xf8 - cycleCount is reset
   0xf9 - pulses are reset
   0xfa - rtes run
   0xfb - rtes stopped
   0xfc - params are reset
   0xfe - readings
   0xff - params
*/
#define ver "5.0"
#define slave Serial2
#include<Arduino_JSON.h>

//variables
bool mode;
unsigned int f2wPulseRatio;
uint8_t engineOffTimeout;
float flowRateBias;
float solShotBias;
unsigned int solOnTime;
float waterPercentage;
float denominator;
uint8_t checkpointPeriod;
bool testMode;
bool changesMade = false;
unsigned long solOnTimePrevMillis;
unsigned long totalWaterPulse;
String cmd;
JSONVar params, readings;
void setup()
{
  slave.begin(38400);
  Serial.begin(9600);
  //  pinMode(gnd, OUTPUT);
  //  pinMode(vin, OUTPUT);
  //  digitalWrite(gnd, LOW);
  //  digitalWrite(vin, HIGH);
  Serial.println("RTES initialized");
  slave.write(0x84);
  while (!slave.available()) {}
  if (slave.read() == 0xfb)
  {
    mode = true;
    slave.write(0x80);
  }
  while (!slave.available()) {}
  parseSlave();
}
void loop()
{
  if (Serial.available())
    parseCMD();
  if (slave.available())
    parseSlave();
}
void flushSerial()
{
  while (Serial.available())
    Serial.read();
}
