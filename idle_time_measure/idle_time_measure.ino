const byte sensor = 2, len = 10;
volatile unsigned long prevMillis;
volatile float period, sum, avrg;
void setup()
{
  Serial.begin(115200);
  pinMode(sensor, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(sensor), measure, FALLING);
  prevMillis = millis();
}
void loop(){}
void measure()
{
  static volatile byte index = 0;
  period = millis() - prevMillis;
  sum += period;
  prevMillis = millis();
  Serial.println(period);
  index++;
  if (index == len)
  {
    avrg = sum / len;
    Serial.println("average: " + String(avrg) + " ms");
    index = 0;
  }
}
