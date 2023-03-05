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
          if (val > 0 && val < 3.4028235E38)
          {
            params[5] = waterPercentage = val;
            calculate_f2wPulseRatio();
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
          int val = valStr.toInt();
          if (val > 0 && val < 65536)
          {
            slave.write(RESET_COUNTERS);
            while (!slave.available()) {}
            if (slave.read() == NEW_PARAMS_RECEIVED) {}
            else
              Serial.println("Failed to reset cycle count");
            params[0] = f2wPulseRatio = val;
            calculate_denominator();
            calculate_waterPercentage();
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
        else if (mode == 2)
          break;
        float val = valStr.toFloat();
        if (val >= 0 && val < 3.4028235E38)
        {
          params[2] = flowRateBias = val;
          calculate_f2wPulseRatio();
          calculate_denominator();
          calculate_waterPercentage();
          params[0] = f2wPulseRatio;
          params[5] = waterPercentage;
          changesMade = true;
          printSettings();
        }
        else
          Serial.println("Input is out of range");
        break;
      }
    case 'd': case 'D':
      {
        if (mode == 0)
          Serial.println("Press 's' to enter settings");
        else if (mode == 1)
        {
          float val = valStr.toFloat();
          if (val > 0 && val < 3.4028235E38)
          {
            params[3] = solShotBias = val;
            calculate_solOnTime();
            calculate_f2wPulseRatio();
            calculate_denominator();
            params[4] = solOnTime;
            params[0] = f2wPulseRatio;
            changesMade = true;
            printSettings();
          }
          else
            Serial.println("Input is out of range");
        }
        else
        {
          Serial.print("Are you sure you want to delete the file? (Y/N)");
          bool wait = timeoutUI(5);
          char choice = Serial.read();
          if (choice == 'Y' || choice == 'y')
          {
            Serial.println();
            int val = valStr.toInt();
            String fileName = JSON.stringify(fileConfig["contents"][val - 1]);
            fileName = fileName.substring(1, fileName.length() - 1);
            //            fileName = '/' + fileName.substring(1, fileName.length() - 1);
            //            Serial.println(fileName);
            //            short len = fileName.length() + 1;
            //            char fileNameChar[len];
            //            fileName.toCharArray(fileNameChar, len);
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
        break;
      }
    case 'e': case 'E':
      {
        if (mode == 0)
        {
          Serial.println("Press 's' to enter settings");
          break;
        }
        else if (mode == 2)
        {
          mode = 1;
          writeConfigFile(SPIFFS);
          printSettings();
          break;
        }
        int val = valStr.toInt();
        if (val > 0 && val < 65536)
        {
          params[4] = solOnTime = val;
          calculate_solShotBias();
          calculate_denominator();
          calculate_waterPercentage();
          calculate_f2wPulseRatio();
          params[3] = solShotBias;
          params[5] = waterPercentage;
          params[0] = f2wPulseRatio;
          changesMade = true;
          printSettings();
        }
        else
          Serial.println("Input is out of range");
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
        if (mode == 0) {}
        else if (mode == 1)
        {
          mode = 2;
          spiffsUI();
        }
        break;
      }
    case 'o': case'O':
      {
        if (mode != 2)
          break;
        short val = valStr.toInt();
        String fileName = JSON.stringify(fileConfig["contents"][val - 1]);
        fileName = fileName.substring(1, fileName.length() - 1);
        Serial.println(fileName);
        //        fileName = '/' + fileName.substring(1, fileName.length() - 1);
        //        short len = fileName.length() + 1;
        //        char fileNameChar[len];
        //        fileName.toCharArray(fileNameChar, len);
        readFile(SPIFFS, fileName.c_str());
        Serial.println();
        spiffsUI();
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
          Serial.println("Enter the file new name");
          while (!Serial.available()) {}
          String newName = Serial.readStringUntil('\r\n');
          newName.trim();
          short val = valStr.toInt();
          String oldName = JSON.stringify(fileConfig["contents"][val - 1]);
          oldName = oldName.substring(1, oldName.length() - 1);
          Serial.println(oldName);
          //          short lenOld = oldName.length() + 1;
          //          char oldNameChar[lenOld];
          //          oldName.toCharArray(oldNameChar, lenOld);
          newName = '/' + newName + ".txt";
          //          short lenNew = newName.length() + 1;
          //          char newNameChar[lenNew];
          //          newName.toCharArray(newNameChar, lenNew);
          renameFile(SPIFFS, oldName.c_str(), newName.c_str());
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
              prevParams[2] = flowRateBias;
              prevParams[3] = solShotBias;
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
        if (mode == 0 || mode == 1) {}
        else
        {
          short val = valStr.toInt();
          String fileName = JSON.stringify(fileConfig["contents"][val - 1]);
          fileName = fileName.substring(1, fileName.length() - 1);
          if (Firebase.ready())
          {
            Serial.println("\nUpload file: \"" + fileName + "\"");
            if (!Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID, fileName, mem_storage_type_flash, fileName.substring(1), "text/plain", fcsUploadCallback))
              Serial.println(fbdo.errorReason());
          }
          spiffsUI();
        }
        break;
      }
    default:
      Serial.println("Unknown command");
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
