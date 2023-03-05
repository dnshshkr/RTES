//void calculate_f2wPulseRatio()
//{
//  f2wPulseRatio = floor((solShotBias * (1 - (waterPercentage / 100))) / (flowRateBias * (1 - (1 - (waterPercentage / 100))))) - 1;
//}
//
//void calculate_solOnTime()
//{
//  /*
//     | based on our thorough experiments done on a repetition, it is concluded that for a 250 ms of solenoid trigger, the volume of water measured is about 0.81 mL
//  */
//  solOnTime = (250 / 0.81) * solShotBias;
//}
//
//void calculate_solShotBias()
//{
//  solShotBias = round2d(solOnTime / (250 / 0.81));
//}
//
void calculate_denominator()  //calculation of the denominator of the fraction equation used to calculate the fuel-water ratio
{
  denominator = round2d(flowRateBias * (float)(f2wPulseRatio + 1) + solShotBias);
}
//
//void calculate_waterPercentage()
//{
//  waterPercentage = round2d(solShotBias / denominator * 100.0);
//}

double round2d(float val)
{
  return (int)(val * 100 + 0.5) / 100.0;
}
float round1d(float val)
{
  return (int)(val * 10 + 0.5) / 10.0;
}
