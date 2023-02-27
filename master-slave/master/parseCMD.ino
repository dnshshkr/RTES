void parseCMD()
{
  cmd = Serial.readStringUntil('\r\n');
  cmd.trim();
  char alph = cmd.charAt(0);
  String valStr = cmd.substring(1, cmd.length());
  switch (alph)
  {
    case'a': case'A':
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
    case'b': case'B':
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          break;
        }
        int val = valStr.toInt();
        if (val > 0 && val < 65536)
        {
          Serial2.write(0x86);
          while (!Serial2.available()) {}
          if (Serial2.read() == 0xf8) {}
          else
          {
            Serial.println("Failed to reset cycleCount");
          }
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
    case'c': case'C':
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
    case'd': case'D':
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
    case'e': case'E':
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          break;
        }
        int val = valStr.toInt();
        if (val > 0 && val < 65536)
        {
          solOnTime = val;
          params[4] = solOnTime;
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
    case'f': case'F':
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
    case'g': case'G':
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
    case'h': case'H':
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          break;
        }
        Serial2.write(0x86);
        while (!Serial2.available()) {}
        if (Serial2.read() == 0xf9)
          Serial.println("Counters have been reset");
        break;
      }
    case'r': case'R':
      {
        Serial.print("Are you sure you want to reset to factory settings? (Y/N)");
        bool wait = timeoutUI(10);
        char choice = Serial.read();
        if (choice == 'Y' || choice == 'y')
        {
          Serial2.write(0x81);
          while (!Serial2.available()) {}
          if (Serial2.read() == 0xfc)
          {
            Serial.print("\nRestored to factory settings");
            delay(1000);
            Serial.println();
          }
        }
        else
        {
          Serial.print("\nFactory reset aborted");
          delay(1000);
          Serial.println();
        }
        printSettings();
        break;
      }
    case's': case'S':
      {
        if (mode && changesMade)
        {
          Serial.println("Save changes? (Y/N)");
          bool wait = timeoutUI(5);
          char choice = Serial.read();
          if (choice == 'Y' || choice == 'y')
          {
            Serial2.write(0x82); Serial.println(params);
            while (!Serial2.available()) {}
            if (Serial2.read() == 0xf7)
            {
              Serial.println("Changes have been saved. Requesting new parameters to verify new changes");
              Serial2.write(0x80);
              while(!Serial2.available()){}
              parseSerial2();
            }
            else
              Serial.println("Failed to save changes");
            changesMade = false;
          }
          else
          {
            Serial.print("\nChanges discarded");
            delay(1000);
            Serial.println();
          }
        }
        Serial2.write(0x83);
        break;
      }
    case't': case'T':
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
