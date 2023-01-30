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
    sprayStarted = false;
    sprayCompleted = true;
    digitalWrite(solenoidWater, LOW);
    digitalWrite(motorWater, LOW);
  }
  if (pulse_fuelToWaterRatioCount > pulse_fuelToWaterRatio)
    sprayStarted = true;
  else if (pulse_fuelToWaterRatioCount >= pulse_fuelToWaterRatio + solenoidOnPulse)
    sprayedOnce = true;
  if (sprayStarted && !sprayedOnce && sprayCompleted)
  {
    prevSolOnTime = millis();
    digitalWrite(solenoidWater, HIGH);
    digitalWrite(motorWater, HIGH);
    sprayedOnce = true;
    sprayStarted = false;
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
