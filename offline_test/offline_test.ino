void setup()
{
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  randomSeed(analogRead(0));
}
void loop()
{
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  delay(random(0, 6000));
}
