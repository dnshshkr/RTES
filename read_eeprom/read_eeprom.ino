#include<EEPROM.h>
void setup()
{
  Serial.begin(38400);
  unsigned long pin;
  EEPROM.get(17, pin);
  Serial.println(pin);
  for (int i = 0; i < 26; i++)
  {
    Serial.print(String(i) + " - " + String(EEPROM.read(i)) + ", ");
    Serial.println(EEPROM.read(i), HEX);
  }
}
void loop() {}
