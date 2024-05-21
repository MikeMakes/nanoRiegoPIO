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
  EEPROM.put(EEPROM_timeAddress, DEFAULT_TIME);
}

SoftwareSerial bluetooth(PIN_BLUETOOTH_RX, PIN_BLUETOOTH_TX); //RX,TX
Relay pump(pinPump, true);
Relay valves[]={Relay(pinValve1, true), Relay(pinValve2, true), Relay(pinValve3, true)};
//Riego* riego = new Riego(&pump, valves);
Riego riego(&pump, valves);
Gui* gui;
//Gui gui(&riego, &bluetooth);

void alarmRiego(){
  //SERIAL_PRINTLN("void alarmRiego()");
  riego.check();
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
  #endif

  SERIAL_BEGIN(115200);
  SERIAL_PRINTLN("setup()");
  bluetooth.begin(9600);
  //bluetooth.println(1);
  //bluetooth.print("adiosadiosadios");


  setEEPROM();
  time_t startTime;
  EEPROM.get(EEPROM_timeAddress,startTime);

  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  //RTC.set((time_t)DEFAULT_TIME);
  if(timeStatus()!= timeSet){ //tbd: || RTC.get()>startTime; 
     //SERIAL_PRINTLN("Unable to sync with the RTC");
     SERIAL_PRINTLN("RTC'nt");
     RTC.set(startTime);
     setTime(startTime);
  }
  else{
     SERIAL_PRINTLN("RTC has set the system time");   
     //SERIAL_PRINTLN(now()); 
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

  riego.setProgramTime(ALARM_HOUR-1, ALARM_MINUTE, ALARM_SECOND);
  Alarm.alarmRepeat(ALARM_HOUR-1, ALARM_MINUTE, ALARM_SECOND, alarmRiego);
  Alarm.delay(10);
  //SERIAL_PRINTLN("finish setup");
}

bool rotation[3] = {false,false,false};
bool rotationDir[3] = {false,false,false};
bool press[3] = {false,false,false};
bool longPress[3] = {false,false,false};
bool triggerButtonAction = false;

void loop(){
  SERIAL_PRINTLN("void loop()");
  bluetooth.println("void loop()");
  //debug_message("loop");

  static time_t buttonPressedAt = now();

  for(int i=0; i<3; i++){
    if (versatile_encoder[i].ReadEncoder()){// Do the encoder reading and processing
      // Do something here whenever an encoder action is read
      SERIAL_PRINT("R");
      if(triggerButtonAction == false){
        //if(now()-buttonPressedAt>200){
          triggerButtonAction = true;
          buttonPressedAt = now();
          //riego->buttonPressedAt(now());
        //}
      }
    }
        
  }

  if(triggerButtonAction){
    //SERIAL_PRINTLN("triggerButtonAction");
    //Aquí antes que nada deberías checkear si quieres hacer algo
    //con 2 o más botones a la vez
    for(int i=0; i<3; i++){
      if(press[0]) gui->setup();
      if(rotation[0]) gui->nextState(rotationDir[0]);

      if(press[1]) riego.selections[1] = gui->selection();
      if(rotation[1]) gui->shiftField(rotationDir[1]);

      if(longPress[i]) riego.longPress(i);
      if(press[i]) riego.press(i);
      if(rotation[i]) riego.rotation(i,rotationDir[i]);

      longPress[i]=false;
      press[i]=false;
      rotation[i]=false;
    }
    triggerButtonAction=false;
  }

  gui->run();
  Alarm.delay(10);
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