#define ver "4.1.0"
/*===============================================================================================================
  ; Title:  RTES V4.0 Serial Interface
  ; Authors: Wan Ahmad V3.08v2 V3.09 V4.0 V4.0.1 (wahmad.wazhar@gmail.com) & Syamiladnin V3.07 (syamiladnin@gmail.com) & Danish V4.1.x (danish.shukor@outlook.com)

  ; Date:   11 Aug 22
  ;
  ; Log:
  ;   22 Sept 2021:
  ;   1- Water injection depends on counted pulse.
  ;   2- Debounced interrupt pin
  ;
  ;   30 Sept 2021:
  ;   1- 2sec (max) per cycle flowrate when idle
  ;   2- 1.14ml per rotation flowrate
  ;   3- 1.4ml per solenoid shot
  ;   4- solenoid shot using delay()
  ;
  ;   18 Oct 2021:
  ;   1- testing IO function added
  ;
  ;   20 Oct 2021:
  ;   1- testing IO function use millis()
  ;
  ;   27 Oct 2021:
  ;   1- Add function to toggle current sensor
  ;
  ;   11 Nov 2021:
  ;   1- update for "if (interrupt_time - last_interrupt_time > 100) "

   17 DEC 2021:
   Update Save setting to EEPROM

   7 Jan 2021:
   Add manual Mode for individual pump

   Blue= OldBotloader

   11 Aug 22:
   Version 4.0 is released.
   New Serial setting.
   Based code for GUI development.

   13 Oct 22:
   Add flowrate measurement on print_data (ml/min)
  ;================================================================================================================*/
/*
   | packages inclusion
*/
#include<EEPROM.h>
#include<SoftwareSerial.h>

/*
   | EEPROM memory address
*/
#define addr1 0 //2 bytes
#define addr2 2 //1 byte
#define addr3 3 //4 bytes
#define addr4 7 //4 bytes
#define addr5 11 //2 bytes
#define addr6 13 //4 bytes
#define addr7 17 //1 byte
#define addr8 18 //4 bytes - admin pin
uint32_t pwd_default;

/*
   | pinouts
*/
const uint8_t flowrateSensor = 2;
const uint8_t fuelMotorCurrentPin = A3;
const uint8_t solenoidCurrentPin = A2;
const uint8_t waterPumpCurrentPin = A4;
const uint8_t motorFuel = 8;
const uint8_t solenoidWater = 9;
const uint8_t motorWater = 10;
const uint8_t btrx = 12;
const uint8_t bttx = 11;
const uint8_t btState = 5;

/*
   | current amperage - not in use
*/
//float ampMotorFuel, ampSolenoid, ampMotorWater;

/*
   | variables
*/
volatile float measuredPulsePerMin;
volatile float fuelPulsePeriod;
volatile unsigned long totalFuelPulse = 0;
volatile unsigned long pulseMeasurePrevMillis = 0;
volatile unsigned long prevMillisEngOff = 0;
volatile uint8_t pulse_fuelToWaterRatioCount = 0;
bool settingMode = true;
bool manualPrintData = false;
bool adminState;
bool pulseDataPrint = false;
bool cmdAvailable;
bool engOffStatusPrintOnce;
bool sprayedOnce = false;
bool sprayStarted = false;
bool sprayCompleted = true;
bool solenoidManualState = false;
bool waterPumpManualState = false;
bool toggleAllState = false;
const uint8_t solenoidOnPulse = 1;
uint8_t engineOffTimeOut;
uint8_t currentSensorType = 1; //'0'=ACS713 '1'=ACS712
unsigned int pulse_fuelToWaterRatio; //pulse per water shot //CMD
unsigned int solenoidOnTime;
float denom;
float quickWaterPercentage;
//float motorFuelAmpLim = 5.0 //set limit current motor feul pump
//float solenoidAmpLimit = 5.0 //set limit current solenoid
//float motorWaterAmpLimit = 5.0 //set limit current motor water
float flowRateBias; //flowrate mL per pulse //CMD
float solShotBias; //solenoid mL per shot //CMD
unsigned long prevSolOnTime;
unsigned long totalWaterPulse = 0;
String cmd;

/*********************CheckFuelPumpCurrent***********************************************************************************/
//float ampMotorFuelLow = 3.6;
//float ampMotorFuelHigh = 4.1;

SoftwareSerial bt(btrx, bttx);

void setup()
{
  EEPROM.get(addr8, pwd_default);
  Serial.begin(57600);
  bt.begin(38400);
  pinMode(flowrateSensor, INPUT_PULLUP);
  pinMode(motorFuel, OUTPUT);
  pinMode(motorWater, OUTPUT);
  pinMode(solenoidWater, OUTPUT);
  pinMode(fuelMotorCurrentPin, INPUT);
  pinMode(solenoidCurrentPin, INPUT);
  pinMode(waterPumpCurrentPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //  pinMode(btrx, INPUT);
  //  pinMode(bttx, OUTPUT);
  pinMode(btState, INPUT);
  attachInterrupt(digitalPinToInterrupt(flowrateSensor), countPulse, FALLING);
  loadSettings(); //load settings
  settingMode = false;
  digitalWrite(solenoidWater, LOW);
  digitalWrite(motorWater, LOW);
  printSettings();
  Serial.println("RTES Initialized");
  bt.println("RTES Initialized");
  if (adminState)
    adminSettings();
  Serial.println("RTES mode entered");
  bt.println("RTES mode entered");
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  if (millis() - prevMillisEngOff >= engineOffTimeOut * 1000 && !settingMode)
  {
    if (!engOffStatusPrintOnce)
    {
      Serial.println("*Engine is off*");
      bt.println("*Engine is off*");
      //pulse_fuelToWaterRatioCount = 1;
      engOffStatusPrintOnce = true;
    }
  }
  else
    engOffStatusPrintOnce = false;

  /********************CMD Parser***************************************************************************************/
  if (Serial.available())
  {
    cmd = Serial.readStringUntil('\r\n');
    cmdAvailable = true;
  }
  else if (digitalRead(btState) && bt.available())
  {
    cmd = bt.readStringUntil('\r\n');
    cmdAvailable = true;
  }
  else
    cmdAvailable = false;
  cmd.trim();
  if (cmdAvailable)
    cmdParser();
  if (!settingMode && !adminState)
  {
    rtesSystem();
    if (pulseDataPrint)
      printData();
  }
  else if (adminState && manualPrintData)//Stop RTES
    printData();
  /***********************END*******************************************************************************************/
}
void flushSerial()
{
  do
  {
    Serial.read();
    bt.read();
    delay(1);
  } while (Serial.available() || bt.available()); \
}
