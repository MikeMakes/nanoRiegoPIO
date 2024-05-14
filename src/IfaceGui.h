#ifndef IFACEGUI_H
#define IFACEGUI_H

#include "Constants.h"
#include "IfaceRiego.h"

class IfaceGui{
  public:
    enum STATES { FRONTPANEL, TIMEPANEL, PROGRAMPANEL};
    
    IfaceGui(IfaceRiego* riego):
    _riego(riego)
    {}

    virtual void run();
    virtual void setState(STATES state);

  public:
    IfaceRiego* _riego;
};

#endif
