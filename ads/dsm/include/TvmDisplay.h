// MODULE:  TvmDisplay.h
// PURPOSE: Csys Labs TVM24128 Status Display Class
// DATE:    3/15/95
// AUTHOR:  REIF HECK

#ifndef TVMDISPLAY_H
#define TVMDISPLAY_H

#include <string.h>
#include <vxWorks.h>
#include <stdlib.h>
#include <taskLib.h>

#include <SccClass.h>

/*********** KEYPRESS CODE DEFINES ***********/

/**************** SERIAL CHANNEL DEFINES ***************/

#define DISPLAY_BAUD      9600 
#define DISPLAY_BITS      8 // 8 Bits/Char
#define DISPLAY_PARITY    0 // No Parity
#define DISPLAY_STOP      1 // One Stop Bit


class TvmDisplay : public SccClass 
{
  public:
    TvmDisplay(); /* Constructor */
    int chkKeypress();
    int getKeycode();
    void clearDisplay(); 
    void backLightOn();
    void backLightOff();
    void revVideoOn();
    void revVideoOff();
    void beepOn();
    void beepOff();
    void contrastUp();
    void contrastDown();

  private:
    void setContrast();

    int contrast_level;
    int blStatus;
    int rvStatus;
    int bpStatus;
    char read_buffer[256];
};

#endif // TVMDISPLAY_H 
