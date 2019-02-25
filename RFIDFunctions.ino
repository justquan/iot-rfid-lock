String getCurrentRFID() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {//If no card is detected
    return "none";  //return "none" string
  }
  // Select one of the cards
  else if (!mfrc522.PICC_ReadCardSerial()) {
    return "none";  //return "none" string
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print("Received UID tag :");
  String content = "";
  byte letter;
  //Puts RFID code into a string "XX XX XX XX"
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "); //from coinmonk jan 12
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();

  Serial.println();
  String returnedString = content.substring(1);//takes off first character
  return returnedString;
}

//UNTESTED 2/10
//For processing RFID UID when entering
void checkRFIDEnter(String sensedUID) { //TODO: TEST. Especially string addign with +
  Serial.print("Sensed UID: ");
  Serial.println(sensedUID);
  String studentName = getFirebaseStudentName(sensedUID);
  if (studentName == "") {//empty string returned from accessing nonexistent UID in FIrebase means student is not in system
    Serial.println(" Access Denied. Name doesn't exist. ");
    delay(50); //TODO: remove later, inefficient
  }
  else if (checkStudentInBuilding(sensedUID)) {  //if student is already in the building, then deny access.
    Serial.println(" Access Denied. Student is already in building. ");
    delay(50); //TODO: remove later, inefficient
  }
  else {
    updateFBStudentStatus(sensedUID, "in");
    updateFBStudentLastLoc(sensedUID, lockBuildingName);
    doorTempUnlock();
  }
}

//For processing RFID UID when exiting
void checkRFIDExit (String sensedUID) {
  updateFBStudentStatus(sensedUID, "out");
  updateFBStudentLastLoc(sensedUID, lockBuildingName); //redundant
}
