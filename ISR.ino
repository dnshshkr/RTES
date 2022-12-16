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
  measuredPulsePerMin = 1 / ((1 / flowRateBias) * (millis() - measPlsPreviousTime) * (1 / 60));
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
