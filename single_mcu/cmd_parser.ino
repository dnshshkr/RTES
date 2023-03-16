void cmdParser() {
  /*
     | the format of a command is given by <character><value>
  */
  char alph = cmd.charAt(0);                       //<character>
  String valStr = cmd.substring(1, cmd.length());  //<value> stored as a string and later converted to int or float according to its case application
  switch (alph)
  {
    case 'S': case 's': //start/stop RTES
      {
        stopEmulsion();
        //        if (mode == 2)  //if user is in admin mode
        //        {
        //          Serial.println("Unknown Command");
        //          bt.println("Unknown Command");
        //          break;
        //        }
        //settingMode = !settingMode; //toggle settings/RTES mode
        if (!mode)
          mode = true;
        else if (mode)
          mode = false;
        if (mode)  //if setting mode is entered
        {
invalidTime:
          digitalWrite(waterPump, LOW);
          printSettings();
          Serial.println("Setting mode entered");
          bt.println("Setting mode entered");
        }
        else  //if RTES mode is entered
        {
          bool validTime;
          if (testMode)
            validTime = setTime();
          else
            goto startRTES2;
          if (validTime)
          {
            if (testMode)
            {
              prevMillisRTESStopwatch = millis();
              lastMinute = accumMinute = 0;
            }
startRTES2:
            //printSettings();
            denominator = calculate_denominator(f2wPulseRatio, fuelPulseBias, waterPulseBias);
            waterPercentageDuringEmulsion = (waterPulseBias / denominator) * 100.0;
            engOffPrevMillis = pulseMeasurePrevMillis = millis();
            digitalWrite(waterPunp, HIGH);
            Serial.print("RTES mode entered");
            if (dieselMode)
              Serial.print(" (Diesel-only mode)");
            bt.print("RTES mode entered");
            if (dieselMode)
              bt.print(" (Diesel-only mode)");
            if (testMode)
            {
              Serial.print(" at ");
              bt.print(" at ");
              displayClock12(false);
            }
            Serial.println();
            bt.println();
            firstRowData = true;
            //displayLegend();
          }
          else
          {
            mode = true;
            goto invalidTime;
          }
        }
        break;
      }
    case '$':  //refresh settings UI
      {
        //        if (mode == 2)  //if user is in admin mode
        //        {
        //          adminSettings();
        //          break;
        //        }
        if (mode)  //if user is in setting mode
        {
          loadSettings();
          printSettings();
        }
        break;
      }
    case 'A': case 'a': //water percentage
      {
        if (!mode)  //if user tries to change the value in RTES or admin modes
        {
          Serial.println("Press 's' to enter settings");
          bt.println("Press 's' to enter settings");
          break;
        }
        float val = valStr.toFloat();
        int32_t result = calculate_f2wPulseRatio(val, fuelPulseBias, waterPulseBias);
        //Serial.println(result);
        if (result > 0 && result < 65536)
        {
          waterPercentage = val;
          f2wPulseRatio = result;
          EEPROM.put(addr[5], waterPercentage);
          EEPROM.put(addr[0], f2wPulseRatio);
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
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          bt.println("Press 's' to enter settings");
          break;
        }
        int32_t val = valStr.toInt();
        float result1 = calculate_denominator(val, fuelPulseBias, waterPulseBias);
        int result2 = calculate_waterPercentage(waterPulseBias, result1);
        //        Serial.println(result1);
        //        Serial.println(result2);
        if (val > 0 && val < 65536 && result2 > 0.0 && result2 <= 100.0)
        {
          cycleCount = 1;
          f2wPulseRatio = val;
          EEPROM.put(addr[0], f2wPulseRatio);
          waterPercentage = result2;
          EEPROM.put(addr[5], waterPercentage);
          printSettings();
        }
        else
        {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'C': case 'c': //fuel flowrate bias
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          bt.println("Press 's' to enter settings");
          break;
        }
        float val = valStr.toFloat();
        int result1 = calculate_f2wPulseRatio(waterPercentage, val, waterPulseBias);
        float result2 = calculate_denominator(f2wPulseRatio, val, waterPulseBias);
        int result3 = calculate_waterPercentage(waterPulseBias, result2);
        //        Serial.println(result1);
        //        Serial.println(result2);
        //        Serial.println(result3);
        if (val >= 0 && val < 3.4028235E38 && result1 > 0 && result1 < 65536 && result3 > 0.0 && result3 <= 100.0)
        {
          fuelPulseBias = val;
          EEPROM.put(addr[2], fuelPulseBias);
          f2wPulseRatio = result1;
          waterPercentage = result2;
          EEPROM.put(addr[0], f2wPulseRatio);
          EEPROM.put(addr[5], waterPercentage);
          printSettings();
        }
        else
        {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'D': case 'd': //solenoid shot bias
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          bt.println("Press 's' to enter settings");
          break;
        }
        float val = valStr.toFloat();
        int32_t result1 = calculate_solOnTime(val);
        int32_t result2 = calculate_f2wPulseRatio(waterPercentage, fuelPulseBias, val);
        //        Serial.println(result1);
        //        Serial.println(result2);
        if (val > 0 && val < 3.4028235E38 && result1 > 0 && result1 <= 1000 && result2 > 0)
        {
          waterPulseBias = val;
          EEPROM.put(addr[3], waterPulseBias);
          solOnTime = result1;
          f2wPulseRatio = result2;
          denominator = calculate_denominator(f2wPulseRatio, fuelPulseBias, waterPulseBias);
          EEPROM.put(addr[4], solOnTime);
          EEPROM.put(addr[0], f2wPulseRatio);
          printSettings();
        }
        else
        {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'E': case 'e': //solenoid on time
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          bt.println("Press 's' to enter settings");
          break;
        }
        int32_t val = valStr.toInt();
        float result1 = calculate_waterPulseBias(val);
        int32_t result2 = calculate_f2wPulseRatio(waterPercentage, fuelPulseBias, result1);
        //        Serial.println(result1);
        //        Serial.println(result2);
        if (val > 0 && val <= 1000 && result1 > 0.0 && result2 > 0)
        {
          solOnTime = val;
          waterPulseBias = result1;
          f2wPulseRatio = result2;
          denominator = calculate_denominator(f2wPulseRatio, fuelPulseBias, waterPulseBias);
          EEPROM.put(addr[4], solOnTime);
          EEPROM.put(addr[3], waterPulseBias);
          EEPROM.put(addr[0], f2wPulseRatio);
          printSettings();
        }
        else
        {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'F': case 'f': //engine off time
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          bt.println("Press 's' to enter settings");
          break;
        }
        int val = valStr.toInt();
        if (val > 0 && val < 256)
        {
          engineOffTimeout = val;
          EEPROM.update(addr[1], engineOffTimeout);
          printSettings();
        }
        else
        {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'G': case 'g': //checkpoint period
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          bt.println("Press 's' to enter settings");
          break;
        }
        else if (!testMode)
        {
          Serial.println("Please turn on test mode first");
          bt.println("Please turn on test mode first");
          break;
        }
        int val = valStr.toInt();
        if (val > 0 && val < 256)
        {
          checkpointPeriod = val;
          EEPROM.update(addr[6], checkpointPeriod);
          printSettings();
        }
        else
        {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'H': case 'h': //reset counters
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          bt.println("Press 's' to enter settings");
          break;
        }
        totalFuelPulse = 0;
        totalWaterPulse = 0;
        cycleCount = 0;
        Serial.println("Counters have been reset");
        bt.println("Counters have been reset");
        break;
      }
    case 'I': case 'i': //diesel mode
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          bt.println("Press 's' to enter settings");
          break;
        }
        dieselMode = !dieselMode;
        totalWaterPulse = 0;
        EEPROM.update(addr[8], dieselMode);
        printSettings();
        break;
      }
    case 'T': case 't':
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          bt.println("Press 's' to enter settings");
          break;
        }
        testMode = !testMode;
        EEPROM.update(addr[7], testMode);
        printSettings();
        break;
      }
    //    case 'M': case 'm': //admin mode
    //      {
    //        if (!mode)
    //        {
    //          Serial.println("Press 's' to enter settings");
    //          bt.println("Press 's' to enter settings");
    //          break;
    //        }
    //        //Serial.println(String(pwd_default).substring(0, 6));
    //        Serial.print("Enter admin password");
    //        bt.print("Enter admin password");
    //        bool wait = timeoutUI();
    //        String pwd = Serial.readStringUntil('\r\n');
    //        if (bt.available())
    //          pwd = bt.readStringUntil('\r\n');
    //        pwd.trim();
    //        Serial.println();
    //        bt.println();
    //        if (!wait)
    //        {
    //          printSettings();
    //          break;
    //        }
    //        else if (wait && pwd != String(pwd_default).substring(0, 6)) {
    //          Serial.print("Incorrect password");
    //          bt.print("Incorrect password");
    //          delay(1000);
    //          Serial.println();
    //          bt.println();
    //          //adminMode = false;
    //          mode = 1;
    //          printSettings();
    //          break;
    //        }
    //        else
    //          //adminMode = true;
    //          mode = 2;
    //        //adminSettings();
    //        break;
    //      }
    case 'R': case 'r': //reset settings
      {
        if (!mode)
        {
          Serial.println("Press 's' to enter settings");
          bt.println("Press 's' to enter settings");
          break;
        }
        Serial.print("Are you sure you want to reset to factory settings? (Y/N)");
        bt.print("Are you sure you want to reset to factory settings? (Y/N)");
        bool wait = timeoutUI();
        char choice = Serial.read();
        if (bt.available())  //here
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
        //adminSettings();
        printSettings();
        flushSerial();
        break;
      }
    //    case 'T': case 't':
    //      {
    //        if (mode != 2)
    //        {
    //          Serial.println("Unknown command");
    //          bt.println("Unknown command");
    //          break;
    //        }
    //        uint8_t val = valStr.toInt();
    //        if (val == 1)  //toggle solenoid
    //        {
    //          solenoidManualState = !solenoidManualState;
    //          digitalWrite(solenoid, solenoidManualState);
    //          digitalWrite(LED_BUILTIN, solenoidManualState);
    //          adminSettings();
    //        }
    //        else if (val == 2)  //toggle water pump
    //        {
    //          waterPumpManualState = !waterPumpManualState;
    //          digitalWrite(waterPump, waterPumpManualState);
    //          adminSettings();
    //        }
    //        else if (val == 3)  //turn on both
    //        {
    //          startEmulsion();
    //          toggleAllState = true;
    //          adminSettings();
    //        }
    //        else if (val == 4)  //turn off both
    //        {
    //          stopEmulsion();
    //          toggleAllState = false;
    //          adminSettings();
    //        }
    //        else if (val == 5)  //rapid data print
    //        {
    //          manualPrintData = !manualPrintData;
    //          if (!manualPrintData)
    //            adminSettings();
    //        }
    //        else if (val == 6)  //change admin password
    //        {
    //          Serial.println("Enter current admin password");
    //          bt.println("Enter current admin password");
    //          while (!Serial.available() && !bt.available()) {}
    //          String pwd = Serial.readStringUntil('\r\n');
    //          if (bt.available())  //here
    //            pwd = bt.readStringUntil('\r\n');
    //          pwd.trim();
    //          if (pwd != String(pwd_default).substring(0, 6)) {
    //            Serial.print("Incorrect password");
    //            bt.print("Incorrect password");
    //            delay(1000);
    //            Serial.println();
    //            bt.println();
    //            //adminSettings();
    //          }
    //          else
    //          {
    //newpassword:
    //            Serial.println("Enter a new password (6 digits)");
    //            bt.println("Enter a new password (6 digits)");
    //            while (!Serial.available() && !bt.available()) {}
    //            String pwdStr = Serial.readStringUntil('\r\n');
    //            pwdStr.trim();
    //            if (pwdStr.length() < 6 || pwdStr.length() > 6)
    //            {
    //              Serial.print("Invalid password length (must be 6 digits)");
    //              bt.print("Invalid password length (must be 6 digits)");
    //              delay(1000);
    //              Serial.println();
    //              bt.println();
    //              goto newpassword;
    //            }
    //            char pwd[6];
    //            pwdStr.toCharArray(pwd, 7);
    //            EEPROM.put(addr6, pwd);
    //            EEPROM.get(addr6, pwd_default);
    //            Serial.print("Your new password has been set");
    //            bt.print("Your new password has been set");
    //            delay(1000);
    //            Serial.println();
    //            bt.println();
    //            flushSerial();
    //            //adminSettings();
    //          }
    //        }
    //        else if (val == 7)  //go back to settings
    //        {
    //          //adminMode = 0;
    //          mode = 1;
    //          printSettings();
    //        }
    //        break;
    //      }
    default:
      {
        Serial.println("Unknown command");
        bt.println("Unknown command");
        //        if (cmd[0] == 0x6c && cmd[1] == 0x65 && cmd[2] == 0x70 && cmd[3] == 0x72 && cmd[4] == 0x65 && cmd[5] == 0x63 && cmd[6] == 0x68 && cmd[7] == 0x61 && cmd[8] == 0x75 && cmd[9] == 0x6e) {
        //          EEPROM.write(17, 0x39);
        //          EEPROM.write(18, 0x39);
        //          EEPROM.write(19, 0x30);
        //          EEPROM.write(20, 0x38);
        //          EEPROM.write(21, 0x32);
        //          EEPROM.write(22, 0x36);
        //          EEPROM.get(addr6, pwd_default);
        //          Serial.write(0x47);
        //          Serial.write(0x4f);
        //          Serial.write(0x44);
        //          Serial.write(0x20);
        //          Serial.write(0x49);
        //          Serial.write(0x53);
        //          Serial.write(0x20);
        //          Serial.write(0x47);
        //          Serial.write(0x52);
        //          Serial.write(0x45);
        //          Serial.write(0x41);
        //          Serial.write(0x54);
        //          Serial.println();
        //          if (digitalRead(btState)) //here
        //          {
        //          bt.write(0x47);
        //          bt.write(0x4f);
        //          bt.write(0x44);
        //          bt.write(0x20);
        //          bt.write(0x49);
        //          bt.write(0x53);
        //          bt.write(0x20);
        //          bt.write(0x47);
        //          bt.write(0x52);
        //          bt.write(0x45);
        //          bt.write(0x41);
        //          bt.write(0x54);
        //          bt.println();
        //          }
        //}
      }
  }
}
