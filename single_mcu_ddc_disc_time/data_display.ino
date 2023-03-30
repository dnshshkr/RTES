void printData()
{
  float waterPercentage, P_f;
  static unsigned long prevTotalWaterPulse;
  if (cycleCount <= f2wPulseRatio)
  {
    waterPercentage = 0.0;
    P_f = ((fuelPulseBias * (float)cycleCount) / denominator) * 100.0;
  }
  else
  {
    waterPercentage = waterPercentageDuringEmulsion;
    P_f = 100.0 - waterPercentage;
  }

  //  Serial.print(String(cycleCount) + " counter\t");
  //  Serial.print(String(totalFuelPulse) + " total fuel pulse \t");
  //  Serial.print(String(fuelPulsePeriod) + " s\t");
  //  Serial.print(fuelFlowRate, 3);
  //  Serial.print(" mL/min\t");
  //  Serial.print(String(totalWaterPulse) + " total water pulse\t");
  //  Serial.print(waterPercentage, 1);
  //  Serial.print(" % water\t");
  //  Serial.print(P_f, 1);
  //  Serial.println("% fuel");

  Serial.print(String(cycleCount) + '\t');
  Serial.print(String(totalFuelPulse) + '\t');
  Serial.print(String(fuelPulsePeriod) + '\t');
  Serial.print(String(fuelFlowRate) + '\t');
  if (!idleMode)
    Serial.print(String(totalWaterPulse) + '\t');
  else if (idleMode && totalWaterPulse != prevTotalWaterPulse)
    Serial.print("idle\t");
  Serial.print(String(waterPercentage) + '\t');
  Serial.println(P_f);

  //  bt.print(String(totalFuelPulse) + " total fuel pulse \t");
  //  bt.print(String(fuelPulsePeriod) + " s\t");
  //  bt.print(fuelFlowRate, 3);
  //  bt.print(" mL/min\t");
  //  bt.print(String(cycleCount) + " counter\t");
  //  bt.print(String(totalWaterPulse) + " total water pulse\t");
  //  bt.print(waterPercentage, 1);
  //  bt.print(" % water\t");
  //  bt.print(P_f, 1);
  //  bt.println("% fuel");

  bt.print(String(totalFuelPulse) + '\t');
  if (!idleMode)
    bt.print(String(totalWaterPulse) + '\t');
  else if (idleMode && totalWaterPulse != prevTotalWaterPulse)
    bt.print("idle\t");
  bt.print(String(fuelPulsePeriod) + '\t');
  bt.print(fuelFlowRate, 3);
  bt.write('\t');
  bt.print(String(cycleCount) + '\t');
  bt.print(waterPercentage, 1);
  bt.write('\t');
  bt.println(P_f, 1);
  // pulseDataPrint = false;  //change to false after printing the data and wait for next fuel pulse
  prevTotalWaterPulse = totalWaterPulse;
}
