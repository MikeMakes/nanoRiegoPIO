#include "FrontPanel.h"
#include "Riego.h"

FrontPanel::FrontPanel(SoftwareSerial* serial):Panel(serial){}
//FrontPanel::FrontPanel(char commandID, char panelID, SoftwareSerial* serial):Panel(commandID, panelID, serial){}

void FrontPanel::body(){
    bluetooth->println(F("add_led(3,3,2,R,0,0,0)"));
    bluetooth->println(F("add_led(7,3,2,G,0,0,0)"));
    bluetooth->println(F("add_led(11,3,2,B,0,0,0)"));
}
//void FrontPanel::loop(){}
void FrontPanel::loop(){
    //("FrontPanel::loop()");
    //bluetooth->println("FrontPanel::loop()");
        
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

        for(int i=0; i<numValves; i++){
            //SERIAL_PRINTLN(valveLeds[i]);
            //valveLeds[i] = riego->getValve(i);

            // Update LED Color 
            if(valveLeds[i]){
                red=0; green=255; blue=0; // <--- Set RGB color here 
            } else{
                red=255; green=0; blue=0; // <--- Set RGB color here 
            }
            //bluetooth->print("*RR"+String(red)+"G"+String(green)+"B"+String(blue)+"*");
            switch(i){
                case 0:
                    bluetooth->print("*RR");
                    break;
                case 1:
                    bluetooth->print("*GR");
                    break;
                case 2:
                    bluetooth->print("*BR");
                    break;
            }
                
            //bluetooth->print("*RR");
            bluetooth->print(red);
            bluetooth->print("G");
            bluetooth->print(green);
            bluetooth->print("B");
            bluetooth->print(blue);
            bluetooth->print("*");
        }//for
    }
}

void FrontPanel::update(IfaceRiego* riego, IfaceGui* gui){
        for(int i=0; i<numValves; i++){ //if(static_cast<Riego*>(riego)->getValve(i)!=valveLeds[i]){
            //SERIAL_PRINTLN(riego->getValve(i));
            valveLeds[i] = riego->getValve(i);
        }
}

void FrontPanel::shiftField(bool next){};
unsigned int FrontPanel::getField(){
    return 0;
}