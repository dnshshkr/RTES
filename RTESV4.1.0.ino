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
bool sprayStarted = false;
/*********************Change value accordingly********************************************************************/
bool emulsionTrig = 1;                  // '1'=ON '0'=OFF emulsion //CMD
int pulse_fuelToWaterRatio = 11;                   //pulse per water shot //CMD
//unsigned long pulseCountTime = 10000;   //milisecond to complete on pulse fuel for idle time //CMD
float motorFuelAmpLim = 5;              //set limit current motor feul pump
float solenoidAmpLimit = 5;             //set limit current solenoid
float motorWaterAmpLimit = 5;           //set limit current motor water
float flowRateBias = 1.45;              //flowrate mililliter per pulse //CMD
float solShotBias = 1.4;                //solenoid mililliter per shot //CMD
unsigned long manualTimePeriod = 5000;  //set time for manual mode
//unsigned int pulsePeriodTime = 1000;             //set period time for pulse sensor capturing data (millisecond) 10=280
bool dummy = true;
uint8_t currentSensorType = 1;              //'0'=ACS713 '1'=ACS712
//int noiseRejection = 150;               //in ms //CMD
uint8_t solenoidOnPulse = 1;
unsigned int solenoidOnTime = 100;


//load EEPROM
/*****************************************************************************************************************/
#include <EEPROM.h>

/*********************Pinout*************************************************************************************/
const byte pulseFlowrate = 2;
//const int manualButton = 3;
const byte waterLevel = 4;

const byte fuelMotorCurrentPin = A3;
const byte solenoidCurrentPin = A2;
const byte waterPumpCurrentPin = A4;

const byte motorFuel = 8;
const byte solenoidWater = 9;
const byte motorWater = 10;

//const int sevenSegmentDIO = 11;
//const int sevenSegmentCLK = 12;
//const int buzzer = 13;

/*********************Seven segment display***********************************************************************/
// #include <TM1637Display.h>
// TM1637Display display(sevenSegmentCLK, sevenSegmentDIO);

/*********************current amperage****************************************************************************/
//unsigned long lastExecutedMillis = 0;
float ampMotorFuel, ampSolenoid, ampMotorWater;
int ampA, ampB, ampC; //ampCountLoop = 0;

/*********************variables***********************************************************************************/
//bool buzzerToggle = 0;
//bool pulseIncToggle = 0;
volatile float measuredPulsePerMin = 0;
volatile float fuelPulsePeriod;
volatile uint8_t pulseCounter = 1;
volatile unsigned long totalFuelPulse = 0;
volatile uint8_t pulse_fuelToWaterRatioCount = 1;
volatile int flagManual = 0;
//unsigned long buzzerPreviousTime = 0;
volatile unsigned long measPlsPreviousTime = 0;
//unsigned long measCntPreviousTime = 0;
//unsigned long rtesPreviousTime = 0;
//unsigned long rtesPreviousTime2 = 0;
//unsigned long manualPreviousTime = 0;
unsigned long pulseInc = 0;
//unsigned long rtesCurrentTime = 0;
//unsigned long rtesCurrentTime2 = 0;
//unsigned long measCntCurrentTime = 0;
//unsigned long testIOPreviousTime = 0;
//int stallVar = 0;
int pulseCnt = 0;
//int pulseCntFlag = 0;
//int pulseCntDebounceFlag = 0;
//unsigned long opening = 0;   //valve opening period in millisecond
//unsigned long freq = 60000;  //valve frequency in millisecond
unsigned long prevSolOnTime;
float denom;// = flowRateBias * pulse_fuelToWaterRatio + solShotBias * solenoidOnPulse;

/*********************CmdParser***********************************************************************************/
String sdata = "";  // Initialised to nothing.
bool SettingMode = 1;
bool printState = 1;

/*********************CheckFuelPumpCurrent***********************************************************************************/
int ampMotorFuelLow = 3.6;
int ampMotorFuelHigh = 4.1;
bool stopEmulsion = 0;

void setup()
{
  Serial.begin(250000);
  pinMode(pulseFlowrate, INPUT_PULLUP);
  pinMode(waterLevel, INPUT_PULLUP);
  //pinMode(manualButton, INPUT_PULLUP);
  pinMode(motorFuel, OUTPUT);
  pinMode(motorWater, OUTPUT);
  pinMode(solenoidWater, OUTPUT);
  // pinMode(buzzer, OUTPUT);
  pinMode(fuelMotorCurrentPin, INPUT);
  pinMode(solenoidCurrentPin, INPUT);
  pinMode(waterPumpCurrentPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(pulseFlowrate), countPulse, FALLING);

  /********************The main Delay***********************************************************************************/
  currentLimitedOut(1, 0, 1);  //(fuel,sol,wat) //MUST ON FUEL AND WATER PUMP
  //display.setBrightness(0x0f);
  //delay(50);
  //display.showNumberHexEx(0xbbbb);

  pulseCounter = 0;
  totalFuelPulse = 0;
  pulseCnt = 0;

  //LOAD SETTING
  //SaveSetting();
  LoadSetting();

  //********Initial shot water (if needed)
  //  currentLimitOutput(1, emulsionTrig, 1); //(fuel,sol,wat)
  //  delay(100); // added this value to count pulse function
  //  currentLimitOutput(1, 0, 1);//(fuel,sol,wat)
  //  pulseInc++;

  printSetting();

  Serial.println("RTES Initialized ");
  if (manualPumpState)
    printSettingManual();
  digitalWrite(motorWater, HIGH);
}
void loop()
{
  //  unsigned long measuredLoopTime = micros();

  /********************CMD Parser***************************************************************************************/
  CmdParser();
  /********************RTES SYSTEM**************************************************************************************/
  //digitalWrite(motorWater,HIGH);
  if (pulseDataPrint)
  {
    //measurePulse();  //calculate flowrate
    //countPulse();
    measureAmperage();  //read current sensor
  }
  /********************PRINT DATA***************************************************************************************/
  if (!SettingMode && !manualPumpState && pulseDataPrint)
  {
    if (digitalRead(waterLevel) == 1 && flagManual == 0 && !stopEmulsion)
      rtesSystem();
    printData();
    //Serial.print("Fuel: ");Serial.print(fuelTrig);Serial.print("\t");Serial.print("Water: ");Serial.print(waterTrig);Serial.print("\t");Serial.print("Sol: ");Serial.println(emulsionTrig);

  }
  else if (manualPumpState)  //Stop RTES
  {
    fuelPump(fuelTrig);
    solPump(emulsionTrig);
    waterPump(waterTrig);
    if (manualPrintData)
      printData();

  }
  else if (digitalRead(waterLevel) == 1 && flagManual == 0)
    rtesSystem();  //Only SettingMode
  //testIO();
  /***********************END*******************************************************************************************/

  //  Serial.println("      " + String(micros() - measuredLoopTime) + " microsecond per loop");
}
