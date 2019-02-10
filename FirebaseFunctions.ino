
//get returns a student ID from Firebase given a certain path
//if the path is just in one group (i.e. "students"), you don't need a / in the beginning.
//if the path is in two groups (i.e. "UID" in "students"), the PATH is "students/UID".
//NOTE: IF NOTHING FOUND / PATH DOES NOT EXIST, THEN IT RETURNS AN EMPTY "" STRING.
//NOTE: Seems like if there's no / short (< 100ms) delays, the connections does not cut out. If idle for long periods, WiFI may fail.

String getFirebaseData (String path) {
  String firebaseData = Firebase.getString(path);  //Gets string at PATH
  checkFirebaseFail("getting UID from Firebase");  //Check is Firebase.getString(String path) failed
  //  yield();
  delay(100);
  return firebaseData; //returns UID
}

String getFirebaseStudentName (String sensedUID) {
  String checkedFirebasePath = "/Students/" + sensedUID + "/Name/"; //concatonates sensed UID to "/Students/" to create checkable PATH
  String studentName = getFirebaseData(checkedFirebasePath);  //checked student's name according to the database/ If UID doesn't exist in Firebase, returns empty string.
  checkFirebaseFail("getting student name from Firebase");  //Check is Firebase.getString(String path) failed
  Serial.print("Student name: ");
  Serial.println(studentName);
}

boolean checkStudentInBuilding (String sensedUID) {
  String statusFirebasePath = "/Students/" + sensedUID + "/Status/"; //concatonates sensed UID to "/Students/" to create checkable PATH
  String lastlocFirebasePath = "/Students/" + sensedUID + "/Status/"; //concatonates sensed UID to "/Students/" to create checkable PATH
  String studentStatus = getFirebaseData(statusFirebasePath);  //checked student's status (in/out) according to the database/ If UID doesn't exist in Firebase, returns empty string.
  String studentLastLoc = getFirebaseData(lastlocFirebasePath);  //checked student's lastloc(like 'Building A') according to the database/ If UID doesn't exist in Firebase, returns empty string.
  checkFirebaseFail("getting student lastloc and status from Firebase");  //Check is Firebase.getString(String path) failed
  if (studentStatus == "in" && studentLastLoc == lockBuildingName) {  //if student is in building affiliated with this particular lock device
    return false;
  }
  else {
    return true;
  }
}

void checkFirebaseFail(String msg) { //string is the message / the instance in which using Firebase failed
  if (Firebase.failed()) {  //if the last Firebase function failed
    Serial.print("Firebase failed while ");
    Serial.print(msg);  //prints given message / place where Firebase failed
    Serial.println(Firebase.error()); //Sometimes doesn't print the error
  }
}
