<<<<<<< HEAD
void startEmulsion()
{
  if (!dieselMode)
  {
    digitalWrite(solenoid, HIGH);
    digitalWrite(waterPump, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
    stopEmulsion();
}

void stopEmulsion()
{
  digitalWrite(solenoid, LOW);
  digitalWrite(waterPump, LOW);
  digitalWrite(LED_BUILTIN, LOW);
}
=======
void startEmulsion()
{
  if (!dieselMode && runRTES)
  {
    digitalWrite(solenoid, HIGH);
    digitalWrite(waterPump, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
    stopEmulsion();
}

void stopEmulsion()
{
  digitalWrite(solenoid, LOW);
  digitalWrite(waterPump, LOW);
  digitalWrite(LED_BUILTIN, LOW);
}
>>>>>>> origin/master
