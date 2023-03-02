void assignParams()
{
  f2wPulseRatio = (int)params[0];
  engineOffTimeout = (int)params[1];
  flowRateBias = (double)params[2];
  solShotBias = (double)params[3];
  solOnTime = (int)params[4];
  waterPercentage = (double)params[5];
  checkpointPeriod = (int)params[6];
  testMode = (bool)params[7];
  dieselMode = (bool)params[8];
}
