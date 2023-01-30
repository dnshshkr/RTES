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
  Serial.println("% fuel");
  //Serial.println(">");  //for data logger
  bt.print(String(totalFuelPulse) + " total fuel pulse \t");
  bt.print(String(fuelPulsePeriod) + " s\t");
  bt.print(measuredPulsePerMin, 3);
  bt.print(" ml/min\t");
  bt.print(String(pulse_fuelToWaterRatioCount) + " counter\t");
  bt.print(String(pulseInc) + " total water pulse\t");
  bt.print(waterPercentage, 1);
  bt.print(" % water\t");
  bt.print(fuelPercentage, 1);
  bt.println("% fuel");
  pulseDataPrint = false;
}
