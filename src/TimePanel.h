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

  /*
    const String whiteAddTextBoxH = "add_text_box(1,4,6,C,24,245,240,245,h)";
    const String yellowAddTextBoxH = "add_text_box(1,4,6,C,24,245,240,100,h)";
    const String greenAddTextBoxH = "add_text_box(1,4,6,C,24,50,240,50,h)";

    const String whiteAddTextBoxB = "add_text_box(9,4,6,C,24,color D)";
    const String yellowAddTextBoxB = "add_text_box(9,4,6,C,24,245,240,100,D)";
    const String greenAddTextBoxB = "add_text_box(9,4,6,C,24,50,240,50,D)";
    */

    /*
    const char *yellowTexta = "245,240,245,";

    const char whiteText[13] = "245,240,245,";
    const char yellowText[13] = "245,240,100,";
    const char greenText[13] = "50,240,50,";

    const char whiteAddTextBoxH[39] = "add_text_box(1,4,6,C,24,245,240,245,h)";
    const char yellowAddTextBoxH[39] = "add_text_box(1,4,6,C,24,245,240,100,h)";
    const char greenAddTextBoxH[39] = "add_text_box(1,4,6,C,24,50,240,50,h)";

    const char whiteAddTextBoxB[39] = "add_text_box(9,4,6,C,24,color D)";
    const char yellowAddTextBoxB[39] = "add_text_box(9,4,6,C,24,245,240,100,D)";
    const char greenAddTextBoxB[39] = "add_text_box(9,4,6,C,24,50,240,50,D)";
    */
};

#endif
