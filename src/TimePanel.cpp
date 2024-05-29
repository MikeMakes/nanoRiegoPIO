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
    bluetooth->println("add_text_box(1,4,6,C,24,245,240,245,h)");
    bluetooth->println("add_text_box(9,4,6,C,24,245,240,245,D)");

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

    }

    /////////////  Send Data to Android device
    unsigned long t=millis();
    if ((t-last_time)>update_interval){
        last_time=t;

        // Update Text Element 
        //text="abc"; // <--- Set text to send here 
        //bluetooth->print("*S"+text+"*");

        //actualTime = riego->getSystemTime();
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
        /*
        bluetooth->print("*h");
        bluetooth->print(actualTime.hour);
        bluetooth->print("*");
        bluetooth->print("*m");
        bluetooth->print(actualTime.minute);
        bluetooth->print("*");
        bluetooth->print("*s");
        bluetooth->print(actualTime.second);
        bluetooth->print("*");
        bluetooth->print("*D");
        bluetooth->print(actualTime.day);
        bluetooth->print("*");
        bluetooth->print("*M");
        bluetooth->print(actualTime.month);
        bluetooth->print("*");
        bluetooth->print("*Y");
        bluetooth->print(actualTime.year);
        bluetooth->print("*");
        */
    }
}

void TimePanel::update(IfaceRiego* const riego, IfaceGui* const gui){
    if(changingSystemTime){
        riego->setSystemTime(actualTime);
        changingSystemTime=false;
    }
    actualTime = riego->getSystemTime();
}

void TimePanel::shiftHour(bool next){
    changingSystemTime = true;
    if(next) actualTime.hour++;
    else actualTime.hour--;
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