#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include "LiquidCrystal_I2C.h"
#include "GPIO.h"

#define LCD_L 2
#define LCD_W 16

using namespace std;

class Screen{
private:
    LiquidCrystal_I2C *lcd;
    int cursor;
public:
    Screen(LiquidCrystal_I2C*);
    ~Screen(){delete lcd;};
    void init();
    void updateLCD(vector<String>, int);
    void lineSet(int);
};

#endif // SCREEN