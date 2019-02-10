//IMPORTANT NOTE: Make sure to include firebase-arduino library (https://github.com/FirebaseExtended/firebase-arduino)
//Also need to download this Arduino json library for firebase to work (https://github.com/bblanchon/ArduinoJson)

//IMPORTANT NOTE 2: If getting error related to JSON files, make sure you are using an earlier of the ArduinoJson library (5 or earlier), NOT THE BETA VERSION
//set vs push. 'set' changes a value in the firebase to a value. Push appends a compltely new variable to the path location

//Notes for UUID
//In Firebase, I should create each student with a UUID using a UUID generator.
//When checking the Firebase list, I should check every UUID.

////Pin reassignments for ESP
//static const uint8_t D0   = 16;
//static const uint8_t D1   = 5;
//static const uint8_t D2   = 4;
//static const uint8_t D3   = 0;
//static const uint8_t D4   = 2;
//static const uint8_t D5   = 14;
//static const uint8_t D6   = 12;
//static const uint8_t D7   = 13;
//static const uint8_t D8   = 15;
//static const uint8_t D9   = 3;
//static const uint8_t D10  = 1;

//Libraries
#include <ESP8266WiFi.h>  //for ESP devices
#include <FirebaseArduino.h>  //firebase library
#include <SPI.h>
#include <MFRC522.h>  //rfid reader
#include <CapacitiveSensor.h> //capacitative sensor door handles

//Firebase and Wifi
#define FIREBASE_HOST "secondasrtest.firebaseio.com"  //Firebase name. Format: INSERT_NAME.firebaseio.com (Don't inlude "https" or "/")
#define FIREBASE_AUTH "z30eo7UMbHrcrSlgPpmWvSCHY01JTrnSKDTHZu6X"  //Firebase secret. Deprecated, may want to find another way.
#define WIFI_SSID "justinq" //Network name
#define WIFI_PASSWORD "funwithjoemar" //Network password

//RFID Pins
#define SS_PIN 4  //4 goes to D2 on the ESP
#define RST_PIN 5 //5 goes to D1 on the ESP
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

//FIX PIN VALUES:  https://arduino.stackexchange.com/questions/34135/in-esp-12e-nodemcu-whats-the-pin-number-of-a0

//(send pin , receive pin). Connect handle to receive pin side of resistor.
CapacitiveSensor handleEnter = CapacitiveSensor(15, 0); // 500-1000 ohm resistor between pins D8 (send) and D3 (receive).
CapacitiveSensor handleExit = CapacitiveSensor(15, 2); // 500-1000 ohm resistor between pins D8 (send) and D4 (receive).
int statuss = 0;
long enterThreshold = 100;
long exitThreshold = 100;
boolean enterState = true;  //true = last person was entering. false = last person was exiting.
String lockBuildingName = "Building A"; //building name associated with this particular lock

void setup() {
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
  //mfrc522.PCD_WriteRegister(RFCfgReg, (0x07<<4));//supposed to apparently increase gain of the RC522 and extend the range?

  //  handleEnter.set_CS_AutocaL_Millis(0xFFFFFFFF);  // turn off autocalibrate on channel 1
  //  handleExit.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on channel 1
}

//NE
void loop() {
  delay(200); //TODO inefficient. 200 ms delay
  updateEnterState();
  if (!enterState && exitHandleTouched()) { //if exit handle last touched and currently touched
    doorUnlock(); //still needs to sense rfid at some point. TODO: still sense RFID
  }

  String sensedUID = getCurrentRFID(); //gets current RFID card from reader
  if (sensedUID == "none") { //if no valid RFID detected by reader
    return; //goes back to start of the loop.
  }
  else {  //there is an RFID element present
    if (enterState) {
      checkRFIDEnter(sensedUID);  //passes on UID to a method to check if the UID is valid and to perform the appropriate actions
    }
    else {
      checkRFIDExit(sensedUID);
    }
  }
}
//
//void loop() {
//
//    checkExit();  //If person is exiting, open the door automatically. TODO: add code for sending RFID in certain scenarios.
//    delay(200); //TODO inefficient. 200 ms delay.
//    String sensedUID = getCurrentRFID(); //gets current RFID card from reader
//    if (sensedUID == "none") { //if no valid RFID detected by reader
//      return; //goes back to start of the loop.
//    }
//    else {  //there is an RFID element present
//      checkRFID(sensedUID);  //passes on UID to a method to check if the UID is valid and to perform the appropriate actions
//    }
//}

//Trying to receive data:
//  Serial.println(getFirebaseData("/Students/80 C4 12 BB/Name"));
//  delay(50);
//  Serial.println(getFirebaseData("/Students/0001/Title"));
//  delay(50);
//  Serial.println(getFirebaseData("/Students/eee"));
//  delay(50);

//Printing capacitative sensor values:
//printEnterHandle();
//printExitHandle();

