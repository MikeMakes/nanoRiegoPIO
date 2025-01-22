#ifndef IFACERIEGO_H
#define IFACERIEGO_H

#include <Arduino.h>
#include "Constants.h"
#include "Relay.h"
//#include "IfaceGui.h"

class IfaceGui; //Evitar dependencias cirulares

struct systemTime{
  int hour, minute, second, day, month, year;
};

struct programTime{
  unsigned int hour, minute, second;
  bool programDays[7]; //bool monday,thursday,wednesday,tuesday,friday,saturday,sunday;
  bool programEnabled;
  unsigned int delay;
};

class IfaceRiego{
  public:
    IfaceRiego(){}

    virtual void gui(IfaceGui* const gui){
      _gui=gui;
    }
    
    virtual void setValve(int valve, bool state);
    virtual void toggleValve(int valve);
    virtual bool getValve(int valve);
    virtual void turnOff();
    virtual void runProgram();
    virtual void check();

    virtual void changeProgramTime();
    virtual programTime getProgramTime();
    virtual unsigned int getProgramTime(unsigned int field);
    virtual programTime* const getProgramTimePtr();
    virtual systemTime getSystemTime();
    virtual unsigned int getSystemTime(unsigned int field);
    //void setProgramTime(programTime programTime_);

    virtual void setSystemTime(systemTime time);
    //virtual void setProgramTime(programTime time);

    IfaceGui* _gui;

    bool _running = false;
};

#endif