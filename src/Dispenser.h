#ifndef DISPENSER_H
#define DISPENSER_H

#include "OperatingPanel.h"
#include "Pump.h"
#include "Sensor.h"
#include "Motor.h"
#include "ControlLED.h"
#include "OptionManual.h"
#include <vector>

using namespace std;

class Dispenser{
private:
public:
    OperatingPanel operatingPanel;
    Motor servo;
    ControlLED leds;
    vector<Pump> pumps;
    vector<Sensor> sensors;
    OptionManual manual;
    int option;

public:
    Dispenser(LiquidCrystal_I2C*, Servo*, CRGB*, vector<BfButton*>, unsigned long&);
    void init();
    void repeat();
    void syncDigitalSensorsLED();
    void syncAnalogSensorsLED();
    int updateMenu();
    int manualMode();
    void setOption(int i){option=i;};
    int getOption(){return option;};
    bool isKeyChanged(){return operatingPanel.isKeysChanged();};
    void cancel(){operatingPanel.cancel();};
};

#endif // DISPENSER