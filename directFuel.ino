void directFuel()
{
  //display.showNumberHexEx(0xdead);
  //////////////////////////////////////////////Buzzer/////////////////////////////////////////////////////////////////////
  unsigned long buzzerCurrentTime = millis();
  if (buzzerCurrentTime - buzzerPreviousTime >= 1000)
  {
    buzzerToggle = !buzzerToggle;
    buzzerPreviousTime = buzzerCurrentTime;
  }
  else
    digitalWrite(buzzer, LOW);
  if (buzzerToggle == 0 && millis() < 4000)
    digitalWrite(buzzer, HIGH);
  if (buzzerToggle == 1)
    digitalWrite(buzzer, LOW);
  //////////////////////////////////////////////END_Buzzer/////////////////////////////////////////////////////////////////

  currentLimitedOut(1, 0, 0);  //(fuel,sol,wat)
}
