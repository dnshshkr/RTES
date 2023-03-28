<<<<<<< HEAD
/**
   Created by K. Suwatchai (Mobizt)

   Email: k_suwatchai@hotmail.com

   Github: https://github.com/mobizt/Firebase-ESP-Client

   Copyright (c) 2023 mobizt

*/

// This example shows how to upload file to Firebase storage bucket using the Firebase Storage API.
// The file media.mp4 in the data folder should be uploaded to the device flash memory before test.

// If SD Card used for storage, assign SD card type and FS used in src/FirebaseFS.h and
// change the config for that card interfaces in src/addons/SDHelper.h

#include <Arduino.h>
#include "FS.h"
#include <WiFi.h>

#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the SD card interfaces setting and mounting
#include <addons/SDHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Mesh_Amin"
#define WIFI_PASSWORD "1234123456"

/* 2. Define the API Key */
#define API_KEY "AIzaSyCi1wz8zPrEiqk_pBtu8G5jSPFr98EIYkg"

/* 3. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "danish44945@gmail.com"
#define USER_PASSWORD "butokimak"

/* 4. Define the Firebase storage bucket ID e.g bucket-name.appspot.com */
#define STORAGE_BUCKET_ID "rtes-378707.appspot.com"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool taskCompleted = false;

void setup()
{

  Serial.begin(115200);


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
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

  // The WiFi credentials are required for Pico W
  // due to it does not have reconnect feature.

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  /* Assign upload buffer size in byte */
  // Data to be uploaded will send as multiple chunks with this size, to compromise between speed and memory used for buffering.
  // The memory from external SRAM/PSRAM will not use in the TCP client internal tx buffer.
  config.fcs.upload_buffer_size = 512;

  // if use SD card, mount it.
  //SD_Card_Mounting(); // See src/addons/SDHelper.h
}

// The Firebase Storage upload callback function
void fcsUploadCallback(FCS_UploadStatusInfo info)
{
  if (info.status == fb_esp_fcs_upload_status_init)
  {
    Serial.printf("Uploading file %s (%d) to %s\n", info.localFileName.c_str(), info.fileSize, info.remoteFileName.c_str());
  }
  else if (info.status == fb_esp_fcs_upload_status_upload)
  {
    Serial.printf("Uploaded %d%s, Elapsed time %d ms\n", (int)info.progress, "%", info.elapsedTime);
  }
  else if (info.status == fb_esp_fcs_upload_status_complete)
  {
    Serial.println("Upload completed\n");
    FileMetaInfo meta = fbdo.metaData();
    Serial.printf("Name: %s\n", meta.name.c_str());
    Serial.printf("Bucket: %s\n", meta.bucket.c_str());
    Serial.printf("contentType: %s\n", meta.contentType.c_str());
    Serial.printf("Size: %d\n", meta.size);
    Serial.printf("Generation: %lu\n", meta.generation);
    Serial.printf("Metageneration: %lu\n", meta.metageneration);
    Serial.printf("ETag: %s\n", meta.etag.c_str());
    Serial.printf("CRC32: %s\n", meta.crc32.c_str());
    Serial.printf("Tokens: %s\n", meta.downloadTokens.c_str());
    Serial.printf("Download URL: %s\n\n", fbdo.downloadURL().c_str());
  }
  else if (info.status == fb_esp_fcs_upload_status_error)
  {
    Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
  }
}

void loop()
{

  // Firebase.ready() should be called repeatedly to handle authentication tasks.

  if (Firebase.ready() && !taskCompleted)
  {
    taskCompleted = true;
    listDir(SPIFFS, "/", 0);
    String fileName = "/run1.txt";
    File file = DEFAULT_FLASH_FS.open("/run1.txt", FILE_READ);
    if (!file)
      Serial.println("cant read");
    else
    {
      while (file.available())
        Serial.write(file.read());
    }
    Serial.println("\nUpload file...\n");

    // MIME type should be valid to avoid the download problem.
    // The file systems for flash and SD/SDMMC can be changed in FirebaseFS.h.
    if (!Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */, "/run1.txt" /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, "run1.txt" /* path of remote stored in the bucket */, "text/plain" /* mime type */, fcsUploadCallback /* callback function */))
      Serial.println(fbdo.errorReason());
  }
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  byte index = 0;
  Serial.printf("Storage list: %s\r\n", dirname);
  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }
  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
        listDir(fs, file.name(), levels - 1);
    }
    else if (file.name())
    {
      String fileName = file.name();
      Serial.println(String(index + 1) + ". " + fileName + "\tSIZE: " + String(file.size()));
    }
    file = root.openNextFile();
  }
}
=======
/**
   Created by K. Suwatchai (Mobizt)

   Email: k_suwatchai@hotmail.com

   Github: https://github.com/mobizt/Firebase-ESP-Client

   Copyright (c) 2023 mobizt

*/

