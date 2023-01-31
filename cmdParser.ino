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
        if (!settingMode || manualPumpState)
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
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          break;
        }
        int val = valStr.toInt();
        if (val > 0)
        {
          engineOffTimeOut = val;
          EEPROM.put(addr2, engineOffTimeOut);
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
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          break;
        }
        totalFuelPulse = 0;
        pulse_fuelToWaterRatioCount = 0;
        Serial.println("Counter has been reset");
        bt.println("Counter has been reset");
        break;
      }
    case 'M': case 'm':
      {
        if (!settingMode || manualPumpState)
        {
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
          break;
        }
        manualPumpState = 1;
        EEPROM.put(addr6, manualPumpState);
        printSettingManual();
        break;
      }
    case 'R': case 'r':
      {
        if (!settingMode || manualPumpState)
        {
          Serial.println("Not in settings mode");
          bt.println("Not in settings mode");
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
          } while ((!Serial.available() || (digitalRead(btState) && !bt.available())) && countDown >= 0);
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
        if (val == 1) //on off solenoid
        {
          solenoidManualState = !solenoidManualState;
          digitalWrite(solenoidWater, solenoidManualState);
          printSettingManual();
        }
        else if (val == 2) //on off water pump
        {
          waterPumpManualState = !waterPumpManualState;
          digitalWrite(motorWater, waterPumpManualState);
          printSettingManual();
        }
        else if (val == 3)
        {
          solenoidManualState = HIGH;
          waterPumpManualState = HIGH;
          digitalWrite(solenoidWater, solenoidManualState);
          digitalWrite(motorWater, waterPumpManualState);
          toggleAllState = true;
          printSettingManual();
        }
        else if (val == 4)
        {
          solenoidManualState = LOW;
          waterPumpManualState = LOW;
          digitalWrite(solenoidWater, solenoidManualState);
          digitalWrite(motorWater, waterPumpManualState);
          toggleAllState = false;
          printSettingManual();
        }
        else if (val == 5) //OFF ALL
        {
          manualPrintData = !manualPrintData;
          if (!manualPrintData)
            printSettingManual();
        }
        else if (val == 6)
        {
          manualPumpState = 0;
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
  Serial.print("A: Fuel Pulse Count: " + String(pulse_fuelToWaterRatio) + " pulse");
  (pulse_fuelToWaterRatio == 0 || pulse_fuelToWaterRatio > 1) ? Serial.println("s") : Serial.println();
  Serial.println("B: Engine Off Timeout: " + String(engineOffTimeOut) + " s");
  Serial.println("C: Fuel Flow Rate Bias: " + String(flowRateBias) + " ml/pulse");
  Serial.println("D: Water Shot Bias: " + String(solShotBias) + " ml/pulse");
  Serial.println("E: Solenoid On Time: " + String(solenoidOnTime) + " ms");
  Serial.println("F: Water percentage: " + String(quickWaterPercentage) + "%");
  Serial.println("G: Reset total fuel pulse counter");
  Serial.println("M: Enter Manual Mode");
  Serial.println("$: Refresh Settings");
  Serial.println("R: Reset to Factory Settings");
  Serial.println("S: Enter Settings/Exit Settings/Start RTES System");
  Serial.println("***************************************************************");
  if (digitalRead(btState))
  {
    bt.println("**************************ALL SETTING**************************");
    bt.println("RTES v" + String(ver));
    bt.print("A: Fuel Pulse Count: " + String(pulse_fuelToWaterRatio) + " pulse");
    (pulse_fuelToWaterRatio == 0 || pulse_fuelToWaterRatio > 1) ? bt.println("s") : bt.println();
    bt.println("B: Engine Off Timeout: " + String(engineOffTimeOut) + " s");
    bt.println("C: Fuel Flow Rate Bias: " + String(flowRateBias) + " ml/pulse");
    bt.println("D: Water Shot Bias: " + String(solShotBias) + " ml/pulse");
    bt.println("E: Solenoid On Time: " + String(solenoidOnTime) + " ms");
    bt.println("F: Water percentage: " + String(quickWaterPercentage) + "%");
    bt.println("G: Reset total fuel pulse counter");
    bt.println("M: Enter Manual Mode");
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
  Serial.print("T1: ON/OFF Solenoid: "); solenoidManualState ? Serial.println("ON") : Serial.println("OFF");
  Serial.print("T2: ON/OFF Water Pump: "); waterPumpManualState ? Serial.println("ON") : Serial.println("OFF");
  Serial.print("T3: ON All: "); toggleAllState ? Serial.println("ON") : Serial.println();
  Serial.print("T4: OFF All: "); toggleAllState ? Serial.println("OFF") : Serial.println();
  Serial.print("T5: ON/OFF Print Data: "); manualPrintData ? Serial.println("ON") : Serial.println("OFF");
  Serial.println("T6: Return to RTES Mode");
  Serial.println("$: Refresh Settings");
  Serial.println("**************************************************************");
  if (digitalRead(btState))
  {
    bt.println("**********************MANUAL MODE SETTING*********************");
    bt.println("Manual Mode RTES v" + String(ver));
    bt.print("T1: ON/OFF Solenoid: "); solenoidManualState ? bt.println("ON") : bt.println("OFF");
    bt.print("T2: ON/OFF Water Pump: "); waterPumpManualState ? bt.println("ON") : bt.println("OFF");
    bt.println("T3: ON All"); toggleAllState ? bt.println("ON") : bt.println();
    bt.println("T4: OFF All"); toggleAllState ? bt.println("OFF") : bt.println();
    bt.print("T5: ON/OFF Print Data: "); manualPrintData ? bt.println("ON") : bt.println("OFF");
    bt.println("T6: Return to RTES Mode");
    bt.println("$: Refresh Settings");
    bt.println("**************************************************************");
  }
}
