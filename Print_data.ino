void printData()
{
  //Serial.print(digitalRead(pulseFlowrate))          ; Serial.print('\t');
  //  Serial.print(digitalRead(manualButton))           ; Serial.print('\t');
  //  Serial.print(digitalRead(waterLevel))             ; Serial.print('\t');
  //    Serial.print(flagManual)                        ;Serial.print('\t');
  Serial.print(ampMotorFuel, 3);
  Serial.print("A \t");
  Serial.print(ampSolenoid, 3);
  Serial.print("A \t");
  Serial.print(ampMotorWater, 3);
  Serial.print("A \t");
  Serial.print(String(totalFuelPulse) + " total fuel pulse \t");
  Serial.print(fuelPulsePeriod);
  Serial.print(" s\t");
  Serial.print(measuredPulsePerMin, 3);
  Serial.print(" ml/min");
  Serial.print('\t');
  //  Serial.print(rtesCurrentTime2)                    ;Serial.print('\t');
  //  Serial.print(rtesPreviousTime2)                   ;Serial.print('\t');
  //  Serial.print(rtesCurrentTime2 - rtesPreviousTime2);Serial.print('\t');
  //    Serial.print(stallVar)                            ;Serial.print('\t');
  Serial.print(pulse_fuelToWaterRatioCount);
  Serial.print(" counter");
  Serial.print('\t');
  // Serial.print(pulseCounter2);
  Serial.print(pulseInc);
  Serial.print(" total water pulse");
  Serial.print('\t');
  //Serial.print(((pulseInc * solShotBias) / ((pulseCounter * flowRateBias) + (pulseInc * solShotBias))) * 100);
  //Serial.print(" %");
  denom = flowRateBias * pulse_fuelToWaterRatio + solShotBias * solenoidOnPulse;
  Serial.print(((solShotBias * sprayStarted) / denom) * 100.0);
  Serial.print(" % water\t");
  Serial.print(((flowRateBias * pulse_fuelToWaterRatioCount) / denom) * 100.0);
  Serial.print("% fuel\t");
  //  Serial.print(measCntCurrentTime); Serial.print('\t');
  //  Serial.print(measCntPreviousTime); Serial.print('\t');
  //  Serial.print(measCntCurrentTime - measCntPreviousTime); Serial.print('\t');
  Serial.println(">");  //for data logger
  pulseDataPrint = false;
}
