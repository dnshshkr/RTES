void parseMaster(uint8_t reqCode)
{
  switch (reqCode)
  {
    case 0x80:
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
        master.write(0xff), master.println(params);
        break;
      }
    case 0x81:
      {
        resetParams();
        master.write(0xfc);
        break;
      }
    case 0x82:
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
        master.write(0xf7);
        //Serial.println(params);
        break;
      }
    case 0x83:
      {
        runRTES = !runRTES;
        if (runRTES)
        {
          pulseMeasurePrevMillis = millis();
          master.write(0xfa);
        }
        else
          master.write(0xfb);
        break;
      }
    case 0x84:
      {
        runRTES = true;
        pulseMeasurePrevMillis = millis();
        master.write(0xfa);
        break;
      }
    case 0x85:
      {
        runRTES = false;
        master.write(0xfb);
        break;
      }
    case 0x86:
      {
        totalFuelPulse = 0;
        totalWaterPulse = 0;
        cycleCount = 0;
        master.write(0xf9);
      }
  }
}
