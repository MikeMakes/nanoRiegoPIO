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

//#include "Relay.h"
//#include "IfaceRiego.h"
#include "Riego.h"
#include "Gui.h"

void setEEPROM(){
  EEPROM.put(EEPROM_timeAddress, DEFAULT_TIME);
}

SoftwareSerial bluetooth(PIN_BLUETOOTH_RX, PIN_BLUETOOTH_TX); //RX,TX
Relay pump(pinPump, true);
Relay valves[]={Relay(pinValve1, true), Relay(pinValve2, true), Relay(pinValve3, true)};
Riego* riego = new Riego(&pump, valves);
Gui* gui;

#include <Versatile_RotaryEncoder.h>
Versatile_RotaryEncoder versatile_encoder[3]={Versatile_RotaryEncoder(PIN_SELECTOR_B1, PIN_SELECTOR_A1, PIN_BUTTON_1), Versatile_RotaryEncoder(PIN_SELECTOR_B2, PIN_SELECTOR_A2, PIN_BUTTON_2), Versatile_RotaryEncoder(PIN_SELECTOR_B3, PIN_SELECTOR_A3, PIN_BUTTON_3)};

// Functions prototyping to be handled on each Encoder Event
void handleRotate1(int8_t rotation);
void handlePress1();
void handleLongPress1();
void handleRotate2(int8_t rotation);
void handlePress2();
void handleLongPress2();
void handleRotate3(int8_t rotation);
void handlePress3();
void handleLongPress3();

void alarmRiego(){
  SERIAL_PRINTLN("void alarmRiego()");
  riego->checkAlarm();
}

void setup() {
  #ifdef DEBUG_BUILD 
  debug_init(); //debug only
  #endif

  SERIAL_BEGIN(115200);
  SERIAL_PRINTLN("setup()");
  bluetooth.begin(9600);

  setEEPROM();
  time_t startTime;
  EEPROM.get(EEPROM_timeAddress,startTime);

  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  //RTC.set((time_t)DEFAULT_TIME);
  if(timeStatus()!= timeSet){ //tbd: || RTC.get()>startTime; 
     SERIAL_PRINTLN("Unable to sync with the RTC");
     RTC.set(startTime);
     setTime(startTime);
  }
  else{
     SERIAL_PRINTLN("RTC has set the system time");   
     SERIAL_PRINTLN(now()); 
  }

  SERIAL_PRINTLN("brkp1");

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

  SERIAL_PRINTLN("brkp2");

  SERIAL_FLUSH();
  SERIAL_PRINTLN("brkp3");
  gui = new Gui(riego, &bluetooth);

  SERIAL_PRINTLN("brkp4");

  riego->setProgramTime(ALARM_HOUR-1, ALARM_MINUTE, ALARM_SECOND);
  Alarm.alarmRepeat(ALARM_HOUR-1, ALARM_MINUTE, ALARM_SECOND, alarmRiego);

  SERIAL_PRINTLN("finish setup");
}

bool changeState = false;
bool directionState = true;
bool press = false;
bool longPress[3] = {false,false,false};

bool changeField = false;
bool directionField = true;
bool toggleField, toggleValue;
//bool changeValue = false;
//bool directionValue = true;

void loop(){
  SERIAL_PRINTLN("void loop()");
  bluetooth.println("void loop()");
  //debug_message("loop");

  for(int i=0; i<3; i++){
    if (versatile_encoder[i].ReadEncoder()) {    // Do the encoder reading and processing
        //SERIAL_PRINTLN("ReadEncoder");
        // Do something here whenever an encoder action is read
        if(press){
          press=false;
          gui->setup();
        }

        if(changeState){
          changeState = false;
          gui->nextState(directionState);
        }

        if(longPress[i]){
          longPress[i] = false;
          riego->toggleValve(i);
        }

        if(changeField){
          changeField=false;
          gui->shiftField(directionField);
        }

        if(toggleField){
          toggleField=false;
          unsigned int selection = gui->selection();
          if(selection==7) riego->toggleProgramEnabled();
          else riego->toggleProgramDays(selection);
        }
        
        /*
        if(changeValue){ // Mostrar cambio de time
          changeValue=false;
          gui->shiftValue(directionValue);
        }
        */
        if(toggleValue){ // Guardar cambio de time mostrado
          toggleValue=false;
          systemTime t = riego->getSystemTime();
          t.hour = t.hour + gui->selection();
          riego->setSystemTime(t);
        }
    }
  }
  
  gui->run();
  Alarm.delay(1);
}

// Implement your functions here accordingly to your needs
void handleRotate1(int8_t rotation) {
  changeState = true;
  if (rotation > 0)
    directionState=false;
  else
    directionState=true;
}
void handlePress1() {
  press=true;
}
void handleLongPress1() {
  longPress[0]=true;
}

void handleRotate2(int8_t rotation) {
  changeField = true;
  if (rotation > 0)
    directionField=true;
  else
    directionField=false;
}
void handlePress2() {
  toggleField = true;
}
void handleLongPress2() {
  longPress[1]=true;
}

void handleRotate3(int8_t rotation) {
  /*
    changeValue = true;
    if (rotation > 0)
      directionValue=true;
    else
      directionValue=false;
      */
}
void handlePress3() {
  SERIAL_PRINTLN("handlePress3()");
  bluetooth.println("handlePress3");
  toggleValue = true;
}
void handleLongPress3() {
  //SERIAL_PRINTLN("handlePress()");
  //bluetooth.println("handlePress");
  longPress[2]=true;
}