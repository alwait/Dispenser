#include "Dispenser.h"

Dispenser::Dispenser(LiquidCrystal_I2C* screen, Servo* servo, CRGB* leds, vector<BfButton*> buttons): 
    operatingPanel(screen, buttons), servo(servo), leds(leds), manual(), automatic(), volume(){

    pumps.push_back(Pump(PWM_PUMP_1));
    pumps.push_back(Pump(PWM_PUMP_2));
    
    sensors.push_back(SENSOR_1);
    sensors.push_back(SENSOR_2);
    sensors.push_back(SENSOR_3);
    sensors.push_back(SENSOR_4);

    option=0;
    drinkOption=0;
    drinkVolume[0]=20;
    drinkVolume[1]=200;
    drinkVolume[2]=120;
    drinkMixPercentage=20;
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
    return operatingPanel.updateMenuScreen(drinkOption);
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
        servo.setPosition(manual.getSensor());
        
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
            for (Pump &pump : pumps){
                pump.setTimer(millis());
            }
        }
        else if(automatic.isLock() && digitalRead(SELECT_PIN)==HIGH){
        }
        break;
    default:
        break;
    }
    if(automatic.isProcessing() && !automatic.isStop()){
        if(automatic.getChangeStage() && automatic.serviceNow()!=NUM_LEDS+1){
            unsigned long pumpTimer[2];
            pumpTimer[0]=pumps.at(0).getTimer();
            pumpTimer[1]=pumps.at(1).getTimer();
            if(millis()-pumpTimer[0]>automatic.getTime(0)/3 && millis()-pumpTimer[1]>automatic.getTime(1)/3){
            servo.setPosition(automatic.serviceNow());
            }
            if(millis()-pumpTimer[0]>automatic.getTime(0)/1.5 && (drinkOption==0 || drinkOption==1)){
            pumps.at(0).turnOn();
            automatic.setChangeStage(false);
            }
            if(millis()-pumpTimer[1]>automatic.getTime(1)/1.5 && (drinkOption==1 || drinkOption==2)){
            pumps.at(1).turnOn();
            automatic.setChangeStage(false);
            }
        }
        else if(automatic.getChangeStage()){
            if(millis()-pumps.at(0).getTimer()>automatic.getTime(0)/3 && (drinkOption==0 || drinkOption==1)){
                pumps.at(0).turnOff();
            }
            if(millis()-pumps.at(1).getTimer()>automatic.getTime(1)/1.5 && (drinkOption==1 || drinkOption==2)){
                pumps.at(1).turnOff();
            }
            if((millis()-pumps.at(0).getTimer()>automatic.getTime(0)/3 || drinkOption==2)
             && (millis()-pumps.at(1).getTimer()>automatic.getTime(1)/1.5 || drinkOption==0)){
                automatic.setProcessing(false);
                automatic.unlockCursor();
                automatic.setChangeStage(false);
                automatic.resetAuto();
                cancel();
                return 0;
            }
        }
        if(!automatic.checkSensorsState(sensors) && automatic.serviceNow()!=NUM_LEDS+1) automatic.emergencyStop();

        
        if(((millis()-pumps.at(0).getTimer()>automatic.getTime(0)/2 || (drinkOption==2))
        && (millis()-pumps.at(1).getTimer()>automatic.getTime(1) || (drinkOption==0)))
        && !automatic.getChangeStage()){
            for (Pump &pump : pumps){
                pump.turnOff();
                pump.setTimer(millis());
            }
            automatic.serviced(automatic.serviceNow());
            automatic.setChangeStage(true);
            automatic.setEnd(true);
        }
        else if(((millis()-pumps.at(0).getTimer()>automatic.getTime(0)/3 || (drinkOption==2)))
        && !automatic.getChangeStage()){
            pumps.at(0).turnOff();
        }
        else if(((millis()-pumps.at(1).getTimer()>automatic.getTime(1) || (drinkOption==0)))
        && !automatic.getChangeStage()){
            pumps.at(1).turnOff();
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
        switch (drinkOption)
        {
        case 0:
            drinkLine += "Wodka";
            drinkLine += " ";
            break;
        case 1:
            drinkLine += "Drink";
            drinkLine += " ";
            break;
        case 2:
            drinkLine += "Napoj";
            drinkLine += " ";
            break;
        default:
            break;
        }
        
        drinkLine += drinkVolume[drinkOption];
        drinkLine += "ml ";
        if(drinkOption==1){drinkLine += drinkMixPercentage; drinkLine += "%";}

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

int Dispenser::volumeMode(){

    if(operatingPanel.getKey(key_cancel)==1 && !volume.isLock() && !volume.isChanging()){operatingPanel.setCursorPosition(0); volume.setChangeOption(DRINK_TYPES); return 0;}
    else if(operatingPanel.getKey(key_cancel)==1 && !volume.isLock() && volume.isChanging()){operatingPanel.setCursorPosition(0); volume.setChangeOption(DRINK_TYPES);}
    else if(operatingPanel.getKey(key_down)==1 && operatingPanel.getCursorPosition()<2 && !volume.isLock() && !volume.isChanging()) operatingPanel.cursorDown();
    else if(operatingPanel.getKey(key_up)==1 && operatingPanel.getCursorPosition()>0 && !volume.isLock() && !volume.isChanging()) operatingPanel.cursorUp();
    else if(operatingPanel.getKey(key_down)==2 && !volume.isLock()&& !volume.isChanging()) operatingPanel.setCursorPosition(2);
    else if(operatingPanel.getKey(key_up)==2 && !volume.isLock()&& !volume.isChanging()) operatingPanel.setCursorPosition(0);

    else if(operatingPanel.getKey(key_down)==1 && !volume.isLock() && volume.isChanging()){
        if(drinkVolume[operatingPanel.getCursorPosition()]>5);
            drinkVolume[operatingPanel.getCursorPosition()]-=5;
    }
    else if(operatingPanel.getKey(key_up)==1 && !volume.isLock() && volume.isChanging()){
        int max;
        switch (operatingPanel.getCursorPosition()){
        case 0: max=50; break;
        case 1: max=250; break;
        case 2: max=250; break;
        default: break;
        }
        if(drinkVolume[operatingPanel.getCursorPosition()]<=max);  
            drinkVolume[operatingPanel.getCursorPosition()]+=5;
    }
    else if(operatingPanel.getKey(key_down)==2 && !volume.isLock() && volume.isChanging() && operatingPanel.getCursorPosition()==1){
        if(drinkMixPercentage>2)
            drinkMixPercentage-=2;
    }
    else if(operatingPanel.getKey(key_up)==2 && !volume.isLock() && volume.isChanging() && operatingPanel.getCursorPosition()==1){
        if(drinkMixPercentage<=60)
            drinkMixPercentage+=2;
    }

    if(operatingPanel.getKey(key_select)== press_long && !volume.isLock() && !volume.isChanging()){
        volume.setChangeOption(operatingPanel.getCursorPosition());
        volume.lockCursor(); 
    }
    else if(volume.isLock() && digitalRead(SELECT_PIN)==HIGH){
        volume.unlockCursor();
    }

    bool refresh=operatingPanel.isKeysChanged();

    if(refresh){
        if(!volume.isChanging()){
            String aLine="Wodka ";
            aLine += drinkVolume[0];
            aLine += "ml";

            String bLine="Drink ";
            bLine += drinkVolume[1];
            bLine += "ml ";
            bLine += drinkMixPercentage;
            bLine += "%";

            String cLine="Napoj ";
            cLine += drinkVolume[2];
            cLine += "ml";

            operatingPanel.updateScreen({aLine, bLine, cLine}, operatingPanel.getCursorPosition());
            operatingPanel.lineSet(0);
        }
        else{
            String changeLine="-";
            switch (volume.getChangeOption()){
            case 0:
                changeLine+=drinkVolume[0];
                changeLine+="ml+";
                operatingPanel.updateScreen({"     " + changeLine, "     Wodka"}, 0);
                break;
            case 1:
                changeLine+=drinkVolume[1];
                changeLine+="ml+  -";
                changeLine+=drinkMixPercentage;
                changeLine+="%+";
                operatingPanel.updateScreen({" " + changeLine, "     Drink"}, 0);
                break;
            case 2:
                changeLine+=drinkVolume[2];
                changeLine+="ml+";
                operatingPanel.updateScreen({"     " + changeLine, "     Napoj"}, 0);
                break;
            default:
                break;
            }
        }
    }
    if(refresh) operatingPanel.resetKeys();
    return 2;
}

