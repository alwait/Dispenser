#ifndef LED_H
#define LED_H

#include "FastLED.h"
#include "GPIO.h"

class ControlLED{
private:
    CRGB* leds;
public:
    ControlLED(CRGB*);
    ~ControlLED(){delete leds;};
    void init();
    void setColor(int, CRGB);
    void update();
};

#endif // LED