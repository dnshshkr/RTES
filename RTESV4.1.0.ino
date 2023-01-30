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

/*********************Set limit***********************************************************************************/
//Save Initial Setting EEPROM
int addr1 = 0;
int addr2 = 5;
int addr3 = 10;
int addr4 = 15;
int addr5 = 20;
int addr6 = 25;
int addr7 = 30;
int addr8 = 35;
int addr9 = 40;
int addr10 = 45;

bool read1 = 0;
int read2 = 0;
unsigned long read3 = 0;
float read4 = 0;
float read5 = 0;
int read6 = 0;
bool read7 = 0;
bool read8 = 0;
bool read9 = 0;
int read10 = 0;
bool manualPrintData = 0;
bool manualPumpState = 0;
bool AllState = 0;
bool fuelTrig = true;
bool waterTrig = true;
bool pulseDataPrint = false;

//load EEPROM
/*****************************************************************************************************************/
#include<EEPROM.h>
#include<SoftwareSerial.h>

/*********************Pinout*************************************************************************************/
const uint8_t pulseFlowrate = 2;
//const int manualButton = 3;
//const int waterLevel = 4;

const uint8_t fuelMotorCurrentPin = A3;
const uint8_t solenoidCurrentPin = A2;
const uint8_t waterPumpCurrentPin = A4;

const uint8_t motorFuel = 8;
const uint8_t solenoidWater = 9;
const uint8_t motorWater = 10;

const uint8_t btrx = 12;
const uint8_t bttx = 11;
const uint8_t btState = 5;

/*********************current amperage****************************************************************************/
//unsigned long lastExecutedMillis = 0;
float ampMotorFuel, ampSolenoid, ampMotorWater;
//int ampA, ampB, ampC; //ampCountLoop = 0;

/*********************variables***********************************************************************************/
volatile float measuredPulsePerMin = 0;
volatile float fuelPulsePeriod;
volatile uint8_t pulseCounter = 1;
volatile unsigned long totalFuelPulse = 0;
volatile uint8_t pulse_fuelToWaterRatioCount = 0;
volatile int flagManual = 0;
bool sprayedOnce = false;
bool startSpray = false;
bool sprayCompleted = true;
volatile unsigned long measPlsPreviousTime = 0;
unsigned long pulseInc = 0;
int pulseCnt = 0;
unsigned long prevSolOnTime;
float denom;
float quickWaterPercentage = 10;
unsigned long prevMillisEngOff;
bool engOffStatusPrintOnce;
const uint8_t solenoidOnPulse = 1;
bool emulsionTrig = 1;                  // '1'=ON '0'=OFF emulsion //CMD
int pulse_fuelToWaterRatio = 11;                   //pulse per water shot //CMD
//unsigned long pulseCountTime = 10000;   //milisecond to complete on pulse fuel for idle time //CMD
float motorFuelAmpLim = 5;              //set limit current motor feul pump
float solenoidAmpLimit = 5;             //set limit current solenoid
float motorWaterAmpLimit = 5;           //set limit current motor water
float flowRateBias = 1.45;              //flowrate mililliter per pulse //CMD
float solShotBias = 1.4;                //solenoid mililliter per shot //CMD
//unsigned int pulsePeriodTime = 1000;             //set period time for pulse sensor capturing data (millisecond) 10=280
unsigned int engineOffTimeOut = 10000;
uint8_t currentSensorType = 1;              //'0'=ACS713 '1'=ACS712
unsigned int solenoidOnTime = 100;
String msg;
bool cmdAvailable;

/*********************CmdParser***********************************************************************************/
String sdata = "";  // Initialised to nothing.
bool SettingMode = true;
bool printState = true;

/*********************CheckFuelPumpCurrent***********************************************************************************/
float ampMotorFuelLow = 3.6;
float ampMotorFuelHigh = 4.1;
bool stopEmulsion = 0;

SoftwareSerial bt(btrx, bttx);

void setup()
{
  Serial.begin(115200);
  bt.begin(57600);
  pinMode(pulseFlowrate, INPUT_PULLUP);
  //pinMode(waterLevel, INPUT_PULLUP);
  pinMode(motorFuel, OUTPUT);
  pinMode(motorWater, OUTPUT);
  pinMode(solenoidWater, OUTPUT);
  pinMode(fuelMotorCurrentPin, INPUT);
  pinMode(solenoidCurrentPin, INPUT);
  pinMode(waterPumpCurrentPin, INPUT);
  pinMode(btrx, INPUT);
  pinMode(bttx, OUTPUT);
  pinMode(btState, INPUT);
  attachInterrupt(digitalPinToInterrupt(pulseFlowrate), countPulse, FALLING);
  pulseCounter = 0;
  totalFuelPulse = 0;
  pulseCnt = 0;
  LoadSetting(); //load settings
  SettingMode = true;
  digitalWrite(solenoidWater, LOW);
  digitalWrite(motorWater, LOW);
  printSetting();
  Serial.println("RTES Initialized");
  if (digitalRead(btState))
    bt.println("RTES Initialized");
  if (manualPumpState)
    printSettingManual();
  SettingMode = false;
  Serial.println("RTES mode entered");
  if (digitalRead(btState))
    Serial.println("RTES mode entered");
}

void loop()
{
  if (millis() - prevMillisEngOff >= engineOffTimeOut && !SettingMode)
  {
    if (!engOffStatusPrintOnce)
    {
      Serial.println("Engine is off");
      if (digitalRead(btState))
        bt.println("Engine is off");
      //pulse_fuelToWaterRatioCount = 1;
      engOffStatusPrintOnce = true;
    }
  }
  else
    engOffStatusPrintOnce = false;
  //  unsigned long measuredLoopTime = micros();

  /********************CMD Parser***************************************************************************************/
  if (Serial.available())
  {
    msg = Serial.readStringUntil('\r\n');
    cmdAvailable = true;
  }
  else if (digitalRead(btState) && bt.available())
  {
    msg = bt.readStringUntil('\r\n');
    cmdAvailable = true;
  }
  else
    cmdAvailable = false;
  if (cmdAvailable)
  {
    msg.trim();
    cmdParser();
  }

  /********************RTES SYSTEM**************************************************************************************/
  //  if (pulseDataPrint)
  //    measureAmperage();
  /********************PRINT DATA***************************************************************************************/
  if (!SettingMode && !manualPumpState && pulseDataPrint)
  {
    //    if (digitalRead(waterLevel) == 1 && flagManual == 0 && !stopEmulsion)
    //    {
    //      measureAmperage();  //read current sensor
    //      rtesSystem();
    //    }
    if (!flagManual && !stopEmulsion)
      rtesSystem();
    printData();
  }
  else if (manualPumpState && manualPrintData)  //Stop RTES
    printData();
  else if (!flagManual)
    rtesSystem();  //Only SettingMode
  /***********************END*******************************************************************************************/
}
