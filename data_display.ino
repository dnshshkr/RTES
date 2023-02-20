void printData() {
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
  Serial.println(" % fuel");

  //if(bt)
  //{
  bt.print(String(totalFuelPulse) + ",");
  bt.print(String(totalWaterPulse) + ",");
  bt.print(String(fuelPulsePeriod) + ",");
  bt.print(measuredPulsePerMin, 3);
  bt.print(",");
  bt.print(String(f2wPulseRatioCount) + ",");
  bt.print(waterPercentage, 1);
  bt.print(",");
  bt.print(fuelPercentage, 1);
  bt.println();
  //}
  pulseDataPrint = false;  //change to false after printing the data and wait for next fuel pulse
}
