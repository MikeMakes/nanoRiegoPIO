#ifndef PANEL_H
#define PANEL_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Constants.h"

#include "IfaceRiego.h"
#include "IfaceGui.h"

class Panel{
  public:
    Panel(IfaceGui* const gui = nullptr,IfaceRiego* const riego = nullptr, SoftwareSerial* const serial = nullptr):
    _gui(gui),
    _riego(riego),
    bluetooth(serial){

    }

    //char serialID[2];
    //char commandIn;
    char data_in;

    int update_interval=1000; // time interval in ms for updating panel indicators 
    unsigned long last_time=0; // time of last update

    /*Panel(char commandID, char panelID, SoftwareSerial* serial):bluetooth(serial){
      serialID[0]=commandID;
      serialID[1]=panelID;
    }*/

    SoftwareSerial* bluetooth;

    void setSerial(SoftwareSerial* const serial){
      bluetooth = serial;
    }

    void header(){
      bluetooth->println(F("*.kwl"));
      bluetooth->println(F("clear_panel()"));
      bluetooth->println(F("set_grid_size(16,8)"));
      bluetooth->println(F("add_text_box(4,1,8,C,Riego,245,240,245,S)"));
    }

    virtual void body();

    void foot(){
      bluetooth->println(F("set_panel_notes(,,,)"));
      bluetooth->println(F("run()"));
      bluetooth->println(F("*"));
    }

    void setup(){
      header();
      body();
      foot();
    }

    virtual void loop(IfaceRiego* const riego, IfaceGui* const gui);

    virtual void shiftField(bool next);
    virtual unsigned int getField();

  private:
    IfaceGui* _gui;
    IfaceRiego* _riego;
};

#endif
