#ifndef GUI_H
#define GUI_H

#include "IfaceRiego.h"
#include "IfaceGui.h"

#include "Panel.h"

#include "FrontPanel.h"
#include "TimePanel.h"
#include "ProgramPanel.h"

class Gui:public IfaceGui{
  public:    
    Gui(IfaceRiego* const riego, SoftwareSerial* const serial);

    void run();
    void setState(STATES state);
    void nextState(bool right = true);
    void setup();
    void shiftField(bool directionField = true);
    //void shiftValue(bool directionValue = true);
    unsigned int selection();

  private:
    Gui::STATES _state = STATES::FRONTPANEL;
    bool _update = false;

    SoftwareSerial* _bluetooth = nullptr;

    Panel* _panels[3] = {new FrontPanel(_bluetooth), new TimePanel(_bluetooth), new ProgramPanel(_bluetooth)};
    //Panel* _panels[3] = {new FrontPanel('*','F',_bluetooth), new TimePanel('*','T',_bluetooth), new ProgramPanel('*','P',_bluetooth)};
    //Panel* _panels[3] = {new FrontPanel('*','F',_bluetooth), new FrontPanel('*','F',_bluetooth), new FrontPanel('*','F',_bluetooth)};
};

#endif
