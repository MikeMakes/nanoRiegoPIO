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

    //void setup();
    void update();
    
    void run() override;
    void setState(STATES state) override;
    void nextState(bool right = true) override;
    void loop() override;

    // Function prototypes for action handlers
    void handleCmdSystemTime(const IfaceGui::Message *msg) override;
    void handleCmdSystemDate(const IfaceGui::Message *msg) override;

    void handleCmdAutoEnable(const IfaceGui::Message *msg) override;
    void handleCmdAutoRun(const IfaceGui::Message *msg) override;
    void handleCmdAutoTime(const IfaceGui::Message *msg) override;
    //void handleCmdAutoRepetition(const IfaceGui::Message *msg) override;
    void handleCmdAutoDays(const IfaceGui::Message *msg) override;
    void handleCmdAutoDuration(const IfaceGui::Message *msg) override;

    void handleCmdManualValve(const IfaceGui::Message *msg) override;
    void handleCmdManualDuration(const IfaceGui::Message *msg) override;

    void handleCmdUnknown(const IfaceGui::Message *msg) override;
    void handleCmds() override;

    Message findHandler(char cmdID);
    Message msg;

  private:
    unsigned long last_time=0; // time of last update
    unsigned long update_interval=GUI_UPDATE_INTERVAL; // time interval in ms for updating panel indicators 
  
    bool _update = false;

    SoftwareSerial* _bluetooth = nullptr;
};

#endif
