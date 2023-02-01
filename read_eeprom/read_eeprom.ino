#include<EEPROM.h>
void setup()
{
  Serial.begin(38400);
  for (int i = 0; i < 26; i++)
  {
    Serial.print(String(i) + " - " + String(EEPROM.read(i)) + ", ");
    Serial.println(EEPROM.read(i), HEX);
  }
}
void loop() {}
