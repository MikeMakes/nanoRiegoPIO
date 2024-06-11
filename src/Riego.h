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
    Riego(Relay* pump, Relay valves[numValves], unsigned long programDelaySeconds = PROGRAM_DELAY):
    _pump(pump)
    {
      _programDelay = programDelaySeconds; //s
      valveRunning = -1;
      for(int i=0; i<numValves; i++){
        _valves[i]=valves[i];
      }
    }

    void setValve(int valve, bool state){
      if(valve>numValves || valve<0) return;
      for(int i=0; i<numValves; i++){
        if(i==valve) continue;
        _valves[i].setState(false);
      }
      _pump->setState(state);
      _valves[valve].setState(state);
    }
    bool getValve(int valve){
      //SERIAL_PRINTLN("bool Riego::getValve(int valve)");
      return _valves[valve].getState();
    }
    void toggleValve(int valve){
      setValve(valve, !_valves[valve].getState());
    }

    void setProgramEnabled(bool programEnabled){
      _programTimePtr.programEnabled=programEnabled;
    }
    bool getProgramEnabled(){
      return _programTimePtr.programEnabled;
    }
    void toggleProgramEnabled(){
      _programTimePtr.programEnabled=!_programTimePtr.programEnabled;
    }
    void turnOff(){
      for(int i=0; i<numValves; i++){
        setValve(i,false);
      }
    }
    void runProgram(time_t programDelay=0){
      if(!_running && programDelay==0) programDelay=_programDelay;

      _running = true;
      _lastRunTime = now();
      valveRunning++;
      if(!(valveRunning<numValves)){
        turnOff();
        valveRunning=-1;
        _running=false;
        return;
      }
      setValve(valveRunning,true);
    }
    void testValves(){
      runProgram(3);
    }

    void check(){
      checkAlarm();
    }
    void checkAlarm(){ 
      if(!_programTimePtr.programEnabled) return;
      _actualTime = now();
      if(_programTimePtr.programDays[weekday()-1]){
        runProgram(_programDelay);
      }
    }

    void setProgramDays(unsigned int day, bool enabled){
        if(day<0 || day>6) return;
        _programTimePtr.programDays[day] = enabled;
    }
    void toggleProgramDays(unsigned int day){
        if(day<0 || day>6) return;
        _programTimePtr.programDays[day] = !_programTimePtr.programDays[day];
    }
    void setProgramTime(int hour, int min, int sec){
      _programTimePtr.hour = hour;
      _programTimePtr.minute = min;
      _programTimePtr.second = sec;
    }
    void addProgramTime(unsigned int minutes){
      changedProgramTime = true;
      _programTimePtr.minute = _programTimePtr.minute + minutes;
      if(_programTimePtr.minute>59){
        _programTimePtr.minute = _programTimePtr.minute - 59;
        _programTimePtr.hour++;
      }
    }
    void substractProgramTime(unsigned int minutes){
      changedProgramTime = true;
      if(_programTimePtr.minute<minutes){
        minutes -= _programTimePtr.minute;
        _programTimePtr.minute = 59 - minutes;
        _programTimePtr.hour--;// = _programTimePtr.hour--;
      } else{
        _programTimePtr.minute = _programTimePtr.minute - minutes;
      }
    }

    programTime getProgramTime(){
      return _programTimePtr;
    }
    unsigned int getProgramTime(unsigned int field){
      //if(field>2) field = 2;
      //return _programTime[field];
      switch(field){
        case 0:
          return _programTimePtr.hour;
          break;
        case 1:
          return _programTimePtr.minute;
         break;
        default:
          return _programTimePtr.second;
          break;
      }
    }
    systemTime getSystemTime(){
      systemTime actualTime = {hour(),minute(),second(),day(),month(),year()};
      return actualTime;
    }
    unsigned int getSystemTime(unsigned int field){
      if(field>5) field = 5;
      int systemTime[6]={hour(),minute(),second(),day(),month(),year()};
      return systemTime[field];
    }
    void setSystemTime(systemTime time){
      setTime(time.hour,time.minute,time.second,time.day,time.month,time.year);
      RTC.set(now());
    }

    void press(int button){
      switch(button){
        case 0:
          break;

        case 1:
          //if(selections[1]==7) this->toggleProgramEnabled();
          //else this->toggleProgramDays(selections[1]);
          break;

        case 2:
          break;

        default:
          //SERIAL_PRINTLN("****BUTTON_OUT_OF_INDEX****");
          break;
      }
    }

    void longPress(int button){
      //SERIAL_PRINTLN("void longPress(int button)");
      this->toggleValve(button);
    }

    void rotation(int button, bool rotationDir){
      switch(button){
        case 0:
          break;

        case 1:
          //gui->shiftField(rotationDir);
          break;

        case 2:
          //shiftProgramHour
          break;

        default:
          //SERIAL_PRINTLN("****BUTTON_OUT_OF_INDEX****");
          break;
      }
    }

    programTime _programTimePtr{ .hour = ALARM_HOUR, .minute = ALARM_MINUTE, .second = ALARM_SECOND, .programDays = {true,false,true,false,true,false,true}, .programEnabled = true };
    programTime* const getProgramTimePtr(){
      return &_programTimePtr;
    }


    void changeProgramTime(){
      changedProgramTime = true;
    }
    bool programTimeChanged(){
      bool cpt = changedProgramTime;
      changedProgramTime=false;
      return cpt;
    }

    unsigned long _programDelay = 15 * 60;
    bool _running = false;

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
