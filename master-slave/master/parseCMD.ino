void parseCMD()
{
  cmd = Serial.readStringUntil('\r\n');
  cmd.trim();
  char alph = cmd.charAt(0);
  switch (alph)
  {
    case'r': case'R':
      {
        Serial.print("Are you sure you want to reset to factory settings? (Y/N)");
        bool wait = timeoutUI();
        char choice = Serial.read();
        if (choice == 'Y' || choice == 'y')
        {
          slave.write(0x81);
          while (!slave.available()) {}
          if (slave.read() == 0xfd)
          {
            Serial.print("\nRestored to factory settings");
            delay(1000);
            Serial.println();
            break;
          }
        }
        else
        {
          Serial.print("\nFactory reset aborted");
          delay(1000);
          Serial.println();
        }
      }
    case's': case'S':
      {
        slave.write(0x83);
        //        while (!slave.available()) {}
        //        uint8_t respCode = slave.read();
        //        if (respCode == 0xfb)
        //          mode = true;
        //        else if (respCode == 0xfc)
        //          mode = false;
        break;
      }
  }
}
