#include "Pump.h"

Pump::Pump(int pin, unsigned long& timer): pin(pin), timer(timer){
    power=100;
    signal=false;
};

void Pump::init(){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Pump::repeat(){
    digitalWrite(pin, signal);
}