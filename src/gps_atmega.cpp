#include <Arduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <TheThingsNetwork.h>

SoftwareSerial loraSerial = SoftwareSerial(7,8); //rx, tx

//SoftwareSerial loraSerial = SoftwareSerial(13,12); //rx, tx

#define debugSerial Serial

#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);


static const int RXPin = 12,  TXPin = 13;       //rx and tx pins for the GPS NEO-6

//static const int RXPin = 8,  TXPin = 7;       //rx and tx pins for the GPS NEO-6

static const uint32_t GPSBaud = 9600;

//The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS devicepio
SoftwareSerial ss(12,13);


//device eui-0004a30b00283615 for 03_08_2021
const char *appEui = "0000000000000000";

//gps2 appkey
//const char *appKey = "ECED770178E4DB84FA02EABDECA391CF";

//gps1 appkey
//const char *appKey = "239761EA601F9CDAC65C88104F5EF193";

//gps3 appkey
//const char *appKey = "272979DB5F7382C2DC6348AAFB0A9F6C";

//gps4 appkey
const char *appKey = "DEE629FE13EE0888033820A6EA513C36";


void setup(){
  pinMode(6, OUTPUT);
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  debugSerial.println("-- STATUS");
  ttn.showStatus();
  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
//Serial.begin(9600);/
 ss.begin(GPSBaud);
debugSerial.println("==========================");
debugSerial.println("GPS");
debugSerial.println("===========================");
}

void loop(){
  digitalWrite(6, LOW);
  delay(50);
  digitalWrite(6, HIGH);
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
      delay(2000);


      uint8_t payload[8];

     uint32_t lat = (gps.location.lat()) * 10000;
     uint32_t lng = (gps.location.lng()) *10000;

      payload[0] = lat;
      payload[1] = lat >> 8;
      payload[2] = lat >> 16;
      payload[3] = lat >> 24;

      payload[4] = lng;
      payload[5] = lng >> 8;
      payload[6] = lng >> 16;
      payload[7] = lng >> 24;

      debugSerial.print("Latitude= ");
      debugSerial.print(lat);
      debugSerial.print(" Longitude= ");
      debugSerial.println(lng);

      ttn.sendBytes(payload, sizeof(payload));
      delay(10000);
    }
  }
}
