#include "TimePanel.h"

TimePanel::TimePanel(SoftwareSerial* serial):Panel(serial){}
//TimePanel::TimePanel(char commandID, char panelID, SoftwareSerial* serial):Panel(commandID, panelID, serial){}

void TimePanel::body(){
    String color;
    if(changingSystemTime){
        color = yellowText;
    }else{
        color = whiteText;
    }
    //bluetooth->println(whiteAddTextBoxH);
    //bluetooth->println(greenAddTextBoxB);
    //bluetooth->println("add_text_box(1,4,6,C,24,"+color+"h)");
    //bluetooth->println("add_text_box(9,4,6,C,24,"+color+"D)");
    bluetooth->println(F("add_text_box(4,1,8,C,Fecha,245,240,245,)"));
    bluetooth->println("add_text_box(4,3,4,R,24,245,240,245,h)");
    bluetooth->println("add_text_box(8,3,4,L,24,245,240,245,D)");
    bluetooth->println(F("add_4way_pad(1,4,H,M,h,m,,50,,)"));
    bluetooth->println(F("add_4way_pad(12,4,X,D,x,d,,50,,)"));

    //SERIAL_PRINTLN("void TimePanel::body()");
    //SERIAL_PRINTLN("add_text_box(1,4,6,C,24,-------,h)");
    //SERIAL_PRINTLN(color);
    //SERIAL_PRINTLN("add_text_box(1,4,6,C,24,"+color+",h)");
    //SERIAL_PRINTLN("add_text_box(9,4,6,C,24,"+color+",D)");
}
//void TimePanel::loop(){}
void TimePanel::loop(){
    //SERIAL_PRINTLN("TimePanel::loop()");
    //bluetooth->println("TimePanel::loop()");
        
    /////////////   Receive and Process Data
    if (bluetooth->available()){
        data_in=bluetooth->read();  //Get next character 
        switch (data_in){
            case 'H':
                shiftHour(true);
                break;
            case 'M':
                shiftMinute(true);
                break;
            case 'h':
                shiftHour(false);
                break;
            case 'm':
                shiftMinute(false);
                break;
            case 'X':
                shiftMonth(true);
                break;
            case 'D':
                shiftDay(true);
                break;
            case 'x':
                shiftMonth(false);
                break;
            case 'd':
                shiftDay(false);
                break;
            default:
                break;
        }

        if(data_in==previousStateChar){
            changeState = true;
            nextState = false;
        }
        if(data_in==nextStateChar){
            changeState = true;
            nextState = true;
        }
    }

    /////////////  Send Data to Android device
    unsigned long t=millis();
    if ((t-last_time)>update_interval){
        last_time=t;

        bluetooth->print("*h");
        bluetooth->print(actualTime.hour);
        bluetooth->print(":");
        bluetooth->print(actualTime.minute);
        bluetooth->print(":");
        bluetooth->print(actualTime.second);
        bluetooth->print("*");

        bluetooth->print("*D");
        bluetooth->print(actualTime.day);
        bluetooth->print("/");
        bluetooth->print(actualTime.month);
        bluetooth->print("/");
        bluetooth->print(actualTime.year);
        bluetooth->print("*");
    }
}

void TimePanel::update(IfaceRiego* const riego, IfaceGui* const gui){
    if(changingSystemTime){
        riego->setSystemTime(actualTime);
        changingSystemTime=false;
    }
    actualTime = riego->getSystemTime();

    if(changeState){
        gui->nextState(nextState);
        changeState=false;
    }
}

void TimePanel::shiftHour(bool next){
    changingSystemTime = true;
    if(next) actualTime.hour++;
    else actualTime.hour--;
    if(actualTime.hour>23){
        actualTime.hour=0;
        shiftDay(true);
    }
    if(actualTime.hour<0){
        actualTime.hour=23;
        shiftDay(false);
    }
}

void TimePanel::shiftMinute(bool next){
    changingSystemTime = true;
    if(next) actualTime.minute++;
    else actualTime.minute--;
    if(actualTime.minute>59) {
        actualTime.minute=0;
        shiftHour(true);
    }
    if(actualTime.minute<0){
        actualTime.minute=59;
        shiftHour(false);
    }
}

void TimePanel::shiftMonth(bool next){
    changingSystemTime = true;
    if(next) actualTime.month++;
    else actualTime.month--;
    if(actualTime.month>12){
        actualTime.month=1;
        shiftYear(true);
    }
    if(actualTime.month<1) {
        actualTime.month=12;
        shiftYear(false);
    }
}

void TimePanel::shiftDay(bool next){
    changingSystemTime = true;
    if(next) actualTime.day++;
    else actualTime.day--;
    int dayMax = 31;
    if((actualTime.month%2)==0){
        if(actualTime.month == 2) dayMax=29;
        else {
            if(actualTime.month<7) dayMax=30;
        }
    } else{
        if(actualTime.month>=7) dayMax=30;
    }
    if(actualTime.day>dayMax){
        actualTime.day=1;
        shiftMonth(true);
    }
    if(actualTime.day<1){
        actualTime.day=dayMax;
        shiftMonth(false);
    }
}

void TimePanel::shiftYear(bool next){
    changingSystemTime = true;
    if(next) actualTime.year++;
    else actualTime.year--;
    if (actualTime.year<1970) actualTime.year =1970;
}

unsigned int TimePanel::getHour(){
    return actualTime.hour;
}


void TimePanel::shiftField(bool next){
    shiftHour(next);
}
unsigned int TimePanel::getField(){
    return getHour();
}