void printSettings()
{
  Serial.println("**************************ALL SETTING**************************");
  Serial.println("RTES v" + String(ver));
  Serial.println("A: Water Percentage: " + String(waterPercentage) + "%");
  Serial.print("B: Fuel Pulse Count: " + String(f2wPulseRatio) + " pulse");
  (f2wPulseRatio == 0 || f2wPulseRatio > 1) ? Serial.println('s') : Serial.println();
  Serial.println("C: Fuel Flow Rate Bias: " + String(flowRateBias) + " mL/pulse");
  Serial.println("D: Water Shot Bias: " + String(solShotBias) + " mL/pulse");
  Serial.println("E: Solenoid On Time: " + String(solenoidOnTime) + " ms");
  Serial.print("F: Engine Off Timeout: " + String(engineOffTimeout) + " s");
  if (testMode)
    Serial.print("\nG: Checkpoint Period: " + String(checkpointPeriod) + " minute");
  (checkpointPeriod > 1 && testMode) ? Serial.println('s') : Serial.println();
  Serial.println("H: Reset Total Fuel Pulse Counter");
  Serial.print("T: Toggle Test Mode: ");
  Serial.println(testMode);
  Serial.println("R: Reset to Factory Settings");
  //Serial.println("M: Enter Admin Settings");
  Serial.println("$: Refresh Settings");
  Serial.print("S: Enter ");
  if (mode)
    Serial.print('(');
  Serial.print("Settings");
  if (mode)
    Serial.print(')');
  Serial.print('/');
  if (!mode)
    Serial.print('(');
  Serial.print("RTES");
  if (!mode)
    Serial.print(')');
  Serial.println(" Mode");
  Serial.println("***************************************************************");

  //bluetooth
  bt.println("***ALL SETTINGS***");
  bt.println("RTES v" + String(ver));
  bt.println("A: Water Percentage: " + String(waterPercentage) + "%");
  bt.print("B: Fuel Pulse Count: " + String(f2wPulseRatio) + " pulse");
  (f2wPulseRatio == 0 || f2wPulseRatio > 1) ? bt.println('s') : bt.println();
  bt.println("C: Fuel Flow Rate Bias: " + String(flowRateBias) + " mL/pulse");
  bt.println("D: Water Shot Bias: " + String(solShotBias) + " mL/pulse");
  bt.println("E: Solenoid On Time: " + String(solenoidOnTime) + " ms");
  bt.print("F: Engine Off Timeout: " + String(engineOffTimeout) + " s");
  if (testMode)
    bt.print("\nG: Checkpoint Period: " + String(checkpointPeriod) + " minute");
  (checkpointPeriod > 1 && testMode) ? bt.println('s') : bt.println();
  bt.println("H: Reset Total Fuel Pulse Counter");
  bt.println("R: Reset to Factory Settings");
  bt.print("T: Toggle Test Mode: ");
  bt.println(testMode);
  //bt.println("M: Enter Admin Settings");
  bt.println("$: Refresh Settings");
  bt.print("S: Enter ");
  if (mode)
    bt.print('(');
  bt.print("Settings");
  if (mode)
    bt.print(')');
  bt.print('/');
  if (!mode)
    bt.print('(');
  bt.print("RTES");
  if (!mode)
    bt.print(')');
  bt.println(" Mode");
  bt.println("******************");
}

bool timeoutUI() {
  unsigned long prev, prevCD;
  int8_t countDown = 10;
  Serial.print(": " + String(countDown) + (char)32);
  bt.print(": " + String(countDown) + (char)32);
  prev = millis();
  do {
    if (millis() - prev >= 1000) {
      countDown--;
      if (countDown >= 0) {
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
