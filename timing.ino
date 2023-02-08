bool setTime()
{
  Serial.println("Input current time in 24-hour (HHMM)");
  bt.println("Input current time in 24-hour (HHMM)");
  while (!Serial.available() && !bt.available()) {}
  String msg = Serial.readStringUntil('\r\n');
  if (bt.available())
    msg = bt.readStringUntil('\r\n');
  msg.trim();
  for (uint8_t i = 0; i < 4; i++)
  {
    if (!isdigit(msg[i]))
      return false;
  }
  hour = msg.substring(0, 2).toInt();
  if (hour < 0 || hour >= 24)
  {
    Serial.println("Invalid hour");
    bt.println("Invalid hour");
    return false;
  }
  minute = msg.substring(2, 4).toInt();
  if (minute < 0 || minute >= 60)
  {
    Serial.println("Invalid minute");
    bt.println("Invalid minute");
    return false;
  }
  return true;
}
void stopwatch() {
  if (millis() - prevMillisRTESStopwatch >= 1000) {
    second++;
    if (second == 60) {
      minute++;
      second = 0;
    }
    if (minute == 60) {
      hour++;
      minute = 0;
    }
    if (hour == 24)
      hour = 0;
    prevMillisRTESStopwatch = millis();
  }
}
void displayClock12()
{
  uint8_t hour12;
  if (hour > 12)
    hour12 = hour - 12;
  else
    hour12 = hour;
  Serial.print(String(hour12) + ':');
  if (bt)
    bt.print(String(hour12) + 's');
  if (String(minute).length() == 1)
  {
    Serial.print('0');
    if (bt)
      bt.print('0');
  }
  Serial.print(minute);
  if (bt)
    bt.print(minute);
  if (hour >= 0 && hour < 12)
  {
    Serial.print(" AM");
    if (bt)
      bt.print(" AM");
  }
  else
  {
    Serial.print(" PM");
    if (bt)
      bt.print(" PM");
  }
}
