/********************Interrupt function*******************************************************************************/
void interruptRoutine()
{
  engOffPrevMillis = millis();  //engine-off detection
  fuelPulsePeriod = (float)(millis() - pulseMeasurePrevMillis) / 1000.0;
  fuelFlowRate = 1.0 / ((1.0 / fuelPulseBias) * fuelPulsePeriod * (1.0 / 60.0));
  pulseMeasurePrevMillis = millis();
  totalFuelPulse++;
  cycleCount++;
  fuelPulsePeriodSum += fuelPulsePeriod;
  if (cycleCount >= f2wPulseRatio + 2)
  {
    cycleCount = 1;
    fuelPulsePeriodSum = 0;
  }
  //pulseDataPrint = true;
  if (!mode)
  {
    //    if (testMode && accumMinute - lastMinute >= checkpointPeriod || (testMode && firstRowData))
    //    {
    //      displayClock12(true);
    //      Serial.print(" -> ");
    //      bt.println();
    //      lastMinute = accumMinute;
    //      firstRowData = false;
    //    }
    //    else if (testMode)
    //      Serial.print("\t    ");
    printData();
  }
}
