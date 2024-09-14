#ifndef CONSTANTS_H
#define CONSTANTS_H

#ifdef DEBUG_BUILD
    #define SERIAL_BEGIN(x) do {} while (0)
    #define SERIAL_PRINT(x) do {} while (0)
    #define SERIAL_PRINTLN(x) do {} while (0)
    #define SERIAL_FLUSH(x) do {} while(0)
#else
    #define SERIAL_BEGIN(x) Serial.begin(x)
    #define SERIAL_PRINT(x) Serial.print(x)
    #define SERIAL_PRINTLN(x) Serial.println(x)
    #define SERIAL_FLUSH(x)  Serial.flush(x)
#endif

#define numPumps 1
#define pinPump 17//A3

const static int numValves = 3;
const static int pinValve1 = 14;//A0
const static int pinValve2 = 15;//A1
const static int pinValve3 = 16;//A2

const static int PIN_SELECTOR_A1 = 12;
const static int PIN_SELECTOR_B1 = 11;
const static int PIN_BUTTON_1 = 10;

const static int PIN_SELECTOR_A2 = 9;
const static int PIN_SELECTOR_B2 = 8;
const static int PIN_BUTTON_2 = 7;

const static int PIN_SELECTOR_A3 = 6;
const static int PIN_SELECTOR_B3 = 5;
const static int PIN_BUTTON_3 = 4;

const static unsigned int PIN_I2C_SDA = 18;
const static unsigned int PIN_I2C_SCK = 19;

const static unsigned int PIN_BLUETOOTH_RX = 2;
const static unsigned int PIN_BLUETOOTH_TX = 3;

#define EEPROM_timeAddress 0
const static unsigned long DEFAULT_TIME = (unsigned long)1716991560+(2*3600);//GMT+2
const static unsigned int ALARM_HOUR = (unsigned int)07;
const static unsigned int ALARM_MINUTE = (unsigned int)00;
const static unsigned int ALARM_SECOND = (unsigned int)00;
const static unsigned int PROGRAM_DELAY = (unsigned int)15*60; //seconds

const static unsigned int GUI_UPDATE_INTERVAL = (unsigned int) 500; //ms

#endif
