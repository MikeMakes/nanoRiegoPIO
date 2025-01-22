#include "Gui.h"

Gui::Gui(IfaceRiego* const riego, SoftwareSerial* const serial):
IfaceGui(riego),
_bluetooth(serial)
{
    //_state = STATES::FRONTPANEL;

    //update(_riego, this);
    //setup();

    //updateGuiData();
    _update = true;
}

void Gui::updateGuiData(){
  previousGuiData.st = _riego->getSystemTime();
  previousGuiData.pt = _riego->getProgramTime();
  previousGuiData.running = _riego->_running;
  for(int i=0; i<3; ++i){
    previousGuiData.valves[i] = _riego->getValve(i);
  }
}

void Gui::run(){
  //SERIAL_PRINTLN("Gui::run()");
  /*
  if(_update){
    setup();
    _update = false;
  }
  */
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

//void Gui::setup(){}

void Gui::nextState(bool right){
  _update = true;
}

void Gui::update(){ //UNTESTED
  unsigned long t = millis();

  GuiMessage currentGuiData = {};
  currentGuiData.st = _riego->getSystemTime();
  currentGuiData.pt = _riego->getProgramTime();
  currentGuiData.running = _riego->_running;
  for(int i=0; i<3; ++i){
    currentGuiData.valves[i] = _riego->getValve(i);
  }

  if ( currentGuiData!=previousGuiData || (t-last_time)>GUI_UPDATE_INTERVAL_FORCED ){    
    if(currentGuiData.st != previousGuiData.st || (t-last_time)>GUI_UPDATE_INTERVAL_FORCED){
      _bluetooth->print("Sh");
      if(currentGuiData.st.hour<10) _bluetooth->print("0");
      _bluetooth->print(currentGuiData.st.hour);
      _bluetooth->print(":");
      if(currentGuiData.st.minute<10) _bluetooth->print("0");
      _bluetooth->println(currentGuiData.st.minute);

      _bluetooth->print("Sd");
      if(currentGuiData.st.day<10) _bluetooth->print("0");
      _bluetooth->print(currentGuiData.st.day);
      _bluetooth->print("/");
      if(currentGuiData.st.month<10) _bluetooth->print("0");
      _bluetooth->print(currentGuiData.st.month);
      _bluetooth->print("/");
      _bluetooth->println(currentGuiData.st.year);
    }

    if(currentGuiData.pt != previousGuiData.pt || (t-last_time)>GUI_UPDATE_INTERVAL_FORCED){ //tbd change for currentGuiData
      _bluetooth->print("Ae");  //Program enabled
      _bluetooth->println(_riego->getProgramTime().programEnabled);

      _bluetooth->print("Ah");  //Program hour
      if(_riego->getProgramTime().hour<10) _bluetooth->print("0");
      _bluetooth->print(_riego->getProgramTime().hour);
      _bluetooth->print(":");
      if(_riego->getProgramTime().minute<10) _bluetooth->print("0");
      _bluetooth->println(_riego->getProgramTime().minute);

      _bluetooth->print("Aw"); //Program weekdays
      for(int i=0; i<7; i++){
        _bluetooth->print(_riego->getProgramTime().programDays[i]);
        _bluetooth->print(",");
      }
      _bluetooth->println();

      _bluetooth->print("Ad"); //Program Delay
      _bluetooth->println(_riego->getProgramTime().delay/60);
      
    }

    if(currentGuiData.running != previousGuiData.running || (t-last_time)>GUI_UPDATE_INTERVAL_FORCED){
      _bluetooth->print("Ar");
      if(_riego->_running) _bluetooth->println("1");
      else _bluetooth->println("0");
    }

    for(int i=0; i<3; ++i){
      if(currentGuiData.valves[i] != previousGuiData.valves[i] || (t-last_time)>GUI_UPDATE_INTERVAL_FORCED){
        _bluetooth->print("CMv");
        _bluetooth->print(i);
        if(_riego->getValve(i)){
          _bluetooth->println("_ON");
        } else{
          _bluetooth->println("_OFF");
        }
      }
    }

    last_time=t;
    previousGuiData = currentGuiData;
    _update = false;
  }
}
/*
void Gui::update(){ //TESTED AND WORKING
  unsigned long t=millis();
  if (((t-last_time)>update_interval && _update)||(t-last_time)>GUI_UPDATE_INTERVAL_FORCED){
    last_time=t;

    systemTime st = _riego->getSystemTime();
    _bluetooth->print("Sh");
    if(st.hour<10) _bluetooth->print("0");
    _bluetooth->print(st.hour);
    _bluetooth->print(":");
    if(st.minute<10) _bluetooth->print("0");
    _bluetooth->println(st.minute);

    _bluetooth->print("Sd");
    if(st.day<10) _bluetooth->print("0");
    _bluetooth->print(st.day);
    _bluetooth->print("/");
    if(st.month<10) _bluetooth->print("0");
    _bluetooth->print(st.month);
    _bluetooth->print("/");
    _bluetooth->println(st.year);

    //SERIAL_PRINTLN(_riego->getProgramTime().programEnabled);
    _bluetooth->print("Ae");
    _bluetooth->println(_riego->getProgramTime().programEnabled);

    _bluetooth->print("Ar");
    if(_riego->_running) _bluetooth->println("1");
    else _bluetooth->println("0");

    //SERIAL_PRINTLN(_riego->getProgramTime().hour);
    //SERIAL_PRINTLN(_riego->getProgramTime().minute);
    _bluetooth->print("Ah");
    if(_riego->getProgramTime().hour<10) _bluetooth->print("0");
    _bluetooth->print(_riego->getProgramTime().hour);
    _bluetooth->print(":");
    if(_riego->getProgramTime().minute<10) _bluetooth->print("0");
    _bluetooth->println(_riego->getProgramTime().minute);

    _bluetooth->print("Aw"); //weekdays
    for(int i=0; i<7; i++){
      _bluetooth->print(_riego->getProgramTime().programDays[i]);
      _bluetooth->print(",");
    }
    _bluetooth->println();

    _bluetooth->print("Ad"); //ProgramDelay
    _bluetooth->println(_riego->getProgramTime().delay/60);

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
    _update = false;
  }
}
*/

void Gui::loop(){
  if(_bluetooth->available()){
    handleCmds();
  }
}

void Gui::handleCmds(){
  if(_bluetooth->read()!=MESSAGE_PRELIMITER) return;
  
  _update = true;

  char cmdID = _bluetooth->read();
  msg = findHandler(cmdID);
  (this->*(msg.handler))(&msg); //Call function handler
}
// Function prototypes for action handlers
void Gui::handleCmdSystemTime(const IfaceGui::IfaceGui::Message *msg){ //TESTED
  _bluetooth->readBytes(msg->payload, msg->payloadSize);

  systemTime st = _riego->getSystemTime();
  st.hour = (int)(msg->payload[0]-'0')*10+(int)(msg->payload[1]-'0');
  st.minute = (int)(msg->payload[2]-'0')*10+(int)(msg->payload[3]-'0');
  _riego->setSystemTime(st);
}
void Gui::handleCmdSystemDate(const IfaceGui::Message *msg){  //TESTED
  _bluetooth->readBytes(msg->payload, msg->payloadSize);

  systemTime st = _riego->getSystemTime();
  st.day = (int)(msg->payload[0]-'0')*10+(int)(msg->payload[1]-'0');
  st.month = (int)(msg->payload[2]-'0')*10+(int)(msg->payload[3]-'0');
  st.year = (int)(msg->payload[4]-'0')*1000+(int)(msg->payload[5]-'0')*100+(int)(msg->payload[6]-'0')*10+(int)(msg->payload[7]-'0');
  _riego->setSystemTime(st);
}
void Gui::handleCmdAutoEnable(const IfaceGui::Message *msg){ //TESTED
  _bluetooth->readBytes(msg->payload, msg->payloadSize);

  programTime pt = _riego->getProgramTime();
  if(*msg->payload=='1') pt.programEnabled=true;
  else if(*msg->payload=='0') pt.programEnabled=false;
  _riego->setProgramTime(pt);
}
void Gui::handleCmdAutoRun(const IfaceGui::Message *msg){ //TESTED
  //SERIAL_PRINTLN("RUN");
  _riego->runProgram();
}
void Gui::handleCmdAutoTime(const IfaceGui::Message *msg){  //TESTED
  _bluetooth->readBytes(msg->payload, msg->payloadSize);

  programTime pt = _riego->getProgramTime();
  pt.hour = (int)(msg->payload[0]-'0')*10+(int)(msg->payload[1]-'0');
  pt.minute = (int)(msg->payload[2]-'0')*10+(int)(msg->payload[3]-'0');
  _riego->setProgramTime(pt);
}
//void Gui::handleCmdAutoRepetition(const IfaceGui::Message *msg){}
void Gui::handleCmdAutoDays(const IfaceGui::Message *msg){ //TESTED-kinda
  _bluetooth->readBytes(msg->payload, msg->payloadSize);

  int weekday = (int)(msg->payload[0]-'0')-1;

  programTime pt = _riego->getProgramTime();
  if(msg->payload[1]=='1') pt.programDays[weekday] = true;
  else if(msg->payload[1]=='0') pt.programDays[weekday] = false;
  _riego->setProgramTime(pt);
}
void Gui::handleCmdAutoDuration(const IfaceGui::Message *msg){ //TESTED
  _bluetooth->readBytes(msg->payload, msg->payloadSize);

  int minutes = (int)(msg->payload[0]-'0')*10+(int)(msg->payload[1]-'0');
  if(minutes<1 || minutes>60) return;

  programTime pt = _riego->getProgramTime();
  pt.delay = minutes * 60;
  _riego->setProgramTime(pt);
}

void Gui::handleCmdManualValve(const IfaceGui::Message *msg){ //TESTED
  _bluetooth->readBytes(msg->payload, msg->payloadSize);
  int valve = msg->payload[0]-'0';
  _riego->toggleValve(valve); // if (int)msg->payload[0]-'0' is provided it doesnt work (memory pointer thing?)
}
void Gui::handleCmdManualDuration(const IfaceGui::Message *msg){  //NOT IMPLEMENTED

}

void Gui::handleCmdUnknown(const IfaceGui::Message *msg){

}

// Find the handler for a given action character
Gui::Message Gui::findHandler(char cmdID) {
    for (int i = 0; cmdMessages[i].id != '\0'; ++i) {
        if (cmdMessages[i].id == cmdID) {
            return cmdMessages[i];
        }
    }
    return cmdUnknown; // Default for unrecognized actions
}