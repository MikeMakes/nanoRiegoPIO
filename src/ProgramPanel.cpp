#include "ProgramPanel.h"

ProgramPanel::ProgramPanel(SoftwareSerial* serial):Panel(serial){}
//ProgramPanel::ProgramPanel(char commandID, char panelID, SoftwareSerial* serial):Panel(commandID, panelID, serial){}

void ProgramPanel::printProgramTimeSwitches(){
    for(uint8_t i=0; i<8; i++){
        int maxIndex = 23;
        if(i>1 && i!=7) maxIndex = 24;
        //print "add_switch(whatevah,3,4,6,":
        for(int j=0; j<maxIndex; j++){
            //SERIAL_PRINT(switches[i][j]);
            //if(switches[i][j]=='\0') break;
            bluetooth->print(switches[i][j]); 
        }
        //print state:
        if(i!=7) { 
            if(!_nextProgramTimePtr->programEnabled) bluetooth->print((int)false);
            else bluetooth->print((int)_nextProgramTimePtr->programDays[i]);
            //SERIAL_PRINT((int)_nextProgramTime.programDays[i]);
        }
        else {
            bluetooth->print((int)_nextProgramTimePtr->programEnabled);
            //SERIAL_PRINT((int)_nextProgramTime.programEnabled);
        }
        bluetooth->println(")");
        //SERIAL_PRINTLN(")");
    }
}

void ProgramPanel::reloadDynamicBody(){
    //setup();
    bluetooth->println(F("*.kwl"));
    bluetooth->println(F("clear_area(7,3,8,4)"));
    printProgramTimeSwitches();
}

void ProgramPanel::body(){
    bluetooth->println("add_button(5,5,5,n,)");
    bluetooth->println("add_button(2,5,4,p,)");
    bluetooth->print("add_text_box(3,5,2,C,");
    printProgramTimeBox();
    bluetooth->println(",245,240,245,h)"); //Este podriamos ahorranoslo repetir
    printProgramTimeSwitches();
}

void ProgramPanel::printProgramTimeBox(){
    bluetooth->print(_nextProgramTimePtr->hour);
    bluetooth->print(":");
    bluetooth->print(_nextProgramTimePtr->minute);
    //bluetooth->print(":");
    //bluetooth->print(_nextProgramTimePtr->second);
}

//void ProgramPanel::loop(){};
void ProgramPanel::loop(){
    //SERIAL_PRINTLN("ProgramPanel::loop()");
    //bluetooth->println("ProgramPanel::loop()");
        
    /////////////   Receive and Process Data
    if (bluetooth->available()){
        data_in=bluetooth->read();  //Get next character 
        for(int i=0; i<7; i++){
            if(data_in==turnOn[i]){
                _nextProgramTimePtr->programDays[i] = true;
                changingProgramTime = true;
            }
            if(data_in==turnOff[i]){
                _nextProgramTimePtr->programDays[i] = false;

                changingProgramTime = true;
            }
        }
        if(data_in==turnOn[7]){
            _nextProgramTimePtr->programEnabled = true;
            this->reloadDynamicBody();
            changingProgramTime = true;
        }
        if(data_in==turnOff[7]){
            _nextProgramTimePtr->programEnabled = false;
            this->reloadDynamicBody();
            changingProgramTime = true;
        }
        if(data_in==add15min){
            _nextProgramTimePtr->minute = _nextProgramTimePtr->minute + 5;
            if(_nextProgramTimePtr->minute>59){
                _nextProgramTimePtr->minute = 0;//_nextProgramTimePtr->minute - 59;
                _nextProgramTimePtr->hour++;
            }
            changingProgramTime = true;
        }
        if(data_in==substract15min){
            unsigned int minutes = 5;
            if(_nextProgramTimePtr->minute<minutes){
                minutes -= _nextProgramTimePtr->minute;
                _nextProgramTimePtr->minute = 60-minutes;//59 - minutes;
                _nextProgramTimePtr->hour--;
            } else{
                _nextProgramTimePtr->minute = _nextProgramTimePtr->minute - minutes;
            }
            changingProgramTime = true;
        }
    }

    /////////////  Send Data to Android device
    unsigned long t=millis();
    if((t-last_time)>update_interval){
        last_time=t;

        //programTime nextProgramTime = riego->getProgramTime();
        if(_nextProgramTimePtr->programEnabled){
            bluetooth->print("*h");
            printProgramTimeBox();
            bluetooth->print("*");
        }else{
            //bluetooth->print(F("*hPrograma desactivado*"));
        }
    }
}

void ProgramPanel::update(IfaceRiego* const riego, IfaceGui* const gui){
    //if(selectedDay==8) riego->toggleProgramEnabled();
    //else riego->toggleProgramDays(selectedDay);
    if(changingProgramTime){
        riego->changeProgramTime();
        changingProgramTime=false;
    }
    _nextProgramTimePtr = riego->getProgramTimePtr();
    //changingProgramTime=false;
}

void ProgramPanel::selectDay(unsigned int day){
    if(selectedDay>7) return;
    selectedDay = day;
}

void ProgramPanel::previousDay(){
    if(selectedDay==0) selectedDay=7;
    selectedDay--;
}

void ProgramPanel::nextDay(){
    selectedDay++;
    if(selectedDay>8) selectedDay=0;
}

void ProgramPanel::shiftDay(bool next){
    if(next) nextDay();
    else previousDay();
}

void ProgramPanel::shiftField(bool next){
    shiftDay(next);
}

unsigned int ProgramPanel::getField(){
    return selectedDay;
}

void ProgramPanel::shiftHour(bool add1){
    if(add1) _nextProgramTime.hour++;
    else _nextProgramTime.hour--;
}