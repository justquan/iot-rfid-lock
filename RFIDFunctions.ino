
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

void checkRFID(String currentUID) {
  String databaseUID = getFirebaseData("/Students/0001/UID"); //change later to search through list of all students
  Serial.print("Current UID: ");
  Serial.println(currentUID);
  Serial.print("UID in database: ");
  Serial.println(databaseUID);
  if (currentUID == databaseUID) //change UID of the card that you want to give access
  {
    Serial.println(" Access Granted ");
    Serial.println();
    statuss = 1;
  }

  else   {
    Serial.println(" Access Denied ");
    delay(1000);
  }
}
