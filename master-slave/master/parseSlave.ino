void parseSerial2()
{
  uint8_t respCode = Serial2.read();
  if (respCode == 0xfb)
  {
    mode = false;
    printSettings();
    Serial.println("RTES mode entered");
  }
  else if (respCode == 0xfc)
  {
    mode = true;
    printSettings();
    Serial.println("Settings mode entered");
  }
  else if (respCode >= 0xfe)
  {
    String msg = Serial2.readStringUntil('\r\n');
    msg.trim();
    //    Serial.println("response code: " + String(respCode));
    //    Serial.println("msg: " + msg);
    if (respCode == 0xfe)
    {
      readings = JSON.parse(msg);
      displayData();
    }
    else if (respCode == 0xff)
    {
      params = JSON.parse(msg);
      testMode = (bool)params[7];
      printSettings();
    }
  }
}
