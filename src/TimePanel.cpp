#include "TimePanel.h"

TimePanel::TimePanel(IfaceGui* gui, IfaceRiego* riego, SoftwareSerial* serial):Panel(gui, riego, serial){}
//TimePanel::TimePanel(char commandID, char panelID, SoftwareSerial* serial):Panel(commandID, panelID, serial){}

void TimePanel::body(){
    String color;
    if(changingSystemTime){
        color = yellowText;
    }else{
        color = whiteText;
    }
    bluetooth->println("add_text_box(1,4,6,C,24,"+color+",h)");
    bluetooth->println("add_text_box(9,4,6,C,24,"+color+",D)");
    //bluetooth->println("add_text_box(1,4,6,C,24,245,240,245,h)");
    //bluetooth->println("add_text_box(9,4,6,C,24,245,240,245,D)");s
}

void TimePanel::loop(IfaceRiego* riego, IfaceGui* gui){
    Serial.println("TimePanel::loop()");
    bluetooth->println("TimePanel::loop()");
        
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

        actualTime = riego->getSystemTime();
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

void TimePanel::shiftField(bool next){
    changingSystemTime = true;
    actualTime.hour++;
};
unsigned int TimePanel::getField(){
    return actualTime.hour;
};