// This example shows how to upload file to Firebase storage bucket using the Firebase Storage API.
// The file media.mp4 in the data folder should be uploaded to the device flash memory before test.

// If SD Card used for storage, assign SD card type and FS used in src/FirebaseFS.h and
// change the config for that card interfaces in src/addons/SDHelper.h

#include <Arduino.h>
#include "FS.h"
#include <WiFi.h>

#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the SD card interfaces setting and mounting
#include <addons/SDHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Mesh_Amin"
#define WIFI_PASSWORD "1234123456"

/* 2. Define the API Key */
#define API_KEY "AIzaSyCi1wz8zPrEiqk_pBtu8G5jSPFr98EIYkg"

/* 3. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "danish44945@gmail.com"
#define USER_PASSWORD "butokimak"

/* 4. Define the Firebase storage bucket ID e.g bucket-name.appspot.com */
#define STORAGE_BUCKET_ID "rtes-378707.appspot.com"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool taskCompleted = false;

void setup()
{

  Serial.begin(115200);


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
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

  // The WiFi credentials are required for Pico W
  // due to it does not have reconnect feature.

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  /* Assign upload buffer size in byte */
  // Data to be uploaded will send as multiple chunks with this size, to compromise between speed and memory used for buffering.
  // The memory from external SRAM/PSRAM will not use in the TCP client internal tx buffer.
  config.fcs.upload_buffer_size = 512;

  // if use SD card, mount it.
  //SD_Card_Mounting(); // See src/addons/SDHelper.h
}

// The Firebase Storage upload callback function
void fcsUploadCallback(FCS_UploadStatusInfo info)
{
  if (info.status == fb_esp_fcs_upload_status_init)
  {
    Serial.printf("Uploading file %s (%d) to %s\n", info.localFileName.c_str(), info.fileSize, info.remoteFileName.c_str());
  }
  else if (info.status == fb_esp_fcs_upload_status_upload)
  {
    Serial.printf("Uploaded %d%s, Elapsed time %d ms\n", (int)info.progress, "%", info.elapsedTime);
  }
  else if (info.status == fb_esp_fcs_upload_status_complete)
  {
    Serial.println("Upload completed\n");
    FileMetaInfo meta = fbdo.metaData();
    Serial.printf("Name: %s\n", meta.name.c_str());
    Serial.printf("Bucket: %s\n", meta.bucket.c_str());
    Serial.printf("contentType: %s\n", meta.contentType.c_str());
    Serial.printf("Size: %d\n", meta.size);
    Serial.printf("Generation: %lu\n", meta.generation);
    Serial.printf("Metageneration: %lu\n", meta.metageneration);
    Serial.printf("ETag: %s\n", meta.etag.c_str());
    Serial.printf("CRC32: %s\n", meta.crc32.c_str());
    Serial.printf("Tokens: %s\n", meta.downloadTokens.c_str());
    Serial.printf("Download URL: %s\n\n", fbdo.downloadURL().c_str());
  }
  else if (info.status == fb_esp_fcs_upload_status_error)
  {
    Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
  }
}

void loop()
{

  // Firebase.ready() should be called repeatedly to handle authentication tasks.

  if (Firebase.ready() && !taskCompleted)
  {
    taskCompleted = true;
    listDir(SPIFFS, "/", 0);
    String fileName = "/run1.txt";
    File file = DEFAULT_FLASH_FS.open("/run1.txt", FILE_READ);
    if (!file)
      Serial.println("cant read");
    else
    {
      while (file.available())
        Serial.write(file.read());
    }
    Serial.println("\nUpload file...\n");

    // MIME type should be valid to avoid the download problem.
    // The file systems for flash and SD/SDMMC can be changed in FirebaseFS.h.
    if (!Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */, "/run1.txt" /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, "run1.txt" /* path of remote stored in the bucket */, "text/plain" /* mime type */, fcsUploadCallback /* callback function */))
      Serial.println(fbdo.errorReason());
  }
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  byte index = 0;
  Serial.printf("Storage list: %s\r\n", dirname);
  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }
  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
        listDir(fs, file.name(), levels - 1);
    }
    else if (file.name())
    {
      String fileName = file.name();
      Serial.println(String(index + 1) + ". " + fileName + "\tSIZE: " + String(file.size()));
    }
    file = root.openNextFile();
  }
}
>>>>>>> origin/master
