void printData() {
  float waterPercentage;
  if (cycleCount <= f2wPulseRatio)
    waterPercentage = 0.0;
  else
    waterPercentage = (solShotBias / denominator) * 100.0;
  float fuelPercentage = ((flowRateBias * (float)cycleCount) / denominator) * 100.0;

  Serial.print(String(totalFuelPulse) + " total fuel pulse \t");
  Serial.print(String(fuelPulsePeriod) + " s\t");
  Serial.print(fuelFlowRate, 3);
  Serial.print(" mL/min\t");
  Serial.print(String(cycleCount) + " counter\t");
  Serial.print(String(totalWaterPulse) + " total water pulse\t");
  Serial.print(waterPercentage, 1);
  Serial.print(" % water\t");
  Serial.print(fuelPercentage, 1);
  Serial.println("% fuel");

  //  bt.print(String(totalFuelPulse) + " total fuel pulse \t");
  //  bt.print(String(fuelPulsePeriod) + " s\t");
  //  bt.print(fuelFlowRate, 3);
  //  bt.print(" mL/min\t");
  //  bt.print(String(cycleCount) + " counter\t");
  //  bt.print(String(totalWaterPulse) + " total water pulse\t");
  //  bt.print(waterPercentage, 1);
  //  bt.print(" % water\t");
  //  bt.print(fuelPercentage, 1);
  //  bt.println("% fuel");

  bt.print(String(totalFuelPulse) + ",");
  bt.print(String(totalWaterPulse) + ",");
  bt.print(String(fuelPulsePeriod) + ",");
  bt.print(fuelFlowRate, 3);
  bt.print(",");
  bt.print(String(cycleCount) + ",");
  bt.print(waterPercentage, 1);
  bt.print(",");
  bt.print(fuelPercentage, 1);
  bt.println();
  pulseDataPrint = false;  //change to false after printing the data and wait for next fuel pulse
}
