void loadParams()
{
  EEPROM.get(addr[0], f2wPulseRatio);
  EEPROM.get(addr[1], engineOffTimeout);
  EEPROM.get(addr[2], flowRateBias);
  EEPROM.get(addr[3], solShotBias);
  EEPROM.get(addr[4], solOnTime);
  EEPROM.get(addr[5], waterPercentage);
  EEPROM.get(addr[6], checkpointPeriod);
  EEPROM.get(addr[7], testMode);
  calculate_denominator();
}

void resetParams()
{
  EEPROM.put(addr[0], 4);     //0x0
  EEPROM.put(addr[1], 15);    //0x2
  EEPROM.put(addr[2], 1.45);  //0x3
  EEPROM.put(addr[3], 0.81);  //0x7
  EEPROM.put(addr[4], 250);   //0x11
  EEPROM.put(addr[5], 10.0);  //0x13
  EEPROM.put(addr[6], 5);     //0x17
  EEPROM.put(addr[7], 0);     //0x18
  loadParams();
  cycleCount = 1;
}

void saveNewParams()
{
  EEPROM.put(addr[0], f2wPulseRatio);
  EEPROM.put(addr[1], engineOffTimeout);
  EEPROM.put(addr[2], flowRateBias);
  EEPROM.put(addr[3], solShotBias);
  EEPROM.put(addr[4], solOnTime);
  EEPROM.put(addr[5], waterPercentage);
  EEPROM.put(addr[6], checkpointPeriod);
  EEPROM.put(addr[7], testMode);
  calculate_denominator();
}
