#include<EEPROM.h>
void setup()
{
  for (int i = 0; i < 1024; i++)
    EEPROM.write(i, 255);
}
void loop() {}
