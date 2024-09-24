#ifndef LCD_H
#define LCD_H

#include "Screen.h"
#include "Keyboard.h"
#include <Arduino.h>

using namespace std;

struct Option{
    int number;
    String name;
    vector<Option> subOption;

    Option(int Number, String Name) : number(Number), name(Name){}
};

class OperatingPanel{
private:
    Screen screen;
    Keyboard keyboard;
    vector<Option> options;
    vector<int> menuOption;
    int cursor;
public:
    OperatingPanel(LiquidCrystal_I2C*, vector<BfButton*>);
    int updateMenuScreen();
    void updateScreen(vector<String> lines, int startIndex){screen.updateLCD(lines, startIndex);};
    void init();
    void repeat();
    void lineSet(int i){screen.lineSet(i);};
    void cursorUp(){cursor--;};
    void cursorDown(){cursor++;};
    bool isKeysChanged(){return keyboard.isKeysChanged();};
    int getCursorPosition(){return cursor;};
    void setCursorPosition(int i){cursor=i;};
    int getKey(int i){return keyboard.getKey(i);};
    void resetKeys(){keyboard.resetKeys();};
    void cancel(){keyboard.cancelKey();};
};

#endif // LCD