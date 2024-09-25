#include "Pump.h"

Pump::Pump(int pin): pin(pin){
    power=100;
    signal=false;
    timer=0;
};

void Pump::init(){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Pump::repeat(){
    digitalWrite(pin, signal);
}