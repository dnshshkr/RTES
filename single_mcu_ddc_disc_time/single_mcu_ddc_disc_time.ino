#define ver "5.2"
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
const uint8_t addr[10] = {0, 2, 3, 7, 11, 13, 17, 18, 22, 23};
//addr[0]: 2 bytes
//addr[1]: 1 byte,
//addr[2]: 4 bytes
//addr[3]: 4 bytes
//addr[4]: 2 bytes
//addr[5]: 4 bytes
//addr[6]: 1 byte
//addr[7]: 4 bytes
//addr[8]: 1 byte
//addr[9]: 1 byte

/*
   | I/O pins
*/
const byte flowrateSensor = 2;
//const uint8_t fuelMotorCurrentPin = A3;
//const uint8_t solenoidCurrentPin = A2;
//const uint8_t waterPumpCurrentPin = A4;
const byte deviceMotor = 8;
const byte solenoid = 9;
const byte waterPump = 10;
const byte btrx = 12;
const byte bttx = 11;
//const byte btState = 5;
const byte button = 4;

/*
   | current amperage - not in use
*/
//float ampMotorFuel, ampSolenoid, ampwaterPump;
//float ampMotorFuelLow = 3.6;
//float ampMotorFuelHigh = 4.1;

/*
   | variables
*/
//volatile bool pulseDataPrint = false;
volatile byte cycleCount = 0;
volatile float fuelFlowRate;
volatile float fuelPulsePeriod;
volatile unsigned long totalFuelPulse = 0;
volatile unsigned long pulseMeasurePrevMillis = 0;
volatile unsigned long engOffPrevMillis = 0;
//bool btConnectedOnce = false;
//bool manualPrintData = false;
bool cmdAvailable;
bool engOffStatusPrintOnce;
bool sprayStarted = false;
bool sprayCompleted = false;
bool testMode;
//bool firstRowData = true;
//bool solenoidManualState = false;
//bool waterPumpManualState = false;
//bool toggleAllState = false;
bool mode;  //0 - RTES, 1 - settings, 2 - admin
bool dieselMode;
bool deviceState;
byte engineOffTimeout;
//byte hour = 0;
//byte minute = 0;
//byte second = 0;
//byte checkpointPeriod;
//uint8_t currentSensorType = 1; //0 - ACS713, 1 - ACS712
//byte lastMinute;
//byte accumMinute;
byte devicePeriod, deviceOnTime, deviceOffTime;
word f2wPulseRatio;  //fuel pulses per cycle
word solOnTime;
float denominator;  //fraction denominator for fuel-water percentage calculation
float waterPercentage;
float waterPercentageDuringEmulsion;
//float motorFuelAmpLim = 5.0 //set limit current motor feul pump
//float solenoidAmpLimit = 5.0 //set limit current solenoid
//float waterPumpAmpLimit = 5.0 //set limit current motor water
float fuelPulseBias;
float waterPulseBias;
float solConst;
unsigned long solOnTimePrevMillis;
//unsigned long prevMillisRTESStopwatch;
unsigned long totalWaterPulse = 0;
unsigned long devicePrevMillis;
String cmd;

SoftwareSerial bt(btrx, bttx);  //bluetooth module

void setup()
{
  //EEPROM.get(addr6, pwd_default);  //admin password
  Serial.begin(115200);
  bt.begin(38400);
  pinMode(flowrateSensor, INPUT_PULLUP);  //fuel flowrate sensor
  pinMode(waterPump, OUTPUT);
  pinMode(solenoid, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //  pinMode(btState, INPUT);
  pinMode(button, OUTPUT);
  digitalWrite(button, LOW);
  attachInterrupt(digitalPinToInterrupt(flowrateSensor), interruptRoutine, FALLING);
  loadSettings();
  mode = false;
  stopEmulsion();
  waterPercentageDuringEmulsion = (waterPulseBias / denominator) * 100.0;
  printSettings();
  Serial.println("RTES initialized");
  bt.println("RTES initialized");
  //  bool validTime;
  //  if (testMode)
  //    validTime = setTime();
  //  else
  //    goto startRTES1;
  //  if (!validTime)
  //  {
  //    digitalWrite(waterPump, LOW);
  //    mode = true;
  //    printSettings();
  //    Serial.println("Settings mode entered");
  //    bt.println("Settings mode entered");
  //  }
  //  else
  //  {
  //    if (testMode)
  //      prevMillisRTESStopwatch = millis();
  //startRTES1:
  //printSettings();
  engOffPrevMillis = pulseMeasurePrevMillis = millis();
  Serial.print("RTES mode entered");
  bt.print("RTES mode entered");
  if (dieselMode)
  {
    digitalWrite(waterPump, LOW);
    Serial.print(" (Diesel-only mode)");
    bt.print(" (Diesel-only mode)");
  }
  else
    digitalWrite(waterPump, HIGH);
  //  if (testMode)
  //  {
  //    Serial.print(" at ");
  //    bt.print(" at ");
  //    displayClock12(false);
  //  }
  Serial.println();
  bt.println();
  //displayLegend();
  //}
}

void loop() {
  //  if (!mode && testMode)
  //    stopwatch();
  //  if (!btConnectedOnce && digitalRead(btState))
  //  {
  //    printSettings();
  //    bt.println("RTES initialized");
  //    btConnectedOnce = true;
  //  }
  //  else if (btConnectedOnce && !digitalRead(btState))
  //    btConnectedOnce = false;
  /*
     | 1. engine-off detection
  */
  if (!mode && millis() - engOffPrevMillis >= engineOffTimeout * 1000)
  {
    if (!engOffStatusPrintOnce)  //so that it prints the text only once
    {
      Serial.println("*Engine is off*");
      bt.println("*Engine is off*");
      engOffStatusPrintOnce = true;
    }
  }
  else
    engOffStatusPrintOnce = false;

  /*
     | 2. serial command parsing
  */
  if (Serial.available())
  {
    cmd = Serial.readStringUntil('\r\n');
    cmdAvailable = true;
  }
  else if (bt.available())  //overwrite serial command over usb if bluetooth receives a command
  {
    cmd = bt.readStringUntil('\r\n');
    cmdAvailable = true;
  }
  else
    cmdAvailable = false;
  cmd.trim();  //eliminate null character at the end of the string
  if (cmdAvailable)
    cmdParser();

  /*
     | 3. RTES routine
  */
  if (!mode)
  {
    RTES();
    //    if (pulseDataPrint)  //print data only on a fuel pulse detection
    //    {
    //      if (testMode && accumMinute - lastMinute >= checkpointPeriod || (testMode && firstRowData))
    //      {
    //        displayClock12(true);
    //        Serial.print(" -> ");
    //        //if(bt)
    //        bt.println();
    //        lastMinute = accumMinute;
    //        firstRowData = false;
    //      }
    //      else if (testMode)
    //        Serial.print("\t    ");
    //      printData();
    //    }
  }

  /*
     | 4. End
  */
}

void flushSerial()  //clear serial buffer over usb and bluetooth
{
  while (Serial.available() || bt.available())
  {
    Serial.read();
    bt.read();
  }
}
