#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>

using namespace std;

class Pump{
private:
    bool signal;
    int pin;
    float power;
    unsigned long& timer;
public:
    Pump(int, unsigned long&);
    void init();
    void repeat();
    void turnOn(){signal=true;};
    void turnOff(){signal=false;};
};

#endif // PUMP