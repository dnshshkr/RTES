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

  Serial.print(cycleCount), Serial.write((char)9);
  Serial.print(totalFuelPulse), Serial.write((char)9);
  Serial.print(fuelPulsePeriod), Serial.write((char)9);
  Serial.print(fuelFlowRate), Serial.write((char)9);
  totalWaterPulse != prevTotalWaterPulse ? Serial.print(totalWaterPulse) : Serial.print("idle"), Serial.write((char)9);
  Serial.print(waterPercentage), Serial.write((char)9);
  Serial.print(P_f), Serial.write((char)9);
  Serial.print(motorTemp), Serial.write((char)9);
  Serial.print(rtesTemp), Serial.write((char)9);
  Serial.print(envTemp), Serial.write((char)10);

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

  bt.print(String(totalFuelPulse) + ',');
  totalWaterPulse != prevTotalWaterPulse ? bt.print(String(totalWaterPulse)) : bt.print("idle"), bt.write((char)44);
  bt.print(String(fuelPulsePeriod) + ',');
  bt.print(fuelFlowRate, 3);
  bt.write(',');
  bt.print(String(cycleCount) + ',');
  bt.print(waterPercentage, 1);
  bt.write(',');
  bt.print(P_f, 1);
  bt.write(',');
  bt.print(motorTemp, 1);
  bt.write(',');
  bt.print(rtesTemp, 1);
  bt.write(',');
  bt.print(envTemp, 1);
  bt.println();
  // pulseDataPrint = false;  //change to false after printing the data and wait for next fuel pulse
  prevTotalWaterPulse = totalWaterPulse;
}
