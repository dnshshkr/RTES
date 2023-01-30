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
  EEPROM.put(addr1, 0);
  EEPROM.put(addr2, 10);
  EEPROM.put(addr3, 15000);
  EEPROM.put(addr4, 1.45);
  EEPROM.put(addr5, 0.81);
  EEPROM.put(addr6, 250);
  EEPROM.put(addr7, 0);
  EEPROM.put(addr8, 0);
  EEPROM.put(addr9, 1);
  EEPROM.put(addr10, 10.0);
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
  EEPROM.update(addr1, emulsionTrig);
  EEPROM.update(addr2, pulse_fuelToWaterRatio);
  EEPROM.update(addr3, engineOffTimeOut);
  EEPROM.update(addr4, flowRateBias);
  EEPROM.update(addr5, solShotBias);
  EEPROM.update(addr6, solenoidOnTime);
  EEPROM.update(addr7, manualPumpState);
  EEPROM.update(addr8, fuelTrig);
  EEPROM.update(addr9, waterTrig);
  EEPROM.update(addr10, quickWaterPercentage);
}

void loadSettings()
{
  EEPROM.get(addr1, emulsionTrig);
  EEPROM.get(addr2, pulse_fuelToWaterRatio);
  EEPROM.get(addr3, engineOffTimeOut);
  EEPROM.get(addr4, flowRateBias);
  EEPROM.get(addr5, solShotBias);
  EEPROM.get(addr6, solenoidOnTime);
  EEPROM.get(addr7, manualPumpState);
  if (manualPumpState)
    settingMode = 1;
  EEPROM.get(addr8, fuelTrig);
  EEPROM.get(addr9, waterTrig);
  EEPROM.get(addr10, quickWaterPercentage);
  calculatePulse_fuelToWaterRatio();
  calculateSolenoidOnTime();
  calculateDenom();
}
