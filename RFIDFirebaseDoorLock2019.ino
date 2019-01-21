//jan 16. 2019. combo of RFID and ESP Firebase. First real attempt

//IMPORTANT NOTE: Make sure to include firebase-arduino library (https://github.com/FirebaseExtended/firebase-arduino)
//Also need to download this Arduino json library for firebase to work (https://github.com/bblanchon/ArduinoJson)

//set vs push. 'set' changes a value in the firebase to a value. Push appends a compltely new variable to the path location

//Libraries
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <SPI.h>
#include <MFRC522.h>

//Firebase and Wifi
#define FIREBASE_HOST "secondasrtest.firebaseio.com"  //Firebase name. Format: INSERT_NAME.firebaseio.com (Don't inlude "https" or "/")
#define FIREBASE_AUTH "z30eo7UMbHrcrSlgPpmWvSCHY01JTrnSKDTHZu6X"  //Firebase secret. Deprecated, may want to find another way.
#define WIFI_SSID "justinq" //Network name
#define WIFI_PASSWORD "funwithjoemar" //Network password

//RFID Pins
#define SS_PIN 4  //4 goes to D2 on the ESP
#define RST_PIN 5 //5 goes to D1 on the ESP

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;

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
}

void loop() {
  delay(500); //TODO inefficient
  String currentUID = getCurrentRFID();
  if (currentUID == "none") {
    return;
  }
  else {
    checkRFID(currentUID);
  }
}
