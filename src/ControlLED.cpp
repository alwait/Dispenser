#include "ControlLED.h"

ControlLED::ControlLED(CRGB* leds): leds(leds){

}

void ControlLED::init(){
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i]=CRGB(1, 1, 1);
    }
    FastLED.show();
}

void ControlLED::setColor(int index, CRGB color){
    if(index<=NUM_LEDS && index>=0){
        leds[index]=color;
    }
}

void ControlLED::update(){
    FastLED.show();
}