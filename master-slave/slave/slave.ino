#include<Arduino_JSON.h>
#include<SoftwareSerial.h>
#include<EEPROM.h>
const uint8_t rx = A1, tx = A0;
const uint8_t addr[8] = {0, 2, 3, 7, 11, 13, 17, 18};

volatile bool pulseDataPrint = false;
volatile uint8_t f2wPulseRatioCount = 0;
volatile float fuelFlowRate;
volatile float fuelPulsePeriod;
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
bool testMode;
bool runRTES = false;
bool sprayStarted = false;
bool sprayedOnce = false;
bool sprayCompleted = false;
unsigned long solOnTimePrevMillis;
unsigned long totalWaterPulse = 0;

/*
   I/O pinouts
*/
const uint8_t flowrateSensor = 2;
const uint8_t solenoid = 9;
const uint8_t waterPump = 10;
const uint8_t button = 4;

JSONVar readings;
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
  if (master.available())
  {
    uint8_t reqCode = master.read();
    parseCMD(reqCode);
  }
  if (runRTES)
  {
    RTES();
    if (pulseDataPrint)
      pushData();
  }
}
