
#define REQUEST_PARAMS 0x80
#define RESET_PARAMS 0x81
#define SEND_NEW_PARAMS 0x82
#define TOGGLE_RTES 0x83
#define EXCLUSIVE_START_RTES 0x84
#define EXCLUSIVE_STOP_RTES 0x85
#define RESET_COUNTERS 0x86

#define ENGINE_OFF 0xf8
#define NEW_PARAMS_RECEIVED 0xf9
#define COUNTERS_RESET 0xfa
#define RTES_STARTED 0xfb
#define RTES_STOPPED 0xfc
#define PARAMS_RESET 0xfd
#define READINGS 0xfe
#define PARAMS 0xff

#include<Arduino_JSON.h>
#include<SoftwareSerial.h>
#include<EEPROM.h>
const uint8_t rx = A1, tx = A0;
const uint8_t addr[9] = {0, 2, 3, 7, 11, 13, 17, 18, 19};

volatile uint8_t cycleCount = 0;
volatile double fuelFlowRate;
volatile double fuelPulsePeriod;
volatile unsigned long totalFuelPulse = 0;
volatile unsigned long pulseMeasurePrevMillis = 0;
volatile unsigned long engOffPrevMillis = 0;
unsigned int f2wPulseRatio;
uint8_t engineOffTimeout;
float flowRateBias;
float solShotBias;
unsigned int solOnTime;
float waterPercentage;
float denominator;
uint8_t checkpointPeriod;
bool dieselMode;
bool testMode;
bool runRTES = true;
bool sprayStarted = false;
bool sprayedOnce = false;
bool sprayCompleted = false;
bool engOffStatusPrintOnce = false;
unsigned long solOnTimePrevMillis;
unsigned long totalWaterPulse = 0;

/*
   I/O pinouts
*/
const uint8_t flowrateSensor = 2;
const uint8_t solenoid = 9;
const uint8_t waterPump = 10;
const uint8_t button = 4;

JSONVar readings, params;
SoftwareSerial master(rx, tx);
void setup()
{
  master.begin(38400);
  Serial.begin(9600);
  pinMode(flowrateSensor, INPUT_PULLUP);
  pinMode(solenoid, OUTPUT);
  pinMode(waterPump, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(button, OUTPUT);
  digitalWrite(button, LOW);
  attachInterrupt(digitalPinToInterrupt(flowrateSensor), interruptRoutine, FALLING);
  stopEmulsion();
  loadParams();
}
void loop()
{
  if (millis() - engOffPrevMillis >= engineOffTimeout * 1000 && runRTES)
  {
    if (!engOffStatusPrintOnce)
    {
      master.write(0xf8);
      engOffStatusPrintOnce = true;
    }
  }
  else
    engOffStatusPrintOnce = false;
  if (master.available())
  {
    uint8_t reqCode = master.read();
    parseMaster(reqCode);
  }
  if (runRTES)
    RTES();
}
