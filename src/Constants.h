#ifndef CONSTANTS_H
#define CONSTANTS_H

#ifdef DEBUG_BUILD // avr debugger conflicts with SoftwareSerial
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


const static unsigned int numPumps = 1;
const static unsigned int pinPump = 17;//A3

const static int numValves = 3;
const static int pinValve1 = 14;//A0
const static int pinValve2 = 15;//A1
const static int pinValve3 = 16;//A2

const static unsigned int PIN_I2C_SDA = 18;
const static unsigned int PIN_I2C_SCK = 19;

const static unsigned int PIN_BLUETOOTH_RX = 2;
const static unsigned int PIN_BLUETOOTH_TX = 3;

const static unsigned int EEPROM_timeAddress = 0;

const static unsigned long DEFAULT_TIME = (unsigned long)1716991560+(2*3600);//GMT+2
const static unsigned int ALARM_HOUR = (unsigned int)8;
const static unsigned int ALARM_MINUTE = (unsigned int)0;
const static unsigned int ALARM_SECOND = (unsigned int)0;
const static unsigned int PROGRAM_DELAY = (unsigned int)15*60; //seconds

const static unsigned int GUI_UPDATE_INTERVAL = (unsigned int) 400; //ms
const static unsigned int GUI_UPDATE_INTERVAL_FORCED = (unsigned int) 5000; //ms

const static unsigned char MESSAGE_PRELIMITER = '*';
const static unsigned int MAX_MESSAGE_CMD = 10;

#endif
