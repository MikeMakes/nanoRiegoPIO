#include "Gui.h"

Gui::Gui(IfaceRiego* const riego, SoftwareSerial* const serial):
IfaceGui(riego),
_bluetooth(serial)
{
    //_state = STATES::FRONTPANEL;

    //update(_riego, this);
    //setup();
    _update = true;
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

void Gui::update(){
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

    //SERIAL_PRINTLN(_riego->getProgramTimePtr()->hour);
    //SERIAL_PRINTLN(_riego->getProgramTimePtr()->minute);
    _bluetooth->print("Ah");
    _bluetooth->print(_riego->getProgramTimePtr()->hour);
    _bluetooth->print(":");
    _bluetooth->println(_riego->getProgramTimePtr()->minute);

    //SERIAL_PRINTLN(_riego->getProgramTimePtr()->programEnabled);
    _bluetooth->print("Ae");
    _bluetooth->println(_riego->getProgramTimePtr()->programEnabled);

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

void Gui::loop(){
  if(_bluetooth->available()){
    handleCmds();
  }
}

// Function prototypes for action handlers
void Gui::handleCmdSystemTime(const IfaceGui::IfaceGui::Message *msg){
  _bluetooth->readBytes(msg->payload, msg->payloadSize);

  systemTime st = _riego->getSystemTime();
  st.hour = (int)(msg->payload[0]-'0')*10+(int)(msg->payload[1]-'0');
  st.minute = (int)(msg->payload[2]-'0')*10+(int)(msg->payload[3]-'0');
  _riego->setSystemTime(st);
}
void Gui::handleCmdSystemDate(const IfaceGui::Message *msg){
  _bluetooth->readBytes(msg->payload, msg->payloadSize);

  systemTime st = _riego->getSystemTime();
  st.day = (int)(msg->payload[0]-'0')*10+(int)(msg->payload[1]-'0');
  st.month = (int)(msg->payload[2]-'0')*10+(int)(msg->payload[3]-'0');
  st.year = (int)(msg->payload[4]-'0')*1000+(int)(msg->payload[5]-'0')*100+(int)(msg->payload[6]-'0')*10+(int)(msg->payload[7]-'0');
  _riego->setSystemTime(st);
}

void Gui::handleCmdAutoEnable(const IfaceGui::Message *msg){
  _bluetooth->readBytes(msg->payload, msg->payloadSize);

  if(*msg->payload=='1')
    _riego->getProgramTimePtr()->programEnabled=true;
  else if(*msg->payload=='0')
    _riego->getProgramTimePtr()->programEnabled=false;
}

void Gui::handleCmdAutoRun(const IfaceGui::Message *msg){
  _riego->runProgram(0);
}

void Gui::handleCmdAutoTime(const IfaceGui::Message *msg){
  _riego->getProgramTimePtr()->hour=(int)(msg->payload[0]-'0')*10+(int)(msg->payload[1]-'0');
  _riego->getProgramTimePtr()->minute=(int)(msg->payload[2]-'0')*10+(int)(msg->payload[3]-'0');

  SERIAL_PRINTLN(_riego->getProgramTimePtr()->hour);
  SERIAL_PRINTLN(_riego->getProgramTimePtr()->minute);
}
void Gui::handleCmdAutoRepetition(const IfaceGui::Message *msg){

}
void Gui::handleCmdAutoDuration(const IfaceGui::Message *msg){

}

void Gui::handleCmdManualValve(const IfaceGui::Message *msg){
  _bluetooth->readBytes(msg->payload, msg->payloadSize);
  int valve = msg->payload[0]-'0';
  _riego->toggleValve(valve); // if (int)msg->payload[0]-'0' is provided it doesnt work (memory pointer thing?)
}
void Gui::handleCmdManualDuration(const IfaceGui::Message *msg){

}

void Gui::handleCmdUnknown(const IfaceGui::Message *msg){

}
void Gui::handleCmds(){
  if(_bluetooth->read()!=MESSAGE_PRELIMITER) return;
  
  _update = true;

  char cmdID = _bluetooth->read();
  msg = findHandler(cmdID);
  (this->*(msg.handler))(&msg); //Call function handler
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