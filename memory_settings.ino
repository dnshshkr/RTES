void factoryReset()  //factory reset
{
  EEPROM.put(addr0, 4);     //0x0
  EEPROM.put(addr1, 15);    //0x2
  EEPROM.put(addr2, 1.45);  //0x3
  EEPROM.put(addr3, 0.81);  //0x7
  EEPROM.put(addr4, 250);   //0x11
  EEPROM.put(addr5, 10.0);  //0x13
  loadSettings();
  f2wPulseRatioCount = 1;
  Serial.print("\nRestored to factory settings");
  bt.print("\nRestored to factory settings");
  delay(1000);
  Serial.println();
  bt.println();
}

void saveSettings() {
  EEPROM.update(addr0, f2wPulseRatio);
  EEPROM.update(addr1, engineOffTimeout);
  EEPROM.update(addr2, flowRateBias);
  EEPROM.update(addr3, solShotBias);
  EEPROM.update(addr4, solenoidOnTime);
  EEPROM.update(addr5, waterPercentage);
}

void loadSettings() {
  EEPROM.get(addr0, f2wPulseRatio);
  EEPROM.get(addr1, engineOffTimeout);
  EEPROM.get(addr2, flowRateBias);
  EEPROM.get(addr3, solShotBias);
  EEPROM.get(addr4, solenoidOnTime);
  EEPROM.get(addr5, waterPercentage);
  //  calculate_f2wPulseRatio();
  //  calculate_solenoidOnTime();
  //  calculate_denominator();
}
