void currentLimitedOut(int a, int b, int c)
{
  if (ampMotorFuel < motorFuelAmpLim && a == 1)
    digitalWrite(motorFuel, HIGH);
  else
    digitalWrite(motorFuel, LOW);

  if (ampSolenoid < solenoidAmpLimit && b == 1)
    digitalWrite(solenoidWater, HIGH);
  else
    digitalWrite(solenoidWater, LOW);

  if (ampMotorWater < motorWaterAmpLimit && c == 1)
    digitalWrite(motorWater, HIGH);
  else
    digitalWrite(motorWater, LOW);
}

void fuelPump(int a)
{
  if (ampMotorFuel < motorFuelAmpLim && a == 1)
    digitalWrite(motorFuel, HIGH);
  else
    digitalWrite(motorFuel, LOW);
}

void solPump(int b)
{
  if (ampMotorFuel < motorFuelAmpLim && b == 1)
    digitalWrite(solenoidWater, HIGH);
  else
    digitalWrite(solenoidWater, LOW);
}

void waterPump(int c)
{
  if (ampMotorFuel < motorFuelAmpLim && c == 1)
    digitalWrite(motorWater, HIGH);
  else
    digitalWrite(motorWater, LOW);
}
