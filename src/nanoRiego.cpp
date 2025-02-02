#include <Arduino.h>
#ifdef DEBUG_BUILD
  #include "avr8-stub.h" //debug only
  #include "app_api.h"  //debug only
#endif

#include <EEPROM.h>
#include <TimeLib.h>
#include <Wire.h>
#include <TimeAlarms.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
#include <SoftwareSerial.h>
#include <Versatile_RotaryEncoder.h>

#include "Constants.h"

#include "Riego.h"
#include "Gui.h"

void setEEPROM(){
  EEPROM.put(EEPROM_timeAddress, (time_t) DEFAULT_TIME);
}

SoftwareSerial bluetooth(PIN_BLUETOOTH_RX, PIN_BLUETOOTH_TX); //RX,TX
Relay pump(pinPump, true);
Relay valves[]={Relay(pinValve1, true), Relay(pinValve2, true), Relay(pinValve3, true)};
Riego riego(&pump, valves, PROGRAM_DELAY);
Gui* gui;

void saveEEPROM(){
  //EEPROM.put(EEPROM_timeAddress, (time_t) DEFAULT_TIME);
  //EEPROM.put(sizeof(EEPROM_timeAddress), (unsigned int) riego.getProgramTime().hour);
  //EEPROM.put(sizeof(EEPROM_timeAddress)+sizeof(unsigned int), (unsigned int) riego.getProgramTime().delay);
  EEPROM.put(30, (unsigned int) riego.getProgramTime().hour);
  EEPROM.put(40, (unsigned int) riego.getProgramTime().minute);
  EEPROM.put(60, (unsigned int) riego.getProgramTime().delay);
}

void loadEEPROM(){
  //EEPROM.put(EEPROM_timeAddress, (time_t) DEFAULT_TIME);
  unsigned int hour,minute,delay;
  EEPROM.get(30, hour);
  EEPROM.get(40, minute);
  EEPROM.get(60, delay);
  programTime pt = riego.getProgramTime();
  pt.hour = hour;
  pt.delay = delay;
  riego.setProgramTime(pt);
}

AlarmID_t alarmID;
void alarmRiego(){
  riego.check();
}

void alarmProgram(){
  riego.runProgram();
}

Versatile_RotaryEncoder versatile_encoder[3]={Versatile_RotaryEncoder(PIN_SELECTOR_B1, PIN_SELECTOR_A1, PIN_BUTTON_1), Versatile_RotaryEncoder(PIN_SELECTOR_B2, PIN_SELECTOR_A2, PIN_BUTTON_2), Versatile_RotaryEncoder(PIN_SELECTOR_B3, PIN_SELECTOR_A3, PIN_BUTTON_3)};

// Functions prototyping to be handled on each Encoder Event
void handleRotate1(int8_t rotationDirection);
void handlePress1();
void handleLongPress1();
void handleRotate2(int8_t rotationDirection);
void handlePress2();
void handleLongPress2();
void handleRotate3(int8_t rotationDirection);
void handlePress3();
void handleLongPress3();

void setup() {
  #ifdef DEBUG_BUILD 
  debug_init(); //debug only
  debug_message("void setup()");
  #endif

  SERIAL_BEGIN(115200);
  //SERIAL_PRINTLN("setup()");
  bluetooth.begin(9600);

  setEEPROM();
  time_t startTime;
  EEPROM.get(EEPROM_timeAddress,startTime);

  //RTC.set(startTime);
  //setTime(startTime);

  setSyncProvider(RTC.get);   // the function to get the time from the RTC

  if(timeStatus()!= timeSet){ //tbd: || RTC.get()>startTime;  ??
     //SERIAL_PRINTLN("RTC'nt");      //Unable to sync with the RTC
     RTC.set(startTime);
     setTime(startTime);
  }

  // Load to the encoder all nedded handle functions here (up to 9 functions)
  versatile_encoder[0].setHandleRotate(handleRotate1);
  versatile_encoder[0].setHandlePress(handlePress1);
  versatile_encoder[0].setHandleLongPress(handleLongPress1);
  versatile_encoder[1].setHandleRotate(handleRotate2);
  versatile_encoder[1].setHandlePress(handlePress2);
  versatile_encoder[1].setHandleLongPress(handleLongPress2);
  versatile_encoder[2].setHandleRotate(handleRotate3);
  versatile_encoder[2].setHandlePress(handlePress3);  
  versatile_encoder[2].setHandleLongPress(handleLongPress3);

  //SERIAL_FLUSH();
  gui = new Gui(&riego, &bluetooth);
  riego.gui(gui);

  loadEEPROM();
  alarmID = Alarm.alarmRepeat(riego.getProgramTime().hour, riego.getProgramTime().minute, riego.getProgramTime().second, alarmRiego);
  Alarm.delay(10);
}

