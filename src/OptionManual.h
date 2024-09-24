#ifndef MANUAL_H
#define MANUAL_H

using namespace std;

class OptionManual{
private:
    int activeSensor;
    int activePump;
    bool lock;
public:
    OptionManual();
    void setSensor(int i){activeSensor=i;};
    void setPump(int i){activePump=i;};
    int getSensor(){return activeSensor;};
    int getPump(){return activePump;};
    void lockCursor(){lock=true;};
    void unlockCursor(){lock=false;};
    int isLock(){return lock;};
};

#endif // MANUAL