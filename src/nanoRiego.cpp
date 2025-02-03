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

  //SERIAL_FLUSH();
  gui = new Gui(&riego, &bluetooth);
  riego.gui(gui);

  loadEEPROM();
  alarmID = Alarm.alarmRepeat(riego.getProgramTime().hour, riego.getProgramTime().minute, riego.getProgramTime().second, alarmRiego);
  Alarm.delay(10);
}

bool wasRunning=false;
AlarmID_t alarmProgramID;

void loop(){  
  //SERIAL_PRINTLN("void loop()");

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