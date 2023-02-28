void parseSlave()
{
  uint8_t respCode = slave.read();
  if (respCode == 0xfa)
  {
    mode = false;
    printSettings();
    Serial.print("RTES mode entered");
    if (dieselMode)
      Serial.println(" (Diesel-only mode)");
    Serial.println();
  }
  else if (respCode == 0xfb)
  {
    mode = true;
    printSettings();
    Serial.println("Settings mode entered");
  }
  else if (respCode >= 0xfe)
  {
    String body = slave.readStringUntil('\r\n');
    body.trim();
    //    Serial.println("response code: " + String(respCode));
    //    Serial.println("body: " + body);
    if (respCode == 0xfe)
    {
      readings = JSON.parse(body);
      engOffPrevMillis = millis();
      displayData();
    }
    else if (respCode == 0xff)
    {
      //Serial.println(body);
      params = JSON.parse(body);
      assignParams();
    }
  }
}
