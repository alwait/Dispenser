#ifndef VOLUME_H
#define VOLUME_H

#include "GPIO.h"

using namespace std;

class OptionVolume{
private:
    bool lock;
    int changeOption;
public:
    OptionVolume();
    void lockCursor(){lock=true;};
    void unlockCursor(){lock=false;};
    int isLock(){return lock;};
    void setChangeOption(int i){changeOption=i;};
    int getChangeOption(){return changeOption;};
    bool isChanging(){if(changeOption!=DRINK_TYPES) return true; else return false;};
};

#endif // VOLUME