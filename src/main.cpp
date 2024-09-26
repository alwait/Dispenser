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
    
Dispenser dispenser(&lcd, &servo, leds, buttons);
    
void setup() {

  Serial.begin(115200);

  dispenser.init();
  delay(1000);
  dispenser.updateMenu();
  dispenser.cancel();
}

void loop() {
  switch (dispenser.getOption()){
  case 0:
    dispenser.setOption(dispenser.updateMenu());
    break;
  case 1:
    dispenser.setOption(dispenser.autoMode());
    break;
  case 2:
    dispenser.setOption(dispenser.volumeMode());
    break;
  case 3:
    dispenser.setOption(dispenser.manualMode());
    break;
  default:
    break;
  }
  dispenser.repeat();
  dispenser.syncAnalogSensorsLED();
  delay(10);
}

