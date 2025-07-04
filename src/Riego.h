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
    int valveRunning = -1;
    Riego(Relay* pump, Relay valves[numValves], unsigned long programDelaySeconds);

    void setValve(int valve, bool state);
    bool getValve(int valve);
    void toggleValve(int valve);
    void turnOff();

    void runProgram();
    void check();

    void checkAlarm();

    void testValves();

    systemTime getSystemTime();
    void setSystemTime(systemTime time);

    const programTime& getProgramTime(); //const (non modifiable) ref (make impossible to change without passing by serProgramTime)
    void setProgramTime(const programTime& newProgramTime);
    bool changedProgramTime();

  private:
    Relay* _pump;
    Relay _valves[numValves];
    int _numValves = numValves;

    //unsigned long _actualTimeMillis;
    unsigned long _lastRunTimeMillis;
    const static time_t _defaultTime = DEFAULT_TIME;
    time_t _actualTime = DEFAULT_TIME;
    time_t _lastRunTime;

    programTime _programTime{ .hour = ALARM_HOUR, .minute = ALARM_MINUTE, .second = ALARM_SECOND, .programDays = {true,false,true,false,true,false,true}, .programEnabled = true, .delay = PROGRAM_DELAY };
    bool _changedProgramTime = false;
};
#endif
