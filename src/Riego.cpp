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
    if(!_programTime.programEnabled) return;
    _actualTime = now();
    int day = weekday(); //this function returns 1 for sunday, 2 monday, ..., 7 saturday
    if(day==1) day = 6; //remapping index to common sense
    else day = day-2;
    if(_programTime.programDays[day]){
        runProgram();
    }
}

const programTime& Riego::getProgramTime(){
    return _programTime;
}
void Riego::setProgramTime(const programTime& newProgramTime) {
    if(_programTime!=newProgramTime){
        _programTime = newProgramTime;
        _changedProgramTime = true; // Indicate the object has changed
    }
}
bool Riego::changedProgramTime(){
    return _changedProgramTime;
}

systemTime Riego::getSystemTime(){
    systemTime actualTime = {hour(),minute(),second(),day(),month(),year()};
    return actualTime;
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