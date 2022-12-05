void measurePulse()
{
  if (millis() - measPlsPreviousTime >= pulsePeriodTime)
  {
    //Read pulse
    measuredPulsePerMin = pulseCounter * flowRateBias * 60;
    pulseCounter = 0;
    measPlsPreviousTime = millis();
    // } else
    //   pulseCounter++;
  }
}
void countPulse()
{
  //  measCntCurrentTime = millis();
  //  if (measCntCurrentTime - measCntPreviousTime >= pulseCountTime)
  //  {
  //    if (measCntCurrentTime - measCntPreviousTime >= pulseCountTime + 100)
  //      measCntPreviousTime = measCntCurrentTime;
  //  }
  //  else
  //  {
  //    if (pulseCnt <= pulseCntMax)
  //      pulseCnt++;
  //    measCntPreviousTime = measCntCurrentTime;
  //  }
  //
  //  //  if (millis() - measCntPreviousTime >= pulseCountTime) {
  //  //    if (millis() - measCntPreviousTime >= pulseCountTime + 100) {
  //  //      measCntPreviousTime = millis();
  //  //    }
  //  //  } else {
  //  //    if (pulseCnt <= pulseCntMax)
  //  //      pulseCnt++;
  //  //    measCntPreviousTime = millis();
  //  //  }
}
