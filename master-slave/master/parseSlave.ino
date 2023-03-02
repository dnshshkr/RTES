void parseSlave()
{
  uint8_t respCode = slave.read();
  if (respCode == 0xfa)
  {
    mode = 0;
    stream = SPIFFS.open("/temp.txt", FILE_WRITE);
    printSettings();
    Serial.print("RTES mode entered");
    if (dieselMode)
      Serial.println(" (Diesel-only mode)");
    Serial.println();
  }
  else if (respCode == 0xfb)
  {
    if (firstData)
    {
      stream.close();
      Serial.println("Enter a name for the log file");
      while (!Serial.available()) {}
      String fileName = Serial.readStringUntil('\r\n');
      fileName.trim();
      fileName = '/' + fileName + ".txt";
      short len = fileName.length() + 1;
      char fileNameChar[len];
      fileName.toCharArray(fileNameChar, len);
      if (SPIFFS.rename("/temp.txt", fileNameChar))
        Serial.println("Log file saved successfully");
      else
        Serial.println("Failed to save log file");
      firstData = false;
    }
    else
    {
      stream.close();
      SPIFFS.remove("/temp.txt");
    }
    mode = 1;
    printSettings();
    Serial.println("Settings mode entered");
  }
  else if (respCode >= 0xfe)
  {
    String body = slave.readStringUntil('\r\n');
    body.trim();
    //    Serial.println("response code: " + String(respCode));
    //    Serial.println("body: " + body);
    if (respCode == 0xfe)
    {
      readings = JSON.parse(body);
      stream.println(body);
      if (!firstData)
        firstData = true;
      engOffPrevMillis = millis();
      displayData();
    }
    else if (respCode == 0xff)
    {
      //Serial.println(body);
      params = JSON.parse(body);
      assignParams();
    }
  }
}
