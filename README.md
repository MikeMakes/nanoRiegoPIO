# Arduino Nano Watering Control System

Made for fun and to be cheap, a DIY resource wasteful watering control system. Based on Arduino Nano and MIT App Inventor.

# Features
* Automatic watering schedule using DS1307 RTC
* Control of 1 water pump and 3 valves using 4 relays
* Bluetooth communication with mobile app using HC-05 module

# Hardware Requirements
* Arduino Nano
* DS1307 RTC module
* HC-05 Bluetooth module
* 4 relays (1 for pump, 3 for valves)
* Water pump and valves

# Software Requirements to build
* Platform IO
* MIT App Inventor for mobile app development

# Notes
For some reason I did not bother to spend too much time into ([more info here](https://community.platformio.org/t/project-inspection-error-register-r24-r26-r28-or-r30-required/18648/2)), if headers files are placed in the include folder compiling in debug mode returns:
* Error: register r24, r26, r28 or r30 required
To evade it, either place all files in src folder or modify SoftwareSerial library for debug enviroments as follows.
* In files ".pio/libdeps/nanoatmega328/SoftwareSerial/SoftwareSerial.cpp" and ".pio/libdeps/nanoatmega328new/SoftwareSerial/SoftwareSerial.cpp"
* Change the following (line 329):
> /* static */  
> inline void SoftwareSerial::tunedDelay(uint16_t delay) {  
>   uint8_t tmp=0;  
>   
>   asm volatile("sbiw    %0, 0x01 \n\t"  
>     "ldi %1, 0xFF \n\t"  
>     "cpi %A0, 0xFF \n\t"  
>     "cpc %B0, %1 \n\t"  
>     "brne .-10 \n\t"  
>     : "+r" (delay), "+a" (tmp)  
>     : "0" (delay)  
>     );  
> }  
* For this:
> /* static */   
> inline void SoftwareSerial::tunedDelay(uint16_t delay) {   
>   uint8_t tmp=0;  
>   
>   asm volatile("sbiw    %0, 0x01 \n\t"  
>     "ldi %1, 0xFF \n\t"  
>     "cpi %A0, 0xFF \n\t"  
>     "cpc %B0, %1 \n\t"  
>     "brne .-10 \n\t"  
>     : "+w" (delay), "+a" (tmp)  
>     : "0" (delay)  
>     );  
> }  

# License
NanoRiegoPIO is released under the MIT License.
