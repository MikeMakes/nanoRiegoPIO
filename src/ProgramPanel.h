#ifndef PROGRAMPANEL_H
#define PROGRAMPANEL_H

#include "Panel.h"

class ProgramPanel : public Panel {
  public:
    ProgramPanel(SoftwareSerial* serial);
    //ProgramPanel(char commandID, char panelID, SoftwareSerial* serial);
    ~ProgramPanel();

    void printProgramTimeSwitches();
    void reloadDynamicBody();
    void body();
    void loop();
    void update(IfaceRiego* const riego, IfaceGui* const gui);
    void printProgramTimeBox();

    String text; // String for text elements
    const char add15min = 'n';
    const char substract15min = 'p';
    char turnOn[8] = {'L','M','X','J','V','S','D','A'};
    char turnOff[8] = {'l','m','x','j','v','s','d','a'};
    const char switchL[23] = {'a', 'd', 'd', '_', 's', 'w', 'i', 't', 'c', 'h', '(', '8', ',', '4', ',', '3', ',', 'L', ',', 'l', ',', '0', ','};
    const char switchM[23] = {'a', 'd', 'd', '_', 's', 'w', 'i', 't', 'c', 'h', '(', '9', ',', '4', ',', '3', ',', 'M', ',', 'm', ',', '0', ','};
    const char switchX[24] = {'a', 'd', 'd', '_', 's', 'w', 'i', 't', 'c', 'h', '(', '1', '0', ',', '4', ',', '3', ',', 'X', ',', 'x', ',', '0', ','};
    const char switchJ[24] = {'a', 'd', 'd', '_', 's', 'w', 'i', 't', 'c', 'h', '(', '1', '1', ',', '4', ',', '3', ',', 'J', ',', 'j', ',', '0', ','};
    const char switchV[24] = {'a', 'd', 'd', '_', 's', 'w', 'i', 't', 'c', 'h', '(', '1', '2', ',', '4', ',', '3', ',', 'V', ',', 'v', ',', '0', ','};
    const char switchS[24] = {'a', 'd', 'd', '_', 's', 'w', 'i', 't', 'c', 'h', '(', '1', '3', ',', '4', ',', '3', ',', 'S', ',', 's', ',', '0', ','};
    const char switchD[24] = {'a', 'd', 'd', '_', 's', 'w', 'i', 't', 'c', 'h', '(', '1', '4', ',', '4', ',', '3', ',', 'D', ',', 'd', ',', '0', ','};
    const char switchA[23] = {'a', 'd', 'd', '_', 's', 'w', 'i', 't', 'c', 'h', '(', '3', ',', '3', ',', '4', ',', 'A', ',', 'a', ',', '0', ','};
    const char *switches[8] = {switchL,switchM,switchX,switchJ,switchV,switchS,switchD,switchA};

    unsigned int selectedDay = 0;
    void selectDay(unsigned int day=0);
    void previousDay();
    void nextDay();
    void shiftDay(bool next);
    void shiftField(bool next);
    unsigned int getField();

    void shiftHour(bool add1);
    bool changingProgramTime=false;

    programTime *_nextProgramTimePtr = new programTime{ .hour = ALARM_HOUR, .minute = ALARM_MINUTE, .second = ALARM_SECOND, .programDays = {true,true,true,true,true,true,true}, .programEnabled = true };
    //programTime *_nextProgramTimePtr;// = new programTime{ .hour = 13, .minute = 0, .second = 0, .programDays = {false,false,false,false,false,false,false}, .programEnabled = false };
};

#endif
