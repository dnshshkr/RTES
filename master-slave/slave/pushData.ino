void pushData()
{
  float waterPercentage;
  if (cycleCount <= f2wPulseRatio)
    waterPercentage = 0.0;
  else
    waterPercentage = (solShotBias / denominator) * 100.0;
  float fuelPercentage = ((flowRateBias * (float)cycleCount) / denominator) * 100.0;

  readings[0] = cycleCount;
  readings[1] = totalFuelPulse;
  readings[2] = fuelPulsePeriod;
  readings[3] = fuelFlowRate;
  readings[4] = totalWaterPulse;
  readings[5] = waterPercentage;
  readings[6] = fuelPercentage;
  master.write(0xfe), master.println(readings);
  pulseDataPrint = false;
  //
  //  Serial.print(String(totalFuelPulse) + " total fuel pulse \t");
  //  Serial.print(String(fuelPulsePeriod) + " s\t");
  //  Serial.print(fuelFlowRate, 3);
  //  Serial.print(" mL/min\t");
  //  Serial.print(String(cycleCount) + " counter\t");
  //  Serial.print(String(totalWaterPulse) + " total water pulse\t");
  //  Serial.print(waterPercentage, 1);
  //  Serial.print(" % water\t");
  //  Serial.print(fuelPercentage, 1);
  //  Serial.println("% fuel");
}
