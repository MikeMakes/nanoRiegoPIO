#ifndef TIMEPANEL_H
#define TIMEPANEL_H

#include "Panel.h"

class TimePanel : public Panel {
  public:
    TimePanel(IfaceGui* gui, IfaceRiego* riego, SoftwareSerial* serial);
    //TimePanel(char commandID, char panelID, SoftwareSerial* serial);
    ~TimePanel();

    void body();
    void loop(IfaceRiego* riego, IfaceGui* gui);

    void shiftField(bool next);
    unsigned int getField();

    String text; // String for text elements

    systemTime actualTime;
    bool changingSystemTime = false;
    const String whiteText = "245,240,245,";
    const String yellowText = "245,240,100,";
    const String greenText = "50,240,50,";
};

#endif
