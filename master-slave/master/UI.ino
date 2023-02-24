void printSettings()
{
  Serial.println("*************************ALL SETTINGS**************************");
  Serial.println("RTES v" + String(ver));
  Serial.println("A: Water Percentage: " + String((double)params[5]) + "%");
  Serial.print("B: Fuel Pulse Count: " + String((int)params[0]) + " pulse");
  ((int)params[0] == 0 || (int)params[0] > 1) ? Serial.println('s') : Serial.println();
  Serial.println("C: Fuel Flow Rate Bias: " + String((double)params[2]) + " mL/pulse");
  Serial.println("D: Water Shot Bias: " + String((double)params[3]) + " mL/pulse");
  Serial.println("E: Solenoid On Time: " + String((int)params[4]) + " ms");
  Serial.print("F: Engine Off Timeout: " + String((int)params[1]) + " s");
  if (testMode)
    Serial.print("\nG: Checkpoint Period: " + String((int)params[6]) + " minute");
  ((int)params[6] > 1 && testMode) ? Serial.println('s') : Serial.println();
  Serial.println("H: Reset Total Fuel Pulse Counter");
  Serial.print("T: Toggle Test Mode: ");
  Serial.println(testMode);
  Serial.println("R: Reset to Factory Settings");
  //Serial.println("M: Enter Admin Settings");
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
bool timeoutUI()
{
  unsigned long prev, prevCD;
  int8_t countDown = 10;
  Serial.print(": " + String(countDown) + (char)32);
  prev = millis();
  do {
    if (millis() - prev >= 1000) {
      countDown--;
      if (countDown >= 0) {
        Serial.print(String(countDown) + (char)32);
      }
      prev = millis();
    }
  } while (!Serial.available() && countDown >= 0);
  if (countDown <= 0)
    return false;
  else
    return true;
}
