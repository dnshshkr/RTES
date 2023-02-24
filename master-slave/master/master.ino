#define ver "5.0"
/*
   request codes
   0x80 - request params
   0x81 - reset params
   0x82 - send new params
   0x83 - run/stop RTES
   0x84 - special stop RTES
   0x85 - request RTES status

   response codes
   0xfb - rtes run
   0xfc - rtes stopped
   0xfd - params are reset
   0xfe - params
   0xff - params
*/
#include<Arduino_JSON.h>
//pinouts
const uint8_t gnd = 13, vin = 12, rx = 27, tx = 26;

//variables
bool mode;
//unsigned int f2wPulseRatio;
//uint8_t engineOffTimeout;
//float flowRateBias;
//float solShotBias;
//unsigned int solOnTime;
//float waterPercentage;
//float denominator;
//uint8_t checkpointPeriod;
bool testMode;
unsigned long solOnTimePrevMillis;
unsigned long totalWaterPulse;
String cmd;
JSONVar params, readings;
void setup()
{
  Serial2.begin(38400);
  Serial.begin(9600);
  pinMode(gnd, OUTPUT);
  pinMode(vin, OUTPUT);
  digitalWrite(gnd, LOW);
  digitalWrite(vin, HIGH);
  Serial.println("RTES initialized");
  Serial2.write(0x84);
  while (!Serial2.available()) {}
  if (Serial2.read() == 0xfc)
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
void flushSerial()
{
  while (Serial.available())
    Serial.read();
}
