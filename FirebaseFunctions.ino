//get returns a student ID from Firebase given a certain path
//if the path is just in one group (i.e. "students"), you don't need a / in the beginning.
//if the path is in two groups (i.e. "UID" in "students"), the PATH is "students/UID".
//NOTE: IF NOTHING FOUND / PATH DOES NOT EXIST, THEN IT RETURNS AN EMPTY "" STRING.
//NOTE: Seems like if there's no / short (< 100ms) delays, the connections does not cut out. If idle for long periods, WiFI may fail.
//Firebase Library doc: https://media.readthedocs.org/pdf/firebase-arduino/latest/firebase-arduino.pdf
//set vs push. 'set' changes a value in the firebase to a value. Push appends a compltely new variable to the path location

String generateStatusPath (String UID) {
  String statusFirebasePath = "/" + dataCategory + "/" + UID + "/status/"; //concatonates sensed UID to "/Students/" to create checkable PATH
  return statusFirebasePath;
}

String generateLastLocPath (String UID) {
  String lastlocFirebasePath = "/" + dataCategory + "/" + UID + "/lastloc/"; //concatonates sensed UID to "/Students/" to create checkable PATH
  return lastlocFirebasePath;
}

String generateStudentNamePath(String UID) {
  String nameFirebasePath = "/" + dataCategory + "/" + UID + "/Name/"; //concatonates sensed UID to "/Students/" to create checkable PATH
  return nameFirebasePath;
}

String getFirebaseData (String path) {
  String firebaseData = Firebase.getString(path);  //Gets string at PATH
  checkFirebaseFail("getting data from Firebase");  //Check is Firebase.getString(String path) failed
  yield();  //delays for a tiny amount of time
  return firebaseData; //returns UID
}

String getFirebaseStudentName (String sensedUID) {
  String checkedFirebasePath = generateStudentNamePath(sensedUID);
  String studentName = getFirebaseData(checkedFirebasePath);  //checked student's name according to the database/ If UID doesn't exist in Firebase, returns empty string.
  checkFirebaseFail("getting student name from Firebase");  //Check is Firebase.getString(String path) failed
//  if (!PRINTING) {
    Serial.print("Student name: ");
    Serial.println(studentName);
//  }
  return studentName;
}

boolean checkStudentInBuilding (String sensedUID) {
  String statusFirebasePath = generateStatusPath(sensedUID);
  String lastlocFirebasePath = generateLastLocPath(sensedUID);
  String studentStatus = getFirebaseData(statusFirebasePath);  //checked student's status (in/out) according to the database/ If UID doesn't exist in Firebase, returns empty string.
  String studentLastLoc = getFirebaseData(lastlocFirebasePath);  //checked student's lastloc(like 'Building A') according to the database/ If UID doesn't exist in Firebase, returns empty string.
  checkFirebaseFail("getting student lastloc and status from Firebase");  //Check is Firebase.getString(String path) failed
  if (studentStatus == "in" && studentLastLoc == lockBuildingName) {  //if student is in building affiliated with this particular lock device
    return true;  //student IS in this building
  }
  else {  //student IS NOT in this building
    return false;
  }
}

void checkFirebaseFail(String msg) { //string is the message / the instance in which using Firebase failed
  if (Firebase.failed()) {  //if the last Firebase function failed
    Serial.print("Firebase failed while ");
    Serial.print(msg);  //prints given message / place where Firebase failed
    Serial.println(Firebase.error()); //Sometimes doesn't print any error but error still exists
  }
}

//TODO: TEST. UNTESTED 2/10. ststring() untested
void updateFBStudentStatus(String UID, String updatedVal) {
  String path = generateStatusPath(UID);
  Firebase.setString(path, updatedVal);
  Serial.print("Updated Firebase status at");
  Serial.print(path);
  Serial.print(" with value: ");
  Serial.println(updatedVal);
}

void updateFBStudentLastLoc(String UID, String updatedVal) {
  String path = generateLastLocPath (UID);
  Firebase.setString(path, updatedVal);
  Serial.print("Updated Firebase lastloc at");
  Serial.print(path);
  Serial.print(" with value: ");
  Serial.println(updatedVal);
}

//checks to see if students exists in Firebase database according to their UID
boolean studentExists(String UID) { //TODO: address how this is based on the name only, so if a FIrebase entry with a UID only has the lastloc and status, it would be considered an invalid student
  String studentName = getFirebaseStudentName(UID);
  if (studentName == "") {//empty string returned from accessing nonexistent UID in FIrebase means student is not in system
    return false;
  }
  else {
    return true;
  }
}

//Only searches through a single UID, so might want to rand/ UID each time or search through actula UIDs in the system.
//for gathering data nad averaging the amount of time it takes to find and update info for a particular student
//Can use to test response time with a UID that actually does match in the system.
void testFBDelay(String UID, int trials) {
  Serial.println("Start of testFBDelay()");
  checkRFIDEnter(generateRandomUID());  //This is a 'throwaway test', because the first check always takes 1000=1500 ms instead of the normal ~200 ms to process.
  Serial.println("First UID uncounted test done");
  long totalTime = 0; //cumulative value of all of the time taken.
  for (int i = 0; i < trials; i++) {
    timeElapsed = 0;  //resetting variable, because it starts counting after device is reset, and at all times otherwise.
    //do stuff
    checkRFIDEnter(UID);  //TODO: change in firebase and check a "test database" with 100, 1000, and 10000 made-up UIDs.
    //end of doing stuff
    Serial.println(timeElapsed);
    totalTime += timeElapsed;
  }
  long averageTime = totalTime / trials;
  Serial.print("Avg time: ");
  Serial.println(averageTime);
  Serial.println("END of testFBDelay()");
  delay(10000); //10s delay
}

//untested 2/27
//same as testFBDelay() but with random UIDs made each time.
//Tests with (very likely) all random UIDs not matching. Matching UIDs may vary the response time.
void testRandomFBDelay(int trials) {
  Serial.println("Start of testRandomFBDelay()");
  checkRFIDEnter(generateRandomUID());  //This is a 'throwaway test', because the first check always takes 1000=1500 ms instead of the normal ~200 ms to process.
  Serial.println("First UID uncounted test done");
  long totalTime = 0; //cumulative value of all of the time taken.
  for (int i = 0; i < trials; i++) {
    String randUID = generateRandomUID(); //not included in timeElapsed, because randomUID isn't generated during actual process
    timeElapsed = 0;  //resetting variable, because it starts counting after device is reset, and at all times otherwise.
    //do stuff
    checkRFIDEnter(randUID);  //TODO: change in firebase and check a "test database" with 100, 1000, and 10000 made-up UIDs.
    //end of doing stuff
    Serial.println(timeElapsed);
    totalTime += timeElapsed;
  }
  long averageTime = totalTime / trials;
  Serial.print("Avg time: ");
  Serial.println(averageTime);
  Serial.println("END of testRandomFBDelay()");
  delay(10000); //10s delay
}
