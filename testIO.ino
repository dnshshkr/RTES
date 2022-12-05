void testIO() {
  //display.showNumberHexEx(0xFFFF);
  unsigned long testIOCurrentTime = millis();
  if (testIOCurrentTime - testIOPreviousTime > 9000)
    testIOPreviousTime = testIOCurrentTime;
  if (testIOCurrentTime - testIOPreviousTime < 2000)
    currentLimitedOut(1, 0, 0);
  if (testIOCurrentTime - testIOPreviousTime > 2000 && testIOCurrentTime - testIOPreviousTime < 3000)
    currentLimitedOut(0, 0, 0);
  if (testIOCurrentTime - testIOPreviousTime > 3000 && testIOCurrentTime - testIOPreviousTime < 5000)
    currentLimitedOut(0, 1, 0);
  if (testIOCurrentTime - testIOPreviousTime > 5000 && testIOCurrentTime - testIOPreviousTime < 6000)
    currentLimitedOut(0, 0, 0);
  if (testIOCurrentTime - testIOPreviousTime > 6000 && testIOCurrentTime - testIOPreviousTime < 8000)
    currentLimitedOut(0, 0, 1);
  if (testIOCurrentTime - testIOPreviousTime > 8000 && testIOCurrentTime - testIOPreviousTime < 9000)
    currentLimitedOut(0, 0, 0);
}
