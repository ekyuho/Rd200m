/*
//    FILE: read_rd200m_values.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: April 10, 2018
// Released to the public domain
*/
// https://www.slideshare.net/radonFTlabkorea/ftlabdatasheet-rd200-mv12eng 

#define INTERVAL 60  //sec
#include <SoftwareSerial.h>
SoftwareSerial r(D4, D3);  // Rx,Tx

#include <Rd200m.h>
Rd200m radon(&r);

void getit() {
  Serial.print(String(radon.value()) + String(" pCi"));
  String s = radon.status();
  if (s != "") Serial.println(String(" (")+ s +String(")"));
  else Serial.println();
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nRadon Sensor RD200M V1.0 April 10, 2018");
  radon.debug(0);
  radon.onPacket(getit);
}

void loop() {   
  static unsigned mark = 0;
  
  if (millis() > mark) {
	  mark = millis() + (INTERVAL * 1000);
	  radon.request();
  }
  radon.update();
}
