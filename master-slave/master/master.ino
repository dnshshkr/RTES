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
#define RTES_VERSION "5.0"
#define slave Serial2
#define FORMAT_SPIFFS_IF_FAILED true
#define WIFI_SSID "iRokiah"
#define WIFI_PASSWORD "w1943sei"
#define API_KEY "AIzaSyCi1wz8zPrEiqk_pBtu8G5jSPFr98EIYkg"
#define USER_EMAIL "danish.shukor@outlook.com"
#define USER_PASSWORD "w1943sei"
#define STORAGE_BUCKET_ID "rtes-378707.appspot.com"

#include <Arduino_JSON.h>
#include "FS.h"
#include "SPIFFS.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>

//variables
bool firstData = false;
bool dieselMode;
bool testMode;
bool changesMade = false;
bool engOffStatusPrintOnce = false;
bool fileManageMode = false;
bool uploadCompleted = false;
byte mode;
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
File stream;
JSONVar readings, params, fileConfig;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
void setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  slave.begin(38400);
  Serial.begin(115200);
  
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
    Serial.println("Failed to mount SPIFFS. Continuing...");
  else
    Serial.println("SPIFFS mounted");
  readConfigFile(SPIFFS);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi: " + String(WIFI_SSID));
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(300);
  }
  Serial.print("\nConnected to " + String(WIFI_SSID) + " with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  config.fcs.upload_buffer_size = 512;
  
  slave.write(0x85);
  while (!slave.available()) {}
  if (slave.read() == 0xfb)
  {
    mode = 1;
    slave.write(0x80);
  }
  while (!slave.available()) {}
  parseSlave();
  Serial.println("RTES initialized");
  engOffPrevMillis = millis();
  slave.write(0x84);
  while (!slave.available()) {}
  parseSlave();
}
void loop()
{
  if (Serial.available())
    parseCMD();
  if (slave.available())
    parseSlave();
  if (millis() - engOffPrevMillis >= engineOffTimeout * 1000 && mode == 0)
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
