#include "Motor.h"

Motor::Motor(Servo* servo): servo(servo){
    for (size_t i = 0; i < NUM_LEDS; i++){
        positions[i]=60*((NUM_LEDS-1)-i);
    }

}

void Motor::init(){
    servo->setPeriodHertz(50); // standard 50 hz servo
    servo->attach(SERVO_PIN, 500, 2400); // attaches the servo on pin 18 to the servo object
    servo->write(positions[1]);
}