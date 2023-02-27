void displayData()
{
  Serial.print(String((int)readings[0]) + " counter\t");
  Serial.print(String((int)readings[1]) + " total fuel pulse \t");
  Serial.print(String((double)readings[2]) + " s\t");
  Serial.print((double)readings[3], 3);
  Serial.print(" mL/min\t");
  Serial.print(String((int)readings[4]) + " total water pulse\t");
  Serial.print((double)readings[5], 1);
  Serial.print(" % water\t");
  Serial.print((double)readings[6], 1);
  Serial.println("% fuel");
}
