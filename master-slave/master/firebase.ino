void updateFirebase()
{
  //!Firebase.RTDB.getJSON(&fbdo, "/params/update") ? Serial.println(fbdo.errorReason()) : Serial.print("");
  if (mode == 0 && Firebase.RTDB.getJSON(&fbdo, "/params/update") && (bool)JSON.parse(fbdo.to<FirebaseJson>().raw())["change"])
  {
    Serial.println("Wireless parameter update received. Assigning new parameters");
    if (Firebase.RTDB.getJSON(&fbdo, "/params"))
    {
      params = JSON.parse(fbdo.to<FirebaseJson>().raw());
      FirebaseJson resp;
      resp.add("change", false);
      Firebase.RTDB.setJSON(&fbdo, "/params/update", &resp);
      assignParamsFb();
      mainMenuUI();
    }
    else
      Serial.println(fbdo.errorReason() + " Retrying...");
  }
  else if (Firebase.RTDB.getJSON(&fbdo, "/commands/responded") && !(bool)JSON.parse(fbdo.to<FirebaseJson>().raw())["change"])
  {
    Serial.println("Firebase command received");
    Firebase.RTDB.getJSON(&fbdo, "/commands");
    JSONVar cmds = JSON.parse(fbdo.to<FirebaseJson>().raw());
    Serial.println(cmds);
    switch ((bool)cmds["RTES mode"])
    {
      case true:
        {
          slave.write(EXCLUSIVE_START_RTES);
          parseSlave();
          break;
        }
      case false:
        {
          slave.write(EXCLUSIVE_STOP_RTES);
          parseSlave();
          break;
        }
    }
    FirebaseJson states, resp;
    states.add("RTES mode", mode);
    resp.add("change", true);
    Firebase.RTDB.setJSON(&fbdo, "/states", &states);
    Firebase.RTDB.setJSON(&fbdo, "/commands/responded", &resp);
  }
}

void listRemoteFiles()
{
  if (Firebase.ready())
  {
    Serial.printf("List files... % s\n", Firebase.Storage.listFiles(&fbdo, STORAGE_BUCKET_ID) ? "ok" : fbdo.errorReason().c_str());

    if (fbdo.httpCode() == FIREBASE_ERROR_HTTP_CODE_OK)
    {
      remoteFiles = fbdo.fileList();
      for (size_t i = 0; i < remoteFiles->items.size(); i++)
        Serial.printf(" % d. % s, bucket: % s\n", i + 1, remoteFiles->items[i].name.c_str(), remoteFiles->items[i].bucket.c_str());
    }

    // To clear file list
    //fbdo.fileList()->items.clear();
  }
}

void fcsUploadCallback(FCS_UploadStatusInfo info)
{
  if (info.status == fb_esp_fcs_upload_status_init)
    Serial.printf("Uploading file % s ( % d) to % s\n", info.localFileName.c_str(), info.fileSize, info.remoteFileName.c_str());
  else if (info.status == fb_esp_fcs_upload_status_upload)
    Serial.printf("Uploaded % d % s, Elapsed time % d ms\n", (int)info.progress, " % ", info.elapsedTime);
  else if (info.status == fb_esp_fcs_upload_status_complete)
  {
    Serial.println("Upload completed\n");
    FileMetaInfo meta = fbdo.metaData();
    Serial.printf("Name: % s\n", meta.name.c_str());
    Serial.printf("Bucket: % s\n", meta.bucket.c_str());
    Serial.printf("contentType: % s\n", meta.contentType.c_str());
    Serial.printf("Size: % d\n", meta.size);
    Serial.printf("Generation: % lu\n", meta.generation);
    Serial.printf("Metageneration: % lu\n", meta.metageneration);
    Serial.printf("ETag: % s\n", meta.etag.c_str());
    Serial.printf("CRC32: % s\n", meta.crc32.c_str());
    Serial.printf("Tokens: % s\n", meta.downloadTokens.c_str());
    Serial.printf("Download URL: % s\n\n", fbdo.downloadURL().c_str());
  }
  else if (info.status == fb_esp_fcs_upload_status_error)
    Serial.printf("Upload failed, % s\n", info.errorMsg.c_str());
}

void fcsDownloadCallback(FCS_DownloadStatusInfo info)
{
  if (info.status == fb_esp_fcs_download_status_init)
    Serial.printf("Downloading file % s ( % d) to % s\n", info.remoteFileName.c_str(), info.fileSize, info.localFileName.c_str());
  else if (info.status == fb_esp_fcs_download_status_download)
    Serial.printf("Downloaded % d % s, Elapsed time % d ms\n", (int)info.progress, " % ", info.elapsedTime);
  else if (info.status == fb_esp_fcs_download_status_complete)
    Serial.println("Download completed\n");
  else if (info.status == fb_esp_fcs_download_status_error)
    Serial.printf("Download failed, % s\n", info.errorMsg.c_str());
}
