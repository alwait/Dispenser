#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include "GPIO.h"

class Sensor{
private:
    int sensivity;
    int pin;
public:
    Sensor(int);
    bool digitalSensorRead();
    float analogSensorRead();
};

#endif // SENSOR