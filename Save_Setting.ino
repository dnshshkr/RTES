void ResetSetting()
{
  SaveSetting();
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
  LoadSetting();
  settingMode = 1;
  manualPumpState = 0;
  pulse_fuelToWaterRatioCount = 1;
  Serial.println("Restored to factory settings");
  bt.println("Restored to factory settings");
  delay(1000);
}

void SaveSetting()
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

void LoadSetting()
{
  EEPROM.get(addr1, read1);
  //Serial.print("A: Emulsion State: "); Serial.println(read1);
  emulsionTrig = read1;

  EEPROM.get(addr2, read2);
  //Serial.print("B: Pulse Count Max: "); Serial.println(read2);
  pulse_fuelToWaterRatio = read2;

  EEPROM.get(addr3, read3);
  //Serial.print("C: Pulse time for idle in ms: "); Serial.println(read3);
  engineOffTimeOut = read3;

  EEPROM.get(addr4, read4);  //Float
  //Serial.print("D: Flow Rate Bias in mm/pulse: ");  Serial.println(read4,2);
  flowRateBias = read4;

  EEPROM.get(addr5, read5);  //Float
  //Serial.print("E: Solenoid Shot Bias in mm/pulse: "); Serial.println(read5,2);
  solShotBias = read5;

  EEPROM.get(addr6, read6);
  //Serial.print("F: Solenoid On Time ms: "); Serial.println(read6);
  solenoidOnTime = read6;

  EEPROM.get(addr7, read7);
  //Serial.print("G: Manual Mode State: "); Serial.println(read7);
  manualPumpState = read7;
  if (manualPumpState) settingMode = 1;

  EEPROM.get(addr8, read8);
  //Serial.print("G: Manual Mode State: "); Serial.println(read7);
  fuelTrig = read8;

  EEPROM.get(addr9, read9);
  waterTrig = read9;

  EEPROM.get(addr10, read10);
  quickWaterPercentage = read10;
  calculatePulse_fuelToWaterRatio();
  calculateSolenoidOnTime();
  calculateDenom();
}
