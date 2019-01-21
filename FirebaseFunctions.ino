
//get returns a student ID from Firebase given a certain path
//if the path is just in one group (i.e. "students"), you don't need a / in the beginning.
//if the path is in two groups (i.e. "UID" in "students"), the PATH is "students/UID".
String getFirebaseData (String path) {
  String firebaseData = Firebase.getString(path);  //Gets string at PATH
  checkFirebaseFail("getting UID from Firebase");  //Check is Firebase.getString(String path) failed
  //  yield();
  delay(100);
  return firebaseData; //returns UID
}

void checkFirebaseFail(String msg) { //string is the message / the instance in which using Firebase failed
  if (Firebase.failed()) {  //if the last Firebase function failed
    Serial.print("Firebase failed while ");
    Serial.print(msg);  //prints given message / place where Firebase failed
    Serial.println(Firebase.error()); //Sometimes doesn't print the error
  }
}
