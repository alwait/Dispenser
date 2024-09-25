#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>

using namespace std;

class Pump{
private:
    bool signal;
    int pin;
    float power;
    unsigned long timer;
public:
    Pump(int);
    void init();
    void repeat();
    void turnOn(){signal=true; timer=millis();};
    void turnOff(){signal=false; timer=0;};
    unsigned long getTimer(){return timer;};
    void setTimer(unsigned long t){timer=t;};
    bool getSignal(){return signal;};
};

#endif // PUMP