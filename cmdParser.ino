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
        if (manualPumpState)
        {
          printSettingManual();
          break;
        }
        settingMode = !settingMode;
        printSetting();
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
        if (manualPumpState)
        {
          printSettingManual();
          break;
        }
        if (settingMode)
          printSetting();
        break;
      }
    case 'A': case 'a':
      {
        int val = valStr.toInt();
        if (!settingMode || manualPumpState)
        {
          Serial.println("Not in settings mode!");
          bt.println("Not in settings mode!");
          break;
        }
        if (val == 0 || val == 1)
        {
          emulsionTrig = val;
          EEPROM.put(addr1, emulsionTrig);
          printSetting();
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
        if (!settingMode || manualPumpState)
        {
          Serial.println("Not in settings mode!");
          bt.println("Not in settings mode!");
          break;
        }
        int val = valStr.toInt();
        if (val > 0)
        {
          pulse_fuelToWaterRatioCount = 1;
          pulse_fuelToWaterRatio = val;
          EEPROM.put(addr2, pulse_fuelToWaterRatio);
          calculateDenom();
          printSetting();
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
        if (!settingMode || manualPumpState)
        {
          Serial.println("Not in settings mode!");
          bt.println("Not in settings mode!");
          break;
        }
        int val = valStr.toInt();
        if (val > 0)
        {
          engineOffTimeOut = val;
          EEPROM.put(addr3, engineOffTimeOut);
          printSetting();
        }
        else
        {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'D': case'd':
      {
        if (!settingMode || manualPumpState)
        {
          Serial.println("Not in settings mode!");
          bt.println("Not in settings mode!");
          break;
        }
        float val = valStr.toFloat();
        if (val > 0)
        {
          flowRateBias = val;
          EEPROM.put(addr4, flowRateBias);
          calculatePulse_fuelToWaterRatio();
          calculateDenom();
          printSetting();
        }
        else
        {
          Serial.println("Input is out of range");
          bt.println("Input is out of range");
        }
        break;
      }
    case 'E': case'e':
      {
        if (!settingMode || manualPumpState)
        {
          Serial.println("Not in settings mode!");
          bt.println("Not in settings mode!");
          break;
        }
        float val = valStr.toFloat();
        if (val > 0)
        {
          solShotBias = val;
          EEPROM.put(addr5, solShotBias);
          calculateSolenoidOnTime();
          calculatePulse_fuelToWaterRatio();
          calculateDenom();
          printSetting();
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
        if (!settingMode || manualPumpState)
        {
          Serial.println("Not in settings mode!");
          bt.println("Not in settings mode!");
          break;
        }
        int val = valStr.toInt();
        if (val > 0)
        {
          solenoidOnTime = val;
          EEPROM.put(addr6, solenoidOnTime);
          calculatePulse_fuelToWaterRatio();
          printSetting();
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
        if (!settingMode || manualPumpState)
        {
          Serial.println("Not in settings mode!");
          bt.println("Not in settings mode!");
          break;
        }
        else
        {
          Serial.println("Are you sure you want to reset to factory settings? (Y/N)");
          bt.println("Are you sure you want to reset to factory settings? (Y/N)");
          unsigned short current;
          unsigned long prev = millis();
          while ((!Serial.available() || !bt.available()) && current <= 10000)
            current = millis() - prev;
          char choice = Serial.read();
          choice = bt.read();
          Serial.println(choice);
          if (digitalRead(btState))
            bt.println(choice);
          if (choice == 'Y' || choice == 'y')
            ResetSetting();
        }
        printSetting();
        break;
      }
  }
  //  byte ch;
  //  String valStr;
  //  int val;
  //  float valf;
  //
  //  if (Serial.available() || (bt.available() && digitalRead(btState)))
  //  {
  //    ch = Serial.read();
  //    if (digitalRead(btState))
  //      ch = bt.read();
  //    sdata += (char)ch;
  //    if (ch == '\r')
  //    { // Command received and ready.
  //      sdata.trim();
  //      // Process command in sdata.
  //      switch (sdata.charAt(0))
  //      {
  //        case 'S': case's':
  //          {
  //            digitalWrite(solenoidWater, LOW);
  //            digitalWrite(motorWater, LOW);
  //            if (manualPumpState)
  //            {
  //              printSettingManual();
  //              break;
  //            }
  //            settingMode = !settingMode;
  //            printSetting();
  //            if (settingMode)
  //            {
  //              Serial.println("Setting mode entered");
  //              if (digitalRead(btState))
  //                bt.println("Setting mode entered");
  //            }
  //            else
  //            {
  //              Serial.println("RTES mode entered");
  //              if (digitalRead(btState))
  //                bt.println("RTES mode entered");
  //            }
  //            break;
  //          }
  //        case '$':
  //          {
  //            if (manualPumpState)
  //            {
  //              printSettingManual();
  //              break;
  //            }
  //            if (settingMode)
  //              printSetting();
  //            break;
  //          }
  //        case 'A': case'a':
  //          {
  //            if (!settingMode || manualPumpState)
  //            {
  //              Serial.println("Not in settings mode!");
  //              if (digitalRead(btState))
  //                bt.println("Not in settings mode!");
  //              break;
  //            }
  //            if (sdata.length() > 1) {
  //              valStr = sdata.substring(1);
  //              val = valStr.toInt();
  //            }
  //            if (val == 0 || val == 1)
  //            {
  //              emulsionTrig = val;
  //              EEPROM.put(addr1, emulsionTrig);
  //              printSetting();
  //            }
  //            else
  //            {
  //              Serial.println("Input is out of range");
  //              if (digitalRead(btState))
  //                bt.println("Input is out of range");
  //            }
  //            break;
  //          }
  //        case 'B': case'b': //Pulse Count Max
  //          {
  //            if (!settingMode || manualPumpState)
  //            {
  //              Serial.println("Not in settings mode!");
  //              if (digitalRead(btState))
  //                bt.println("Not in settings mode!");
  //              break;
  //            }
  //            if (sdata.length() > 1)
  //            {
  //              valStr = sdata.substring(1);
  //              val = valStr.toInt();
  //            }
  //            if (val > 0)
  //            {
  //              pulse_fuelToWaterRatioCount = 1;
  //              pulse_fuelToWaterRatio = val;
  //              EEPROM.put(addr2, pulse_fuelToWaterRatio);
  //              calculateDenom();
  //              printSetting();
  //            }
  //            else
  //            {
  //              Serial.println("Input is out of range");
  //              if (digitalRead(btState))
  //                bt.println("Input is out of range");
  //            }
  //            break;
  //          }
  //        case 'C': case'c': //Pulse time for idle
  //          {
  //            if (!settingMode || manualPumpState)
  //            {
  //              Serial.println("Not in settings mode!");
  //              if (digitalRead(btState))
  //                bt.println("Not in settings mode!");
  //              break;
  //            }
  //            if (sdata.length() > 1)
  //            {
  //              valStr = sdata.substring(1);
  //              val = valStr.toInt();
  //            }
  //            if (val > 0)
  //            {
  //              engineOffTimeOut = val;
  //              EEPROM.put(addr3, engineOffTimeOut);
  //              printSetting();
  //            }
  //            else
  //            {
  //              Serial.println("Input is out of range");
  //              if (digitalRead(btState))
  //                bt.println("Input is out of range");
  //            }
  //            break;
  //          }
  //        case 'D': case'd': //Flow Rate Bias
  //          {
  //            if (!settingMode || manualPumpState)
  //            {
  //              Serial.println("Not in settings mode!");
  //              if (digitalRead(btState))
  //                bt.println("Not in settings mode!");
  //              break;
  //            }
  //            if (sdata.length() > 1)
  //            {
  //              valStr = sdata.substring(1);
  //              valf = valStr.toFloat();
  //            }
  //            if (valf > 0)
  //            {
  //              flowRateBias = valf;
  //              EEPROM.put(addr4, flowRateBias);
  //              calculatePulse_fuelToWaterRatio();
  //              calculateDenom();
  //              printSetting();
  //            }
  //            else
  //            {
  //              Serial.println("Input is out of range");
  //              if (digitalRead(btState))
  //                bt.println("Input is out of range");
  //            }
  //            break;
  //          }
  //        case 'E': case'e': //Solenoid Shot Bias
  //          {
  //            if (!settingMode || manualPumpState)
  //            {
  //              Serial.println("Not in settings mode!");
  //              if (digitalRead(btState))
  //                bt.println("Not in settings mode!");
  //              break;
  //            }
  //            if (sdata.length() > 1) {
  //              valStr = sdata.substring(1);
  //              valf = valStr.toFloat();
  //            }
  //            if (valf > 0)
  //            {
  //              solShotBias = valf;
  //              EEPROM.put(addr5, solShotBias);
  //              calculateSolenoidOnTime();
  //              calculatePulse_fuelToWaterRatio();
  //              calculateDenom();
  //              printSetting();
  //            }
  //            else
  //            {
  //              Serial.println("Input is out of range");
  //              if (digitalRead(btState))
  //                bt.println("Input is out of range");
  //            }
  //            break;
  //          }
  //        case 'F': case'f':
  //          {
  //            if (!settingMode || manualPumpState)
  //            {
  //              Serial.println("Not in settings mode!");
  //              if (digitalRead(btState))
  //                bt.println("Not in settings mode!");
  //              break;
  //            }
  //            if (sdata.length() > 1)
  //            {
  //              valStr = sdata.substring(1);
  //              val = valStr.toInt();
  //            }
  //            if (val > 0 )
  //            {
  //              solenoidOnTime = val;
  //              EEPROM.put(addr6, solenoidOnTime);
  //              calculatePulse_fuelToWaterRatio();
  //              printSetting();
  //            }
  //            else
  //            {
  //              Serial.println("Input is out of range");
  //              if (digitalRead(btState))
  //                bt.println("Input is out of range");
  //            }
  //            break;
  //          }
  //        case 'G': case'g': //Reset Setting
  //          {
  //            while (Serial.available() || bt.available())
  //              String trash = Serial.readStringUntil('\r\n');
  //            if (!settingMode || manualPumpState)
  //            {
  //              Serial.println("Not in settings mode!");
  //              if (digitalRead(btState))
  //                bt.println("Not in settings mode!");
  //              break;
  //            }
  //            else
  //            {
  //              Serial.println("Are you sure you want to reset to factory settings? (Y/N)");
  //              if (digitalRead(btState))
  //                bt.println("Are you sure you want to reset to factory settings? (Y/N)");
  //              unsigned short current;
  //              unsigned long prev = millis();
  //              while ((!Serial.available() || !bt.available()) && current <= 10000)
  //                current = millis() - prev;
  //              char choice = Serial.read();
  //              if (digitalRead(btState))
  //                choice = bt.read();
  //              Serial.println(choice);
  //              if (digitalRead(btState))
  //                bt.println(choice);
  //              if (choice == 'Y' || choice == 'y')
  //                ResetSetting();
  //            }
  //            printSetting();
  //            break;
  //          }
  //        case 'H': case'h': //Manual Mode
  //          {
  //            if (!settingMode || manualPumpState)
  //            {
  //              Serial.println("Not in settings mode!");
  //              if (digitalRead(btState))
  //                bt.println("Not in settings mode");
  //              break;
  //            }
  //            manualPumpState = 1;
  //            EEPROM.put(addr7, manualPumpState);
  //            printSettingManual();
  //            break;
  //          }
  //        case 'I': case 'i':
  //          {
  //            if (!settingMode || manualPumpState)
  //            {
  //              Serial.println("Not in settings mode!");
  //              if (digitalRead(btState))
  //                bt.println("Not in settings mode!");
  //              break;
  //            }
  //            else
  //            {
  //              if (sdata.length() > 1)
  //              {
  //                valStr = sdata.substring(1);
  //                valf = valStr.toFloat();
  //              }
  //              if (valf > 0)
  //              {
  //                quickWaterPercentage = valf;
  //                EEPROM.put(addr10, quickWaterPercentage);
  //                calculatePulse_fuelToWaterRatio();
  //                calculateDenom();
  //                printSetting();
  //              }
  //              else
  //              {
  //                Serial.println("Input is out of range");
  //                if (digitalRead(btState))
  //                  bt.println("Input is out of range");
  //              }
  //              break;
  //            }
  //          }
  //        case 'T': case't': //MANUAL FUEL PUMP
  //          {
  //            if (!manualPumpState)
  //            {
  //              Serial.println("Not in manual mode!");
  //              if (digitalRead(btState))
  //                bt.println("Not in manual mode!");
  //              break;
  //            }
  //            if (sdata.length() > 1)
  //            {
  //              valStr = sdata.substring(1);
  //              val = valStr.toInt();
  //            }
  //            if (val == 1) //ON OFF FUEL
  //            {
  //              if (fuelTrig == 0)
  //              {
  //                fuelTrig = 1;
  //                EEPROM.put(addr8, fuelTrig);
  //              }
  //              else
  //              {
  //                fuelTrig = 0;
  //                EEPROM.put(addr8, fuelTrig);
  //              }
  //              printSettingManual();
  //
  //            }
  //            else if (val == 2) //ON OFF SOL
  //            {
  //              if (emulsionTrig == 0)
  //              {
  //                emulsionTrig = 1;
  //                EEPROM.put(addr1, emulsionTrig);
  //              }
  //              else
  //              {
  //                emulsionTrig = 0;
  //                EEPROM.put(addr1, emulsionTrig);
  //              }
  //              printSettingManual();
  //
  //            }
  //            else if (val == 3) //ON OFF WATERPUMP
  //            {
  //              if (waterTrig == 0)
  //              {
  //                waterTrig = 1;
  //                EEPROM.put(addr9, waterTrig);
  //              }
  //              else
  //              {
  //                waterTrig = 0;
  //                EEPROM.put(addr9, waterTrig);
  //              }
  //              printSettingManual();
  //            }
  //            else if (val == 4) //ON ALL
  //            {
  //              fuelTrig = 1;
  //              emulsionTrig = 1;
  //              waterTrig = 1;
  //              EEPROM.put(addr1, emulsionTrig);
  //              EEPROM.put(addr8, fuelTrig);
  //              EEPROM.put(addr9, waterTrig);
  //              printSettingManual();
  //            }
  //            else if (val == 5) //OFF ALL
  //            {
  //              fuelTrig = 0;
  //              emulsionTrig = 0;
  //              waterTrig = 0;
  //              EEPROM.put(addr1, emulsionTrig);
  //              EEPROM.put(addr8, fuelTrig);
  //              EEPROM.put(addr9, waterTrig);
  //
  //              printSettingManual();
  //            }
  //            else if (val == 6)
  //            {
  //              manualPrintData = !manualPrintData;
  //              if (!manualPrintData) printSettingManual();
  //            }
  //            else if (val == 7)
  //            {
  //              manualPumpState = 0;
  //              //fuelTrig = 1;
  //              //EEPROM.get(addr1,read1);
  //              //emulsionTrig = read1;
  //              //waterTrig = 1;
  //
  //              EEPROM.put(addr7, manualPumpState);
  //
  //              printSetting();
  //            }
  //            break;
  //          }
  //        default:
  //          {
  //            Serial.println(sdata);
  //            if (digitalRead(btState))
  //              bt.println(sdata);
  //          }
  //      } // switch
  //      sdata = ""; // Clear the string ready for the next command.
  //    }
  //  }
}
void printSetting()
{
  Serial.println("**************************ALL SETTING**************************");
  Serial.println("RTES v" + String(ver));
  Serial.print("A: Emulsion State {0,1}: "); Serial.println(emulsionTrig);
  Serial.print("B: Fuel Pulse Count: " + String(pulse_fuelToWaterRatio) + " pulse");
  pulse_fuelToWaterRatio > 1 ? Serial.println("s") : Serial.println();
  Serial.println("C: Engine Off Timeout: " + String(engineOffTimeOut) + " ms");
  Serial.println("D: Fuel Flow Rate Bias: " + String(flowRateBias) + " ml/pulse");
  Serial.println("E: Water Shot Bias: " + String(solShotBias) + " ml/pulse");
  Serial.println("F: Solenoid On Time: " + String(solenoidOnTime) + " ms");
  Serial.println("G: Reset to Factory Settings");
  Serial.println("H: Enter Manual Mode");
  Serial.println("I: Water percentage: " + String(quickWaterPercentage) + "%");
  Serial.println("$: Refresh Settings");
  Serial.println("S: Enter Settings/Exit Settings/Start RTES System");
  Serial.println("***************************************************************");
  if (digitalRead(btState))
  {
    bt.println("**************************ALL SETTING**************************");
    bt.println("RTES v" + String(ver));
    bt.print("A: Emulsion State {0,1}: "); bt.println(emulsionTrig);
    bt.print("B: Fuel Pulse Count: " + String(pulse_fuelToWaterRatio) + " pulse");
    pulse_fuelToWaterRatio > 1 ? bt.println("s") : bt.println();
    bt.println("C: Engine Off Timeout: " + String(engineOffTimeOut) + " ms");
    bt.println("D: Fuel Flow Rate Bias: " + String(flowRateBias) + " ml/pulse");
    bt.println("E: Water Shot Bias: " + String(solShotBias) + " ml/pulse");
    bt.println("F: Solenoid On Time: " + String(solenoidOnTime) + " ms");
    bt.println("G: Reset to Factory Settings");
    bt.println("H: Enter Manual Mode");
    bt.println("I: Water percentage: " + String(quickWaterPercentage) + "%");
    bt.println("$: Refresh Settings");
    bt.println("S: Enter Settings/Exit Settings/Start RTES System");
    bt.println("***************************************************************");
  }
}

void printSettingManual()
{
  Serial.println("**********************MANUAL MODE SETTING*********************");
  Serial.println("Manual Mode RTES v" + String(ver));
  Serial.print("T1: ON/OFF Fuel Pump: "); Serial.println(fuelTrig);
  Serial.print("T2: ON/OFF Solenoid: "); Serial.println(emulsionTrig);
  Serial.print("T3: ON/OFF Water Pump: "); Serial.println(waterTrig);
  Serial.println("T4: ON All Pump: ");
  Serial.println("T5: OFF All Pump: ");
  Serial.print("T6: ON/OFF Print Data: "); manualPrintData ? Serial.println("ON") : Serial.println("OFF");
  Serial.println("T7: Return to RTES Mode");
  Serial.println("$: Refresh Settings");
  Serial.println("**************************************************************");
  if (digitalRead(btState))
  {
    bt.println("**********************MANUAL MODE SETTING*********************");
    bt.println("Manual Mode RTES v" + String(ver));
    bt.print("T1: ON/OFF Fuel Pump: "); bt.println(fuelTrig);
    bt.print("T2: ON/OFF Solenoid: "); bt.println(emulsionTrig);
    bt.print("T3: ON/OFF Water Pump: "); bt.println(waterTrig);
    bt.println("T4: ON All Pump: ");
    bt.println("T5: OFF All Pump: ");
    bt.print("T6: ON/OFF Print Data: "); manualPrintData ? bt.println("ON") : bt.println("OFF");
    bt.println("T7: Return to RTES Mode");
    bt.println("$: Refresh Settings");
    bt.println("**************************************************************");
  }
}
