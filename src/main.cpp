#include <Arduino.h>
#include "Dispenser.h"

//Dispenser dispenser();
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;
CRGB leds[NUM_LEDS];
BfButton btn_select(BfButton::STANDALONE_DIGITAL, SELECT_PIN, true, LOW);
BfButton btn_cancel(BfButton::STANDALONE_DIGITAL, CANCEL_PIN, true, LOW);
BfButton btn_up(BfButton::STANDALONE_DIGITAL, UP_PIN, true, LOW);
BfButton btn_down(BfButton::STANDALONE_DIGITAL, DOWN_PIN, true, LOW);

vector<BfButton*> buttons={&btn_select, &btn_cancel, &btn_up, &btn_down};
unsigned long timer=0;
    
Dispenser dispenser(&lcd, &servo, leds, buttons, timer);
    
void setup() {

  Serial.begin(115200);

  dispenser.init();
  delay(1000);
  dispenser.updateMenu();
  dispenser.cancel();
  //dispenser.setOption(1);
}

void loop() {
  switch (dispenser.getOption()){
  case 0:
    dispenser.setOption(dispenser.updateMenu());
    break;
  case 3:
    dispenser.setOption(dispenser.manualMode());
    break;
  default:
    break;
  }
  timer=millis();
  dispenser.repeat();
  dispenser.syncAnalogSensorsLED();
  delay(10);
}

