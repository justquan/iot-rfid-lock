#include "Map.h"  //Includes pin assignments and libraries

void setup() {
  //  handleSensorPowerOff();
  pinMode(lockRelayPin, OUTPUT);
  //  pinMode(handleSensorSwitch, OUTPUT);//pin to base of transistor controlling the connection of the capacitive sensors on handles with 820 ohm resistor. Turned off at the start to prevent errors.
  Serial.begin(9600); //9600 baud
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //Start WiFI with the defined credentials
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) { //Prints '.' every 500 ms before connected
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP()); //Prints IP
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); //Start Firebase with defined credentials
  //  handleSensorPowerOn(); //handleSensor is on now, so capacitive sensors are now enabled, since they do not interfere with start at this point. Note: if not connected, it starts having stacks errors, not sure why.
  //mfrc522.PCD_WriteRegister(RFCfgReg, (0x07<<4));//supposed to apparently increase gain of the RC522 and extend the range?

  //  handleEnter.set_CS_AutocaL_Millis(0xFFFFFFFF);  // turn off autocalibrate on channel 1
  //  handleExit.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on channel 1
}

void loop() { //TODO: fix. also conditional on the fact that person entering must touch door handle before rfid reader picks up UID.
  delay(50); //TODO inefficient.
  updateEnterState(); //checks door handles
  if (!enterState && exitHandleTouched()) { //if exit handle last touched and currently touched
    doorTempUnlock();
  }
  String sensedUID = getCurrentRFID(); //gets current RFID card from reader
  if (sensedUID == "none") { //if no valid RFID detected by reader
    return; //goes back to start of the loop.
  }
  else {  //there is an RFID element present
    if (enterState) { //if enter side handle was touched last
      checkRFIDEnter(sensedUID);  //passes on UID to a method to check if the UID is valid and to perform the appropriate actions
    }
    else {  //if exit side handle was touched last
      checkRFIDExit(sensedUID);
    }
  }
constFBComms(); //necessary to prevent firebase from timing out and losing connection

//testRandomFBDelay(20);
//testFBDelay("AA AA AA AA", 30);
//delay(1000000);
}

