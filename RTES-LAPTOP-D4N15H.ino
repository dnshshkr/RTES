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
#include <EEPROM.h>
#include <SoftwareSerial.h>

/*
   | EEPROM memory address
*/
#define addr0 0   //2 bytes
#define addr1 2   //1 byte
#define addr2 3   //4 bytes
#define addr3 7   //4 bytes
#define addr4 11  //2 bytes
#define addr5 13  //4 bytes
#define addr6 17  //6 bytes byte - admin
char pwd_default[6];

/*
   | pinouts
*/
const uint8_t flowrateSensor = 2;
//const uint8_t fuelMotorCurrentPin = A3;
//const uint8_t solenoidCurrentPin = A2;
//const uint8_t waterPumpCurrentPin = A4;
//const uint8_t motorFuel = 8;
const uint8_t solenoid = 9;
const uint8_t waterPump = 10;
const uint8_t btrx = 12;
const uint8_t bttx = 11;
const uint8_t btState = 5;

/*
   | current amperage - not in use
*/
//float ampMotorFuel, ampSolenoid, ampwaterPump;
//float ampMotorFuelLow = 3.6;
//float ampMotorFuelHigh = 4.1;

/*
   | variables
*/
volatile float measuredPulsePerMin;
volatile float fuelPulsePeriod;
volatile unsigned long totalFuelPulse = 0;
volatile unsigned long pulseMeasurePrevMillis = 0;
volatile unsigned long prevMillisEngOff = 0;
volatile uint8_t f2wPulseRatioCount = 0;
bool manualPrintData = false;
bool pulseDataPrint = false;
bool cmdAvailable;
bool engOffStatusPrintOnce;
bool sprayedOnce = false;
bool sprayStarted = false;
bool sprayCompleted = true;
bool solenoidManualState = false;
bool waterPumpManualState = false;
bool toggleAllState = false;
uint8_t mode;  //0 - RTES, 1 - settings, 2 - admin
uint8_t engineOffTimeout;
uint8_t hour = 0;
uint8_t minute = 0;
uint8_t lastMinute;
uint8_t second = 0;
//uint8_t currentSensorType = 1; //0 - ACS713, 1 - ACS712
unsigned int f2wPulseRatio;  //fuel pulses per cycle
unsigned int solenoidOnTime;
float denominator;  //fraction denominatorinator for fuel-water percentage calculation
float waterPercentage;
//float motorFuelAmpLim = 5.0 //set limit current motor feul pump
//float solenoidAmpLimit = 5.0 //set limit current solenoid
//float waterPumpAmpLimit = 5.0 //set limit current motor water
float flowRateBias;
float solShotBias;
unsigned long prevSolOnTime;
unsigned long prevMillisRTESStopwatch;
unsigned long totalWaterPulse = 0;
String cmd;

SoftwareSerial bt(btrx, bttx);  //bluetooth module

void setup() {
  EEPROM.get(addr6, pwd_default);  //admin password
  Serial.begin(19200);
  bt.begin(38400);
  pinMode(flowrateSensor, INPUT_PULLUP);  //fuel flowrate sensor
  pinMode(waterPump, OUTPUT);
  pinMode(solenoid, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(btState, INPUT);
  attachInterrupt(digitalPinToInterrupt(flowrateSensor), interruptRoutine, FALLING);
  loadSettings();
  //settingMode = false; //start RTES mode immediately at startup
  mode = 0;
  stopEmulsion();
  printSettings();
  Serial.println("RTES initialized");
  bt.println("RTES initialized");
  bool validTime = setTime();
  if (!validTime) {
    printSettings();
    Serial.println("Settings mode entered");
    bt.println("Settings mode entered");
  } else {
    prevMillisRTESStopwatch = millis();
    Serial.println("RTES mode entered at ");
    bt.println("RTES mode entered at ");
    displayClock12();
  }
}

void loop() {
  if (mode == 0)
    stopwatch();
  /*
     | 1. engine-off detection
  */
  if (millis() - prevMillisEngOff >= engineOffTimeout * 1000 && mode == 0) {
    if (!engOffStatusPrintOnce)  //so that it prints the text only once
    {
      Serial.println("*Engine is off*");
      bt.println("**Engine is off**");
      engOffStatusPrintOnce = true;
    }
  } else
    engOffStatusPrintOnce = false;

  /*
     | 2. serial command parsing
  */
  if (Serial.available()) {
    cmd = Serial.readStringUntil('\r\n');
    cmdAvailable = true;
  } else if (bt.available())  //overwrite serial command over usb if bluetooth receives a command
  {
    cmd = bt.readStringUntil('\r\n');
    cmdAvailable = true;
  } else
    cmdAvailable = false;
  cmd.trim();  //eliminate null character at the end of the string
  if (cmdAvailable)
    cmdParser();

  /*
     | 3. RTES routine
  */
  if (mode == 0) {
    RTES();
    if (pulseDataPrint)  //print data only on a fuel pulse detection
    {
      if (minute - lastMinute >= 3) {
        Serial.println(String(hour) + ':' + String(minute) + " -> ");
        bt.println(String(hour) + ':' + String(minute) + " -> ");
        lastMinute = minute;
      }
      printData();
    }
  } else if (mode == 2 && manualPrintData)
    printData();

  /*
     | 4. End
  */
}

void flushSerial()  //clear serial buffer over usb and bluetooth
{
  while (Serial.available() || bt.available()) {
    Serial.read();
    bt.read();
    delay(1);
  }
}