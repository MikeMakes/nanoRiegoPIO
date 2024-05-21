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
    Riego(Relay* pump, Relay valves[numValves], unsigned long programDelayMinutes = 15):
    _pump(pump)
    {
      _actualTimeMillis = millis();
      _programDelay = programDelayMinutes * 60000; //ms
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
      _programEnabled=programEnabled;
    }
    bool getProgramEnabled(){
      return _programEnabled;
    }
    void toggleProgramEnabled(){
      _programEnabled=!_programEnabled;
    }
    
    void turnOff(){
      for(int i=0; i<numValves; i++){
        setValve(i,false);
      }
    }

    static void runProgramNextState(){}

    void runProgramTimer(time_t programDelay){
      _running = true;
      _lastRunTime = now();
      for(int i=0; i<numValves; i++){
        setValve(i,true);

        Alarm.timerOnce(_programDelay,runProgramNextState);
      }
      turnOff();
      _running = false; //todo
    }
        
    void runProgram(unsigned long programDelay){
      _running = true;
      _lastRunTimeMillis = millis();
      _lastRunTime = now();
      for(int i=0; i<numValves; i++){
        setValve(i,true);

        Alarm.delay(programDelay);
      }
      turnOff();
      _running = false; //todo
    }
    void testValves(){
      runProgram(1500);
    }

    void check(){
      //checkMillis();
      checkAlarm();
    }
    void checkMillis(){
      if(!_programEnabled) return;
      _actualTimeMillis = millis();
      //if( (_actualTime - _lastRunTimeMillis) > oneDay) ){
      if((_actualTimeMillis - _lastRunTimeMillis) > oneMin){
        runProgram(_programDelay);
      }
    }
    void checkAlarm(){    //Alarm.alarmRepeat(dowMonday, 9,15,0, check);
      if(!_programEnabled) return;
      _actualTime = now();
      if(_programedDays[weekday()-1]){
        runProgram(_programDelay);
      }
    }

    void setProgramDays(unsigned int day, bool enabled){
        if(day<0 || day>6) return;
        _programedDays[day] = enabled;
    }
    void toggleProgramDays(unsigned int day){
        if(day<0 || day>6) return;
        _programedDays[day] = !_programedDays[day];
    }
    void setProgramTime(int hour, int min, int sec){
      _programTime[0] = hour;
      _programTime[1] = min;
      _programTime[2] = sec;
    }
    void addProgramTime(unsigned int field){
      if(field<0 || field>2) return;
      if (field != 0) {
        _programTime[field] = _programTime[field] - 10;
        if (_programTime[field] > 59 ) _programTime[field] = 59;
      }
      else {
        _programTime[field]--;
        if (_programTime[field] > 23 ) _programTime[field] = 23;
      }
    }
    void substractProgramTime(unsigned int field){
      if(field<0 || field>2) return;
      if (field != 0) {
        _programTime[field] = _programTime[field] - 10;
        if (_programTime[field] > 59 ) _programTime[field] = 59;
      }
      else {
        _programTime[field]--;
        if (_programTime[field] > 23 ) _programTime[field] = 23;
      }
    }

    programTime getProgramTime(){
      programTime actualTime;
      actualTime.hour=_programTime[0];
      actualTime.minute=_programTime[1];
      actualTime.second=_programTime[2];
      for(int i=0; i<7; i++) actualTime.programDays[i]=_programedDays[i];
      actualTime.programEnabled=_programEnabled;
      return actualTime;
    }
    unsigned int getProgramTime(unsigned int field){
      if(field>2) field = 2;
      return _programTime[field];
    }
    systemTime getSystemTime(){
      systemTime actualTime = {hour(),minute(),second(),day(),month(),year()};
      return actualTime;
    }
    unsigned int getSystemTime(unsigned int field){
      if(field>5) field = 5;
      unsigned int systemTime[6]={hour(),minute(),second(),day(),month(),year()};
      return systemTime[field];
    }
    void setSystemTime(systemTime time){
      //struct systemTime unsigned int hour, minute, second, day, month, year;
      time_t unixTime=0;
      unixTime += time.hour * 3600;
      unixTime += time.minute * 60;
      unixTime += time.second; //* 1;
      unixTime += time.day * 86400;
      unixTime += time.month * 2629743;
      unixTime += (time.year-1970) * 2629743;
      RTC.set(unixTime);
      setTime(unixTime);
    }
    void setProgramTime(programTime time){
      programTime actualProgramTime = getProgramTime();
      if((time.hour != actualProgramTime.hour) || (time.minute != actualProgramTime.minute) || (time.second != actualProgramTime.second)){
        _programTime[0]=time.hour;
        _programTime[1]=time.minute;
        _programTime[2]=time.second;
      }
      //for(int i=0; i<8; i++) _programedDays[i] = time.programDays[i];
      //_programEnabled = time.programEnabled;
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

  private:
    Relay* _pump;
    Relay _valves[numValves];
    int _numValves = numValves;

    bool _running = false;
    bool _programEnabled = true;
    unsigned long _programDelay = 5;
    unsigned long _actualTimeMillis;
    unsigned long _lastRunTimeMillis;
    const static time_t _defaultTime = DEFAULT_TIME;
    time_t _actualTime = DEFAULT_TIME;
    //time_t _programTime = DEFAULT_TIME;
    time_t _lastRunTime;
    bool _programedDays[7] = {true,true,true,true,true,true,true};
    unsigned int _programTime[3]={9,00,00};

  public:
    unsigned int selections[3]={0,0,0};
};
#endif
