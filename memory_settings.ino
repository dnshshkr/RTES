void factoryReset() //factory reset
{
  EEPROM.put(addr1, 10);
  EEPROM.put(addr2, 15);
  EEPROM.put(addr3, 1.45);
  EEPROM.put(addr4, 0.81);
  EEPROM.put(addr5, 250);
  EEPROM.put(addr6, 10.0);
  EEPROM.put(addr7, 0);
  loadSettings();
  settingMode = 1;
  adminState = 0;
  pulse_fuelToWaterRatioCount = 1;
  Serial.println("Restored to factory settings");
  bt.println("Restored to factory settings");
  delay(1000);
}

void saveSettings()
{
  EEPROM.update(addr1, pulse_fuelToWaterRatio);
  EEPROM.update(addr2, engineOffTimeOut);
  EEPROM.update(addr3, flowRateBias);
  EEPROM.update(addr4, solShotBias);
  EEPROM.update(addr5, solenoidOnTime);
  EEPROM.update(addr6, quickWaterPercentage);
  EEPROM.update(addr7, adminState);
}

void loadSettings()
{
  EEPROM.get(addr1, pulse_fuelToWaterRatio);
  EEPROM.get(addr2, engineOffTimeOut);
  EEPROM.get(addr3, flowRateBias);
  EEPROM.get(addr4, solShotBias);
  EEPROM.get(addr5, solenoidOnTime);
  EEPROM.get(addr6, quickWaterPercentage);
  EEPROM.get(addr7, adminState);
  if (adminState)
    settingMode = 1;
  calculatePulse_fuelToWaterRatio();
  calculateSolenoidOnTime();
  calculateDenom();
}
