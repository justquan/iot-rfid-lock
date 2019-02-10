
String getCurrentRFID() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) //If no card is detected
  {
    return "none";  //return "none" string
  }
  // Select one of the cards
  else if ( ! mfrc522.PICC_ReadCardSerial())
  {
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
//
////OLD. use checkrfidexit and checkrfidenter()
//void checkRFID(String currentUID) {
//  String databaseUID = getFirebaseData("/Students/0001/UID"); //TODO: change to search through list of all students
//  Serial.print("Current UID: ");
//  Serial.println(currentUID);
//  Serial.print("UID in database: ");
//  Serial.println(databaseUID);
//  if (currentUID == databaseUID) //change UID of the card that you want to give access
//  {
//    Serial.println(" Access Granted ");
//    Serial.println();
//    statuss = 1;
//  }
//  else   {
//    Serial.println(" Access Denied ");
//    delay(1000);
//  }
//}

//UNTESTED 2/10
//For processing RFID UID when entering
void checkRFIDEnter(String sensedUID) { //TODO: TEST. Especially string addign with +
  Serial.print("Sensed UID: ");
  Serial.println(sensedUID);
  String studentName = getFirebaseStudentName(sensedUID);
  if (studentName = "") {//empty string returned from accessing nonexistent UID in FIrebase means student is not in system
    Serial.println(" Access Denied. Name doesn't exist. ");
    delay(500);
  }
  else if (checkStudentInBuilding(sensedUID)) {  //if student is already in the building, then deny access.
    Serial.println(" Access Denied. Student is already in building. ");
    delay(500);
  }
  else {
    //TODO: add code / function updating firebase values
    doorUnlock();
  }
}

//For processing RFID UID when exiting
void checkRFIDExit (String sensedUID) {
    //TODO: add code / function updating firebase values
}

