void assignParams()
{
  f2wPulseRatio = (int)params[0];
  engineOffTimeout = (int)params[1];
  fuelPulseBias = (double)params[2];
  waterPulseBias = (double)params[3];
  solOnTime = (int)params[4];
  waterPercentage = (double)params[5];
  checkpointPeriod = (int)params[6];
  testMode = (bool)params[7];
  dieselMode = (bool)params[8];
}

void assignParamsFb()
{
  f2wPulseRatio = (int)params["f2wPulseRatio"];
  engineOffTimeout = (int)params["engineOffTimeout"];
  fuelPulseBias = (double)params["fuelPulseBias"];
  waterPulseBias = (double)params["waterPulseBias"];
  solOnTime = (int)params["solOnTime"];
  waterPercentage = (double)params["waterPercentage"];
  checkpointPeriod = (int)params["checkpointPeriod"];
  testMode = (double)params["testMode"];
  dieselMode = (bool)params["dieselMode"];
}
