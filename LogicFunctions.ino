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

