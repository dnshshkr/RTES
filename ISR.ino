/********************Interrupt function*******************************************************************************/
void countPulse()
{
  //  static unsigned long last_interrupt_time = 0;
  //  unsigned long interrupt_time = millis();
  //  //Serial.println("Test");
  //  if (interrupt_time - last_interrupt_time > noiseRejection) //Noise variable
  //  {
  //    pulseCounter++;
  //    countPulse();
  //  }
  //  last_interrupt_time = interrupt_time;
  fuelPulsePeriod = float((millis() - measPlsPreviousTime)) / 1000;
  measuredPulsePerMin = 1.0 / ((1.0 / flowRateBias) * fuelPulsePeriod * (1.0 / 60.0));
  measPlsPreviousTime = millis();
  //pulseCounter++;
  totalFuelPulse++;
  pulse_fuelToWaterRatioCount++;
  //measurePulse();
  pulseDataPrint = true;
}
//void manualBtn()
//{
//  static unsigned long last_interrupt_time = 0;
//  unsigned long interrupt_time = millis();
//  if (interrupt_time - last_interrupt_time > 100) flagManual = 1;
//  last_interrupt_time = interrupt_time;
//}
