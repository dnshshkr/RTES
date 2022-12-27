//void rtesSystem2() {
//  display.showNumberHexEx(0xA000);
//  rtesCurrentTime2 = millis();
//  if (pulseCnt == pulseCntMax) {
//    if (rtesCurrentTime2 - rtesPreviousTime2 >= 1000) rtesPreviousTime2 = rtesCurrentTime2;
//    if (rtesCurrentTime2 - rtesPreviousTime2 < 50) {
//      currentLimitedOut(1, 1, 1); //(fuel,sol,wat)
//      if (pulseIncToggle == 1) {
//        pulseInc++;
//        pulseIncToggle = 0;
//        rtesPreviousTime2 = rtesCurrentTime2;
//      }
//    } else {
//      currentLimitedOut(1, 0, 1);//(fuel,sol,wat)
//      pulseIncToggle = 1;
//    }
//  }
//}

//void rtesSystem2()
//{
//  //display.showNumberHexEx(0xA000);
//  rtesCurrentTime2 = millis();
//  //Serial.println("RTES");
//  //  if (pulseCnt >= 4)
//  //    currentLimitedOut(fuelTrig, 1, waterTrig);//(fuel,sol,wat) //(1,0,1) MUST ON FUEL AND WATER PUMP //comment ni nanti
//  if (pulseCnt >= pulseCntMax)
//  {
//    //currentLimitedOut(fuelTrig, 0, waterTrig);//(fuel,sol,wat) //(1,0,1)MUST ON FUEL AND WATER PUMP
//    //fuelPump(fuelTrig);
//    //solPump(emulsionTrig);
//    //waterPump(waterTrig);
//    currentLimitedOut(fuelTrig, emulsionTrig, waterTrig);  //(1,emulsionTrig,1)   MUST ON solenoid emulsionTrig AND WATER PUMP //uncomment ni nanti
//    //delay(100); // added this value to count pulse function
//    pulseInc++;  //Sol
//    pulseCnt = 0;
//  }
//  else
//    currentLimitedOut(fuelTrig, 0, waterTrig);  //(fuel,sol,wat) //(1,0,1)MUST ON FUEL AND WATER PUMP


//  if (pulse_fuelToWaterRatioCount >= pulse_fuelToWaterRatio && !sprayStarted)
//  {
//    prevSolOnTime = millis();
//    currentLimitedOut(fuelTrig, 1, waterTrig);
//    sprayStarted = true;
//  }
//  else if (sprayStarted)
//  {
//    if (pulse_fuelToWaterRatioCount >= pulse_fuelToWaterRatio + solenoidOnPulse + 1)
//    {
//      pulse_fuelToWaterRatioCount = 1;
//      pulseInc++;
//      sprayStarted = false;
//    }
//    if (millis() - prevSolOnTime >= solenoidOnTime)
//      currentLimitedOut(fuelTrig, 0, waterTrig);
//  }
//}
