#include <TinyGPS++.h>  // link to latest version http://arduiniana.org/libraries/tinygpsplus/
#include <SoftwareSerial.h>
#include <String.h>
#include <stdlib.h>

static const int WBID = 1234;  // unique identifier for the GEWB
static const String USERNAME = "mabdrabo";
static String CATEGORY = "police";

static const int RXPin = 9, TXPin = 3;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);

}

void loop() {
  float latt = gps.location.lat();
  float lon = gps.location.lng();
  String params = "";
  String params2 = "";
  char lattTemp[9];
  dtostrf(latt,1,6,lattTemp);
  char lonTemp[9];
  dtostrf(lon,1,6,lonTemp);
  params = lonTemp;
  params2 = lattTemp;
  params = params.substring(0,9);
  String data = "lon=" + params +"&lat=" + params2 + "&uname=" + USERNAME + "&wbid=" + WBID + "&cat=" + CATEGORY;
  Serial.println(data);
//  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
//  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  Serial.println();
  
  smartDelay(1000);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec) {
  if (!valid) {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}
