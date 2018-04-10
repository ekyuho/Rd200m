/*
//    FILE: Rd200m.cpp
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: April 10, 2018
// Released to the public domain
*/
#include "Arduino.h"
#include <SoftwareSerial.h>
#include "Rd200m.h"


Rd200m::Rd200m(SoftwareSerial* s) {
  radon = s;
  radon->begin(19200);
  _stat = "";
  _debug = false;
  _ready = false;
}

void Rd200m::request(void) {
  radon->write(req, 4);    
  _stat = "";
  _ready = false;
  LOG("request radon\n");
}

void Rd200m::reset(void) {
  radon->write(_reset, 4);    
  _stat = "";
  _ready = false;
  LOG("reset radon\n");
}

void Rd200m::update(void) {
  while (radon->available() > 0) {
    byte c = radon->read();
    LOG("("+ String(state)+")");
    if (!(c&0xf0)) LOG("0");
    LOG(c, HEX);
    LOG(" ");

    switch (state) {
      case 0: if (c == 0x02) { state = 1; csum=0; _stat = ""; } break;
      case 1: if (c == 0x10) { state = 2; csum += c; } else state = 0; break;
      case 2: if (c == 0x04) { state = 3; csum += c; } else state = 0; break;
      case 3: state = 4; csum += c; 
        if (c == 0xE0) _stat += "Shocked; ";
        _up = 0x3&c;
        if (_up < 2) _stat += upmsg[_up];
        break;
      case 4: 
       _elapsed = c; csum += c; state = 5;
       if (_stat) _stat += " ("+ String(_elapsed) +" min up)";
        break;
      case 5: _value = c; state = 6; csum += c; break;
      case 6: _value += c/100.; state = 7; csum += c; break;
      case 7: csum += c; 
        if (0xff != csum) { 
          LOG(" Checksum error\n"); 
          LOG(csum, HEX); LOG("\n");
          if (_elapsed < 2) _stat = upmsg[_elapsed];
           _stat += "checksum error; ";
        } else
          LOG(" Good value\n");
        _ready = true;
        state = 0; 
        if (_onPacket) _onPacket();
        break;
    }
  }
}

float Rd200m::value(void) {
  return(_value);
}

String Rd200m::uptime(void) {
  return(upmsg[_up]);
}

int Rd200m::elapsed(void) {
  return(_elapsed);
}

String Rd200m::status(void) {
  return(_stat);
}

void Rd200m::debug(int level) {
  _debug = level;
  LOG("Debug ON\n");
}

void Rd200m::LOG(String s) {
  if (_debug) Serial.print(s);
}

void Rd200m::LOG(byte a, int b) {
  if (_debug) Serial.print(a, b);
}

void Rd200m::onPacket(void (*function)(void) ) { _onPacket = function; }
void Rd200m::stop(void) { _onPacket = NULL; }

bool Rd200m::ready(void) { return(_ready); }
