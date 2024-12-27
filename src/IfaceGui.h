#ifndef IFACEGUI_H
#define IFACEGUI_H

#include "Constants.h"
#include "IfaceRiego.h"

struct msgGui{
  const static char id='*';
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

    comms cmdSystemTime{.id='T',.payloadSize=sizeof(int)*2};
    comms cmdSystemDate{.id='D',.payloadSize=sizeof(int)*3};

    comms cmdAutoEnable{.id='e',.payloadSize=sizeof(int)};
    comms cmdAutoRun{.id='r',.payloadSize=0};
    comms cmdAutoTime{.id='h',.payloadSize=sizeof(int)*2};
    //comms cmdAutoRepetition{.id='r',.payloadSize=0};
    comms cmdAutoDuration{.id='D',.payloadSize=sizeof(int)};

    comms cmdManualValve{.id='v',.payloadSize=sizeof(int)};
    //comms cmdManualDuration{.id='m',.payloadSize=sizeof(int)};

  public:
    IfaceRiego* _riego;
};

#endif
