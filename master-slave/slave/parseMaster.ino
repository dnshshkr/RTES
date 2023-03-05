void parseMaster(uint8_t reqCode)
{
  switch (reqCode)
  {
    case REQUEST_PARAMS:
      {
        loadParams();
        params[0] = f2wPulseRatio;
        params[1] = engineOffTimeout;
        params[2] = flowRateBias;
        params[3] = solShotBias;
        params[4] = solOnTime;
        params[5] = waterPercentage;
        params[6] = checkpointPeriod;
        params[7] = testMode;
        params[8] = dieselMode;
        master.write(PARAMS), master.println(params);
        break;
      }
    case RESET_PARAMS:
      {
        resetParams();
        master.write(PARAMS_RESET);
        break;
      }
    case SEND_NEW_PARAMS:
      {
        String body = master.readStringUntil('\r\n');
        body.trim();
        params = JSON.parse(body);
        f2wPulseRatio = (int)params[0];
        engineOffTimeout = (int)params[1];
        flowRateBias = (double)params[2];
        solShotBias = (double)params[3];
        solOnTime = (int)params[4];
        waterPercentage = (double)params[5];
        checkpointPeriod = (int)params[6];
        testMode = (bool)params[7];
        dieselMode = (bool)params[8];
        saveNewParams();
        master.write(NEW_PARAMS_RECEIVED);
        break;
      }
    case TOGGLE_RTES:
      {
        runRTES = !runRTES;
        if (runRTES)
        {
          engOffPrevMillis = pulseMeasurePrevMillis = millis();
          master.write(RTES_STARTED);
        }
        else
          master.write(RTES_STOPPED);
        break;
      }
    case EXCLUSIVE_START_RTES:
      {
        runRTES = true;
        engOffPrevMillis = pulseMeasurePrevMillis = millis();
        master.write(RTES_STARTED);
        break;
      }
    case EXCLUSIVE_STOP_RTES:
      {
        runRTES = false;
        master.write(RTES_STOPPED);
        break;
      }
    case RESET_COUNTERS:
      {
        totalFuelPulse = 0;
        totalWaterPulse = 0;
        cycleCount = 0;
        master.write(COUNTERS_RESET);
        break;
      }
  }
}
