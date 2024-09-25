#ifndef AUTO_H
#define AUTO_H

using namespace std;

#include <vector>
#include "Sensor.h"
#include "GPIO.h"
#include "Arduino.h"

class OptionAuto{
private:
    bool checkedSensors[NUM_LEDS];
    bool servicedSensors[NUM_LEDS];
    bool lock;
    bool processing;
    bool estop;
    unsigned long times[2];
    bool changeStage;
    bool endAuto;
public:
    OptionAuto();
    void lockSensors(vector<Sensor>);
    void lockCursor(){lock=true;};
    void unlockCursor(){lock=false;};
    int isLock(){return lock;};
    bool checkSensorsState(vector<Sensor>);
    void emergencyStop(){estop=true;};
    void emergencyStopReset(){estop=false;};
    bool isStop(){return estop;};
    void setProcessing(bool t){processing=t;};
    bool isProcessing(){return processing;};
    unsigned long getTime(int i){return times[i];};
    int serviceNow();
    void lockService();
    bool getChangeStage(){return changeStage;};
    void setChangeStage(int t){changeStage=t;};
    void serviced(int i){servicedSensors[i]=true;}
    void resetAuto();
    bool getEnd(){return endAuto;};
    void setEnd(bool t){endAuto=t;};
};

#endif // AUTO