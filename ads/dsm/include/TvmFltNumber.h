// MODULE:  TvmFltNumber.h
// PURPOSE: Class Definition and needed includes and defines
// DATE:    3/28/95
// AUTHOR:  Reif Heck 

#ifndef TVMFLTNUMBER_H
#define TVMFLTNUMBER_H

#include "mv162.h" /* mvme162 includes */
#include <vxWorks.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdioLib.h>
 
#include <tvmDefs.h>

#include <TvmDisplay.h>
 
// DEFINE KEYCODES FOR FLIGHT NUMBER DISPLAY

#define ZERO 	      0
#define ONE	      1
#define TWO	      2
#define THREE	      3
#define FOUR	      4
#define FIVE	      5
#define SIX	      6
#define SEVEN	      7
#define EIGHT	      8
#define NINE	      9
#define R	      52
#define T	      54
#define FLIGHT_NUMBER 150
#define ENTER         180
#define CLEAR         185
 
class TvmFltNumber
{
  public:
    TvmFltNumber(TvmDisplay& td);
    void initDisplay();
    void clearDisplay();
    int processKeys();
    void displayFlightNumber();

    inline char *flightNumber()	{return flt_num;}

  private:
    TvmDisplay &tvm_display;
    char flt_num[4];
    int count;
    int display_active;
};

#endif // TVMFLTNUMBER_H
