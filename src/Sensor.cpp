#include "Sensor.h"

Sensor::Sensor(int Pin){
    pin=Pin,
    sensivity=30;
}

bool Sensor::digitalSensorRead(){
    if(analogRead(pin)>=((int)round((float)ANALOG_MAX*(float)sensivity/100.))){
        return true;
    }
    else return false;
}

float Sensor::analogSensorRead(){
    return (float)map(analogRead(pin), 0, ANALOG_MAX, 0, 100);
}
