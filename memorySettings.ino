void resetSettings()
{
  //saveSettings();
  //  emulsionTrig = 0;
  //  pulse_fuelToWaterRatio = 3;
  //  engineOffTimeOut=15000;
  //  flowRateBias=1.45;
  //  solShotBias=0.81;
  //  solenoidOnTime=250;
  //  manualPumpState=0;
  //  fuelTrig=0;
  //  waterTrig=0;
  //  quickWaterPercentage=10;
  EEPROM.put(addr1, 10);
  EEPROM.put(addr2, 15);
  EEPROM.put(addr3, 1.45);
  EEPROM.put(addr4, 0.81);
  EEPROM.put(addr5, 250);
  EEPROM.put(addr6, 0);
  EEPROM.put(addr7, 10.0);
  loadSettings();
  settingMode = 1;
  manualPumpState = 0;
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
  EEPROM.update(addr6, manualPumpState);
  EEPROM.update(addr7, quickWaterPercentage);
}

void loadSettings()
{
  EEPROM.get(addr1, pulse_fuelToWaterRatio);
  EEPROM.get(addr2, engineOffTimeOut);
  EEPROM.get(addr3, flowRateBias);
  EEPROM.get(addr4, solShotBias);
  EEPROM.get(addr5, solenoidOnTime);
  EEPROM.get(addr6, manualPumpState);
  if (manualPumpState)
    settingMode = 1;
  EEPROM.get(addr7, quickWaterPercentage);
  calculatePulse_fuelToWaterRatio();
  calculateSolenoidOnTime();
  calculateDenom();
}
