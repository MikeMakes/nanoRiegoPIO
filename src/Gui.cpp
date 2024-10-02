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
  _update = true;
}

void Gui::update(){
  unsigned long t=millis();
  if ((t-last_time)>update_interval){
    last_time=t;


    //SERIAL_PRINTLN(_riego->getProgramTimePtr()->hour);
    //SERIAL_PRINTLN(_riego->getProgramTimePtr()->minute);
    _bluetooth->print("Ah");
    _bluetooth->print(_riego->getProgramTimePtr()->hour);
    _bluetooth->print(":");
    _bluetooth->print(_riego->getProgramTimePtr()->minute);

    //SERIAL_PRINTLN(_riego->getProgramTimePtr()->programEnabled);
    _bluetooth->print("Ae");
    _bluetooth->print(_riego->getProgramTimePtr()->programEnabled);

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
  msgGui msg;
  
  if(_bluetooth->available()){
    SERIAL_PRINTLN("_bluetooth->available()");
    
    if(_bluetooth->read()!=msg.id) return;
    
    _bluetooth->readBytes(msg.payload, sizeof(msg.payload));

    SERIAL_PRINT(msg.payload[0]);
    SERIAL_PRINTLN(msg.payload[1]);

    switch(msg.payload[0]){
      case 'v':
        //SERIAL_PRINTLN("v");
        _riego->toggleValve(msg.payload[1]-'0');
        break;

      case 'e':
        SERIAL_PRINTLN("e");
        SERIAL_PRINTLN(msg.payload[1]);
        if(msg.payload[1]=='1')
          _riego->getProgramTimePtr()->programEnabled=true;
        else if(msg.payload[1]=='0')
          _riego->getProgramTimePtr()->programEnabled=false;
        break;

      case 'r':
        SERIAL_PRINTLN("r");
        _riego->runProgram(0);
        break;

      default:
        //SERIAL_PRINTLN("Unknown cmd");
        break;
    }
  }
}