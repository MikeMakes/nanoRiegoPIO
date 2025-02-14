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

void Gui::checkConnection(){
  bool bluetoothState = digitalRead(PIN_BLUETOOTH_ST);
  if(bluetoothState != _connected){
    _connected = bluetoothState;
    //if(_connected) newConnection(); //update(forced = true);
    //else disconnection(); //nothing? flush serial?
  }
}

Gui::GuiMessage& Gui::updateGuiData(GuiMessage& guiMessage){
  guiMessage.st = _riego->getSystemTime();
  guiMessage.pt = _riego->getProgramTime();
  guiMessage.running = _riego->_running;
  for(int i=0; i<numValves; ++i){
    guiMessage.valves[i] = _riego->getValve(i);
  }
  return guiMessage;
}

void Gui::sendTimestamp(){
  _bluetooth->print(";");
  _bluetooth->println(millis());
}
void Gui::sendSystemHour(const systemTime& st){
  _bluetooth->print("*T");
  if (st.hour < 10) _bluetooth->print("0");
  _bluetooth->print(st.hour);
  _bluetooth->print(":");
  if (st.minute < 10) _bluetooth->print("0");
  _bluetooth->print(st.minute);

  sendTimestamp();
}
void Gui::sendSystemDate(const systemTime& st){
  _bluetooth->print("*D");
  if (st.day < 10) _bluetooth->print("0");
  _bluetooth->print(st.day);
  _bluetooth->print("/");
  if (st.month < 10) _bluetooth->print("0");
  _bluetooth->print(st.month);
  _bluetooth->print("/");
  _bluetooth->print(st.year);

  sendTimestamp();
}
void Gui::sendSystemTime(const systemTime& st) {
  sendSystemHour(st);
  sendSystemDate(st);
}
void Gui::sendProgramEnabled(const programTime& pt) {
  _bluetooth->print("*e");
  _bluetooth->print(pt.programEnabled);

  sendTimestamp();
}
void Gui::sendProgramHour(const programTime& pt) {
  _bluetooth->print("*h");
  if (pt.hour < 10) _bluetooth->print("0");
  _bluetooth->print(pt.hour);
  _bluetooth->print(":");
  if (pt.minute < 10) _bluetooth->print("0");
  _bluetooth->print(pt.minute);

  sendTimestamp();
}
void Gui::sendProgramWeekdays(const programTime& pt){
  _bluetooth->print("*w");
  for (int i = 0; i < 7; ++i) {
      _bluetooth->print(pt.programDays[i]);
      if (i < 6) _bluetooth->print(",");
  }
  //_bluetooth->println();
  sendTimestamp();
}
void Gui::sendProgramDelay(const programTime& pt){
  _bluetooth->print("*d");
  int delayMinutes = pt.delay/60;
  if (delayMinutes < 10) _bluetooth->print("0");
  _bluetooth->print(delayMinutes);

  sendTimestamp();
}
void Gui::sendProgramTime(const programTime& pt) {
  sendProgramEnabled(pt);
  sendProgramHour(pt);
  sendProgramWeekdays(pt);
  sendProgramDelay(pt);
}
void Gui::sendRunningState(bool running) {
  _bluetooth->print("*r");
  _bluetooth->print(running ? "1" : "0");

  sendTimestamp();
}
void Gui::sendValveState(int index, bool state) {
  _bluetooth->print("*v");
  _bluetooth->print(index);
  _bluetooth->print(state ? "1" : "0");

  sendTimestamp();
}
void Gui::sendGuiMessage(const GuiMessage& guiMessage){
  sendSystemTime(guiMessage.st);
  sendProgramTime(guiMessage.pt);
  sendRunningState(guiMessage.running);
  for(int i=0; i<numValves; ++i){
    sendValveState(i, guiMessage.valves[i]);
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
  updateGuiData(currentGuiData);

  if((t-last_time)>GUI_UPDATE_INTERVAL_FORCED){
    sendGuiMessage(currentGuiData);

    last_time=t;
    previousGuiData = currentGuiData;
    _update = false;
    return;
  }

  if (currentGuiData != previousGuiData){
    if(currentGuiData.st != previousGuiData.st) sendSystemTime(currentGuiData.st);
    if(currentGuiData.pt != previousGuiData.pt) sendProgramTime(currentGuiData.pt);
    if(currentGuiData.running != previousGuiData.running) sendRunningState(currentGuiData.running);
    for(int i=0; i<numValves; ++i){
      if(currentGuiData.valves[i] != previousGuiData.valves[i]) sendValveState(i, currentGuiData.valves[i]);
    }
    previousGuiData = currentGuiData;
    _update = false;
  }
}

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

  _bluetooth->print("*ACK");
  _bluetooth->print("*w");
  _bluetooth->print(weekday+1);
  _bluetooth->print(_riego->getProgramTime().programDays[weekday]);
  sendTimestamp();
}
void Gui::handleCmdAutoDuration(const IfaceGui::Message *msg){ //TESTED
  _bluetooth->readBytes(msg->payload, msg->payloadSize);

  int minutes = (int)(msg->payload[0]-'0')*10+(int)(msg->payload[1]-'0');
  if(minutes<1 || minutes>60) return;

  programTime pt = _riego->getProgramTime();
  pt.delay = minutes * 60;
  _riego->setProgramTime(pt);

  _bluetooth->print("*ACK");
  sendProgramDelay(_riego->getProgramTime());
  sendTimestamp();
  //_bluetooth->print(msg->id);
  //_bluetooth->print(msg->payload[0]);
  //_bluetooth->println(msg->payload[1]);
}

void Gui::handleCmdManualValve(const IfaceGui::Message *msg){ //TESTED
  _bluetooth->readBytes(msg->payload, msg->payloadSize);
  int valve = msg->payload[0]-'0';
  _riego->toggleValve(valve); // if (int)msg->payload[0]-'0' is provided it doesnt work (memory pointer thing?)

  /*
  _bluetooth->print("*ACK");
  _bluetooth->print("*v");
  _bluetooth->print(valve);
  _bluetooth->print(_riego->getValve(valve));
  sendTimestamp();
  */
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