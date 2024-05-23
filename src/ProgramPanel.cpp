#include "ProgramPanel.h"

ProgramPanel::ProgramPanel(SoftwareSerial* serial):Panel(serial){}
//ProgramPanel::ProgramPanel(char commandID, char panelID, SoftwareSerial* serial):Panel(commandID, panelID, serial){}

void ProgramPanel::body(){
    /* 
    // Leds
    bluetooth->println("add_text_box(1,4,6,C,24,245,240,245,h)");
    bluetooth->println("add_led(8,4,1,L,0,0,0)");
    bluetooth->println("add_led(9,4,1,M,0,0,0)");
    bluetooth->println("add_led(10,4,1,X,0,0,0)");
    bluetooth->println("add_led(11,4,1,J,0,0,0)");
    bluetooth->println("add_led(12,4,1,V,0,0,0)");
    bluetooth->println("add_led(13,4,1,S,0,0,0)");
    bluetooth->println("add_led(14,4,1,D,0,0,0)");
    bluetooth->println("add_led(10,2,2,p,0,0,0)");
    */

    /*
    // Texto días
    bluetooth->println("add_text(8,3,xlarge,C,L,245,240,245,)");
    bluetooth->println("add_text(9,3,xlarge,C,M,245,240,245,)");
    bluetooth->println("add_text(10,3,xlarge,C,X,245,240,245,)");
    bluetooth->println("add_text(11,3,xlarge,C,J,245,240,245,)");
    bluetooth->println("add_text(12,3,xlarge,C,V,245,240,245,)");
    bluetooth->println("add_text(13,3,xlarge,C,S,245,240,245,)");
    bluetooth->println("add_text(14,3,xlarge,C,D,245,240,245,)");
    */
   /*
    bluetooth->println("add_text_box(2,6,4,C,6,245,240,245,h)");
    bluetooth->println("add_switch(14,4,3,L,l,0,0)");
    bluetooth->println("add_switch(13,4,3,M,m,0,0)");
    bluetooth->println("add_switch(12,4,3,X,x,0,0)");
    bluetooth->println("add_switch(11,4,3,J,j,0,0)");
    bluetooth->println("add_switch(10,4,3,V,v,0,0)");
    bluetooth->println("add_switch(9,4,3,S,s,0,0)");
    bluetooth->println("add_switch(8,4,3,D,d,0,0)");
    bluetooth->println("add_switch(3,3,4,A,a,0,0)");
    */
    
    //this->setup();
    bluetooth->println("add_text_box(2,6,4,C,6,245,240,245,h)");
    /*
    for(uint8_t i=0; i<8; i++){
        if(i!=7) bluetooth->print((int)_nextProgramTime.programDays[i]);
        else bluetooth->print((int)_nextProgramTime.programEnabled);
        bluetooth->println(")");
    }
    */
    for(uint8_t i=0; i<8; i++){
        int maxIndex = 23;
        if(i>1 && i!=7) maxIndex = 24;
        for(int j=0; j<maxIndex; j++){
            //SERIAL_PRINT(switches[i][j]);
            //if(switches[i][j]=='\0') break;
            bluetooth->print(switches[i][j]);
        }
        if(i!=7) {
            bluetooth->print((int)_nextProgramTimePtr->programDays[i]);
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

//void ProgramPanel::loop(){};
void ProgramPanel::loop(){
    //SERIAL_PRINTLN("ProgramPanel::loop()");
    //bluetooth->println("ProgramPanel::loop()");
        
    /////////////   Receive and Process Data
    if (bluetooth->available()){
        data_in=bluetooth->read();  //Get next character 
        for(int i=0; i<7; i++){
            SERIAL_PRINT("R");
            SERIAL_PRINT(i);
            SERIAL_PRINT(",V1:");
            SERIAL_PRINT(_nextProgramTimePtr->programDays[i]);
            if(data_in==turnOn[i]){
                _nextProgramTimePtr->programDays[i] = true;
                changingProgramTime = true;
            }
            if(data_in==turnOff[i]){
                _nextProgramTimePtr->programDays[i] = false;

                changingProgramTime = true;
            }
            SERIAL_PRINT(",V2:");
            SERIAL_PRINTLN(_nextProgramTimePtr->programDays[i]);
        }
        if(data_in==turnOn[7]){
            _nextProgramTimePtr->programEnabled = true;
            changingProgramTime = true;
        }
        if(data_in==turnOff[7]){
            _nextProgramTimePtr->programEnabled = false;
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
            bluetooth->print(_nextProgramTimePtr->hour);
            bluetooth->print(":");
            bluetooth->print(_nextProgramTimePtr->minute);
            bluetooth->print(":");
            bluetooth->print(_nextProgramTimePtr->second);
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
        for(int i=0; i<7; i++){
            SERIAL_PRINT("U");
            SERIAL_PRINT(i);
            SERIAL_PRINT(",V1:");
            SERIAL_PRINT(_nextProgramTimePtr->programDays[i]);
            SERIAL_PRINT(",V2:");
            SERIAL_PRINTLN(riego->getProgramTimePtr()->programDays[i]);
            SERIAL_PRINT("X");
            SERIAL_PRINT(i);
            SERIAL_PRINT(",V1:");
            SERIAL_PRINT(riego->getProgramTimePtr()->programDays[i]);
            riego->getProgramTimePtr()->programDays[i] = false;
            SERIAL_PRINT(",V2:");
            SERIAL_PRINTLN(riego->getProgramTimePtr()->programDays[i]);
        }
        changingProgramTime=false;
    }
    _nextProgramTimePtr = riego->getProgramTimePtr();
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