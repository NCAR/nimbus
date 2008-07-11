/* GenericButtons.h
   Creates and manages the Generic DIGOUT Buttons (for B57)

   Original Author: Chris Webster
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef GENBUTT_H
#define GENBUTT_H

#include <stdlib.h>
#include <string.h>

#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>
#include <Xm/ToggleB.h>

#include <dsmctl.h>
#include <header.h>
#include <DsmMessage.h>

#define MAX_BUTTS	32

class GenericButtons {
public:
  GenericButtons(Widget parent, DsmMessage &dm,
          void(*toggleButtonCallbackWrap)
            (Widget, XtPointer, XmToggleButtonCallbackStruct*));

  void toggleButtonCallbackProc (Widget w, XtPointer client,
                     XmToggleButtonCallbackStruct *cbs);

  void sendState();


private:
  int readGenericButtonsFile();

  DsmMessage &dsm_msg;                  // message handler class

  Widget frame;
  Widget title;
  Widget RC;
  Widget button[MAX_BUTTS];
  char buttName[MAX_BUTTS][32];		// Button titles.

  str12 dsmLocn[MAX_BUTTS];		// DSM location
  short connector[MAX_BUTTS];		// Digital Connector number
  short channel[MAX_BUTTS];		// Digital channel number, in connector.
  short invert[MAX_BUTTS];		// is 1 off and 0 on?

  int nButtons;				// number of buttons
};

#endif
