void printData()
{
  float waterPercentage = ((solShotBias * (float)sprayedOnce) / denom) * 100.0;
  float fuelPercentage = ((flowRateBias * (float)pulse_fuelToWaterRatioCount) / denom) * 100.0;

  Serial.print(String(totalFuelPulse) + " total fuel pulse \t");
  Serial.print(String(fuelPulsePeriod) + " s\t");
  Serial.print(measuredPulsePerMin, 3);
  Serial.print(" ml/min\t");
  Serial.print(String(pulse_fuelToWaterRatioCount) + " counter\t");
  Serial.print(String(totalWaterPulse) + " total water pulse\t");
  Serial.print(waterPercentage, 1);
  Serial.print(" % water\t");
  Serial.print(fuelPercentage, 1);
  Serial.println("% fuel");

  bt.print(String(totalFuelPulse) + " total fuel pulse \t");
  bt.print(String(fuelPulsePeriod) + " s\t");
  bt.print(measuredPulsePerMin, 3);
  bt.print(" ml/min\t");
  bt.print(String(pulse_fuelToWaterRatioCount) + " counter\t");
  bt.print(String(totalWaterPulse) + " total water pulse\t");
  bt.print(waterPercentage, 1);
  bt.print(" % water\t");
  bt.print(fuelPercentage, 1);
  bt.println("% fuel");
  pulseDataPrint = false;
}
