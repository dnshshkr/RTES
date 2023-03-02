int calculate_f2wPulseRatio(float waterPercentage, float fuelPulseBias, float waterPulseBias)
{
  //return floor((waterPulseBias * (1 - (waterPercentage / 100.0))) / (fuelPulseBias * (1 - (1 - (waterPercentage / 100.0))))) - 1;
  return floor(waterPulseBias * (100.0 / waterPercentage - 1) / fuelPulseBias) - 1;
}

int calculate_solOnTime(float waterPulseBias)
{
  /*
     | based on our thorough experiments done on a repetition, it is concluded that for a 250 ms of solenoid trigger, the volume of water measured is about 0.81 mL
  */
  return (int)((250 / 0.81) * waterPulseBias);
}

float calculate_waterPulseBias(int solOnTime)
{
  return (float)solOnTime / (250 / 0.81);
}

float calculate_denominator(int f2wPulseRatio, float fuelPulseBias, float waterPulseBias) //calculation of the denominator of the fraction equation used to calculate the fuel-water ratio
{
  return fuelPulseBias * (float)(f2wPulseRatio + 1) + waterPulseBias;
}

float calculate_waterPercentage(float waterPulseBias, float denominator)
{
  return waterPulseBias / denominator * 100.0;
}
