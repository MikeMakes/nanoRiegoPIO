#include "Gui.h"

Gui::Gui(IfaceRiego* const riego, SoftwareSerial* const serial):
IfaceGui(riego),
_bluetooth(serial)
{
    //_state = STATES::FRONTPANEL;

    //update(_riego, this);
    setup();
    _update = false;
}

void Gui::run(){
  //SERIAL_PRINTLN("Gui::run()");
  if(_update){
    setup();
    _update = false;
  }
  //update(_riego, this);
  update();
  loop();
}

void Gui::setState(STATES state){
    //Serial1->println("void Gui::setState(STATES state)");
    //Serial1->println(state);
    //if(state!=_state){
      //_state=state;
      _update = true;
      //_panels[_state]->setup();
    //}
}

void Gui::setup(){
}

void Gui::nextState(bool right){
  /*
  //SERIAL_PRINTLN("/.* nextState");
  int s = int(_state);
  if(right){
    if(s>=2) s=-1;
    _state = Gui::STATES(s+1);
    //SERIAL_PRINTLN(_state);
  }
  else{
    if(s<=0) s=3;
    _state = Gui::STATES(s-1);
    //SERIAL_PRINTLN(_state);
  }
  */
  _update = true;
  //SERIAL_PRINTLN("nextState */");
  //SERIAL_FLUSH();
}
/*
void Gui::loopValvesButtons(){
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
void Gui::loopManualControl(){}
void Gui::loopProgramTime(){}
void Gui::loopSystemTime(){}
*/

void Gui::update(){
  unsigned long t=millis();
  if ((t-last_time)>update_interval){
    last_time=t;

    for(int i=0; i<numValves; i++){ //if(static_cast<Riego*>(riego)->getValve(i)!=valveLeds[i]){
          //SERIAL_PRINTLN(riego->getValve(i));
          _bluetooth->print("CMv");
          _bluetooth->print(i);
          if(_riego->getValve(i)){
            _bluetooth->println("_ON");
          } else{
            _bluetooth->println("_OFF");
          }
    }
  }
}

void Gui::loop(){
  //char data_in = '0';
  //String data_in;

  char cmdStart = '*';
  msgManualControl msgMC;

  if(_bluetooth->available()){
    SERIAL_PRINTLN("_bluetooth->available()");
    //data_in = _bluetooth->readString();
    //_bluetooth->readBytes(&cmdStart, sizeof(cmdStart));
    
    if(_bluetooth->read()!=cmdStart) return;
    //_riego->toggleValve(1);

    switch(_bluetooth->read()){
      case msgMC.id:
        _bluetooth->readBytes(msgMC.payload, sizeof(msgMC.payload));
        //SERIAL_PRINT(msgMC.payload[0]);
        //SERIAL_PRINT(msgMC.payload[1]);
        //SERIAL_PRINTLN(msgMC.payload);
        break;
      
      default:
        //SERIAL_PRINTLN("Unknown cmd");
        break;
    }

    //data_in=_bluetooth->read();

    SERIAL_PRINT(msgMC.payload[0]);
    if(msgMC.payload[0]=='v'){
    SERIAL_PRINT(msgMC.payload[1]);
      _riego->toggleValve(msgMC.payload[1]-'0');
      SERIAL_PRINTLN("VALVED");
    }

/*
    if(data_in.indexOf("CMv0") > 0){
      SERIAL_PRINTLN("0");
      _riego->toggleValve(0);
    }
    if(data_in.indexOf("CMv1") > 0){
      SERIAL_PRINTLN("1");
      _riego->toggleValve(1);
    }
    if(data_in.indexOf("CMv2") > 0){
      SERIAL_PRINTLN("2");
      _riego->toggleValve(2);
    }
    int h = data_in.indexOf("CAh");
    if(h > 0){
      _riego->getProgramTimePtr()->hour=data_in.substring(h,h+1).toInt();
    }
      */

  }

  ///String data_in = _bluetooth->readString();


}