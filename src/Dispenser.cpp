#include "Dispenser.h"

Dispenser::Dispenser(LiquidCrystal_I2C* screen, Servo* servo, CRGB* leds, vector<BfButton*> buttons, unsigned long& timer): operatingPanel(screen, buttons), servo(servo), leds(leds), manual(){
    pumps.push_back(Pump(PWM_PUMP_1, timer));
    pumps.push_back(Pump(PWM_PUMP_2, timer));
    
    sensors.push_back(SENSOR_1);
    sensors.push_back(SENSOR_2);
    sensors.push_back(SENSOR_3);
    sensors.push_back(SENSOR_4);

    option=0;
}

void Dispenser::init(){
    operatingPanel.init();
    servo.init();
    leds.init();
    for (Pump &pump : pumps){
        pump.init();
    }
    
}

void Dispenser::repeat(){
    operatingPanel.repeat();
    for (Pump &pump : pumps){
        pump.repeat();
    }
}

void Dispenser::syncDigitalSensorsLED(){
    for (int i = 0; i < NUM_LEDS; i++) {
        if(sensors[i].digitalSensorRead()){
            leds.setColor(i,CRGB(32,32,32));
        }
        else leds.setColor(i,CRGB(1,1,1));
    }
    leds.update();
}

void Dispenser::syncAnalogSensorsLED(){
    for (int i = 0; i < NUM_LEDS; i++) {
        if(sensors[i].digitalSensorRead()){
            int intensivity=(int)round(map(sensors[i].analogSensorRead(), 0, 100, 0, 40));
            leds.setColor(i,CRGB(intensivity, intensivity, intensivity));
        }
        else leds.setColor(i,CRGB(1,1,1));
    }
    leds.update();
}

int Dispenser::updateMenu(){
    return operatingPanel.updateMenuScreen();
}

int Dispenser::manualMode(){

    if(operatingPanel.getKey(key_cancel)==1 && !manual.isLock()){operatingPanel.setCursorPosition(0); return 0;}
    else if(operatingPanel.getKey(key_down)==1 && operatingPanel.getCursorPosition()<2 && !manual.isLock()) operatingPanel.cursorDown();
    else if(operatingPanel.getKey(key_up)==1 && operatingPanel.getCursorPosition()>0 && !manual.isLock()) operatingPanel.cursorUp();
    else if(operatingPanel.getKey(key_down)==2 && !manual.isLock()) operatingPanel.setCursorPosition(2);
    else if(operatingPanel.getKey(key_up)==2 && !manual.isLock()) operatingPanel.setCursorPosition(0);


    bool refresh=operatingPanel.isKeysChanged();

    switch (operatingPanel.getCursorPosition()){
    case 0:
        if(operatingPanel.getKey(key_select)==3){
            pumps.at(manual.getPump()).turnOn();
            manual.lockCursor();    
        }
        else if(manual.isLock() && digitalRead(SELECT_PIN)==HIGH){
            pumps.at(manual.getPump()).turnOff();
            manual.unlockCursor();
        }
        break;
    case 1:
        if(operatingPanel.getKey(key_select)==3){
            manual.setPump(((manual.getPump()+1)%2));
            manual.lockCursor();    
        }
        else if(manual.isLock() && digitalRead(SELECT_PIN)==HIGH){
            manual.unlockCursor();
        }
        break;
    case 2:
        if(operatingPanel.getKey(key_select)==3){
            manual.setSensor(((manual.getSensor()+1)%4));
            servo.setPosition(manual.getSensor());
            manual.lockCursor();    
        }
        else if(manual.isLock() && digitalRead(SELECT_PIN)==HIGH){
            manual.unlockCursor();
        }
        break;
        break;
    default:
        break;
    }
    if(refresh){
        String onLine="Wlacz lanie";

        String pumpLine="Pompa: ";
        pumpLine += manual.getPump();

        String sensorLine="Sensor: ";
        sensorLine += manual.getSensor();

        operatingPanel.updateScreen({onLine, pumpLine, sensorLine}, operatingPanel.getCursorPosition());
        operatingPanel.lineSet(0);
    }
    if(refresh) operatingPanel.resetKeys();
    return 3;
}

