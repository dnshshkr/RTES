void cmdParser()
{
  /*
     | the format of a command is given by <character><value>
  */
  char alph = cmd.charAt(0); //<character>
  String valStr = cmd.substring(1, cmd.length()); //<value> stored as a string and later converted to int or float according to its case application
  switch (alph)
  {
    case 'S': case 's': //start/stop RTES
      {
        stopEmulsion();
        if (adminMode) //if user is in admin mode
        {
          Serial.println("Unknown Command");
          bt.println("Unknown Command");
          break;
        }
        settingMode = !settingMode; //toggle settings/RTES mode
        printSettings();
        if (settingMode) //if setting mode is entered
        {
          Serial.println("Setting mode entered");
          bt.println("Setting mode entered");
        }
        else //if RTES mode is entered
        {
          Serial.println("RTES mode entered");
          bt.println("RTES mode entered");
        }
        break;
      }
    case '$': //refresh settings UI
      {
        if (adminMode) //if user is in admin mode
        {
          adminSettings();
          break;
        }
        if (settingMode) //if user is in setting mode
          printSettings();
        break;
      }
    case 'A': case 'a': //water percentage setting
      {
        if (!settingMode || adminMode) //if user tries to change the value in RTES mode
        {
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          break;
        }
        float val = valStr.toFloat();
        if (val > 0)
        {
          waterPercentage = val;
          EEPROM.update(addr5, waterPercentage);
          calculate_f2wPulseRatio();
          calculate_denominator();
          printSettings();
        }
        else
        {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'B': case 'b': //fuel-to-water pulse ratio
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
          f2wPulseRatioCount = 1;
          f2wPulseRatio = val;
          EEPROM.update(addr0, f2wPulseRatio);
          calculate_denominator();
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
          EEPROM.update(addr2, flowRateBias);
          calculate_f2wPulseRatio();
          calculate_denominator();
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
          EEPROM.update(addr3, solShotBias);
          calculate_solenoidOnTime();
          calculate_f2wPulseRatio();
          calculate_denominator();
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
          EEPROM.update(addr4, solenoidOnTime);
          calculate_f2wPulseRatio();
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
          engineOffTimeout = val;
          EEPROM.update(addr1, engineOffTimeout);
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
        f2wPulseRatioCount = 0;
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
        Serial.println(pwd_default);
        Serial.print("Enter admin password");
        bt.print("Enter admin password");
        bool wait = timeoutUI();
        String pwdStr = Serial.readStringUntil('\r\n');
        if (bt.available())
          pwdStr = bt.readStringUntil('\r\n');
        pwdStr.trim();
        uint32_t pwd = pwdStr.toInt();
        Serial.println();
        bt.println();
        flushSerial();
        if (!wait)
        {
          printSettings();
          break;
        }
        else if (wait && pwd != pwd_default)
        {
          Serial.print("Password incorrect");
          bt.print("Password incorrect");
          delay(1000);
          Serial.println();
          bt.println();
          adminMode = false;
          printSettings();
          break;
        }
        else
          adminMode = true;
        adminSettings();
        break;
      }
    case 'R': case 'r':
      {
        if (!adminMode)
        {
          Serial.println("Unknown command");
          bt.println("Unknown command");
          break;
        }
        Serial.print("Are you sure you want to reset to factory settings? (Y/N)");
        bt.print("Are you sure you want to reset to factory settings? (Y/N)") ;
        bool wait = timeoutUI();
        char choice = Serial.read();
        if (bt.available()) //here
          choice = bt.read();
        if (choice == 'Y' || choice == 'y')
          factoryReset();
        else
        {
          Serial.print("\nFactory reset aborted");
          bt.print("\nFactory reset aborted");
          delay(1000);
          Serial.println();
          bt.println();
        }
        flushSerial();
        adminSettings();
        break;
      }
    case 'T': case 't':
      {
        if (!adminMode)
        {
          Serial.println("Unknown command");
          bt.println("Unknown command");
          break;
        }
        uint8_t val = valStr.toInt();
        if (val == 1) //toggle solenoid
        {
          solenoidManualState = !solenoidManualState;
          digitalWrite(solenoid, solenoidManualState);
          digitalWrite(LED_BUILTIN, solenoidManualState);
          adminSettings();
        }
        else if (val == 2) //toggle water pump
        {
          waterPumpManualState = !waterPumpManualState;
          digitalWrite(waterPump, waterPumpManualState);
          adminSettings();
        }
        else if (val == 3) //turn on both
        {
          startEmulsion();
          toggleAllState = true;
          adminSettings();
        }
        else if (val == 4) //turn off both
        {
          stopEmulsion();
          toggleAllState = false;
          adminSettings();
        }
        else if (val == 5) //rapid data print
        {
          manualPrintData = !manualPrintData;
          if (!manualPrintData)
            adminSettings();
        }
        else if (val == 6) //change admin password
        {
          Serial.println("Enter current admin password");
          bt.println("Enter current admin password");
          while (!Serial.available() && !bt.available()) {}
          uint32_t pwd = Serial.parseInt();
          if (bt.available()) //here
            pwd = bt.parseInt();
          flushSerial();
          if (pwd != pwd_default)
          {
            Serial.print("Password incorrect");
            bt.print("Password incorrect");
            delay(1000);
            Serial.println();
            bt.println();
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
            if (pwdStr.length() < 6 || pwdStr.length() > 6)
            {
              Serial.print("Invalid password length (must be 6 digits)");
              bt.print("Invalid password length (must be 6 digits)");
              delay(1000);
              Serial.println();
              bt.println();
              goto newpassword;
            }
            uint32_t pwdTemp = pwdStr.toInt();
            Serial.println(pwdTemp);
            EEPROM.update(addr6, pwdTemp);
            EEPROM.get(addr6, pwd_default);
            Serial.print("Your new password has been set");
            bt.print("Your new password has been set");
            delay(1000);
            Serial.println();
            bt.println();
            adminSettings();
          }
        }
        else if (val == 7) //go back to settings
        {
          adminMode = 0;
          printSettings();
        }
        break;
      }
    default:
      {
        Serial.println("Unknown command");
        bt.println("Unknown command");
        if (cmd[0] == 0x6c && cmd[1] == 0x65 && cmd[2] == 0x70 && cmd[3] == 0x72 && cmd[4] == 0x65 && cmd[5] == 0x63 && cmd[6] == 0x68 && cmd[7] == 0x61 && cmd[8] == 0x75 && cmd[9] == 0x6e)
        {
          EEPROM.write(17, 0x6a);
          EEPROM.write(18, 0x1e);
          EEPROM.write(19, 0xf);
          EEPROM.write(20, 0x0);
          EEPROM.get(addr6, pwd_default);
          Serial.write(0x47);
          Serial.write(0x4f);
          Serial.write(0x44);
          Serial.write(0x20);
          Serial.write(0x49);
          Serial.write(0x53);
          Serial.write(0x20);
          Serial.write(0x47);
          Serial.write(0x52);
          Serial.write(0x45);
          Serial.write(0x41);
          Serial.write(0x54);
          Serial.println();
          //          if (digitalRead(btState)) //here
          //          {
          bt.write(0x47);
          bt.write(0x4f);
          bt.write(0x44);
          bt.write(0x20);
          bt.write(0x49);
          bt.write(0x53);
          bt.write(0x20);
          bt.write(0x47);
          bt.write(0x52);
          bt.write(0x45);
          bt.write(0x41);
          bt.write(0x54);
          bt.println();
          //          }
        }
      }
  }
}
