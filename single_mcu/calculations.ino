int calculate_f2wPulseRatio(float waterPercentage, float flowRateBias, float solShotBias)
{
  return floor((solShotBias * (1 - (waterPercentage / 100.0))) / (flowRateBias * (1 - (1 - (waterPercentage / 100.0))))) - 1;
}

int calculate_solOnTime(float solShotBias)
{
  /*
     | based on our thorough experiments done on a repetition, it is concluded that for a 250 ms of solenoid trigger, the volume of water measured is about 0.81 mL
  */
  return (int)((250 / 0.81) * solShotBias);
}

float calculate_solShotBias(int solOnTime)
{
  return (float)solOnTime / (250 / 0.81);
}

float calculate_denominator(int f2wPulseRatio, float flowRateBias, float solShotBias) //calculation of the denominator of the fraction equation used to calculate the fuel-water ratio
{
  return flowRateBias * (float)(f2wPulseRatio + 1) + solShotBias;
}

float calculate_waterPercentage(float solShotBias, float denominator)
{
  return solShotBias / denominator * 100.0;
}
