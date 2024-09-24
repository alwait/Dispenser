#ifndef MOTOR_H
#define MOTOR_H

#include "ESP32Servo.h"
#include "GPIO.h"

class Motor{
private:
    Servo* servo;
    int positions[NUM_LEDS];
public:
    Motor(Servo*);
    ~Motor(){delete servo;};
    void init();
    void setPosition(int i){servo->write(positions[i]);};
};

#endif // MOTOR