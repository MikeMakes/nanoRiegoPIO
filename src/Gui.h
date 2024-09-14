#ifndef GUI_H
#define GUI_H

#include <SoftwareSerial.h>

#include "IfaceRiego.h"
#include "IfaceGui.h"

//#include "Panel.h"

//#include "FrontPanel.h"
//#include "TimePanel.h"
//#include "ProgramPanel.h"

class Gui:public IfaceGui{
  public:    
    Gui(IfaceRiego* const riego, SoftwareSerial* const serial);

    void run();
    void setState(STATES state);
    void nextState(bool right = true);
    void setup();

    void update();
    void loop();

  private:
    unsigned long last_time=0; // time of last update
    unsigned long update_interval=GUI_UPDATE_INTERVAL; // time interval in ms for updating panel indicators 
  
    bool _update = false;

    SoftwareSerial* _bluetooth = nullptr;
};

#endif
