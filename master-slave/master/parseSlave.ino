void parseSlave()
{
  uint8_t respCode = slave.read();
  if (respCode == 0xfb)
    mode = true;
  else if (respCode == 0xfc)
    mode = false;
  else if (respCode >= 0xfe)
  {
    String msg = slave.readStringUntil('\r\n');
    msg.trim();
    Serial.println("response code: " + String(respCode));
    Serial.println("msg: " + msg);
    if (respCode == 0xff)
    {
      params = JSON.parse(msg);
      testMode = (bool)params[7];
      printSettings();
    }
  }
}
