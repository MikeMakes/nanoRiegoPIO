#include "Gui.h"

Gui::Gui(IfaceRiego* const riego, SoftwareSerial* const serial):
IfaceGui(riego),
_bluetooth(serial)
{
    //SERIAL_PRINTLN("Gui::Gui");
    //_bluetooth->println("Gui::Gui");
    //serial->print("Gui::Gui");
    //serial->flush();

    _state = STATES::PROGRAMPANEL;

    _panels[_state]->update(_riego, this);
    _panels[_state]->setup();
    _update = false;

    /*
    for(int i=0; i<3; i++){
      _panels[i]->setSerial(serial);
    }
    */
}

void Gui::run(){
  //SERIAL_PRINTLN("Gui::run()");
  if(_update){
    _panels[_state]->setup();
    _update = false;
  }
  _panels[_state]->update(_riego, this);
  _panels[_state]->loop();
}

void Gui::setState(STATES state){
    //Serial1->println("void Gui::setState(STATES state)");
    //Serial1->println(state);
    if(state!=_state){
      _state=state;
      _update = true;
      //_panels[_state]->setup();
    }
}

void Gui::setup(){
  //SERIAL_PRINTLN("Gui::setup()");
  _panels[_state]->setup();
}

void Gui::nextState(bool right){
  //SERIAL_PRINTLN("/* nextState");
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
  _update = true;
  //SERIAL_PRINTLN("nextState */");
  //SERIAL_FLUSH();
}

void Gui::shiftField(bool directionField){
  if(_state==STATES::TIMEPANEL){
    _panels[_state]->shiftField(directionField);
  }
}

unsigned int Gui::selection(){
  //if(_state==STATES::PROGRAMPANEL){
  //  return _panels[_state]->getField();
  //}
  if(_state==STATES::TIMEPANEL){
    return _panels[_state]->getField();
  }
  return 99;
}
/*
void Gui::shiftValue(bool directionValue){
  if(_state==STATES::PROGRAMPANEL){
    _panels[_state]->shiftValue(directionValue);
  }
}
*/