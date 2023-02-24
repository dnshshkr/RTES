void interruptRoutine() {
  engOffPrevMillis = millis();  //engine-off detection
  fuelPulsePeriod = (float)(millis() - pulseMeasurePrevMillis) / 1000.0;
  fuelFlowRate = 1.0 / ((1.0 / flowRateBias) * fuelPulsePeriod * (1.0 / 60.0));
  pulseMeasurePrevMillis = millis();
  totalFuelPulse++;
  f2wPulseRatioCount++;
  if (f2wPulseRatioCount >= f2wPulseRatio + 2)
    f2wPulseRatioCount = 1;
  pulseDataPrint = true;
}
