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
    case 'D': case 'd':
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
    case 'E': case 'e':
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
    case 'H': case 'h':
      {
        if (!settingMode || manualPumpState)
        {
          Serial.println("Not in settings mode!");
          bt.println("Not in settings mode");
          break;
        }
        manualPumpState = 1;
        EEPROM.put(addr7, manualPumpState);
        printSettingManual();
        break;
      }
    case 'I': case 'i':
      {
        if (!settingMode || manualPumpState)
        {
          Serial.println("Not in settings mode!");
          if (digitalRead(btState))
            bt.println("Not in settings mode!");
          break;
        }
        float val = valStr.toFloat();
        if (val > 0)
        {
          quickWaterPercentage = val;
          EEPROM.put(addr10, quickWaterPercentage);
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
    case 'R': case 'r':
      {
        if (!settingMode || manualPumpState)
        {
          Serial.println("Not in settings mode!");
          bt.println("Not in settings mode!");
          break;
        }
        else
        {
          Serial.print("Are you sure you want to reset to factory settings? (Y/N): ");
          bt.print("Are you sure you want to reset to factory settings? (Y/N): ") ;
          unsigned long prev, prevCD;
          int8_t countDown = 10;
          Serial.print(String(countDown) + (char)32);
          bt.print(String(countDown) + (char)32);
          prev = millis();
          do
          {
            if (millis() - prev >= 1000)
            {
              countDown--;
              if (countDown >= 0)
              {
                Serial.print(String(countDown) + (char)32);
                bt.print(String(countDown) + (char)32);
              }
              prev = millis();
            }
          } while ((!Serial.available() || !bt.available()) && countDown >= 0);
          char choice = Serial.read();
          if (digitalRead(btState) && bt.available())
            choice = bt.read();
          Serial.println();
          bt.println();
          if (choice == 'Y' || choice == 'y')
            resetSettings();
          else
          {
            Serial.println("Factory reset aborted");
            bt.println("Factory reset aborted");
          }
        }
        while (Serial.available() || (digitalRead(btState) && bt.available()))
        {
          Serial.read();
          bt.read();
        }
        printSetting();
        break;
      }
    case 'T': case 't':
      {
        if (!manualPumpState)
        {
          Serial.println("Not in manual mode!");
          bt.println("Not in manual mode!");
          break;
        }
        uint8_t val = valStr.toInt();
        if (val == 1) //ON OFF FUEL
        {
          if (fuelTrig == 0)
          {
            fuelTrig = 1;
            EEPROM.put(addr8, fuelTrig);
          }
          else
          {
            fuelTrig = 0;
            EEPROM.put(addr8, fuelTrig);
          }
          printSettingManual();

        }
        else if (val == 2) //ON OFF SOL
        {
          if (emulsionTrig == 0)
          {
            emulsionTrig = 1;
            EEPROM.put(addr1, emulsionTrig);
          }
          else
          {
            emulsionTrig = 0;
            EEPROM.put(addr1, emulsionTrig);
          }
          printSettingManual();

        }
        else if (val == 3) //ON OFF WATERPUMP
        {
          if (waterTrig == 0)
          {
            waterTrig = 1;
            EEPROM.put(addr9, waterTrig);
          }
          else
          {
            waterTrig = 0;
            EEPROM.put(addr9, waterTrig);
          }
          printSettingManual();
        }
        else if (val == 4) //ON ALL
        {
          fuelTrig = 1;
          emulsionTrig = 1;
          waterTrig = 1;
          EEPROM.put(addr1, emulsionTrig);
          EEPROM.put(addr8, fuelTrig);
          EEPROM.put(addr9, waterTrig);
          printSettingManual();
        }
        else if (val == 5) //OFF ALL
        {
          fuelTrig = 0;
          emulsionTrig = 0;
          waterTrig = 0;
          EEPROM.put(addr1, emulsionTrig);
          EEPROM.put(addr8, fuelTrig);
          EEPROM.put(addr9, waterTrig);

          printSettingManual();
        }
        else if (val == 6)
        {
          manualPrintData = !manualPrintData;
          if (!manualPrintData)
            printSettingManual();
        }
        else if (val == 7)
        {
          manualPumpState = 0;
          //fuelTrig = 1;
          //EEPROM.get(addr1,read1);
          //emulsionTrig = read1;
          //waterTrig = 1;

          EEPROM.put(addr7, manualPumpState);

          printSetting();
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
  Serial.println("H: Enter Manual Mode");
  Serial.println("I: Water percentage: " + String(quickWaterPercentage) + "%");
  Serial.println("$: Refresh Settings");
  Serial.println("R: Reset to Factory Settings");
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
    bt.println("H: Enter Manual Mode");
    bt.println("I: Water percentage: " + String(quickWaterPercentage) + "%");
    bt.println("$: Refresh Settings");
    bt.println("R: Reset to Factory Settings");
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
