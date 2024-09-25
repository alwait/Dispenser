#include "OperatingPanel.h"

OperatingPanel::OperatingPanel(LiquidCrystal_I2C *screen, vector<BfButton *> buttons) : screen(screen), keyboard(buttons)
{
    cursor = 0;
    Option option0(0, "1.Nalewaj!");
    Option option1(1, "2.Wybierz trunek");
    Option option2(2, "3.Pojemnosc");
    Option option3(3, "4.Tryb reczny");
    Option option4(4, "5.Opcje");
    Option option5(5, "6.Konf. zaawans.");

    Option subOption0(0, "1.Wodka");
    Option subOption1(1, "2.Drink");
    Option subOption2(2, "3.Napoj");

    option1.subOption.push_back(subOption0);
    option1.subOption.push_back(subOption1);
    option1.subOption.push_back(subOption2);

    options = {option0, option1, option2, option3, option4, option5};
}

void OperatingPanel::init()
{
    screen.init();
    keyboard.init();
}

void OperatingPanel::repeat()
{
    keyboard.repeat();
}

int OperatingPanel::updateMenuScreen()
{

    if (keyboard.getKey(key_select) == press_single){menuOption.push_back(cursor); cursor = 0;}
    else if (keyboard.getKey(key_cancel) == press_single){if(menuOption.size() > 0) menuOption.pop_back(); }

    vector<Option> options_temp = options;
    for (size_t i = 0; i < menuOption.size(); i++){
        options_temp = options[menuOption.at(i)].subOption;
    }
 
    if (keyboard.getKey(key_up) == press_single){if(cursor > 0)cursor--;}
    else if (keyboard.getKey(key_down) == press_single){if(cursor < options_temp.size()-1) cursor++;}
    else if (keyboard.getKey(key_up) == press_double){cursor=0;}
    else if (keyboard.getKey(key_down) == press_double){cursor=options_temp.size()-1;}

    vector<String> names;
    for (size_t i = 0; i < options_temp.size(); i++){
        names.push_back(options_temp.at(i).name);
    }
    if(keyboard.isKeysChanged()){
    screen.updateLCD(names, cursor);
    screen.lineSet(0);
    }

    if(menuOption.size()>0){
        if(menuOption.at(0)==3){
            return 3;
        }else if(menuOption.at(0)==0){
            return 1;
        } 
    }

    if (keyboard.isKeysChanged()) keyboard.resetKeys();  

    return 0;
}