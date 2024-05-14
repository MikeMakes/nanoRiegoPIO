#ifndef PROGRAMPANEL_H
#define PROGRAMPANEL_H

#include "Panel.h"

class ProgramPanel : public Panel {
  public:
    ProgramPanel(IfaceGui* gui, IfaceRiego* riego, SoftwareSerial* serial);
    //ProgramPanel(char commandID, char panelID, SoftwareSerial* serial);
    ~ProgramPanel();

    void body();
    void loop(IfaceRiego* riego, IfaceGui* gui);

    String text; // String for text elements
    const String week[7] = {"*L","*M","*X","*J","*V","*S","*D"};

    unsigned int selectedDay = 0;
    void selectDay(unsigned int day=0);
    void previousDay();
    void nextDay();
    void shiftDay(bool next);
    void shiftField(bool next);
    unsigned int getField();

};

#endif
