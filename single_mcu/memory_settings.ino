void factoryReset()  //factory reset
{
  EEPROM.put(addr[0], 4);           //0~1
  EEPROM.update(addr[1], 15);       //2
  EEPROM.put(addr[2], 1.45);        //3~6
  EEPROM.put(addr[3], 0.81);        //7~10
  EEPROM.put(addr[4], 13);          //11~12
  EEPROM.put(addr[5], 10.0);        //13~16
  EEPROM.update(addr[6], 5);        //17
  EEPROM.update(addr[7], 0);        //18
  EEPROM.update(addr[8], 0);        //19
  EEPROM.put(addr[9], (0.81 / 13)); //20~23
  byte usedLen = 24;
  word remaining = EEPROM.length() - usedLen;
  Serial.println("\nResetting the rest " + String(remaining) + " bytes of unused memory cells");
  bt.println("\nResetting the rest " + String(remaining) + " bytes of unused memory cells");
  for (word i = usedLen; i < EEPROM.length(); i++)
    EEPROM.update(i, 0xff);
  loadSettings();
  cycleCount = 0;
  Serial.println("Restored to factory settings");
  bt.println("Restored to factory settings");
  delay(1000);
}

//void saveSettings() {
//  EEPROM.update(addr0, f2wPulseRatio);
//  EEPROM.update(addr1, engineOffTimeout);
//  EEPROM.update(addr2, fuelPulseBias);
//  EEPROM.update(addr3, waterPulseBias);
//  EEPROM.update(addr4, solOnTime);
//  EEPROM.update(addr5, waterPercentage);
//  EEPROM.update(addr7, checkpointPeriod);
//}

void loadSettings()
{
  EEPROM.get(addr[0], f2wPulseRatio);
  EEPROM.get(addr[1], engineOffTimeout);
  EEPROM.get(addr[2], fuelPulseBias);
  EEPROM.get(addr[3], waterPulseBias);
  EEPROM.get(addr[4], solOnTime);
  EEPROM.get(addr[5], waterPercentage);
  EEPROM.get(addr[6], checkpointPeriod);
  EEPROM.get(addr[7], testMode);
  EEPROM.get(addr[8], dieselMode);
  EEPROM.get(addr[9], solConst);
  //  calculate_f2wPulseRatio();
  //  calculate_solOnTime();
  denominator = calculate_denominator(f2wPulseRatio, fuelPulseBias, waterPulseBias);
}
