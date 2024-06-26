void RTES() {
  //  REVISION 1 - spray along the final pulse
  //    if (cycleCount > f2wPulseRatio && !sprayStarted)
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
  //      if (cycleCount >= f2wPulseRatio + 1 + 1)
  //      {
  //        sprayStarted = false;
  //        totalWaterPulse++;
  //        cycleCount = 1;
  //      }
  //    }
  //    else
  //    {
  //      digitalWrite(solenoid, LOW);
  //      digitalWrite(waterPump, LOW);
  //    }

  //REVISION 2 - spray on the final pulse but in a desired period
  //  if (cycleCount >= 1 && cycleCount <= f2wPulseRatio)  //make sure solenoid does not trigger on accident on other pulses rather than the final pulse in every cycle
  //  {
  //    sprayedOnce = false;
  //    sprayStarted = false;
  //    sprayCompleted = true;
  //    stopEmulsion();
  //  }
  //  if (cycleCount > f2wPulseRatio)
  //    sprayStarted = true;
  //  else if (cycleCount >= f2wPulseRatio + 1)
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
  //solenoid control
  if (!sprayStarted && cycleCount > f2wPulseRatio)
  {
    if (!sprayCompleted)
    {
      startEmulsion();
      solOnTimePrevMillis = millis();
      if (!dieselMode)
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

  //mixer device control
  if (deviceState == LOW && millis() - devicePrevMillis >= deviceOffTime * 1000)
  {
    deviceState = HIGH;
    Serial.println("motor on");
    bt.println("motor on");
    devicePrevMillis = millis();
  }
  else if (deviceState == HIGH && millis() - devicePrevMillis >= deviceOnTime * 1000)
  {
    deviceState = LOW;
    Serial.println("motor off");
    bt.println("motor off");
    devicePrevMillis = millis();
  }
  digitalWrite(deviceMotor, deviceState);
}
