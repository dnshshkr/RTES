//void rtesSystem()
//{
//  //display.showNumberHexEx(0xA000);
//  if(measuredPulsePerMin < 80 && stallVar == 0)
//  {
//    opening = 0;
//    freq = 0;
//  }
//  if(measuredPulsePerMin > 150 && stallVar == 0)
//  {
//    opening = 0;
//    freq = 0;
//  }
//  if(measuredPulsePerMin >= 80 && measuredPulsePerMin <= 150 && stallVar == 0)
//  {
//    opening = 100;
//    freq = 30000;
//    stallVar = 1;
//  }
//  rtesSystemTimer(opening, freq);  //(opening, frequency)
//}
//
//void rtesSystemTimer(unsigned long setOpening, unsigned long freq)
//{
//  rtesCurrentTime = millis();
//  if (rtesCurrentTime - rtesPreviousTime >= freq)
//  {
//    rtesPreviousTime = rtesCurrentTime;
//    stallVar = 0;
//  }
//  else
//  {
//    if (rtesCurrentTime - rtesPreviousTime < setOpening)
//    {
//      currentLimitedOut(1, 1, 1);  //(fuel,sol,wat)
//      if (pulseIncToggle == 1)
//      {
//        pulseInc++;
//        pulseIncToggle = 0;
//      }
//    }
//    else
//    {
//      currentLimitedOut(1, 0, 1);  //(fuel,sol,wat)
//      pulseIncToggle = 1;
//    }
//  }
//}
void rtesSystem()
{
  //  METHOD 1
  //    if (pulse_fuelToWaterRatioCount > pulse_fuelToWaterRatio && !sprayStarted)
  //    {
  //      prevSolOnTime = millis();
  //      //currentLimitedOut(fuelTrig, 1, waterTrig);
  //      digitalWrite(solenoidWater, HIGH);
  //      digitalWrite(motorWater, HIGH);
  //      sprayStarted = true;
  //    }
  //    else if (sprayStarted)
  //    {
  //      if (millis() - prevSolOnTime >= solenoidOnTime)
  //      {
  //        digitalWrite(solenoidWater, LOW);
  //        digitalWrite(motorWater, LOW);
  //        //currentLimitedOut(fuelTrig, 0, waterTrig);
  //      }
  //      if (pulse_fuelToWaterRatioCount >= pulse_fuelToWaterRatio + solenoidOnPulse + 1)
  //      {
  //        sprayStarted = false;
  //        pulseInc++;
  //        pulse_fuelToWaterRatioCount = 1;
  //      }
  //    }
  //    else
  //    {
  //      digitalWrite(solenoidWater, LOW);
  //      digitalWrite(motorWater, LOW);
  //    }

  //METHOD 2
  if (pulse_fuelToWaterRatioCount >= 1 && pulse_fuelToWaterRatioCount <= pulse_fuelToWaterRatio)
  {
    sprayedOnce = false;
    startSpray = false;
    sprayCompleted = true;
    digitalWrite(solenoidWater, LOW);
    digitalWrite(motorWater, LOW);
  }
  if (pulse_fuelToWaterRatioCount > pulse_fuelToWaterRatio)
    startSpray = true;
  else if (pulse_fuelToWaterRatioCount >= pulse_fuelToWaterRatio + solenoidOnPulse)
    sprayedOnce = true;
  if (startSpray && !sprayedOnce && sprayCompleted)
  {
    prevSolOnTime = millis();
    digitalWrite(solenoidWater, HIGH);
    digitalWrite(motorWater, HIGH);
    sprayedOnce = true;
    startSpray = false;
    sprayCompleted = false;
    pulseInc++;
  }
  if (!sprayCompleted && millis() - prevSolOnTime >= solenoidOnTime)
  {
    digitalWrite(solenoidWater, LOW);
    digitalWrite(motorWater, LOW);
    sprayCompleted = true;
  }
}
