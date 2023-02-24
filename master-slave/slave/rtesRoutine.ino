void RTES()
{
  if (!sprayStarted && f2wPulseRatioCount > f2wPulseRatio)
  {
    if (!sprayCompleted)
    {
      startEmulsion();
      solOnTimePrevMillis = millis();
      totalWaterPulse++;
      sprayStarted = true;
    }
  }
  else if (sprayStarted && millis() - solOnTimePrevMillis > solOnTime)
  {
    stopEmulsion();
    sprayCompleted = true;
  }
  if (sprayCompleted && f2wPulseRatioCount >= 1 && f2wPulseRatioCount <= f2wPulseRatio)
  {
    sprayStarted = false;
    sprayCompleted = false;
  }
}
