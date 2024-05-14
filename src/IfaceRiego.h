#ifndef IFACERIEGO_H
#define IFACERIEGO_H

#include <Arduino.h>
#include "Constants.h"
#include "Relay.h"

struct systemTime{
  unsigned int hour, minute, second, day, month, year;
};

struct programTime{
  unsigned int hour, minute, second;
  //bool monday,thursday,wednesday,tuesday,friday,saturday,sunday;
  bool programDays[7];
  bool programEnabled;
};

class IfaceRiego{
  public:
    IfaceRiego(){}
    virtual void setValve(int valve, bool state);
    virtual void toggleValve(int valve);
    virtual bool getValve(int valve);
    virtual void setProgramEnabled(bool programEnabled);
    virtual void turnOff();
    virtual void runProgram(unsigned long programDelay);
    virtual void check();

    virtual void addProgramTime(unsigned int field);
    virtual void substractProgramTime(unsigned int field);
    virtual programTime getProgramTime();
    virtual unsigned int getProgramTime(unsigned int field);
    virtual systemTime getSystemTime();
    virtual unsigned int getSystemTime(unsigned int field);
};

#endif
