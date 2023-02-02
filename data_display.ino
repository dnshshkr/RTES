void printData()
{
  float waterPercentage = ((solShotBias * (float)sprayedOnce) / denominator) * 100.0;
  float fuelPercentage = ((flowRateBias * (float)f2wPulseRatioCount) / denominator) * 100.0;

  Serial.print(String(totalFuelPulse) + " total fuel pulse \t");
  Serial.print(String(fuelPulsePeriod) + " s\t");
  Serial.print(measuredPulsePerMin, 3);
  Serial.print(" mL/min\t");
  Serial.print(String(f2wPulseRatioCount) + " counter\t");
  Serial.print(String(totalWaterPulse) + " total water pulse\t");
  Serial.print(waterPercentage, 1);
  Serial.print(" % water\t");
  Serial.print(fuelPercentage, 1);
  Serial.println("% fuel");

  //  if (digitalRead(btState)) //here
  //  {
  bt.print(String(totalFuelPulse) + " total fuel pulse \t");
  bt.print(String(fuelPulsePeriod) + " s\t");
  bt.print(measuredPulsePerMin, 3);
  bt.print(" mL/min\t");
  bt.print(String(f2wPulseRatioCount) + " counter\t");
  bt.print(String(totalWaterPulse) + " total water pulse\t");
  bt.print(waterPercentage, 1);
  bt.print(" % water\t");
  bt.print(fuelPercentage, 1);
  bt.println("% fuel");
  //}

  pulseDataPrint = false; //change to false after printing the data and wait for next fuel pulse
}
