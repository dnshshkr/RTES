
/**
   Created by K. Suwatchai (Mobizt)

   Email: k_suwatchai@hotmail.com

   Github: https://github.com/mobizt/FirebaseJson

   Copyright (c) 2023 mobizt

*/

#include <Arduino.h>
//#include <FirebaseESP32.h>
#include <WiFi.h>
#include <Arduino_JSON.h>

#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Mesh_Amin"
#define WIFI_PASSWORD "1234123456"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyCi1wz8zPrEiqk_pBtu8G5jSPFr98EIYkg"

/* 3. Define the RTDB URL */
#define DATABASE_URL "rtes-378707-default-rtdb.asia-southeast1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "danish44945@gmail.com"
#define USER_PASSWORD "1sampai8"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;
JSONVar params, change;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

void setup()
{

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  // Or use legacy authenticate method
  // config.database_url = DATABASE_URL;
  // config.signer.tokens.legacy_token = "<database secret>";

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);
}

void loop()
{
  // Flash string (PROGMEM and  (FPSTR), String C/C++ string, const char, char array, string literal are supported
  // in all Firebase and FirebaseJson functions, unless F() macro is not supported.

  //Firebase.ready() should be called repeatedly to handle authentication tasks.
  if (Firebase.ready() && Firebase.RTDB.getJSON(&fbdo, "/update") && (bool)JSON.parse(fbdo.to<FirebaseJson>().raw())["change"])
  {
    if (Firebase.RTDB.getJSON(&fbdo, "/params"))
    {
      params = JSON.parse(fbdo.to<FirebaseJson>().raw());
      Serial.println(params);
      FirebaseJson resp;
      resp.add("change", false);
      resp.add("last update", count);
      Firebase.RTDB.setJSON(&fbdo, "/update", &resp);
    }
    else
      Serial.println(fbdo.errorReason() + " Retrying...");
    count++;
  }

  //  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  //  {
  //    long prevmillis = 0;
  //    sendDataPrevMillis = millis();
  //
  //    FirebaseJson json;
  //    json.setDoubleDigits(3);
  //    json.add("value", count);
  //
  //    Serial.printf("Set json... % s\n", Firebase.RTDB.setJSON(&fbdo, " / test / json", &json) ? "ok" : fbdo.errorReason().c_str());
  //    prevmillis = millis();
  //    Serial.printf("Get json... % s, time taken: % d ms\n", Firebase.RTDB.getJSON(&fbdo, " / test / json") ? fbdo.to<FirebaseJson>().raw() : fbdo.errorReason().c_str(), millis() - prevmillis);
  //
  //    prevmillis = millis();
  //    FirebaseJson jVal;
  //    Serial.printf("Get json ref... % s, time takenL % d ms\n", Firebase.RTDB.getJSON(&fbdo, " / test / json", &jVal) ? jVal.raw() : fbdo.errorReason().c_str(), millis() - prevmillis);
  //
  //    FirebaseJsonArray arr;
  //    arr.setFloatDigits(2);
  //    arr.setDoubleDigits(4);
  //    arr.add("a", "b", "c", true, 45, (float)6.1432, 123.45692789);
  //
  //    Serial.printf("Set array... % s\n", Firebase.RTDB.setArray(&fbdo, " / test / array", &arr) ? "ok" : fbdo.errorReason().c_str());
  //
  //    Serial.printf("Get array... % s\n", Firebase.RTDB.getArray(&fbdo, " / test / array") ? fbdo.to<FirebaseJsonArray>().raw() : fbdo.errorReason().c_str());
  //
  //    Serial.printf("Push json... % s\n", Firebase.RTDB.pushJSON(&fbdo, " / test / push", &json) ? "ok" : fbdo.errorReason().c_str());
  //
  //    json.set("value", count + 0.29745);
  //    Serial.printf("Update json... % s\n\n", Firebase.RTDB.updateNode(&fbdo, " / test / push / " + fbdo.pushName(), &json) ? "ok" : fbdo.errorReason().c_str());
  //
  //    count++;
  //  }
}
