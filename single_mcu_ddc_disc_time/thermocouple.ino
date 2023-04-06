void measureTemperatures()
{
  envTemp = thermo_env.readCelsius();
  motorTemp = thermo_motor.readCelsius();
  rtesTemp = thermo_rtes.readCelsius();
}
