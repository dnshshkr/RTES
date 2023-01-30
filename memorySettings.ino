void resetSettings()
{
  saveSettings();
  EEPROM.put(addr1, 0);
  EEPROM.put(addr2, 10);
  EEPROM.put(addr3, 15000);
  EEPROM.put(addr4, 1.45);
  EEPROM.put(addr5, 0.81);
  EEPROM.put(addr6, 250);
  EEPROM.put(addr7, 0);
  EEPROM.put(addr8, 0);
  EEPROM.put(addr9, 1);
  EEPROM.put(addr10, 10);
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
  EEPROM.put(addr1, emulsionTrig);
  EEPROM.put(addr2, pulse_fuelToWaterRatio);
  EEPROM.put(addr3, engineOffTimeOut);
  EEPROM.put(addr4, flowRateBias);
  EEPROM.put(addr5, solShotBias);
  EEPROM.put(addr6, solenoidOnTime);
  EEPROM.put(addr7, manualPumpState);
  EEPROM.put(addr8, fuelTrig);
  EEPROM.put(addr9, waterTrig);
  EEPROM.put(addr10, quickWaterPercentage);
}

void loadSettings()
{
  EEPROM.get(addr1, emulsionTrig);
  //Serial.print("A: Emulsion State: "); Serial.println(read1);

  EEPROM.get(addr2, pulse_fuelToWaterRatio);
  //Serial.print("B: Pulse Count Max: "); Serial.println(read2);

  EEPROM.get(addr3, engineOffTimeOut);
  //Serial.print("C: Pulse time for idle in ms: "); Serial.println(read3);

  EEPROM.get(addr4, flowRateBias);  //Float
  //Serial.print("D: Flow Rate Bias in mm/pulse: ");  Serial.println(read4,2);

  EEPROM.get(addr5, solShotBias);  //Float
  //Serial.print("E: Solenoid Shot Bias in mm/pulse: "); Serial.println(read5,2);

  EEPROM.get(addr6, solenoidOnTime);
  //Serial.print("F: Solenoid On Time ms: "); Serial.println(read6);

  EEPROM.get(addr7, manualPumpState);
  //Serial.print("G: Manual Mode State: "); Serial.println(read7);
  if (manualPumpState)
    settingMode = 1;

  EEPROM.get(addr8, fuelTrig);
  //Serial.print("G: Manual Mode State: "); Serial.println(read7);

  EEPROM.get(addr9, waterTrig);

  EEPROM.get(addr10, quickWaterPercentage);
  calculatePulse_fuelToWaterRatio();
  calculateSolenoidOnTime();
  calculateDenom();
}
