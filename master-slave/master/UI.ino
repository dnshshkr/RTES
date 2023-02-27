void printSettings()
{
  Serial.println("*************************ALL SETTINGS**************************");
  Serial.println("RTES v" + String(ver));
  Serial.println("A: Water Percentage: " + String(waterPercentage) + "%");
  Serial.print("B: Fuel Pulse Count: " + String(f2wPulseRatio) + " pulse");
  (f2wPulseRatio == 0 || f2wPulseRatio > 1) ? Serial.println('s') : Serial.println();
  Serial.println("C: Fuel Flow Rate Bias: " + String(flowRateBias) + " mL/pulse");
  Serial.println("D: Water Shot Bias: " + String(solShotBias) + " mL/pulse");
  Serial.println("E: Solenoid On Time: " + String(solOnTime) + " ms");
  Serial.print("F: Engine Off Timeout: " + String(engineOffTimeout) + " s");
  if (testMode)
    Serial.print("\nG: Checkpoint Period: " + String(checkpointPeriod) + " minute");
  (checkpointPeriod > 1 && testMode) ? Serial.println('s') : Serial.println();
  Serial.println("H: Reset Total Fuel Pulse Counter");
  Serial.print("T: Toggle Test Mode: ");
  Serial.println(testMode);
  Serial.println("R: Reset to Factory Settings");
  Serial.println("$: Refresh Settings");
  Serial.print("S: Enter ");
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
}

bool timeoutUI(int8_t countDown)
{
  unsigned long prev, prevCD;
  Serial.print(": " + String(countDown) + (char)32);
  prev = millis();
  do
  {
    if (millis() - prev >= 1000)
    {
      countDown--;
      if (countDown >= 0)
        Serial.print(String(countDown) + (char)32);
      prev = millis();
    }
  } while (!Serial.available() && countDown >= 0);
  if (countDown <= 0)
    return false;
  else
    return true;
}
