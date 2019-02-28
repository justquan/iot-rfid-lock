//IMPORTANT NOTE: Make sure to include firebase-arduino library (https://github.com/FirebaseExtended/firebase-arduino)
//Also need to download this Arduino json library for firebase to work (https://github.com/bblanchon/ArduinoJson)

//IMPORTANT NOTE 2: If getting error related to JSON files, make sure you are using an earlier of the ArduinoJson library (5 or earlier), NOT THE BETA VERSION

//Notes for UUID
//In Firebase, I should create each student with a UUID using a UUID generator.
//When checking the Firebase list, I should check every UUID.

//Pin reassignments for ESP
//FIX PIN VALUES:  https://arduino.stackexchange.com/questions/34135/in-esp-12e-nodemcu-whats-the-pin-number-of-a0
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
#include <elapsedMillis.h> // library for elapsed time

//Firebase and Wifi
#define FIREBASE_HOST "secondasrtest.firebaseio.com"  //Firebase name. Format: INSERT_NAME.firebaseio.com (Don't inlude "https" or "/")
#define FIREBASE_AUTH "z30eo7UMbHrcrSlgPpmWvSCHY01JTrnSKDTHZu6X"  //Firebase secret. Deprecated, may want to find another way.
#define WIFI_SSID "justinq" //Network name
#define WIFI_PASSWORD "funwithjoemar" //Network password
const String dataCategory = "Students"; //Important string. This is the Firebase category that the program checks for UIDs. i.e. checks /

//Debugging
static boolean DEBUG = true;
static boolean PRINTING = false;
elapsedMillis timeElapsed = 0;  //for debugging. Should periodically reset to 0 to prevent overflow


//RFID Pins
#define SS_PIN 4  //4 goes to D2 on the ESP
#define RST_PIN 5 //5 goes to D1 on the ESP
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

//(send pin , receive pin). Connect handle to receive pin side of resistor.
CapacitiveSensor handleEnter = CapacitiveSensor(15, 0); // 500-1000 ohm resistor between pins D8 (send) and D3 (receive).
CapacitiveSensor handleExit = CapacitiveSensor(15, 2); // 500-1000 ohm resistor between pins D8 (send) and D4 (receive).
//int handleSensorSwitch = 16;  //16 = Pin D0

//With 8.5-9V battery attached to the solenoid, there's a .6A current draw 
int lockRelayPin = 16;  //Pin D0, to Signal pin of relay

const long enterThreshold = 70;
const long exitThreshold = 70;
const int unlockDelay = 2500; //unlock delay in ms
boolean enterState = true;  //true = last person was entering. false = last person was exiting.
const String lockBuildingName = "Building A"; //building name associated with this particular lock

