void pushData()
{
  float waterPercentage = (solShotBias / denominator) * 100.0;
  float fuelPercentage = ((flowRateBias * (float)f2wPulseRatioCount) / denominator) * 100.0;

  readings[0]=totalFuelPulse;
  readings[1]=fuelPulsePeriod;
  readings[2]=fuelFlowRate;
  readigns[3]=f2wPulseRatioCount;
  Serial.print(String(totalFuelPulse) + " total fuel pulse \t");
  Serial.print(String(fuelPulsePeriod) + " s\t");
  Serial.print(fuelFlowRate, 3);
  Serial.print(" mL/min\t");
  Serial.print(String(f2wPulseRatioCount) + " counter\t");
  Serial.print(String(totalWaterPulse) + " total water pulse\t");
  Serial.print(waterPercentage, 1);
  Serial.print(" % water\t");
  Serial.print(fuelPercentage, 1);
  Serial.println("% fuel");
}