bool rotation[3] = {false,false,false};
bool rotationDir[3] = {false,false,false};
bool press[3] = {false,false,false};
bool longPress[3] = {false,false,false};
bool triggerButtonAction = false;


bool wasRunning=false;
AlarmID_t alarmProgramID;
//bool connected = false;
//auto tick = millis();
void loop(){  //SERIAL_PRINTLN("void loop()");

  /*
  if (bluetooth.available() && !connected){
    connected = true;
    gui->run();
  } else if(!bluetooth.available() && 
  */
 /*
  if((millis()-tick)>500){
    tick = millis();
    gui->run();
  }*/

  /*
  for(int i=0; i<3; i++){
    if (versatile_encoder[i].ReadEncoder()){// Do the encoder reading and processing
      //if(press[0]) gui->setup();
      if(rotation[0]) gui->nextState(rotationDir[0]);

      //if(press[1]) riego.selections[1] = gui->selection();
      //if(rotation[1]) gui->shiftField(rotationDir[1]);

      if(longPress[i]) riego.longPress(i);
      if(press[i]) riego.press(i);
      if(rotation[i]) riego.rotation(i,rotationDir[i]);

      longPress[i]=false;
      press[i]=false;
      rotation[i]=false;
    }
  }
  */
  gui->run();

  if(riego.changedProgramTime()){
    Alarm.free(alarmID);
    alarmID = Alarm.alarmRepeat(riego.getProgramTime().hour, riego.getProgramTime().minute, riego.getProgramTime().second, alarmRiego);
  }

  if(riego._running && !wasRunning){
    wasRunning=true;
    alarmProgramID = Alarm.timerRepeat(riego.getProgramTime().delay,alarmProgram);
  }

  if(!riego._running && wasRunning){
    wasRunning=false;
    Alarm.free(alarmProgramID);
  }

  Alarm.delay(5);
  saveEEPROM();
}

void handleRotate(int handleNumber, int8_t rotationDirection){
  rotation[handleNumber] = true;
  if (rotation[handleNumber] > 0)
    rotationDir[handleNumber]=false;
  else
    rotationDir[handleNumber]=false;
  //riego.rotation(handleNumber, rotationDir[handleNumber]);
}
void handlePress(int handleNumber){
  //riego.press(handleNumber);
  press[handleNumber]=true;
}
void handleLongPress(int handleNumber){
  //riego.longPress(handleNumber);
  longPress[handleNumber]=true;
}

// Implement your functions here accordingly to your needs
void handleRotate1(int8_t rotationDirection) {
  //SERIAL_PRINTLN("handleRotate3()");
  handleRotate(0, rotationDirection);
}
void handlePress1() {
  //SERIAL_PRINTLN("handlePress2()");
  handlePress(0);
}
void handleLongPress1() {
  //SERIAL_PRINTLN("handleLongPress2()");
  handleLongPress(0);
}
void handleRotate2(int8_t rotationDirection) {
  //SERIAL_PRINTLN("handleRotate3()");
  handleRotate(1, rotationDirection);
}
void handlePress2() {
  //SERIAL_PRINTLN("handlePress2()");
  handlePress(1);
}
void handleLongPress2() {
  //SERIAL_PRINTLN("handleLongPress2()");
  handleLongPress(1);
}
void handleRotate3(int8_t rotationDirection) {
  //SERIAL_PRINTLN("handleRotate3()");
  handleRotate(2, rotationDirection);
}
void handlePress3() {
  //SERIAL_PRINTLN("handlePress3()");
  handlePress(2);  
}
void handleLongPress3() {
  //SERIAL_PRINTLN("handleLongPress()");
  handleLongPress(2);
}