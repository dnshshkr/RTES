//void measureAmperage()
//{
//  if (currentSensorType == 0)
//  {
//    ampMotorFuel = (2.5 - ((float)analogRead(fuelMotorCurrentPin) * (5.0 / 1024.0))) / 0.185;
//    ampSolenoid = (2.5 - ((float)analogRead(solenoidCurrentPin) * (5.0 / 1024.0))) / 0.185;
//    ampMotorWater = (2.5 - ((float)analogRead(waterPumpCurrentPin) * (5.0 / 1024.0))) / 0.185;
//  }
//  else if (currentSensorType == 1)
//  {
//    ampMotorFuel = (2.5 - ((float)analogRead(fuelMotorCurrentPin) * (5.0 / 1024.0))) / 0.066;
//    ampSolenoid = (2.5 - ((float)analogRead(solenoidCurrentPin) * (5.0 / 1024.0))) / 0.066;
//    ampMotorWater = (2.5 - ((float)analogRead(waterPumpCurrentPin) * (5.0 / 1024.0))) / 0.066;
//  }
//  if (ampMotorFuel < 0)
//    ampMotorFuel = 0;
//  if (ampSolenoid < 0)
//    ampSolenoid = 0;
//  if (ampMotorWater < 0)
//    ampMotorWater = 0;
//}
