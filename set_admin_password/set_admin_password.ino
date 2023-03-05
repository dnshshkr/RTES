#include<EEPROM.h>
void setup()
{
  uint32_t pin = 990826;
  Serial.begin(9600);
  EEPROM.put(18, pin);
  uint32_t readPin;
  EEPROM.get(18, readPin);
  Serial.println(readPin);
  //Serial.println(EEPROM.read(18) | EEPROM.read(19) | EEPROM.read(20) | EEPROM.read(21));
}
void loop() {}
