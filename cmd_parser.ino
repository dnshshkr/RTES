void cmdParser()
{
  char alph = cmd.charAt(0);
  String valStr = cmd.substring(1, cmd.length());
  switch (alph)
  {
    case 'S': case 's':
      {
        digitalWrite(solenoidWater, LOW);
        digitalWrite(motorWater, LOW);
        if (adminMode)
        {
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          adminSettings();
          break;
        }
        settingMode = !settingMode;
        printSettings();
        if (settingMode)
        {
          Serial.println("Setting mode entered");
          bt.println("Setting mode entered");
        }
        else
        {
          Serial.println("RTES mode entered");
          bt.println("RTES mode entered");
        }
        break;
      }
    case '$':
      {
        if (adminMode)
        {
          adminSettings();
          break;
        }
        if (settingMode)
          printSettings();
        break;
      }
    case 'A': case 'a':
      {
        if (!settingMode || adminMode)
        {
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          break;
        }
        float val = valStr.toFloat();
        if (val > 0)
        {
          quickWaterPercentage = val;
          EEPROM.put(addr7, quickWaterPercentage);
          calculatePulse_fuelToWaterRatio();
          calculateDenom();
          printSettings();
        }
        else
        {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'B': case 'b':
      {
        if (!settingMode || adminMode)
        {
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          break;
        }
        int val = valStr.toInt();
        if (val > 0)
        {
          pulse_fuelToWaterRatioCount = 1;
          pulse_fuelToWaterRatio = val;
          EEPROM.put(addr1, pulse_fuelToWaterRatio);
          calculateDenom();
          printSettings();
        }
        else
        {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'C': case 'c':
      {
        if (!settingMode || adminMode)
        {
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          break;
        }
        float val = valStr.toFloat();
        if (val >= 0)
        {
          flowRateBias = val;
          EEPROM.put(addr3, flowRateBias);
          calculatePulse_fuelToWaterRatio();
          calculateDenom();
          printSettings();
        }
        else
        {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'D': case 'd':
      {
        if (!settingMode || adminMode)
        {
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          break;
        }
        float val = valStr.toFloat();
        if (val > 0)
        {
          solShotBias = val;
          EEPROM.put(addr4, solShotBias);
          calculateSolenoidOnTime();
          calculatePulse_fuelToWaterRatio();
          calculateDenom();
          printSettings();
        }
        else
        {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'E': case 'e':
      {
        if (!settingMode || adminMode)
        {
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          break;
        }
        int val = valStr.toInt();
        if (val > 0)
        {
          solenoidOnTime = val;
          EEPROM.put(addr5, solenoidOnTime);
          calculatePulse_fuelToWaterRatio();
          printSettings();
        }
        else
        {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'F': case 'f':
      {
        if (!settingMode || adminMode)
        {
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          break;
        }
        int val = valStr.toInt();
        if (val > 0)
        {
          engineOffTimeOut = val;
          EEPROM.put(addr2, engineOffTimeOut);
          printSettings();
        }
        else
        {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'G': case 'g':
      {
        totalFuelPulse = 0;
        totalWaterPulse = 0;
        pulse_fuelToWaterRatioCount = 0;
        Serial.println("Counter has been reset");
        bt.println("Counter has been reset");
        delay(1000);
        break;
      }
    case 'M': case 'm': //admin mode
      {
        if (!settingMode || adminMode)
        {
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          break;
        }
        //Serial.println(pwd_default);
        Serial.print("Enter admin password");
        bt.print("Enter admin password");
        bool wait = timeoutUI();
        uint32_t pwd = Serial.parseInt();
        if (digitalRead(btState))
          pwd = bt.parseInt();
        flushSerial();
        if (!wait)
        {
          printSettings();
          break;
        }
        else if (wait && pwd != pwd_default)
        {
          Serial.println("Password incorrect");
          bt.println("Password incorrect");
          delay(1000);
          adminMode = false;
          printSettings();
          break;
        }
        else
          adminMode = true;
        EEPROM.put(addr6, adminMode);
        adminSettings();
        break;
      }
    case 'R': case 'r':
      {
        if (!adminMode)
        {
          Serial.println("Command unrecognized");
          bt.println("Command unrecognized");
          break;
        }
        Serial.print("Are you sure you want to reset to factory settings? (Y/N)");
        bt.print("Are you sure you want to reset to factory settings? (Y/N)") ;
        bool wait = timeoutUI();
        char choice = Serial.read();
        if (digitalRead(btState) && bt.available())
          choice = bt.read();
        if (choice == 'Y' || choice == 'y')
          factoryReset();
        else
        {
          Serial.println("Factory reset aborted");
          bt.println("Factory reset aborted");
          delay(1000);
        }
        flushSerial();
        adminSettings();
        break;
      }
    case 'T': case 't':
      {
        if (!adminMode)
        {
          Serial.println("Command unrecognized");
          bt.println("Command unrecognized");
          break;
        }
        uint8_t val = valStr.toInt();
        if (val == 1) //on off solenoid
        {
          solenoidManualState = !solenoidManualState;
          digitalWrite(solenoidWater, solenoidManualState);
          adminSettings();
        }
        else if (val == 2) //on off water pump
        {
          waterPumpManualState = !waterPumpManualState;
          digitalWrite(motorWater, waterPumpManualState);
          adminSettings();
        }
        else if (val == 3)
        {
          solenoidManualState = HIGH;
          waterPumpManualState = HIGH;
          digitalWrite(solenoidWater, solenoidManualState);
          digitalWrite(motorWater, waterPumpManualState);
          toggleAllState = true;
          adminSettings();
        }
        else if (val == 4)
        {
          solenoidManualState = LOW;
          waterPumpManualState = LOW;
          digitalWrite(solenoidWater, solenoidManualState);
          digitalWrite(motorWater, waterPumpManualState);
          toggleAllState = false;
          adminSettings();
        }
        else if (val == 5) //OFF ALL
        {
          manualPrintData = !manualPrintData;
          if (!manualPrintData)
            adminSettings();
        }
        else if (val == 6)
        {
          Serial.println("Enter current admin password");
          bt.println("Enter current admin password");
          while (!Serial.available() && !bt.available()) {}
          uint32_t pwd = Serial.parseInt();
          if (digitalRead(btState))
            pwd = bt.parseInt();
          flushSerial();
          if (pwd != pwd_default)
          {
            Serial.println("Password incorrect");
            bt.println("Password incorrect");
            delay(1000);
            adminSettings();
          }
          else
          {
newpassword:
            Serial.println("Enter a new password (6 digits)");
            bt.println("Enter a new password (6 digits)");
            flushSerial();
            while (!Serial.available() && !bt.available()) {}
            String pwdStr = Serial.readStringUntil('\r\n');
            pwdStr.trim();
            if (pwdStr.length() < 6)
            {
              Serial.println("Invalid password length (must be 6 digits)");
              bt.println("Invalid password length (must be 6 digits)");
              delay(1000);
              goto newpassword;
            }
            EEPROM.update(addr7, pwdStr.toInt());
            EEPROM.get(addr7, pwd_default);
            Serial.println("Your new password has been set");
            bt.println("Your new password has been set");
            delay(1000);
            adminSettings();
          }
        }
        else if (val == 7)
        {
          adminMode = 0;
          printSettings();
        }
        break;
      }
    default:
      {
        Serial.println("Command unrecognized");
        bt.println("Command unrecognized");
      }
  }
}
