void RTES() {
  //  REVISION 1 - spray along the final pulse
  //    if (f2wPulseRatioCount > f2wPulseRatio && !sprayStarted)
  //    {
  //      solOnTimePrevMillis = millis();
  //      //currentLimitedOut(fuelTrig, 1, waterTrig);
  //      digitalWrite(solenoid, HIGH);
  //      digitalWrite(waterPump, HIGH);
  //      sprayStarted = true;
  //    }
  //    else if (sprayStarted)
  //    {
  //      if (millis() - solOnTimePrevMillis >= solOnTime)
  //      {
  //        digitalWrite(solenoid, LOW);
  //        digitalWrite(waterPump, LOW);
  //        //currentLimitedOut(fuelTrig, 0, waterTrig);
  //      }
  //      if (f2wPulseRatioCount >= f2wPulseRatio + 1 + 1)
  //      {
  //        sprayStarted = false;
  //        totalWaterPulse++;
  //        f2wPulseRatioCount = 1;
  //      }
  //    }
  //    else
  //    {
  //      digitalWrite(solenoid, LOW);
  //      digitalWrite(waterPump, LOW);
  //    }

  //REVISION 2 - spray on the final pulse but in a desired period
  //  if (f2wPulseRatioCount >= 1 && f2wPulseRatioCount <= f2wPulseRatio)  //make sure solenoid does not trigger on accident on other pulses rather than the final pulse in every cycle
  //  {
  //    sprayedOnce = false;
  //    sprayStarted = false;
  //    sprayCompleted = true;
  //    stopEmulsion();
  //  }
  //  if (f2wPulseRatioCount > f2wPulseRatio)
  //    sprayStarted = true;
  //  else if (f2wPulseRatioCount >= f2wPulseRatio + 1)
  //    sprayedOnce = true;
  //  if (sprayStarted && !sprayedOnce && sprayCompleted) {
  //    solOnTimePrevMillis = millis();
  //    startEmulsion();
  //    sprayedOnce = true;
  //    sprayStarted = false;
  //    sprayCompleted = false;
  //    totalWaterPulse++;
  //  }
  //  if (!sprayCompleted && millis() - solOnTimePrevMillis >= solOnTime) {
  //    stopEmulsion();
  //    sprayCompleted = true;
  //  }

  //REVISON 3
  if (!sprayStarted && f2wPulseRatioCount > f2wPulseRatio)
  {
    if (!sprayCompleted)
    {
      startEmulsion();
      solOnTimePrevMillis = millis();
      totalWaterPulse++;
      sprayStarted = true;
      sprayedOnce = true;
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
    sprayedOnce = false;
    sprayCompleted = false;
  }
  //  if (f2wPulseRatioCount == f2wPulseRatio)
  //  {
  //    sprayStarted = false;
  //    sprayedOnce = false;
  //    sprayCompleted = false;
  //  }
  //Serial.println("sprayStarted: " + String(sprayStarted) + " sprayedOnce: " + String(sprayedOnce) + " sprayCompleted: " + String(sprayCompleted));
}
