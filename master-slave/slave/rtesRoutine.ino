void RTES()
{
  if (!sprayStarted && cycleCount > f2wPulseRatio)
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
  if (sprayCompleted && cycleCount >= 1 && cycleCount <= f2wPulseRatio)
  {
    sprayStarted = false;
    sprayCompleted = false;
  }
}
