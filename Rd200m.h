/*
//    FILE: Rd200m.h
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: April 10, 2018
// Released to the public domain
*/
#ifndef Rd200m_h
#define Rd200m_h

#include "Arduino.h"

class Rd200m 
{
  public:
    Rd200m(SoftwareSerial*);
    float value(void);
    String uptime(void);
    int elapsed(void);
    String status(void);
    void update(void);
    void request(void);
    void reset(void);
    void debug(int);
    void LOG(String);
    void LOG(byte, int);
    void onPacket(void (*function)(void));
    void stop(void);
    bool ready(void);
  private:
    String upmsg[5] = {"less than  3 min", "less than 1 hour", "more than 1 hour" };
    byte req[4] = { 0x02, 0x01, 0x00, 0xFE };
    byte _reset[4] = { 0x02, 0xA0, 0x00, 0xFF-0xA0 };
    SoftwareSerial* radon;
    float _value;
    int _elapsed;
    int _up;
    int csum;
    int state;
    String _stat;
    bool _debug;
    bool _ready;
    void (*_onPacket)(void);
};
#endif
