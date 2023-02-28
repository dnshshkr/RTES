void factoryReset()  //factory reset
{
  EEPROM.put(addr[0], 4);       //0x0
  EEPROM.update(addr[1], 15);   //0x2
  EEPROM.put(addr[2], 1.45);    //0x3
  EEPROM.put(addr[3], 0.81);    //0x7
  EEPROM.put(addr[4], 250);     //0x11
  EEPROM.put(addr[5], 10.0);    //0x13
  EEPROM.update(addr[6], 5);    //0x17
  EEPROM.update(addr[7], 0);    //0x18
  EEPROM.update(addr[8], 0);    //ox19
  loadSettings();
  cycleCount = 0;
  Serial.print("\nRestored to factory settings");
  //if(bt)
  bt.print("\nRestored to factory settings");
  delay(1000);
  Serial.println();
  //if(bt)
  bt.println();
}

//void saveSettings() {
//  EEPROM.update(addr0, f2wPulseRatio);
//  EEPROM.update(addr1, engineOffTimeout);
//  EEPROM.update(addr2, flowRateBias);
//  EEPROM.update(addr3, solShotBias);
//  EEPROM.update(addr4, solOnTime);
//  EEPROM.update(addr5, waterPercentage);
//  EEPROM.update(addr7, checkpointPeriod);
//}

void loadSettings()
{
  EEPROM.get(addr[0], f2wPulseRatio);
  EEPROM.get(addr[1], engineOffTimeout);
  EEPROM.get(addr[2], flowRateBias);
  EEPROM.get(addr[3], solShotBias);
  EEPROM.get(addr[4], solOnTime);
  EEPROM.get(addr[5], waterPercentage);
  EEPROM.get(addr[6], checkpointPeriod);
  EEPROM.get(addr[7], testMode);
  EEPROM.get(addr[8], dieselMode);
  //  calculate_f2wPulseRatio();
  //  calculate_solOnTime();
  denominator = calculate_denominator(f2wPulseRatio, flowRateBias, solShotBias);
}
