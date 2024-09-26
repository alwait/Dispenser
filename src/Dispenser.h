#ifndef DISPENSER_H
#define DISPENSER_H

#include "OperatingPanel.h"
#include "Pump.h"
#include "Sensor.h"
#include "Motor.h"
#include "ControlLED.h"
#include "OptionManual.h"
#include "OptionAuto.h"
#include "OptionVolume.h"
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
    OptionAuto automatic;
    OptionVolume volume;
    int option;
    int drinkOption;
    int drinkVolume[DRINK_TYPES];
    int drinkMixPercentage;

public:
    Dispenser(LiquidCrystal_I2C*, Servo*, CRGB*, vector<BfButton*>);
    void init();
    void repeat();
    void syncDigitalSensorsLED();
    void syncAnalogSensorsLED();
    int updateMenu();
    int manualMode();
    int volumeMode();
    int autoMode();
    void setOption(int i){option=i;};
    int getOption(){return option;};
    bool isKeyChanged(){return operatingPanel.isKeysChanged();};
    void cancel(){operatingPanel.cancel();};
    bool sensorReady();
    bool pumpsState();
};

#endif // DISPENSER