void parseCMD()
{
  cmd = Serial.readStringUntil('\r\n');
  cmd.trim();
  char alph = cmd.charAt(0);
  switch (alph)
  {
    case'a': case'A':
      {

      }
    case'r': case'R':
      {
        Serial.print("Are you sure you want to reset to factory settings? (Y/N)");
        bool wait = timeoutUI(10);
        char choice = Serial.read();
        if (choice == 'Y' || choice == 'y')
        {
          Serial2.write(0x81);
          while (!Serial2.available()) {}
          if (Serial2.read() == 0xfd)
          {
            Serial.print("\nRestored to factory settings");
            delay(1000);
            Serial.println();
          }
        }
        else
        {
          Serial.print("\nFactory reset aborted");
          delay(1000);
          Serial.println();
        }
        printSettings();
        break;
      }
    case's': case'S':
      {
        if (mode)
        {
          Serial.println("Save changes? (Y/N)");
          bool wait = timeoutUI(5);
          char choice = Serial.read();
          if(choice=='Y'||choice=='y')
          {
            
          }
          else
          {
            Serial.print("\nChanges discarded");
            delay(1000);
            Serial.println();
          }
        }
        Serial2.write(0x83);
        break;
      }
  }
}
s
