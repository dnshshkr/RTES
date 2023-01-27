void printData()
{
  float waterPercentage = ((solShotBias * (float)sprayedOnce) / denom) * 100.0;
  float fuelPercentage = ((flowRateBias * (float)pulse_fuelToWaterRatioCount) / denom) * 100.0;
  //  Serial.print(ampMotorFuel, 3);
  //  Serial.print("A \t");
  //  Serial.print(ampSolenoid, 3);
  //  Serial.print("A \t");
  //  Serial.print(ampMotorWater, 3);
  //  Serial.print("A \t");
  Serial.print(String(totalFuelPulse) + " total fuel pulse \t");
  Serial.print(fuelPulsePeriod);
  Serial.print(" s\t");
  Serial.print(measuredPulsePerMin, 3);
  Serial.print(" ml/min");
  Serial.print('\t');
  Serial.print(pulse_fuelToWaterRatioCount);
  Serial.print(" counter");
  Serial.print('\t');
  Serial.print(pulseInc);
  Serial.print(" total water pulse");
  Serial.print('\t');
  Serial.print(waterPercentage, 1);
  Serial.print(" % water\t");
  Serial.print(fuelPercentage, 1);
  Serial.print("% fuel");
  //Serial.println(">");  //for data logger
  pulseDataPrint = false;
}
