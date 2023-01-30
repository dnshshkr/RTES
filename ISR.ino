/********************Interrupt function*******************************************************************************/
void countPulse()
{
  prevMillisEngOff = millis();
  fuelPulsePeriod = float((millis() - measPlsPreviousTime)) / 1000;
  measuredPulsePerMin = 1.0 / ((1.0 / flowRateBias) * fuelPulsePeriod * (1.0 / 60.0));
  measPlsPreviousTime = millis();
  totalFuelPulse ++;
  pulse_fuelToWaterRatioCount++;
  if (pulse_fuelToWaterRatioCount >= pulse_fuelToWaterRatio + solenoidOnPulse + 1)
    pulse_fuelToWaterRatioCount = 1;
  pulseDataPrint = true;
}
