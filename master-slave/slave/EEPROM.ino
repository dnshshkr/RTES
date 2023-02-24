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
}
