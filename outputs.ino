void startEmulsion()
{
  digitalWrite(solenoid, HIGH);
  digitalWrite(waterPump, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
}
void stopEmulsion()
{
  digitalWrite(solenoid, LOW);
  digitalWrite(waterPump, LOW);
  digitalWrite(LED_BUILTIN, LOW);
}
