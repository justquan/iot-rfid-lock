////old functions from BEFORE 2/10. now switched to new system
//boolean exitingStudent() {
//  long handleVal = handleEnter.capacitiveSensor(30);
//  if (handleVal > exitThreshold) {
//    return true;  //hand on handle
////    Serial.print("Exiting handle value: ");
////    Serial.println(handleVal);
//  }
//  else {
//    return false; //no hand on handle
//  }
//}
//
//boolean enteringStudent() {
//  long handleVal = handleEnter.capacitiveSensor(30);
//  if (handleVal > enterThreshold) {
//    return true;  //hand on handle
//  }
//  else {
//    return false; //no hand on handle
//  }
//}
//boolean checkExit() {
//  if (exitingStudent()) { //if student exiting
//    Serial.println("Student Exiting");
//    return true;
//  }
//  else {
//    return false;
//  }
//}

//Temporary Unlocks door before relocking
void doorTempUnlock() {
  Serial.println("Unlocking Door.");
  digitalWrite(lockRelayPin, HIGH);
  delay(unlockDelay);
  Serial.println("Locking Door.");
  digitalWrite(lockRelayPin, LOW);
}

//void handleSensorPowerOn() {
//  digitalWrite(handleSensorSwitch, HIGH);
//}
//
//void handleSensorPowerOff() {
//  digitalWrite(handleSensorSwitch, LOW);
//}

