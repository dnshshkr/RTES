void manualRtesSystem()
{
  //display.showNumberHexEx(0xb000, 0, true);

  unsigned long manualCurrentTime = millis();
  if (manualCurrentTime - manualPreviousTime < manualTimePeriod)
    flagManual = 1;
  else
  {
    manualPreviousTime = manualCurrentTime;
    flagManual = 0;
  }
  ////////////////////////Manual RTES system start////////////////////////

  ////////////////////////Manual RTES system end//////////////////////////
}
