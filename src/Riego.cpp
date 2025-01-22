#include "Riego.h"

Riego::Riego(Relay* pump, Relay valves[numValves], unsigned long programDelaySeconds = PROGRAM_DELAY):
_pump(pump)
{
    //_programDelay = programDelaySeconds; //s
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
    //SERIAL_PRINTLN(valve);
    setValve(valve, !_valves[valve].getState());
}

void Riego::turnOff(){
    for(int i=0; i<numValves; i++){
    setValve(i,false);
    }
}
void Riego::runProgram(){
    //if(!_running && programDelay==0) programDelay=_programTimePtr.delay;

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
    runProgram();
}

void Riego::check(){
    checkAlarm();
}
void Riego::checkAlarm(){ 
    if(!_programTimePtr.programEnabled) return;
    _actualTime = now();
    int day = weekday(); //this function returns 1 for sunday, 2 monday, ..., 7 saturday
    if(day==1) day = 6; //remapping index to common sense
    else day = day-2;
    if(_programTimePtr.programDays[day]){
        runProgram();
    }
}

void Riego::setProgramTime(int hour, int min, int sec){
    _programTimePtr.hour = hour;
    _programTimePtr.minute = min;
    _programTimePtr.second = sec;
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
    _changedProgramTime = true;
}
bool Riego::programTimeChanged(){
    bool cpt = _changedProgramTime;
    _changedProgramTime=false;
    return cpt;
}