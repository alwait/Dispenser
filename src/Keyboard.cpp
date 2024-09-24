#include "Keyboard.h"

int Keyboard::keys[NUM_BUTTONS] = {0, 0, 0, 0};
int Keyboard::pins[NUM_BUTTONS] = {SELECT_PIN, CANCEL_PIN, UP_PIN, DOWN_PIN};


Keyboard::Keyboard(vector<BfButton*> buttons): buttons(buttons){
}
Keyboard::~Keyboard(){
  for (BfButton* button : buttons) {
    delete button;
  }
  buttons.clear();
}

bool Keyboard::isKeysChanged(){
  for (size_t i = 0; i < NUM_BUTTONS; i++){
    if(keys[i]!=press_none) return true;
  }
  return false;
}

void Keyboard::resetKeys(){
  for (size_t i = 0; i < NUM_BUTTONS; i++){
    resetKey(i);
  }
}

void Keyboard::pressHandler (BfButton *btn, BfButton::press_pattern_t pattern) {
  int pin=btn->getPin();
  if(pin==0) return;
  int key=0;
  for (size_t i = 0; i < NUM_BUTTONS; i++){
    if(pin==pins[i]){
      key=i;
      break;
    }
  }

  switch (pattern) {
    case BfButton::SINGLE_PRESS:
      keys[key]=press_single;
      break;
    case BfButton::DOUBLE_PRESS:
      keys[key]=press_double;
      break;
    case BfButton::LONG_PRESS:
      keys[key]=press_long;
      break;
  }
}

void Keyboard::init(){
    buttons.at(key_select)->onPress(pressHandler)
     .onDoublePress(pressHandler, 200) 
     .onPressFor(pressHandler, 1000); 

    buttons.at(key_cancel)->onPress(pressHandler)
     .onDoublePress(pressHandler, 200) 
     .onPressFor(pressHandler, 1000); 

    buttons.at(key_up)->onPress(pressHandler)
     .onDoublePress(pressHandler, 200) 
     .onPressFor(pressHandler, 1000); 

    buttons.at(key_down)->onPress(pressHandler)
     .onDoublePress(pressHandler, 200) 
     .onPressFor(pressHandler, 1000); 
}

void Keyboard::repeat(){
    for (size_t i = 0; i < buttons.size(); i++){
        buttons.at(i)->read();
    } 
}

