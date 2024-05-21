#ifndef PROGRAMPANEL_H
#define PROGRAMPANEL_H

#include "Panel.h"

class ProgramPanel : public Panel {
  public:
    ProgramPanel(SoftwareSerial* serial);
    //ProgramPanel(char commandID, char panelID, SoftwareSerial* serial);
    ~ProgramPanel();

    void body();
    void loop();
    void update(IfaceRiego* const riego, IfaceGui* const gui);

    String text; // String for text elements
    const String week[7] = {"*L","*M","*X","*J","*V","*S","*D"};

    unsigned int selectedDay = 0;
    void selectDay(unsigned int day=0);
    void previousDay();
    void nextDay();
    void shiftDay(bool next);
    void shiftField(bool next);
    unsigned int getField();

    void shiftHour(bool add1);
    bool changingProgramTime=false;

    programTime _nextProgramTime;

};

#endif
