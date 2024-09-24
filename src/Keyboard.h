#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <BfButton.h>
#include <vector>
#include <Arduino.h>
#include "GPIO.h"

using namespace std;

enum Press{
    press_none,
    press_single,
    press_double,
    press_long,
};

enum Keys{
    key_select,
    key_cancel,
    key_up,
    key_down
};

class Keyboard{
private:
    vector<BfButton*> buttons;
    static int keys[NUM_BUTTONS];
    static int pins[NUM_BUTTONS];
public:
    Keyboard(vector<BfButton*>);
    ~Keyboard();
    void init();
    void repeat();
    static void resetKey(int i){keys[i]=0;};
    static void resetKeys();
    static int getKey(int i){return keys[i];};
    static bool isKeysChanged();
    static void pressHandler(BfButton *btn, BfButton::press_pattern_t pattern);
    static void cancelKey(){keys[key_cancel]=1;};
};

static void pressSelectHandler (BfButton *, BfButton::press_pattern_t);

#endif // KEYBOARD