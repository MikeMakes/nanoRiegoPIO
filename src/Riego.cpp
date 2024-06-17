#include "Riego.h"

Riego::Riego(Relay* pump, Relay valves[numValves], unsigned long programDelaySeconds = PROGRAM_DELAY):
_pump(pump)
{
    _programDelay = programDelaySeconds; //s
    valveRunning = -1;
    for(int i=0; i<numValves; i++){
    _valves[i]=valves[i];
    }
}

void Riego::setValve(int valve, bool state){
    if(valve>numValves || valve<0) return;
    for(int i=0; i<numValves; i++){
    if(i==valve) continue;
    _valves[i].setState(false);
    }
    _pump->setState(state);
    _valves[valve].setState(state);
}
bool Riego::getValve(int valve){
    //SERIAL_PRINTLN("bool Riego::getValve(int valve)");
    return _valves[valve].getState();
}
void Riego::toggleValve(int valve){
    setValve(valve, !_valves[valve].getState());
}

void Riego::setProgramEnabled(bool programEnabled){
    _programTimePtr.programEnabled=programEnabled;
}
bool Riego::getProgramEnabled(){
    return _programTimePtr.programEnabled;
}
void Riego::toggleProgramEnabled(){
    _programTimePtr.programEnabled=!_programTimePtr.programEnabled;
}
void Riego::turnOff(){
    for(int i=0; i<numValves; i++){
    setValve(i,false);
    }
}
void Riego::runProgram(time_t programDelay){
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
void Riego::testValves(){
    runProgram(3);
}

void Riego::check(){
    checkAlarm();
}
void Riego::checkAlarm(){ 
    if(!_programTimePtr.programEnabled) return;
    _actualTime = now();
    if(_programTimePtr.programDays[weekday()-1]){
    runProgram(_programDelay);
    }
}

void Riego::setProgramDays(unsigned int day, bool enabled){
    if(day<0 || day>6) return;
    _programTimePtr.programDays[day] = enabled;
}
void Riego::toggleProgramDays(unsigned int day){
    if(day<0 || day>6) return;
    _programTimePtr.programDays[day] = !_programTimePtr.programDays[day];
}
void Riego::setProgramTime(int hour, int min, int sec){
    _programTimePtr.hour = hour;
    _programTimePtr.minute = min;
    _programTimePtr.second = sec;
}
void Riego::addProgramTime(unsigned int minutes){
    changedProgramTime = true;
    _programTimePtr.minute = _programTimePtr.minute + minutes;
    if(_programTimePtr.minute>59){
    _programTimePtr.minute = _programTimePtr.minute - 59;
    _programTimePtr.hour++;
    }
}
void Riego::substractProgramTime(unsigned int minutes){
    changedProgramTime = true;
    if(_programTimePtr.minute<minutes){
    minutes -= _programTimePtr.minute;
    _programTimePtr.minute = 59 - minutes;
    _programTimePtr.hour--;// = _programTimePtr.hour--;
    } else{
    _programTimePtr.minute = _programTimePtr.minute - minutes;
    }
}

programTime Riego::getProgramTime(){
    return _programTimePtr;
}
unsigned int Riego::getProgramTime(unsigned int field){
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
systemTime Riego::getSystemTime(){
    systemTime actualTime = {hour(),minute(),second(),day(),month(),year()};
    return actualTime;
}
unsigned int Riego::getSystemTime(unsigned int field){
    if(field>5) field = 5;
    int systemTime[6]={hour(),minute(),second(),day(),month(),year()};
    return systemTime[field];
}
void Riego::setSystemTime(systemTime time){
    setTime(time.hour,time.minute,time.second,time.day,time.month,time.year);
    RTC.set(now());
}

void Riego::press(int button){
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

void Riego::longPress(int button){
    //SERIAL_PRINTLN("void longPress(int button)");
    this->toggleValve(button);
}

void Riego::rotation(int button, bool rotationDir){
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

programTime* const Riego::getProgramTimePtr(){
    return &_programTimePtr;
}


void Riego::changeProgramTime(){
    changedProgramTime = true;
}
bool Riego::programTimeChanged(){
    bool cpt = changedProgramTime;
    changedProgramTime=false;
    return cpt;
}