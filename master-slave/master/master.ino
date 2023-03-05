/*
   request codes
   REQUEST_PARAMS - request params
   RESET_PARAMS - reset params
   SEND_NEW_PARAMS - send new params
   TOGGLE_RTES - toggle RTES
   EXCLUSIVE_START_RTES - exclusive start RTES
   EXCLUSIVE_STOP_RTES - exclusive stop RTES
   RESET_COUNTERS - reset counters

   response codes
   NEW_PARAMS_RECEIVED - new params received
   COUNTERS_RESET - counters are reset
   RTES_STARTED - rtes started
   RTES_STOPPED - rtes stopped
   PARAMS_RESET - params are reset
   READINGS - readings
   PARAMS - params
*/
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

#define RTES_VERSION "5.0"
#define slave Serial2
#define FORMAT_SPIFFS_IF_FAILED true
#define WIFI_SSID "Mesh_Amin"
#define WIFI_PASSWORD "1234123456"
#define API_KEY "AIzaSyCi1wz8zPrEiqk_pBtu8G5jSPFr98EIYkg"
#define USER_EMAIL "danish44945@gmail.com"
#define USER_PASSWORD "butokimak"
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
bool fileManageMode = false;
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
  pinMode(LED_BUILTIN, OUTPUT);

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
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  config.fcs.upload_buffer_size = 512;
  //fbdo.setResponseSize(1024);

  slave.write(RESET_COUNTERS); //reset counters
  while (!slave.available()) {}
  parseSlave();
  slave.write(EXCLUSIVE_STOP_RTES); //exclusive stop RTES
  while (!slave.available()) {}
  if (slave.read() == RTES_STOPPED)
    mode = 1;
  slave.write(REQUEST_PARAMS);
  while (!slave.available()) {}
  parseSlave();
  Serial.println("RTES initialized");
  slave.write(EXCLUSIVE_START_RTES);
  while (!slave.available()) {}
  parseSlave();
}
void loop()
{
  if (Serial.available())
    parseCMD();
  if (slave.available())
    parseSlave();
  if (WiFi.status() == WL_CONNECTED)
    digitalWrite(LED_BUILTIN, HIGH);
  else
    digitalWrite(LED_BUILTIN, LOW);
}
void flushSerial()
{
  while (Serial.available())
    Serial.read();
}
