#include "Screen.h"

Screen::Screen(LiquidCrystal_I2C* lcd): lcd(lcd){
}

void Screen::init(){
    lcd->init();
    lcd->backlight();
    lcd->print("   Dyspenser");
}

void Screen::updateLCD(vector<String> lines, int startIndex){
    if(!lines.empty()){ 
        lcd->clear();
        for (int i = 0; i < LCD_L; i++){
            if(i+startIndex<lines.size()){
                lcd->setCursor(0,i);
                lcd->print(lines[i+startIndex]);
            }
        } 
    }
}

void Screen::lineSet(int line){
    lcd->setCursor(LCD_W-1,line);
    lcd->print("<");
}
