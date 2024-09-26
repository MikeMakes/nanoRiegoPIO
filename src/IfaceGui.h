#ifndef IFACEGUI_H
#define IFACEGUI_H

#include "Constants.h"
#include "IfaceRiego.h"

struct msgManualControl{
  const static char id='M';
  char payload[2];
};

class IfaceGui{
  public:
    enum STATES { FRONTPANEL, TIMEPANEL, PROGRAMPANEL};
    
    IfaceGui(IfaceRiego* const riego):
    _riego(riego)
    {}

    virtual void nextState(bool next);
    virtual void run();
    virtual void setState(STATES state);

    virtual void loop();

    struct comms{
      char id;
      uint8_t payloadSize;
      char *payload;
    };

    comms cmdManualControl{.id='M',.payloadSize=sizeof("v0")};


  public:
    IfaceRiego* _riego;
};

#endif
