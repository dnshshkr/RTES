void calculatePulse_fuelToWaterRatio()
{
  pulse_fuelToWaterRatio = floor((solShotBias * (1 - (quickWaterPercentage / 100))) / (flowRateBias * (1 - (1 - (quickWaterPercentage / 100))))) - 1;
}

void calculateSolenoidOnTime()
{
  solenoidOnTime = (250 / 0.81) * solShotBias;
}

void calculateDenom()
{
  denom = flowRateBias * (float)(pulse_fuelToWaterRatio + 1) + solShotBias * (float)solenoidOnPulse;
}