void RTES() {
  //  METHOD 1 - spray along the final pulse
  //    if (f2wPulseRatioCount > f2wPulseRatio && !sprayStarted)
  //    {
  //      prevSolOnTime = millis();
  //      //currentLimitedOut(fuelTrig, 1, waterTrig);
  //      digitalWrite(solenoid, HIGH);
  //      digitalWrite(waterPump, HIGH);
  //      sprayStarted = true;
  //    }
  //    else if (sprayStarted)
  //    {
  //      if (millis() - prevSolOnTime >= solenoidOnTime)
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

  //METHOD 2 - spray on the final pulse but in a desired period
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
  //    prevSolOnTime = millis();
  //    startEmulsion();
  //    sprayedOnce = true;
  //    sprayStarted = false;
  //    sprayCompleted = false;
  //    totalWaterPulse++;
  //  }
  //  if (!sprayCompleted && millis() - prevSolOnTime >= solenoidOnTime) {
  //    stopEmulsion();
  //    sprayCompleted = true;
  //  }

  //METHOD 3
  if (f2wPulseRatioCount > f2wPulseRatio)
  {
    if (lastFuelPulsePos == f2wPulseRatioCount && millis() - prevSolOnTime >= solenoidOnTime)
    {
      Serial.println("fak");
      stopEmulsion();
      return;
    }
    startEmulsion();
    prevSolOnTime = millis();
    lastFuelPulsePos = f2wPulseRatioCount;
    Serial.println(lastFuelPulsePos);
  }
  if (millis() - prevSolOnTime >= solenoidOnTime)
    stopEmulsion();
}
