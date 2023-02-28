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
        master.write(0xff);
        serializeJson(params, master);
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
        deserializeJson(params, body);
        f2wPulseRatio = params[0].as<unsigned int>();
        engineOffTimeout = params[1].as<unsigned short>();
        flowRateBias = params[2].as<float>();
        solShotBias = params[3].as<float>();
        solOnTime = params[4].as<unsigned int>();
        waterPercentage = params[5].as<float>();
        checkpointPeriod = params[6].as<unsigned short>();
        testMode = params[7].as<bool>();
        dieselMode = params[8].as<bool>();
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
        runRTES = false;
        master.write(0xfb);
        break;
      }
    case 0x85:
      {
        totalFuelPulse = 0;
        totalWaterPulse = 0;
        cycleCount = 0;
        master.write(0xf9);
      }
  }
}
