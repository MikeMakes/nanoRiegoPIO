#include "ProgramPanel.h"

ProgramPanel::ProgramPanel(SoftwareSerial* serial):Panel(serial){}
//ProgramPanel::ProgramPanel(char commandID, char panelID, SoftwareSerial* serial):Panel(commandID, panelID, serial){}

void ProgramPanel::body(){
    bluetooth->println("add_text_box(1,4,6,C,24,245,240,245,h)");
    bluetooth->println("add_led(8,4,1,L,0,0,0)");
    bluetooth->println("add_led(9,4,1,M,0,0,0)");
    bluetooth->println("add_led(10,4,1,X,0,0,0)");
    bluetooth->println("add_led(11,4,1,J,0,0,0)");
    bluetooth->println("add_led(12,4,1,V,0,0,0)");
    bluetooth->println("add_led(13,4,1,S,0,0,0)");
    bluetooth->println("add_led(14,4,1,D,0,0,0)");
    bluetooth->println("add_led(10,2,2,p,0,0,0)");
    /*
    bluetooth->println("add_text_box(2,4,1,C,24,245,240,245,h)");
    bluetooth->println("add_text(3,4,xlarge,C,:,245,240,245,)");
    bluetooth->println("add_text_box(4,4,1,C,59,245,240,245,m)");
    bluetooth->println("add_text(5,4,xlarge,C,:,245,240,245,)");
    bluetooth->println("add_text_box(6,4,1,C,59,245,240,245,s)");
    */
}

//void ProgramPanel::loop(){};
void ProgramPanel::loop(){
    //SERIAL_PRINTLN("ProgramPanel::loop()");
    //bluetooth->println("ProgramPanel::loop()");
        
    /////////////   Receive and Process Data
    if (bluetooth->available()){
        data_in=bluetooth->read();  //Get next character 

    }

    /////////////  Send Data to Android device
    unsigned long t=millis();
    if ((t-last_time)>update_interval){
        last_time=t;

        //programTime nextProgramTime = riego->getProgramTime();
        if(_nextProgramTime.programEnabled){
            bluetooth->print("*h");
            bluetooth->print(_nextProgramTime.hour);
            bluetooth->print(":");
            bluetooth->print(_nextProgramTime.minute);
            bluetooth->print(":");
            bluetooth->print(_nextProgramTime.second);
            bluetooth->print("*");
        }else{
            //bluetooth->print(F("*hPrograma desactivado*"));
        }

        int red=0, green=0, blue=0;
        String led;

        for(uint8_t i=0; i<8; i++){
            if(i!=7){ //weekday
                if(_nextProgramTime.programDays[i]){
                    red=0; green=255;
                }else {
                    red=255; green=0;
                }
                led = week[i];
            }else{
                if(_nextProgramTime.programEnabled){
                    red=0; green=255;
                }else{
                    red=255; green=0;
                }
                led ="*p";
            }

            if(i==selectedDay){
                red=255; green=200; blue=0;
            }

            bluetooth->print(led);
            bluetooth->print("R");
            bluetooth->print(red);
            bluetooth->print("G");
            bluetooth->print(green);
            bluetooth->print("B");
            bluetooth->print(blue);
            bluetooth->print("*");
        }
    }
}

void ProgramPanel::update(IfaceRiego* const riego, IfaceGui* const gui){
    if(selectedDay==8) riego->toggleProgramEnabled();
    else riego->toggleProgramDays(selectedDay);
    if(changingProgramTime){
        riego->setProgramTime(_nextProgramTime);
        changingProgramTime=false;
    }
    _nextProgramTime = riego->getProgramTime();
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