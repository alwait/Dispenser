#include "OptionAuto.h"

OptionAuto::OptionAuto(){
    for (size_t i = 0; i < NUM_LEDS; i++)
    {
        checkedSensors[i]=false;
        servicedSensors[i]=false;
    }
    
    lock=false;
    processing=false;
    estop=false;
    changeStage=false;
    endAuto=false;
    for (size_t i = 0; i < 2; i++)
    {
        
        times[i]=1500;
    }
}
void OptionAuto::resetAuto(){
     for (size_t i = 0; i < NUM_LEDS; i++)
    {
        checkedSensors[i]=false;
        servicedSensors[i]=false;
    }
    lock=false;
    processing=false;
    estop=false;
    changeStage=false;
    endAuto=false;
}

void OptionAuto::lockSensors(vector<Sensor> sensors){
    for (size_t i = 0; i < sensors.size(); i++)
    {
        checkedSensors[i]=sensors.at(i).digitalSensorRead();
    }
}

bool OptionAuto::checkSensorsState(vector<Sensor> sensors){
    for (size_t i = 0; i < sensors.size(); i++)
    {
        if(checkedSensors[i]!=sensors.at(i).digitalSensorRead()) return false;
    }
    return true;
}

void OptionAuto::lockService(){
    for (size_t i = 0; i < NUM_LEDS; i++)
    {   
        servicedSensors[i]=!checkedSensors[i];
    }
}

int OptionAuto::serviceNow(){
    for (size_t i = 0; i < NUM_LEDS; i++)
    {   
        if(servicedSensors[i]==false) return i;
    }
    return NUM_LEDS+1;
}

