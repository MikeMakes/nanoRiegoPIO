#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>
#include "Constants.h"

class Relay{
  public:
    Relay(){
      return;
    }
    Relay(int pin, bool NO = true){
      _relayPin = pin;
      _state = true;
      _relayNO = NO;
      pinMode(_relayPin, OUTPUT);
      digitalWrite(_relayPin, NO);
    }
    void setState(bool state){
      if(_relayNO) state = !state;
      _state = state;
      digitalWrite(_relayPin, _state);
      //Guardar en eeprom
    }
    bool getState(){
      if(!_relayNO) return _state;
      return !_state;
    }
  private:
    int _relayPin;
    bool _state = true;
    bool _relayNO = true;
};
#endif