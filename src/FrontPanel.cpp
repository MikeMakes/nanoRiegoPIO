#include "FrontPanel.h"
#include "Riego.h"

FrontPanel::FrontPanel(IfaceGui* gui, IfaceRiego* riego, SoftwareSerial* serial):Panel(gui, riego, serial){}
//FrontPanel::FrontPanel(char commandID, char panelID, SoftwareSerial* serial):Panel(commandID, panelID, serial){}

void FrontPanel::body(){
    bluetooth->println(F("add_led(3,3,2,R,0,0,0)"));
    bluetooth->println(F("add_led(7,3,2,G,0,0,0)"));
    bluetooth->println(F("add_led(11,3,2,B,0,0,0)"));
}

void FrontPanel::loop(IfaceRiego* riego, IfaceGui* gui){
    SERIAL_PRINTLN("FrontPanel::loop()");
    bluetooth->println("FrontPanel::loop()");
        
    /////////////   Receive and Process Data

    if (bluetooth->available()){
        data_in=bluetooth->read();  //Get next character 

    }

    /////////////  Send Data to Android device

    unsigned long t=millis();
    if ((t-last_time)>update_interval){
        last_time=t;

        // Update Text Element 
        //text="Zonas de riego"; // <--- Set text to send here 
        //bluetooth->print("*S"+text+"*");

        for(int i=0; i<3; i++){
            SERIAL_PRINTLN(i);
            SERIAL_PRINTLN(valveLeds[i]);
            SERIAL_PRINTLN(riego->getValve(i));
            //if(static_cast<Riego*>(riego)->getValve(i)!=valveLeds[i]){
                valveLeds[i] = riego->getValve(i);
                switch(i){
                    case 0:
                        // Update LED Color 
                        if(valveLeds[0]){
                            red=0; green=255; blue=0; // <--- Set RGB color here 
                        } else{
                            red=255; green=0; blue=0; // <--- Set RGB color here 
                        }
                        //bluetooth->print("*RR"+String(red)+"G"+String(green)+"B"+String(blue)+"*");
                        bluetooth->print("*RR");
                        bluetooth->print(red);
                        bluetooth->print("G");
                        bluetooth->print(green);
                        bluetooth->print("B");
                        bluetooth->print(blue);
                        bluetooth->print("*");
                        break;

                    case 1:
                        if(valveLeds[1]){
                            red=0; green=255; blue=0; // <--- Set RGB color here 
                        } else{
                            red=255; green=0; blue=0; // <--- Set RGB color here 
                        }
                        //bluetooth->print("*GR"+String(red)+"G"+String(green)+"B"+String(blue)+"*");
                        bluetooth->print("*GR");
                        bluetooth->print(red);
                        bluetooth->print("G");
                        bluetooth->print(green);
                        bluetooth->print("B");
                        bluetooth->print(blue);
                        bluetooth->print("*");
                        break;
                    
                    case 2:
                        if(valveLeds[2]){
                            red=0; green=255; blue=0; // <--- Set RGB color here 
                        } else{
                            red=255; green=0; blue=0; // <--- Set RGB color here 
                        }
                        //bluetooth->print("*BR"+String(red)+"G"+String(green)+"B"+String(blue)+"*");
                        bluetooth->print("*BR");
                        bluetooth->print(red);
                        bluetooth->print("G");
                        bluetooth->print(green);
                        bluetooth->print("B");
                        bluetooth->print(blue);
                        bluetooth->print("*");
                        break;

                    default:
                    // default statements
                        break;
                }//switch
            //}//if
        }//for
    }
}

void FrontPanel::shiftField(bool next){};
unsigned int FrontPanel::getField(){};