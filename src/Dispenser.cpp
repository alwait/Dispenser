#include "Dispenser.h"

Dispenser::Dispenser(LiquidCrystal_I2C* screen, Servo* servo, CRGB* leds, vector<BfButton*> buttons): operatingPanel(screen, buttons), servo(servo), leds(leds), manual(), automatic(){
    pumps.push_back(Pump(PWM_PUMP_1));
    pumps.push_back(Pump(PWM_PUMP_2));
    
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
    if(sensorReady()){
        operatingPanel.print(15,1,'X');
    }
    else operatingPanel.print(15,1,' ');
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

bool Dispenser::sensorReady(){
    for (size_t i = 0; i < sensors.size(); i++)
    {   
        if(sensors.at(i).digitalSensorRead()) return true;
    }
    return false;
}

bool Dispenser::pumpsState(){
    for (size_t i = 0; i < pumps.size(); i++)
    {   
        if(pumps.at(i).getSignal()) return true;
    }
    return false;
}

int Dispenser::manualMode(){

    if(operatingPanel.getKey(key_cancel)==1 && !manual.isLock()){operatingPanel.setCursorPosition(0); return 0;}
    else if(operatingPanel.getKey(key_down)==1 && operatingPanel.getCursorPosition()<3 && !manual.isLock()) operatingPanel.cursorDown();
    else if(operatingPanel.getKey(key_up)==1 && operatingPanel.getCursorPosition()>0 && !manual.isLock()) operatingPanel.cursorUp();
    else if(operatingPanel.getKey(key_down)==2 && !manual.isLock()) operatingPanel.setCursorPosition(3);
    else if(operatingPanel.getKey(key_up)==2 && !manual.isLock()) operatingPanel.setCursorPosition(0);

    if(manual.getSensorCheck() && !sensors.at(manual.getSensor()).digitalSensorRead()) pumps.at(manual.getPump()).turnOff();


    bool refresh=operatingPanel.isKeysChanged();

    switch (operatingPanel.getCursorPosition()){
    case 0:
        if(operatingPanel.getKey(key_select)==3){
            if(manual.getSensorCheck()){
                if(sensors.at(manual.getSensor()).digitalSensorRead()) pumps.at(manual.getPump()).turnOn();
            }
            else pumps.at(manual.getPump()).turnOn();
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
    case 3:
        if(operatingPanel.getKey(key_select)==3){
            manual.setSensorCheck(!manual.getSensorCheck());
            manual.lockCursor();    
        }
        else if(manual.isLock() && digitalRead(SELECT_PIN)==HIGH){
            manual.unlockCursor();
        }
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

        String checkSensorLine="Wykrywanie ";
        if(manual.getSensorCheck()){
        checkSensorLine += "[X]";}
        else checkSensorLine += "[ ]";

        operatingPanel.updateScreen({onLine, pumpLine, sensorLine, checkSensorLine}, operatingPanel.getCursorPosition());
        operatingPanel.lineSet(0);
    }
    if(refresh) operatingPanel.resetKeys();
    return 3;
}

int Dispenser::autoMode(){
    
    if(operatingPanel.getKey(key_cancel)==1 && !automatic.isLock() && !automatic.isProcessing()){operatingPanel.setCursorPosition(0); return 0;}
    else if(operatingPanel.getKey(key_cancel)==1 && automatic.isLock() && automatic.isProcessing()){automatic.emergencyStop(); operatingPanel.resetKeys();}
    else if(operatingPanel.getKey(key_down)==1 && operatingPanel.getCursorPosition()<1 && !automatic.isLock()) operatingPanel.cursorDown();
    else if(operatingPanel.getKey(key_up)==1 && operatingPanel.getCursorPosition()>0 && !automatic.isLock()) operatingPanel.cursorUp();
    else if(operatingPanel.getKey(key_down)==2 && !automatic.isLock()) operatingPanel.setCursorPosition(1);
    else if(operatingPanel.getKey(key_up)==2 && !automatic.isLock()) operatingPanel.setCursorPosition(0);

    bool refresh=operatingPanel.isKeysChanged();

    switch (operatingPanel.getCursorPosition()){
    case 0:
        if(operatingPanel.getKey(key_select)==3){
            automatic.lockSensors(sensors);
            automatic.lockService();
            automatic.setProcessing(true);
            automatic.lockCursor(); 
            automatic.setChangeStage(true);   
            pumps.at(0).setTimer(millis());
        }
        else if(automatic.isLock() && digitalRead(SELECT_PIN)==HIGH){
        }
        break;
    default:
        break;
    }
    if(automatic.isProcessing() && !automatic.isStop()){
        if(automatic.getChangeStage() && automatic.serviceNow()!=NUM_LEDS+1){
            if(millis()-pumps.at(0).getTimer()>automatic.getTime(0)/3.){
            servo.setPosition(automatic.serviceNow());
            }
            if(millis()-pumps.at(0).getTimer()>automatic.getTime(0)/1.5){
            pumps.at(0).turnOn();
            automatic.setChangeStage(false);
            }
        }
        else if(automatic.getChangeStage()){
            if(millis()-pumps.at(0).getTimer()>automatic.getTime(0)/1.5){
                automatic.setProcessing(false);
                automatic.unlockCursor();
                automatic.setChangeStage(false);
                automatic.resetAuto();
                pumps.at(0).turnOff();
                cancel();
                return 0;
            }
        }
        if(!automatic.checkSensorsState(sensors) && automatic.serviceNow()!=NUM_LEDS+1) automatic.emergencyStop();
        if(millis()-pumps.at(0).getTimer()>automatic.getTime(0) && !automatic.getChangeStage()){
            pumps.at(0).turnOff();
            pumps.at(0).setTimer(millis());
            automatic.serviced(automatic.serviceNow());
            automatic.setChangeStage(true);
            automatic.setEnd(true);
        }
    }
    else if(automatic.isProcessing() && automatic.isStop()){
        for (size_t i = 0; i < pumps.size(); i++){
            pumps.at(i).turnOff();
        }
        operatingPanel.updateScreen({"Przerwano lanie",""}, 0);
        automatic.emergencyStopReset();
        automatic.setProcessing(false);
        automatic.unlockCursor();
    }
    if(refresh || (automatic.getEnd() && automatic.serviceNow()==NUM_LEDS+1)){
        String onLine="Wlacz rozlewanie";

        String drinkLine="";
        drinkLine += "Wodka";
        drinkLine += "    ";
        drinkLine += "40ml";

        if(!automatic.isProcessing()){
        operatingPanel.updateScreen({onLine, drinkLine}, operatingPanel.getCursorPosition());
        operatingPanel.lineSet(0);
        }
        else if(automatic.getEnd() && automatic.serviceNow()==NUM_LEDS+1){
            operatingPanel.updateScreen({"   Na zdrowie",""}, 0);
            automatic.setEnd(false);
        }
        else operatingPanel.updateScreen({"   Nalewanie",""}, 0);
    }
    
    if(refresh) operatingPanel.resetKeys();
    return 1;
}

