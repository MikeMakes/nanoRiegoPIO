#ifndef RIEGO_H
#define RIEGO_H

#include <Arduino.h>
#include <Wire.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t

#include "Constants.h"

#include "Relay.h"
#include "IfaceRiego.h"

class Riego:public IfaceRiego{
  public:
    int valveRunning;
    Riego(Relay* pump, Relay valves[numValves], unsigned long programDelaySeconds);

    void setValve(int valve, bool state);
    bool getValve(int valve);
    void toggleValve(int valve);

    void setProgramEnabled(bool programEnabled);
    bool getProgramEnabled();
    void toggleProgramEnabled();
    void turnOff();
    void runProgram(time_t programDelay=0);
    void testValves();

    void check();
    void checkAlarm();

    void setProgramDays(unsigned int day, bool enabled);
    void toggleProgramDays(unsigned int day);
    void setProgramTime(int hour, int min, int sec);
    void addProgramTime(unsigned int minutes);
    void substractProgramTime(unsigned int minutes);

    programTime getProgramTime();
    unsigned int getProgramTime(unsigned int field);
    systemTime getSystemTime();
    unsigned int getSystemTime(unsigned int field);
    void setSystemTime(systemTime time);

    void press(int button);

    void longPress(int button);

    void rotation(int button, bool rotationDir);

    programTime _programTimePtr{ .hour = ALARM_HOUR, .minute = ALARM_MINUTE, .second = ALARM_SECOND, .programDays = {true,false,true,false,true,false,true}, .programEnabled = true, .delay = PROGRAM_DELAY };
    programTime* const getProgramTimePtr();

    void changeProgramTime();
    bool programTimeChanged();

    unsigned long _programDelay = PROGRAM_DELAY;
    //bool _running = false;

  private:
    Relay* _pump;
    Relay _valves[numValves];
    int _numValves = numValves;

    //unsigned long _actualTimeMillis;
    unsigned long _lastRunTimeMillis;
    const static time_t _defaultTime = DEFAULT_TIME;
    time_t _actualTime = DEFAULT_TIME;
    time_t _lastRunTime;

    bool changedProgramTime = false;


  public:
    unsigned int selections[3]={0,0,0};
};
#endif
