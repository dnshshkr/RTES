void parseSlave()
{
  uint8_t respCode = slave.read();
  switch (respCode)
  {
    case RTES_STARTED:
      {
        mode = 0;
        stream = SPIFFS.open("/stream.txt", FILE_WRITE);
        mainMenuUI();
        Serial.print("RTES mode entered");
        if (dieselMode)
          Serial.println(" (Diesel-only mode)");
        Serial.println();
        break;
      }
    case RTES_STOPPED:
      {
        if (firstData)
        {
          stream.close();
          do
          {
            Serial.println("Enter a name for the log file");
          } while (!renameFile(SPIFFS, "/stream.txt"));
          Serial.println("Log file saved successfully");
          firstData = false;
          flushSerial();
        }
        else
        {
          stream.close();
          SPIFFS.remove("/stream.txt");
        }
        mode = 1;
        mainMenuUI();
        Serial.println("Settings mode entered");
        break;
      }
    case COUNTERS_RESET:
      {
        Serial.println("Counters have been reset");
        break;
      }
    case PARAMS_RESET:
      {
        Serial.print("\nRestored to factory settings");
        delay(1000);
        Serial.println();
        break;
      }
    case ENGINE_OFF:
      {
        Serial.println("*ENGINE OFF*");
        break;
      }
    case READINGS: case PARAMS:
      {
        String body = slave.readStringUntil('\r\n');
        body.trim();
        //    Serial.println("response code: " + String(respCode));
        //    Serial.println("body: " + body);
        if (respCode == READINGS)
        {
          readings = JSON.parse(body);
          stream.println(body);
          if (!firstData)
            firstData = true;
          displayData();
        }
        else if (respCode == PARAMS)
        {
          //Serial.println(body);
          params = JSON.parse(body);
          assignParams();
        }
        break;
      }
  }
}
