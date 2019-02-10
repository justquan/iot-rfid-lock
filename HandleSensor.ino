
//prints capacitative sensor value for enter handle. for debugging.
void printEnterHandle() {
  long enterVal = handleEnter.capacitiveSensor(30);
  Serial.print("Enter handle value: ");
  Serial.println(enterVal);
  delay(250);
}

//prints capacitative sensor value for exit handle. for debugging.
void printExitHandle() {
  long exitVal = handleExit.capacitiveSensor(30);
  Serial.print("Exit handle value: ");
  Serial.println(exitVal);
  delay(10);
}

void updateEnterState() {
  long enterVal = handleEnter.capacitiveSensor(30);
  long exitVal = handleExit.capacitiveSensor(30);
  Serial.print("Enter handle value: ");
  Serial.println(enterVal);
  Serial.print("Exit handle value: ");
  Serial.println(exitVal);
  if (exitHandleTouched()) { //if both handles touched at once, it messes up, and favors / assumes only the exit state. TODO: fix
    enterState = false; //currently biased towards exiting student if both handles touched
  }
  else if (enterHandleTouched()) {
    enterState = true;
  }
}

boolean enterHandleTouched() {
  long enterVal = handleEnter.capacitiveSensor(30);
  //  Serial.print("Enter handle value: ");
  //  Serial.println(enterVal);
  if (enterVal >= enterThreshold) {
    Serial.println("Enter handle is being touched!");
    return true;
  }
  else {
    return false;
  }
}

boolean exitHandleTouched() {
  long exitVal = handleEnter.capacitiveSensor(30);
  //  Serial.print("Exit handle value: ");
  //  Serial.println(enterVal);
  if (exitVal >= exitThreshold) {
    Serial.println("Exit handle is being touched!");
    return true;
  }
  else {
    return false;
  }
}

