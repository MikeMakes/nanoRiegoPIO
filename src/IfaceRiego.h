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

  // Define equality operator
  bool operator==(const programTime& other) const {
    if (hour != other.hour || minute != other.minute || second != other.second || programEnabled != other.programEnabled || delay != other.delay) {
      return false;
    }
    for (int i = 0; i < 7; ++i) { // Compare each element in the programDays array
      if (programDays[i] != other.programDays[i]) {
          return false;
      }
    }
    return true;
  }

  // Define inequality operator
  bool operator!=(const programTime& other) const {
      return !(*this == other); // Leverage the == operator
  }
};

class IfaceRiego{
  public:
    IfaceRiego(){}

    virtual void gui(IfaceGui* const gui){
      _gui=gui;
    }
    
    virtual void setValve(int valve, bool state);
    virtual bool getValve(int valve);
    virtual void toggleValve(int valve);
    virtual void turnOff();

    virtual void runProgram();
    virtual void check();

    virtual programTime& getProgramTime();

    virtual void changeProgramTime();
    //virtual programTime getProgramTime();
    //virtual unsigned int getProgramTime(unsigned int field);
    virtual programTime* const getProgramTimePtr();
    virtual systemTime getSystemTime();
    //void setProgramTime(programTime programTime_);

    virtual void setSystemTime(systemTime time);
    //virtual void setProgramTime(programTime time);

    IfaceGui* _gui;

    bool _running = false;
};

#endif