void parseCMD()
{
  cmd = Serial.readStringUntil('\r\n');
  cmd.trim();
  char alph = cmd.charAt(0);
  String valStr = cmd.substring(1, cmd.length());
  switch (alph)
  {
    case 'a': case 'A':
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          break;
        }
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
        break;
      }
    case 'b': case 'B':
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          break;
        }
        int val = valStr.toInt();
        if (val > 0 && val < 65536)
        {
          slave.write(0x86);
          while (!slave.available()) {}
          if (slave.read() == 0xf8) {}
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
        break;
      }
    case 'c': case 'C':
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          break;
        }
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
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          break;
        }
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
        break;
      }
    case 'e': case 'E':
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
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
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          break;
        }
        int val = valStr.toInt();
        if (val > 0 && val < 256)
        {
          params[1] = engineOffTimeout = val;
          changesMade = true;
          printSettings();
        }
        else
          Serial.println("Input is out of range");
        break;
      }
    case 'g': case 'G':
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          break;
        }
        else if (!testMode)
        {
          Serial.println("Please turn on test mode first");
          break;
        }
        int val = valStr.toInt();
        if (val > 0 && val < 256)
        {
          params[6] = checkpointPeriod = val;
          changesMade = true;
          printSettings();
        }
        else
          Serial.println("Input is out of range");
        break;
      }
    case 'h': case 'H':
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          break;
        }
        slave.write(0x86);
        while (!slave.available()) {}
        if (slave.read() == 0xf9)
          Serial.println("Counters have been reset");
        break;
      }
    case 'r': case 'R':
      {
        Serial.print("Are you sure you want to reset to factory settings? (Y/N)");
        bool wait = timeoutUI(10);
        char choice = Serial.read();
        if (choice == 'Y' || choice == 'y')
        {
          slave.write(0x81);
          while (!slave.available()) {}
          if (slave.read() == 0xfc)
          {
            Serial.print("\nRestored to factory settings");
            delay(1000);
            Serial.println();
            slave.write(0x80);
            while (!slave.available()) {}
            parseSlave();
          }
        }
        else
        {
          Serial.print("\nFactory reset aborted");
          delay(1000);
          Serial.println();
          printSettings();
        }
        flushSerial();
        break;
      }
    case 's': case 'S':
      {
        if (mode && changesMade)
        {
          Serial.print("Save changes? (Y/N)");
          bool wait = timeoutUI(5);
          char choice = Serial.read();
          if (choice == 'Y' || choice == 'y')
          {
            slave.write(0x82), slave.println(params);
            while (!slave.available()) {}
            if (slave.read() == 0xf7)
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
              slave.write(0x80);
              while (!slave.available()) {}
              if (slave.read() == 0xff)
              {
                String body = slave.readStringUntil('\r\n');
                body.trim();
                params = JSON.parse(body);
                //Serial.print("prevParams: "), Serial.println(prevParams);
                //Serial.print("newParams: "), Serial.println(params);
                if (params[0] == prevParams[0]) {}
                else
                  goto verifyChangesFail;
                if (params[1] == prevParams[1]) {}
                else
                  goto verifyChangesFail;
                if (round2d((double)params[2]) == round2d((double)prevParams[2])) {}
                else
                  goto verifyChangesFail;
                if (round2d((double)params[3]) == round2d((double)prevParams[3])) {}
                else
                  goto verifyChangesFail;
                if (params[4] == prevParams[4]) {}
                else
                  goto verifyChangesFail;
                if (round2d((double)params[5]) == round2d((double)prevParams[5])) {}
                else
                  goto verifyChangesFail;
                if (params[6] == prevParams[6]) {}
                else
                  goto verifyChangesFail;
                if (params[7] == prevParams[7])
                {
                  Serial.println("Verification completed");
                  goto verifyChangesSuccess;
                }
                else {}
verifyChangesFail:
                Serial.println("Failed to verify changes, reverting to previous parameters");
                params = prevParams;
                assignParams();
                printSettings();
              }
            }
            else
              Serial.println("Failed to save changes");
verifyChangesSuccess:
            changesMade = false;
            printSettings();
          }
          else
          {
            Serial.print("\nChanges discarded");
            slave.write(0x80);
            while (!slave.available());
            parseSlave();
            delay(1000);
            changesMade = false;
            Serial.println();
          }
          flushSerial();
        }
        else
          slave.write(0x83);
        break;
      }
    case 't': case 'T':
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          break;
        }
        testMode = !testMode;
        params[7] = testMode;
        changesMade = true;
        printSettings();
        break;
      }
    default:
      Serial.println("Unknown command");
  }
}
