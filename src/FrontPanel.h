#ifndef FRONTPANEL_H
#define FRONTPANEL_H

#include "Panel.h"

struct FrontPanelState{
  bool valves[3];
};

class FrontPanel : public Panel {
  public:
    FrontPanel(SoftwareSerial* serial);
    //FrontPanel(char commandID, char panelID, SoftwareSerial* serial);
    ~FrontPanel();

    void body();
    void loop();
    void update(IfaceRiego* const riego, IfaceGui* const gui);

    String text; // String for text elements
    int red,green,blue; // RGB color

    bool valveLeds[3] = {false, false, false};

    void shiftField(bool next);
    unsigned int getField();
};

#endif
