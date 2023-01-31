void RTES()
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
  //      if (pulse_fuelToWaterRatioCount >= pulse_fuelToWaterRatio + 1 + 1)
  //      {
  //        sprayStarted = false;
  //        totalWaterPulse++;
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
    digitalWrite(LED_BUILTIN, LOW);
  }
  if (pulse_fuelToWaterRatioCount > pulse_fuelToWaterRatio)
    sprayStarted = true;
  else if (pulse_fuelToWaterRatioCount >= pulse_fuelToWaterRatio + 1)
    sprayedOnce = true;
  if (sprayStarted && !sprayedOnce && sprayCompleted)
  {
    prevSolOnTime = millis();
    digitalWrite(solenoidWater, HIGH);
    digitalWrite(motorWater, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    sprayedOnce = true;
    sprayStarted = false;
    sprayCompleted = false;
    totalWaterPulse++;
  }
  if (!sprayCompleted && millis() - prevSolOnTime >= solenoidOnTime)
  {
    digitalWrite(solenoidWater, LOW);
    digitalWrite(motorWater, LOW);
    digitalWrite(LED_BUILTIN, LOW);
    sprayCompleted = true;
  }
}
