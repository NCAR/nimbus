/* FlightWin.h
   Provides a popup panel for the flight number entry.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <Xm/SelectioB.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/Text.h>

#ifndef FLIGHTWIN_H
#define FLIGHTWIN_H

#include <stdio.h>
#include <stdlib.h>

#include "TextFile.h"

class FlightWin {
public:
  FlightWin(Widget, void(*)(Widget, XtPointer, XmSelectionBoxCallbackStruct*),
		char *);
  int readFlightNumber(Widget, XtPointer, XmSelectionBoxCallbackStruct*);	
  void popupFlightWin();
  void updateFlightNums(char *);

  inline char *flightNumber() 	{return(flight_number);}

private:
  char *strlower(char s[]);

  Widget flightDialog;
  char  *flight_number;		// Entered flight number string
  char  *flightNums[8];		// From hl proj/###/flightNums, only use 4
};

#endif
