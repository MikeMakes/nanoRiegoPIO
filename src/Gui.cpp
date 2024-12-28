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
    //SERIAL_PRINTLN("_bluetooth->available()");
    handleCmds();


    //SERIAL_PRINT(msg.payload[0]);
    //SERIAL_PRINTLN(msg.payload[1]);

    /*
    switch((char)cmdID){
      case 'T':
        
        break;

      case 'D':
        
        break;

      case 'e': // enable/disable program
        //SERIAL_PRINTLN("e");
        //SERIAL_PRINTLN(msg.payload[1]);
        if(msg.payload[1]=='1')
          _riego->getProgramTimePtr()->programEnabled=true;
        else if(msg.payload[1]=='0')
          _riego->getProgramTimePtr()->programEnabled=false;
        break;

      case 'r': // run program now
        //SERIAL_PRINTLN("r");
        _riego->runProgram(0);
        break;

      case 'v': // manual valve turn on/off
        //SERIAL_PRINTLN("v");
        _riego->toggleValve(msg.payload[1]-'0');
        break;

      default:
        //SERIAL_PRINTLN("Unknown cmd");
        break;
    }
    */
  }
}

// Function prototypes for action handlers
void Gui::handleCmdSystemTime(const IfaceGui::IfaceGui::Message *msg){

}
void Gui::handleCmdSystemDate(const IfaceGui::Message *msg){

}

void Gui::handleCmdAutoEnable(const IfaceGui::Message *msg){

}
void Gui::handleCmdAutoRun(const IfaceGui::Message *msg){

}
void Gui::handleCmdAutoTime(const IfaceGui::Message *msg){

}
void Gui::handleCmdAutoRepetition(const IfaceGui::Message *msg){

}
void Gui::handleCmdAutoDuration(const IfaceGui::Message *msg){

}

void Gui::handleCmdManualValve(const IfaceGui::Message *msg){

  /*
  uint8_t *valve;
  _bluetooth->readBytes(valve, 1);
  //SERIAL_PRINTLN(*valve);
  _riego->toggleValve(*valve-'0'); // if msg->payload is used before this It doesnt work
  */

  /*
  char *valve;
  _bluetooth->readBytes(valve, msg->payloadSize);
  _riego->toggleValve(*valve-'0'); // if msg->payload is used before this It doesnt work
  */

  _bluetooth->readBytes(msg->payload, msg->payloadSize);
  //SERIAL_PRINTLN(msg->payload[0]);
  //SERIAL_PRINTLN(msg->payload[0]-'0');
  int valve = msg->payload[0]-'0';
  _riego->toggleValve(valve); // if (int)msg->payload[0]-'0' is provided it doesnt work (memory pointer thing?)
}
void Gui::handleCmdManualDuration(const IfaceGui::Message *msg){

}

void Gui::handleCmdUnknown(const IfaceGui::Message *msg){

}
void Gui::handleCmds(){
  //Message msg;
  //char *cmdID;

  SERIAL_PRINTLN("handleCmds");
  if(_bluetooth->read()!=MESSAGE_PRELIMITER) return;
  
  _update = true;

  //_bluetooth->readBytes(cmdID, sizeof(char));
  char cmdID = _bluetooth->read();

  SERIAL_PRINTLN(cmdID);
  //Message msg = findHandler(*cmdID);
  //Message 
  msg = findHandler(cmdID);
  SERIAL_PRINTLN(msg.id);

  (this->*(msg.handler))(&msg); //Call function handler
  
  //MessageCmdHandler handler = findHandler2(*cmdID);
  //(this->*handler)(&msg); //Call function handler
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

// Find the handler for a given action character
Gui::MessageCmdHandler Gui::findHandler2(char cmdID) {
    for (int i = 0; cmdMessages[i].id != '\0'; ++i) {
        if (cmdMessages[i].id == cmdID) {
            return cmdMessages[i].handler;
        }
    }
    return cmdUnknown.handler; // Default for unrecognized actions
}