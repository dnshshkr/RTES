void printSettings()
{
  Serial.println("***************ALL SETTINGS***************");
  Serial.println("RTES v" + String(RTES_VERSION));
  Serial.println("A - Water Percentage: " + String(waterPercentage) + "%");
  Serial.print("B - Fuel Pulse Count: " + String(f2wPulseRatio) + " pulse");
  (f2wPulseRatio == 0 || f2wPulseRatio > 1) ? Serial.println('s') : Serial.println();
  Serial.println("C - Fuel Flow Rate Bias: " + String(flowRateBias) + " mL/pulse");
  Serial.println("D - Water Shot Bias: " + String(solShotBias) + " mL/pulse");
  Serial.println("E - Solenoid On Time: " + String(solOnTime) + " ms");
  Serial.print("F - Engine Off Timeout: " + String(engineOffTimeout) + " s");
  if (testMode)
    Serial.print("\nG - Checkpoint Period: " + String(checkpointPeriod) + " minute");
  (checkpointPeriod > 1 && testMode) ? Serial.println('s') : Serial.println();
  Serial.println("H - Reset Total Fuel Pulse Counter");
  Serial.print("I - Toggle Diesel-only Mode: ");
  dieselMode ? Serial.println("ON") : Serial.println("OFF");
  Serial.println("J - Browse Storage");
  Serial.print("T - Toggle Test Mode: ");
  testMode ? Serial.println("ON") : Serial.println("OFF");
  Serial.println("R - Reset to Factory Settings");
  Serial.println("$ - Refresh Settings");
  Serial.print("S - Enter ");
  if (mode)
    Serial.write('(');
  Serial.print("Settings");
  if (mode)
    Serial.write(')');
  Serial.write('/');
  if (mode == 0)
    Serial.write('(');
  Serial.print("RTES");
  if (mode == 0)
    Serial.write(')');
  Serial.println(" Mode");
  Serial.println("******************************************");
}

void spiffsUI()
{
  Serial.println("***************FILE BROWSER***************");
  listDir(SPIFFS, "/", 0);
  Serial.println("\nMenu:\nO - Open File");
  Serial.println("R - Rename File");
  Serial.println("D - Delete File");
  Serial.println("U - Upload File");
  Serial.println("E - Exit");
  Serial.println("******************************************");
}

bool timeoutUI(int8_t countDown)
{
  unsigned long prev, prevCD;
  Serial.print(": " + String(countDown) + " ");
  prev = millis();
  do
  {
    if (millis() - prev >= 1000)
    {
      countDown--;
      if (countDown >= 0)
        Serial.print(String(countDown) + " ");
      prev = millis();
    }
  } while (!Serial.available() && countDown >= 0);
  if (countDown <= 0)
    return false;
  else
    return true;
}
