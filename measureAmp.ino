void measureAmperage()
{
  //  ampCountLoop += 1;
  //  ampA += analogRead(A3);
  //  ampB += analogRead(A2);
  //  ampC += analogRead(A4);
  //  unsigned long currentMillis = millis();
  //  if (currentMillis - lastExecutedMillis >= 1000)
  //  {
  //
  //    if (currentSensorType == 0)
  //    {
  //      /*********************ACS713 20A*******************************************************/
  //      ampMotorFuel = (((ampA / ampCountLoop) / 1023 * 5) - 0.5) / 0.185;
  //      ampSolenoid = (((ampB / ampCountLoop) / 1023 * 5) - 0.5) / 0.185;
  //      ampMotorWater = (((ampC / ampCountLoop) / 1023 * 5) - 0.5) / 0.185;
  //    }
  //    else
  //    {
  //      /*********************ACS712 30A*******************************************************/
  //      ampMotorFuel = (((ampA / ampCountLoop) / 1023 * 5) - 2.51) / 0.066;
  //      ampSolenoid = (((ampB / ampCountLoop) / 1023 * 5) - 2.51) / 0.066;
  //      ampMotorWater = (((ampC / ampCountLoop) / 1023 * 5) - 2.51) / 0.066;
  //    }
  //
  //    if (ampMotorFuel < 0)
  //      ampMotorFuel = 0;
  //    if (ampSolenoid < 0)
  //      ampSolenoid = 0;
  //    if (ampMotorWater < 0)
  //      ampMotorWater = 0;
  //    lastExecutedMillis = currentMillis;
  //    ampCountLoop = 0;
  //    ampA = 0;
  //    ampB = 0;
  //    ampC = 0;
  //  }
  static unsigned long prev;
  if (millis() - prev >= 1000)
  {
    if (currentSensorType == 0)
    {
      ampMotorFuel = (2.5 - (analogRead(fuelMotorCurrentPin) * (5.0 / 1024.0))) / 0.185;
      ampSolenoid = (2.5 - (analogRead(solenoidCurrentPin) * (5.0 / 1024.0))) / 0.185;
      ampMotorWater = (2.5 - (analogRead(waterPumpCurrentPin) * (5.0 / 1024.0))) / 0.185;
    }
    else if (currentSensorType == 1)
    {
      ampMotorFuel = (2.5 - (analogRead(fuelMotorCurrentPin) * (5.0 / 1024.0))) / 0.066;
      ampSolenoid = (2.5 - (analogRead(solenoidCurrentPin) * (5.0 / 1024.0))) / 0.066;
      ampMotorWater = (2.5 - (analogRead(waterPumpCurrentPin) * (5.0 / 1024.0))) / 0.066;
    }
    if (ampMotorFuel < 0)
      ampMotorFuel = 0;
    if (ampSolenoid < 0)
      ampSolenoid = 0;
    if (ampMotorWater < 0)
      ampMotorWater = 0;
    prev = millis();
  }

}
