void printSettings()
{
  Serial.println("**************************ALL SETTING**************************");
  Serial.println("RTES v" + String(ver));
  Serial.println("A: Water percentage: " + String(quickWaterPercentage) + "%");
  Serial.print("B: Fuel Pulse Count: " + String(pulse_fuelToWaterRatio) + " pulse");
  (pulse_fuelToWaterRatio == 0 || pulse_fuelToWaterRatio > 1) ? Serial.println('s') : Serial.println();
  Serial.println("C: Fuel Flow Rate Bias: " + String(flowRateBias) + " ml/pulse");
  Serial.println("D: Water Shot Bias: " + String(solShotBias) + " ml/pulse");
  Serial.println("E: Solenoid On Time: " + String(solenoidOnTime) + " ms");
  Serial.println("F: Engine Off Timeout: " + String(engineOffTimeOut) + " s");
  Serial.println("G: Reset Total Fuel Pulse Counter");
  Serial.println("M: Enter Admin Settings");
  Serial.println("$: Refresh Settings");
  Serial.println("R: Reset to Factory Settings");
  Serial.print("S: Enter ");
  if (settingMode)Serial.print('(');
  Serial.print("Settings");
  if (settingMode)Serial.print(')');
  Serial.print('/');
  if (!settingMode)Serial.print('(');
  Serial.print("RTES");
  if (!settingMode)Serial.print(')');
  Serial.println(" mode");
  //Serial.println("S: Enter Settings/Exit Settings/Start RTES System");
  Serial.println("***************************************************************");
  if (digitalRead(btState))
  {
    bt.println("***ALL SETTINGS***");
    bt.println("RTES v" + String(ver));
    bt.println("A: Water percentage: " + String(quickWaterPercentage) + "%");
    bt.print("B: Fuel Pulse Count: " + String(pulse_fuelToWaterRatio) + " pulse");
    (pulse_fuelToWaterRatio == 0 || pulse_fuelToWaterRatio > 1) ? bt.println('s') : bt.println();
    bt.println("C: Fuel Flow Rate Bias: " + String(flowRateBias) + " ml/pulse");
    bt.println("D: Water Shot Bias: " + String(solShotBias) + " ml/pulse");
    bt.println("E: Solenoid On Time: " + String(solenoidOnTime) + " ms");
    bt.println("F: Engine Off Timeout: " + String(engineOffTimeOut) + " s");
    bt.println("G: Reset Total Fuel Pulse Counter");
    bt.println("M: Enter Admin Settings");
    bt.println("$: Refresh Settings");
    bt.println("R: Reset to Factory Settings");
    Serial.print("S: Enter ");
    if (settingMode)bt.print('(');
    bt.print("Settings");
    if (settingMode)bt.print(')');
    bt.print('/');
    if (!settingMode)bt.print('(');
    bt.print("RTES");
    if (!settingMode)bt.print(')');
    bt.println(" mode");
    //bt.println("S: Enter Settings/Exit Settings/Start RTES System");
    bt.println("******************");
  }
}

bool timeoutUI()
{
  unsigned long prev, prevCD;
  int8_t countDown = 10;
  Serial.print(": " + String(countDown) + (char)32);
  bt.print(": " + String(countDown) + (char)32);
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
  } while (!Serial.available() && !bt.available() && countDown >= 0);
  Serial.println();
  bt.println();
  if (countDown <= 0)
    return false;
  else
    return true;
}
void adminSettings()
{
  Serial.println("***************************ADMIN MODE*************************");
  Serial.println("Manual Mode RTES v" + String(ver));
  Serial.print("T1: ON/OFF Solenoid: "); solenoidManualState ? Serial.println("ON") : Serial.println("OFF");
  Serial.print("T2: ON/OFF Water Pump: "); waterPumpManualState ? Serial.println("ON") : Serial.println("OFF");
  Serial.print("T3: ON All: "); toggleAllState ? Serial.println("ON") : Serial.println();
  Serial.print("T4: OFF All: "); !toggleAllState ? Serial.println("OFF") : Serial.println();
  Serial.print("T5: ON/OFF Print Data: "); manualPrintData ? Serial.println("ON") : Serial.println("OFF");
  Serial.println("T6: Change admin password");
  Serial.println("T7: Return to RTES Mode");
  Serial.println("$: Refresh Settings");
  Serial.println("**************************************************************");
  if (digitalRead(btState))
  {
    bt.println("***ADMIN MODE***");
    bt.println("Manual Mode RTES v" + String(ver));
    bt.print("T1: ON/OFF Solenoid: "); solenoidManualState ? bt.println("ON") : bt.println("OFF");
    bt.print("T2: ON/OFF Water Pump: "); waterPumpManualState ? bt.println("ON") : bt.println("OFF");
    bt.println("T3: ON All"); toggleAllState ? bt.println("ON") : bt.println();
    bt.println("T4: OFF All"); !toggleAllState ? bt.println("OFF") : bt.println();
    bt.print("T5: ON/OFF Print Data: "); manualPrintData ? bt.println("ON") : bt.println("OFF");
    bt.println("T6: Return to RTES Mode");
    bt.println("$: Refresh Settings");
    bt.println("****************");
  }
}
