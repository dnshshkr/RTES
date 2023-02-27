void parseSerial2()
{
  uint8_t respCode = Serial2.read();
  if (respCode == 0xfa)
  {
    mode = false;
    printSettings();
    Serial.println("RTES mode entered");
  }
  else if (respCode == 0xfb)
  {
    mode = true;
    printSettings();
    Serial.println("Settings mode entered");
  }
  else if (respCode >= 0xfe)
  {
    String body = Serial2.readStringUntil('\r\n');
    body.trim();
    //    Serial.println("response code: " + String(respCode));
    //    Serial.println("body: " + body);
    if (respCode == 0xfe)
    {
      readings = JSON.parse(body);
      displayData();
    }
    else if (respCode == 0xff)
    {
      params = JSON.parse(body);
      testMode = (bool)params[7];
      printSettings();
    }
  }
}
