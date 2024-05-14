#include "Gui.h"

Gui::Gui(IfaceRiego* riego, SoftwareSerial* serial):
IfaceGui(riego),
_bluetooth(serial)
{
    _state = STATES::FRONTPANEL;
    _update = false;
    _panels[_state]->setup();

    serial->println("Gui::Gui");
    /*
    for(int i=0; i<3; i++){
      _panels[i]->setSerial(serial);
    }*/
}

void Gui::run(){
  Serial.println("Gui::run()");
  if(_update){
    _update = false;
    _panels[_state]->setup();
  }
  _panels[_state]->loop(_riego, this);
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
  Serial.println("Gui::setup()");
  _panels[_state]->setup();
}

void Gui::nextState(bool right){
  //Serial.println("/* nextState");
  int s = int(_state);
  if(right){
    if(s>=2) s=-1;
    _state = Gui::STATES(s+1);
    //Serial.println(_state);
  }
  else{
    if(s<=0) s=3;
    _state = Gui::STATES(s-1);
    //Serial.println(_state);
  }
  _update = true;
  //Serial.println("nextState */");
  //Serial.flush();
}

void Gui::shiftField(bool directionField){
  if(_state==STATES::PROGRAMPANEL || _state==STATES::TIMEPANEL){
    _panels[_state]->shiftField(directionField);
  }
}

unsigned int Gui::selection(){
  if(_state==STATES::PROGRAMPANEL){
    return _panels[_state]->getField();
  }
  if (_state==STATES::TIMEPANEL){
    return _panels[_state]->getField();
  }
}
/*
void Gui::shiftValue(bool directionValue){
  if(_state==STATES::PROGRAMPANEL){
    _panels[_state]->shiftValue(directionValue);
  }
}
*/