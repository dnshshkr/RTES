void parseCMD()
{
  cmd = Serial.readStringUntil('\r\n');
  cmd.trim();
  char alph = cmd.charAt(0);
  String valStr = cmd.substring(1, cmd.length());
  switch (alph)
  {
    case '$':
      {
        if (mode == 1)
        {
          slave.write(REQUEST_PARAMS);
          while (!slave.available()) {}
          parseSlave();
          printSettings();
        }
        break;
      }
    case 'a': case 'A':
      {
        if (mode == 0)
          Serial.println("Press 's' to enter settings");
        else if (mode == 1)
        {
          float val = valStr.toFloat();
          int32_t result = calculate_f2wPulseRatio(val, fuelPulseBias, waterPulseBias);
          if (result > 0 && result < 65536)
          {
            params[5] = waterPercentage = val;
            f2wPulseRatio = result;
            params[0] = f2wPulseRatio;
            changesMade = true;
            printSettings();
          }
          else
            Serial.println("Input is out of range");
        }
        break;
      }
    case 'b': case 'B':
      {
        if (mode == 0)
          Serial.println("Press 's' to enter settings");
        else if (mode == 1)
        {
          int32_t val = valStr.toInt();
          float result1 = calculate_denominator(val, fuelPulseBias, waterPulseBias);
          int result2 = calculate_waterPercentage(waterPulseBias, result1);
          if (val > 0 && val < 65536 && result2 > 0.0 && result2 <= 100.0)
          {
            slave.write(RESET_CYCLE_COUNT);
            while (!slave.available()) {}
            if (slave.read() == CYCLE_COUNT_RESET) {}
            else
              Serial.println("Failed to reset cycle count");
            params[0] = f2wPulseRatio = val;
            waterPercentage = result2;
            params[5] = waterPercentage;
            changesMade = true;
            printSettings();
          }
          else
            Serial.println("Input is out of range");
        }
        break;
      }
    case 'c': case 'C':
      {
        if (mode == 0)
        {
          Serial.println("Press 's' to enter settings");
          break;
        }
        else if (mode == 1)
        {
          float val = valStr.toFloat();
          int result1 = calculate_f2wPulseRatio(waterPercentage, val, waterPulseBias);
          float result2 = calculate_denominator(f2wPulseRatio, val, waterPulseBias);
          int result3 = calculate_waterPercentage(waterPulseBias, result2);
          if (val >= 0 && val < 3.4028235E38 && result1 > 0 && result1 < 65536 && result3 > 0.0 && result3 <= 100.0)
          {
            params[2] = fuelPulseBias = val;
            f2wPulseRatio = result1;
            waterPercentage = result2;
            params[0] = f2wPulseRatio;
            params[5] = waterPercentage;
            changesMade = true;
            printSettings();
          }
          else
            Serial.println("Input is out of range");
        }
        break;
      }
    case 'd': case 'D':
      {
        if (mode == 0)
          Serial.println("Press 's' to enter settings");
        else if (mode == 1)
        {
          float val = valStr.toFloat();
          int32_t result1 = calculate_solOnTime(val);
          int32_t result2 = calculate_f2wPulseRatio(waterPercentage, fuelPulseBias, val);
          if (val > 0 && val < 3.4028235E38 && result1 > 0 && result1 <= SOL_ON_TIME_LIMIT && result2 > 0)
          {
            params[3] = waterPulseBias = val;
            solOnTime = result1;
            f2wPulseRatio = result2;
            denominator = calculate_denominator(f2wPulseRatio, fuelPulseBias, waterPulseBias);
            params[4] = solOnTime;
            params[0] = f2wPulseRatio;
            changesMade = true;
            printSettings();
          }
          else
            Serial.println("Input is out of range");
        }
        else if (mode == 2)
        {
          Serial.print("Are you sure you want to delete the file? (Y/N)");
          bool wait = timeoutUI(5);
          char choice = Serial.read();
          if (choice == 'Y' || choice == 'y')
          {
            Serial.println();
            int val = valStr.toInt();
            String fileName = JSON.stringify(localFileConfig["contents"][val - 1]);
            fileName = '/' + fileName.substring(1, fileName.length() - 1);
            deleteFile(SPIFFS, fileName.c_str());
          }
          else
          {
            Serial.print("\nFile deletion aborted");
            delay(1000);
            Serial.println();
          }
          flushSerial();
          spiffsUI();
        }
        else //mode=3
        {
          size_t val = valStr.toInt();
          if (val <= remoteFiles->items.size())
          {
            Serial.print("Are you sure you want to delete this remote file? (Y/N)");
            bool wait = timeoutUI(5);
            char choice = Serial.read();
            if (choice == 'Y' || choice == 'y')
              Serial.printf("Delete file... %s\n", Firebase.Storage.deleteFile(&fbdo, STORAGE_BUCKET_ID, String(remoteFiles->items[val - 1].name.c_str())) ? "ok" : fbdo.errorReason().c_str());
            else
            {
              Serial.print("\Remote file deletion aborted");
              delay(1000);
              Serial.println();
            }
            flushSerial();
            firebaseUI();
          }
        }
        break;
      }
    case 'e': case 'E':
      {
        if (mode == 0)
          Serial.println("Press 's' to enter settings");
        else if (mode == 1)
        {
          int32_t val = valStr.toInt();
          float result1 = calculate_waterPulseBias(val);
          int32_t result2 = calculate_f2wPulseRatio(waterPercentage, fuelPulseBias, result1);
          if (val > 0 && val <= SOL_ON_TIME_LIMIT && result1 > 0.0 && result2 > 0)
          {
            params[4] = solOnTime = val;
            waterPulseBias = result1;
            f2wPulseRatio = result2;
            denominator = calculate_denominator(f2wPulseRatio, fuelPulseBias, waterPulseBias);
            params[3] = waterPulseBias;
            params[0] = f2wPulseRatio;
            changesMade = true;
            printSettings();
          }
          else
            Serial.println("Input is out of range");
        }
        else if (mode == 2)
        {
          mode = 1;
          writeConfigFile(SPIFFS);
          printSettings();
        }
        else
        {
          mode = 1;
          fbdo.fileList()->items.clear();
          printSettings();
        }
        break;
      }
    case 'f': case 'F':
      {
        if (mode == 0)
          Serial.println("Press 's' to enter settings");
        else if (mode == 1)
        {
          int val = valStr.toInt();
          if (val > 0 && val < 256)
          {
            params[1] = engineOffTimeout = val;
            changesMade = true;
            printSettings();
          }
          else
            Serial.println("Input is out of range");
        }
        break;
      }
    case 'g': case 'G':
      {
        if (mode == 0)
          Serial.println("Press 's' to enter settings");
        else if (mode == 1 && testMode)
        {
          int val = valStr.toInt();
          if (val > 0 && val < 256)
          {
            params[6] = checkpointPeriod = val;
            changesMade = true;
            printSettings();
          }
          else
            Serial.println("Input is out of range");
        }
        break;
      }
    case 'h': case 'H':
      {
        if (mode == 0)
          Serial.println("Press 's' to enter settings");
        else if (mode == 1)
          slave.write(RESET_COUNTERS);
        break;
      }
    case 'i': case 'I':
      {
        if (mode == 0)
          Serial.println("Press 's' to enter settings");
        else if (mode == 1)
        {
          dieselMode = !dieselMode;
          params[8] = dieselMode;
          changesMade = true;
          printSettings();
        }
        break;
      }
    case 'j': case 'J':
      {
        if (mode == 1)
        {
          mode = 2;
          spiffsUI();
        }
        break;
      }
    case 'k': case 'K':
      {
        if (mode == 1)
        {
          mode = 3;
          firebaseUI();
        }
        break;
      }
    case 'o': case 'O':
      {
        if (mode == 2)
        {
          short val = valStr.toInt();
          String fileName = JSON.stringify(localFileConfig["contents"][val - 1]);
          fileName = '/' + fileName.substring(1, fileName.length() - 1);
          readFile(SPIFFS, fileName.c_str());
          Serial.println();
          spiffsUI();
        }
        break;
      }
    case 'r': case 'R':
      {
        if (mode == 0)
          Serial.println("Press 's' to enter settings");
        else if (mode == 1)
        {
          Serial.print("Are you sure you want to reset to factory settings? (Y/N)");
          bool wait = timeoutUI(10);
          char choice = Serial.read();
          if (choice == 'Y' || choice == 'y')
          {
            slave.write(RESET_PARAMS);
            while (!slave.available()) {}
            parseSlave();
            slave.write(REQUEST_PARAMS);
            while (!slave.available()) {}
            parseSlave();
            printSettings();
          }
          else
          {
            Serial.print("\nFactory reset aborted");
            delay(1000);
            Serial.println();
            printSettings();
          }
          flushSerial();
        }
        else
        {
          short val = valStr.toInt();
          String oldPath = JSON.stringify(localFileConfig["contents"][val - 1]);
          oldPath = '/' + oldPath.substring(1, oldPath.length() - 1);
          do
          {
            Serial.println("Enter the file new name");
          } while (!renameFile(SPIFFS, oldPath.c_str()));
          //          while (!Serial.available()) {}
          //          String newName = Serial.readStringUntil('\r\n');
          //          newName.trim();
          //          short val = valStr.toInt();
          //          String oldName = JSON.stringify(localFileConfig["contents"][val - 1]);
          //          oldName = '/' + oldName.substring(1, oldName.length() - 1);
          //          Serial.println(oldName);
          //          newName = '/' + newName + ".txt";
          //          renameFile(SPIFFS, oldName.c_str(), newName.c_str());
          spiffsUI();
        }
        break;
      }
    case 's': case 'S':
      {
        if (mode == 0 || (mode == 1 && !changesMade))
          slave.write(TOGGLE_RTES);
        else if (mode == 1 && changesMade)
        {
          Serial.print("Save changes? (Y/N)");
          bool wait = timeoutUI(5);
          char choice = Serial.read();
          if (choice == 'Y' || choice == 'y')
          {
            slave.write(SEND_NEW_PARAMS), slave.println(params);
            while (!slave.available()) {}
            if (slave.read() == NEW_PARAMS_RECEIVED)
            {
              JSONVar prevParams;
              prevParams[0] = f2wPulseRatio;
              prevParams[1] = engineOffTimeout;
              prevParams[2] = fuelPulseBias;
              prevParams[3] = waterPulseBias;
              prevParams[4] = solOnTime;
              prevParams[5] = waterPercentage;
              prevParams[6] = checkpointPeriod;
              prevParams[7] = testMode;
              Serial.println("\nChanges have been saved. Requesting new parameters to verify changes");
              bool verify = verifyParams(prevParams);
              if (!verify)
              {
                Serial.println("Failed to verify changes, reverting to previous parameters");
                params = prevParams;
                assignParams();
                changesMade = false;
              }
              else
                changesMade = false;
            }
          }
          else
          {
            Serial.print("\nChanges discarded");
            slave.write(REQUEST_PARAMS);
            delay(1000);
            changesMade = false;
            Serial.println();
          }
          flushSerial();
          printSettings();
        }
        break;
      }
    case 't': case 'T':
      {
        if (mode == 0)
          Serial.println("Press 's' to enter settings");
        else if (mode == 1)
        {
          testMode = !testMode;
          params[7] = testMode;
          changesMade = true;
          printSettings();
        }
        break;
      }
    case 'u': case 'U':
      {
        short val = valStr.toInt();
        if (mode == 2)
        {
          String fileName = JSON.stringify(localFileConfig["contents"][val - 1]);
          fileName = '/' + fileName.substring(1, fileName.length() - 1);
          if (Firebase.ready())
          {
            Serial.println("\nUpload file: \"" + fileName + "\"");
            if (!Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID, fileName, mem_storage_type_flash, fileName.substring(1), "text/plain", fcsUploadCallback))
              Serial.println(fbdo.errorReason());
          }
          spiffsUI();
        }
        else if (mode == 3)
        {
          String fileName = String(remoteFiles->items[val].name.c_str());
          if (Firebase.ready())
          {
            Serial.println("\nDownload file...\n");
            if (!Firebase.Storage.download(&fbdo, STORAGE_BUCKET_ID, fileName, '/' + fileName, mem_storage_type_flash, fcsDownloadCallback))
              Serial.println(fbdo.errorReason());
          }
          firebaseUI();
        }
        break;
      }
    default:
      {
        if (mode == 0)
          Serial.println("Press 's' to enter settings");
        else
          Serial.println("Unknown command");
      }
  }
}
bool verifyParams(JSONVar prevParams)
{
  slave.write(REQUEST_PARAMS);
  while (!slave.available()) {}
  if (slave.read() == PARAMS)
  {
    String body = slave.readStringUntil('\r\n');
    body.trim();
    params = JSON.parse(body);
    //Serial.print("prevParams: "), Serial.println(prevParams);
    //Serial.print("newParams: "), Serial.println(params);
    if (params[0] == prevParams[0]) {}
    else
      return false;
    if (params[1] == prevParams[1]) {}
    else
      return false;
    if (round2d((double)params[2]) == round2d((double)prevParams[2])) {}
    else
      return false;
    if (round2d((double)params[3]) == round2d((double)prevParams[3])) {}
    else
      return false;
    if (params[4] == prevParams[4]) {}
    else
      return false;
    if (round2d((double)params[5]) == round2d((double)prevParams[5])) {}
    else
      return false;
    if (params[6] == prevParams[6]) {}
    else
      return false;
    if (params[7] == prevParams[7])
    {
      Serial.println("verification completed");
      return true;
    }
    else
      return true;
  }
}
