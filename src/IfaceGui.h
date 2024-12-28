#ifndef IFACEGUI_H
#define IFACEGUI_H

#include "Constants.h"
#include "IfaceRiego.h"

class IfaceGui{
  public:
    enum STATES { FRONTPANEL, TIMEPANEL, PROGRAMPANEL};
    
    IfaceGui(IfaceRiego* const riego):
      _riego(riego)
    {}
    virtual ~IfaceGui() {}

    virtual void nextState(bool next) = 0;
    virtual void run() = 0;
    virtual void setState(STATES state) = 0;

    virtual void loop() = 0;

    typedef struct Message{
      char id;
      void (IfaceGui::*handler)(const Message *msg);
      uint8_t payloadSize;
      char *payload;
    } Message;

    // Handler lookup table
    typedef void (IfaceGui::*MessageCmdHandler)(const IfaceGui::Message *msg);
    //MessageCmdHandler MessageCmdHandlers[MAX_MESSAGE_CMD];

    // Function prototypes for action handlers
    virtual void handleCmdSystemTime(const IfaceGui::Message *msg) = 0;
    virtual void handleCmdSystemDate(const IfaceGui::Message *msg) = 0;

    virtual void handleCmdAutoEnable(const IfaceGui::Message *msg) = 0;
    virtual void handleCmdAutoRun(const IfaceGui::Message *msg) = 0;
    virtual void handleCmdAutoTime(const IfaceGui::Message *msg) = 0;
    virtual void handleCmdAutoRepetition(const IfaceGui::Message *msg) = 0;
    virtual void handleCmdAutoDuration(const IfaceGui::Message *msg) = 0;

    virtual void handleCmdManualValve(const IfaceGui::Message *msg) = 0;
    virtual void handleCmdManualDuration(const IfaceGui::Message *msg) = 0;

    virtual void handleCmdUnknown(const IfaceGui::Message *msg) = 0;
    virtual void handleCmds() = 0;

    // All commands
    Message cmdSystemTime{.id='T', .handler=&IfaceGui::handleCmdSystemTime, .payloadSize=sizeof(int)*2};
    Message cmdSystemDate{.id='D', .handler=&IfaceGui::handleCmdSystemDate, .payloadSize=sizeof(int)*3};

    Message cmdAutoEnable{.id='e', .handler=&IfaceGui::handleCmdAutoEnable, .payloadSize=sizeof(int)};
    Message cmdAutoRun{.id='r', .handler=&IfaceGui::handleCmdAutoRun, .payloadSize=0};
    Message cmdAutoTime{.id='h', .handler=&IfaceGui::handleCmdAutoTime, .payloadSize=sizeof(int)*2};
    //Message cmdAutoRepetition{.id='r', .handler=&IfaceGui::handleCmdAutoRepetition, .payloadSize=0};
    Message cmdAutoDuration{.id='D', .handler=&IfaceGui::handleCmdAutoDuration, .payloadSize=sizeof(int)};

    Message cmdManualValve{.id='v', .handler=&IfaceGui::handleCmdManualValve, .payloadSize=sizeof(char)};
    //Message cmdManualDuration{.id='m', .handler=&IfaceGui::handleCmdManualDuration, .payloadSize=sizeof(int)};

    Message cmdUnknown{.id='\0', .handler=&IfaceGui::handleCmdUnknown, .payloadSize=0};

    Message cmdMessages[8]={
      cmdSystemTime,
      cmdSystemDate,
      cmdAutoEnable,
      cmdAutoRun,
      cmdAutoTime,
      cmdAutoDuration,
      cmdManualValve,
      cmdUnknown
    };

  public:
    IfaceRiego* _riego;
};

#endif