int calculate_f2wPulseRatio(float waterPercentage, float fuelPulseBias, float waterPulseBias)
{
  //return floor((waterPulseBias * (1 - (waterPercentage / 100.0))) / (fuelPulseBias * (1 - (1 - (waterPercentage / 100.0))))) - 1;
  return floor(waterPulseBias * (100.0 / waterPercentage - 1) / fuelPulseBias) - 1;
}

<<<<<<< HEAD
int calculate_solOnTime(float waterPulseBias)
=======
int calculate_solOnTime(float solConst, float waterPulseBias)
>>>>>>> origin/master
{
  /*
     | based on our thorough experiments done on a repetition, it is concluded that for a 250 ms of solenoid trigger, the volume of water measured is about 0.81 mL
  */
<<<<<<< HEAD
  return (int)((250 / 0.81) * waterPulseBias);
=======
  //return (int)((13 / 0.81) * waterPulseBias);
  return (int)((1 / solConst) * waterPulseBias);
>>>>>>> origin/master
}

float calculate_waterPulseBias(int solOnTime)
{
<<<<<<< HEAD
  return (float)solOnTime / (250 / 0.81);
=======
  //return (float)solOnTime / (13 / 0.81);
  return (float)solOnTime / (1 / solConst);
>>>>>>> origin/master
}

float calculate_denominator(int f2wPulseRatio, float fuelPulseBias, float waterPulseBias) //calculation of the denominator of the fraction equation used to calculate the fuel-water ratio
{
  return fuelPulseBias * (float)(f2wPulseRatio + 1) + waterPulseBias;
}

float calculate_waterPercentage(float waterPulseBias, float denominator)
{
  return waterPulseBias / denominator * 100.0;
}
