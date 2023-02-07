// bool setTime() {
//   Serial.println("Input current time in 24-hour (HHMM)");
//   bt.println("Input current time in 24-hour (HHMM)");
//   while (!Serial.available() && !bt.available()) {}
//   String msg = Serial.readStringUntil('\r\n');
//   if (bt.available())
//     msg = bt.readStringUntil('\r\n');
//   msg.trim();
//   hour = msg.substring(0, 2).toInt();
//   if (hour < 0 || hour >= 24) {
//     Serial.println("Invalid hour");
//     bt.println("Invalid hour");
//     return false;
//   }
//   minute = msg.substring(2, 4).toInt();
//   if (minute < 0 || minute >= 60) {
//     Serial.println("Invalid minute");
//     bt.println("Invalid minute");
//     return false;
//   }
//   Serial.print(hour), Serial.println(minute);
// }
// void stopwatch() {
//   if (millis() - prevMillisRTESStopwatch >= 1000) {
//     second++;
//     if (second == 60) {
//       minute++;
//       second = 0;
//     }
//     if (minute == 60) {
//       hour++;
//       minute = 0;
//     }
//     if (hour == 24)
//       hour = 0;
//     prevMillisRTESStopwatch = millis();
//   }
// }
// void displayClock12() {
//   uint8_t hour12 = hour - 12;
//   Serial.println(String(hour12) + ':' + String(minute) + ' ');
//   if (hour >= 0 && hour < 12) {
//     Serial.println("AM");
//     bt.println("AM");
//   } else {
//     Serial.println("PM");
//     bt.println("PM");
//   }
// }