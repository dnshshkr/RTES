#include<EEPROM.h>
void setup()
{
  Serial.begin(9600);
  for (int i = 0; i < 26; i++)
  {
    Serial.print(String(i + 1) + " - " + String(EEPROM.read(i)) + ", ");
    Serial.println(EEPROM.read(i), HEX);
  }
}
void loop() {}
