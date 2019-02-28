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
  if (PRINTING) {
    Serial.print("Sensed UID: ");
    Serial.println(sensedUID);
  }
  if (!studentExists(sensedUID)) {
    if (PRINTING) {
      Serial.println("Access Denied. Student name doesn't exist.");
    }
    yield();
  }
  else if (checkStudentInBuilding(sensedUID)) {  //if student is already in the building, then deny access.
    if (PRINTING) {
      Serial.println("Access Denied. Student already in building.");
    }
    yield();
  }
  else {  //Note: If the name doesn't exist but somehow the function gets to this point, a new entry will be added to the Firebase database with the UID, with blank information.
    updateFBStudentStatus(sensedUID, "in");
    updateFBStudentLastLoc(sensedUID, lockBuildingName);
    doorTempUnlock();
  }
}

//For processing RFID UID when exiting
void checkRFIDExit (String sensedUID) {
  if (studentExists(sensedUID)) {
    updateFBStudentStatus(sensedUID, "out");
    updateFBStudentLastLoc(sensedUID, lockBuildingName); //redundant
  }
}

//Generates random XX XX XX XX UID
String generateRandomUID() {
  String randomUID = "";
  for (int i = 0; i < 11; i++) {  //loops 11 times because 11 characters (including spaces) in UID
    char letter = ' ';
    if (i != 2 && i != 5 && i != 8) { // if in 3rd, 6th, or 9th spot of the string, use a ' '
      byte randomValue = random(0, 37);
      letter = randomValue + 'a';
      if (randomValue > 26) {
        letter = (randomValue - 26) + '0';
      }
    }
    randomUID += letter;
  }
  randomUID.toUpperCase();
  return randomUID;
}
