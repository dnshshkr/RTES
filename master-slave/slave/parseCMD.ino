void parseCMD(uint8_t reqCode)
{
  switch (reqCode)
  {
    case 0x80:
      {
        loadParams();
        JSONVar params;
        params[0] = f2wPulseRatio;
        params[1] = engineOffTimeout;
        params[2] = flowRateBias;
        params[3] = solShotBias;
        params[4] = solOnTime;
        params[5] = waterPercentage;
        params[6] = checkpointPeriod;
        params[7] = testMode;
        master.write(0xff), master.println(params);
        break;
      }
    case 0x81:
      {
        resetParams();
        master.write(0xfd);
        break;
      }
    case 0x83:
      {
        runRTES = !runRTES;
        if (runRTES)
          master.write(0xfb);
        else
          master.write(0xfc);
        break;
      }
  }
}
