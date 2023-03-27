void printSettings()
{
  Serial.println("*************************ALL SETTINGS**************************");
  Serial.println("RTES v" + String(ver));
  Serial.println("A - Water Percentage: " + String(waterPercentage) + "%");
  Serial.print("B - Fuel Pulse(s) Per Cycle: " + String(f2wPulseRatio + 1) + " pulse");
  (f2wPulseRatio + 1 == 0 || f2wPulseRatio + 1 > 1) ? Serial.println('s') : Serial.println();
  Serial.println("C - Fuel Flow Rate Bias: " + String(fuelPulseBias) + " mL/pulse");
  Serial.println("D - Solenoid Constant: " + String(solConst * 1000) + " µL/ms");
  Serial.println("E - Water Shot Bias: " + String(waterPulseBias) + " mL/pulse");
  Serial.println("F - Solenoid On Time: " + String(solOnTime) + " ms");
  Serial.print("G - Engine Off Timeout: " + String(engineOffTimeout) + " s");
  if (testMode)
    Serial.print("\nH - Checkpoint Period: " + String(checkpointPeriod) + " minute");
  (checkpointPeriod > 1 && testMode) ? Serial.println('s') : Serial.println();
  Serial.println("I - Reset Total Fuel Pulse Counter");
  Serial.print("J - Diesel-only Mode: ");
  dieselMode ? Serial.println("ON") : Serial.println("OFF");
  Serial.println("K - Mixer Device Period: " + String(devicePeriod) + " s");
  Serial.println("L - Mixer Device On Time: " + String(deviceOnTime) + " s");
  Serial.print("T - Toggle Test Mode: ");
  testMode ? Serial.println("ON") : Serial.println("OFF");
  Serial.println("R - Reset to Factory Settings");
  //Serial.println("M: Enter Admin Settings");
  Serial.println("$ - Refresh Settings");
  Serial.print("S - Enter ");
  if (mode)
    Serial.write('(');
  Serial.print("Settings");
  if (mode)
    Serial.write(')');
  Serial.write('/');
  if (!mode)
    Serial.write('(');
  Serial.print("RTES");
  if (!mode)
    Serial.write(')');
  Serial.println(" Mode");
  Serial.println("***************************************************************");

  //bluetooth
  bt.println("***ALL SETTINGS***");
  bt.println("RTES v" + String(ver));
  bt.println("A - Water Percentage: " + String(waterPercentage) + " % ");
  bt.print("B - Fuel Pulse Count: " + String(f2wPulseRatio) + " pulse");
  (f2wPulseRatio == 0 || f2wPulseRatio > 1) ? bt.println('s') : bt.println();
  bt.println("C - Fuel Flow Rate Bias: " + String(fuelPulseBias) + " mL / pulse");
  bt.println("D - Solenoid Constant: " + String(solConst * 1000) + " µL/ms");
  bt.println("E - Water Shot Bias: " + String(waterPulseBias) + " mL / pulse");
  bt.println("F - Solenoid On Time: " + String(solOnTime) + " ms");
  bt.print("G - Engine Off Timeout: " + String(engineOffTimeout) + " s");
  if (testMode)
    bt.print("\nH - Checkpoint Period: " + String(checkpointPeriod) + " minute");
  (checkpointPeriod > 1 && testMode) ? bt.println('s') : bt.println();
  bt.println("I - Reset Total Fuel Pulse Counter");
  bt.print("J - Diesel-only Mode: ");
  dieselMode ? bt.println("ON") : bt.println("OFF");
  bt.println("K - Mixer Device Period: " + String(devicePeriod) + " s");
  bt.println("L - Mixer Device On Time: " + String(deviceOnTime) + " s");
  bt.print("T - Toggle Test Mode: ");
  testMode ? bt.println("ON") : bt.println("OFF");
  bt.println("R - Reset to Factory Settings");
  //bt.println("M: Enter Admin Settings");
  bt.println("$ - Refresh Settings");
  bt.print("S - Enter ");
  if (mode)
    bt.write('(');
  bt.print("Settings");
  if (mode)
    bt.write(')');
  bt.write('/');
  if (!mode)
    bt.write('(');
  bt.print("RTES");
  if (!mode)
    bt.write(')');
  bt.println(" Mode");
  bt.println("******************");
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
  if (countDown <= 0)
    return false;
  else
    return true;
}

//void displayLegend()
//{
//  Serial.println("Cycle | Total Fuel Pulse | Fuel Pulse Period (s) | Fuel Flow Rate (mL/min) | Total Water Pulse | Water Percentage (%/cycle) | Fuel Percentage (%/cycle)");
//  bt.println("Total Fuel Pulse, Total Water Pulse, Fuel Pulse Period (s), Fuel Flow Rate (mL/min), Cycle Count, Water Percentage (%), Fuel Percentage (%)");
//}
//void adminSettings() {
//  Serial.println("***************************ADMIN MODE*************************");
//  Serial.println("Manual Mode RTES v" + String(ver));
//  Serial.print("T1: Toggle Solenoid: ");
//  solenoidManualState ? Serial.println("ON") : Serial.println("OFF");
//  Serial.print("T2: Toggle Water Pump: ");
//  waterPumpManualState ? Serial.println("ON") : Serial.println("OFF");
//  Serial.print("T3: ON All: ");
//  toggleAllState ? Serial.println("ON") : Serial.println();
//  Serial.print("T4: OFF All: ");
//  !toggleAllState ? Serial.println("OFF") : Serial.println();
//  Serial.print("T5: Toggle Print Data: ");
//  manualPrintData ? Serial.println("ON") : Serial.println("OFF");
//  Serial.println("T6: Change Admin Password");
//  Serial.println("T7: Return to RTES Mode");
//  Serial.println("$: Refresh Settings");
//
//  Serial.println("**************************************************************");
//  bt.println("***ADMIN MODE***");
//  bt.println("Manual Mode RTES v" + String(ver));
//  bt.print("T1: Toggle Solenoid: ");
//  solenoidManualState ? bt.println("ON") : bt.println("OFF");
//  bt.print("T2: Toggle Water Pump: ");
//  waterPumpManualState ? bt.println("ON") : bt.println("OFF");
//  bt.println("T3: ON All");
//  toggleAllState ? bt.println("ON") : bt.println();
//  bt.println("T4: OFF All");
//  !toggleAllState ? bt.println("OFF") : bt.println();
//  bt.print("T5: Toggle Print Data: ");
//  manualPrintData ? bt.println("ON") : bt.println("OFF");
//  bt.println("T6: Change Admin Password");
//  bt.println("T7: Return to RTES Mode");
//  bt.println("$: Refresh Settings");
//  bt.println("R: Reset to Factory Settings");
//  bt.println("****************");
//}
