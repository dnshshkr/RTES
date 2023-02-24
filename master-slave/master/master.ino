#define ver "5.0"
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
#include<Arduino_JSON.h>
//pinouts
//const uint8_t gnd = 13, vin = 12, rx = 27, tx = 26;

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
  Serial2.begin(38400);
  Serial.begin(9600);
  //  pinMode(gnd, OUTPUT);
  //  pinMode(vin, OUTPUT);
  //  digitalWrite(gnd, LOW);
  //  digitalWrite(vin, HIGH);
  Serial.println("RTES initialized");
  Serial2.write(0x84);
  while (!Serial2.available()) {}
  if (Serial2.read() == 0xfb)
  {
    mode = true;
    Serial2.write(0x80);
  }
}
void loop()
{
  if (Serial.available())
    parseCMD();
  if (Serial2.available())
    parseSerial2();
}
//void flushSerial()
//{
//  while (Serial.available())
//    Serial.read();
//}
