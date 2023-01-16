void quickCalculateWaterPercentage()
{
  pulse_fuelToWaterRatio = floor((solShotBias * (1 - (quickWaterPercentage / 100))) / (flowRateBias * (1 - (1 - (quickWaterPercentage / 100))))) - 1;
}
void calculateDenom()
{
  denom = flowRateBias * (float)(pulse_fuelToWaterRatio + 1) + solShotBias * (float)solenoidOnPulse;
}
