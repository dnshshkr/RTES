/********************Interrupt function*******************************************************************************/
void countPulse()
{
  prevMillisEngOff = millis();
  fuelPulsePeriod = float((millis() - pulseMeasurePrevMillis)) / 1000;
  measuredPulsePerMin = 1.0 / ((1.0 / flowRateBias) * fuelPulsePeriod * (1.0 / 60.0));
  pulseMeasurePrevMillis = millis();
  totalFuelPulse ++;
  pulse_fuelToWaterRatioCount++;
  if (pulse_fuelToWaterRatioCount >= pulse_fuelToWaterRatio + solenoidOnPulse + 1)
    pulse_fuelToWaterRatioCount = 1;
  pulseDataPrint = true;
}
