/*
   request codes
   0x80 - request params
   0x81 - reset params
   0x82 - send new params
   0x83 - toggle RTES
   0x84 - exclusive start RTES
   0x85 - exclusive stop RTES
   0x86 - reset total pulses
   0x87 - reset cycleCount

   response codes
   0xf7 - new params received
   0xf8 - cycleCount is reset
   0xf9 - pulses are reset
   0xfa - rtes started
   0xfb - rtes stopped
   0xfc - params are reset
   0xfe - readings
   0xff - params
*/
#define ver "5.0"
#define slave Serial2
#define FORMAT_SPIFFS_IF_FAILED true
#include<Arduino_JSON.h>
#include "FS.h"
#include "SPIFFS.h"

//variables
bool mode;
bool dieselMode;
bool testMode;
bool changesMade = false;
bool engOffStatusPrintOnce = false;
bool fileManageMode = false;
unsigned int f2wPulseRatio;
uint8_t engineOffTimeout;
float flowRateBias;
float solShotBias;
unsigned int solOnTime;
float waterPercentage;
float denominator;
uint8_t checkpointPeriod;
unsigned long solOnTimePrevMillis;
unsigned long totalWaterPulse;
unsigned long engOffPrevMillis;
String cmd;
JSONVar readings, params;
void setup()
{
  slave.begin(38400);
  Serial.begin(9600);
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
    Serial.println("Failed to mount SPIFFS. Continuing...");
  else
    Serial.println("SPIFFS mounted");
  //  pinMode(gnd, OUTPUT);
  //  pinMode(vin, OUTPUT);
  //  digitalWrite(gnd, LOW);
  //  digitalWrite(vin, HIGH);
  slave.write(0x85);
  while (!slave.available()) {}
  if (slave.read() == 0xfb)
  {
    mode = true;
    slave.write(0x80);
  }
  while (!slave.available()) {}
  parseSlave();
  printSettings();
  Serial.println("RTES initialized");
  engOffPrevMillis = millis();
  slave.write(0x84);
  while (!slave.available()) {}
  if (slave.read() == 0xfa)
  {
    Serial.print("RTES mode entered");
    if (dieselMode)
      Serial.print(" (Diesel-only mode)");
    Serial.println();
  }
  else
  {
    Serial.println("Failed to start RTES");
    return;
  }
}
void loop()
{
  if (Serial.available())
    parseCMD();
  if (slave.available())
    parseSlave();
  if (millis() - engOffPrevMillis >= engineOffTimeout * 1000 && !mode)
  {
    if (!engOffStatusPrintOnce)
    {
      Serial.println("*Engine is off");
      engOffStatusPrintOnce = true;
    }
  }
  else
    engOffStatusPrintOnce = false;
}
void flushSerial()
{
  while (Serial.available())
    Serial.read();
}
