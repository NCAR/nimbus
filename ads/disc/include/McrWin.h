/* McrWin.h
   Creates and manages the MCR control window.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef MCRWIN_H
#define MCRWIN_H

#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#include <Xm/Xm.h>

#include <signal.h>
#include <unistd.h>
#include <dsmctl.h>
#include <header.h>
#include <messageDefs.h>

#include <DsmMessage.h>

class McrWin {
public:
  McrWin (Widget parent, DsmMessage &dm, char *dsm_loc,
          void(*toggleButtonCallbackWrap)
            (Widget, XtPointer, XmToggleButtonCallbackStruct*));

  void toggleButtonCallbackProc (Widget w, XtPointer client,
		XmToggleButtonCallbackStruct *cbs, void (*to)(int));
  void displayState (int action, int value); // set button and status bit states
  void sendState();
  void turnOffHeater()	{ XmToggleButtonSetState(heater_button, false, true); }

private:
  void displayStatus (int status);	// display status word bit values
  DsmMessage &dsm_msg;                  // message handler class
  str12 dsm_locn;			// dsm location for the mcr

  Widget frame;
  Widget title;
  Widget form;
  Widget mcr_check_box;
  Widget remote_button;			// enable/disable dsm control button
  Widget preamp_button;			// turn preamp power on/off button
  Widget door_enable_button;		// enable/disable door motors button
  Widget door_button;			// open/close doors button
  Widget heater_button;			// turn heaters on/off

  Widget preamp_on_label;		// preamp on status bit label
  Widget preamp_off_label;		// preamp off status bit label
  Widget rdoor_open_label;		// right door open status bit label
  Widget rdoor_closed_label;		// right door closed status bit label
  Widget ldoor_open_label;		// left door open status bit label
  Widget ldoor_closed_label;		// left door closed status bit label
  Widget dsm_active_label;		// dsm activity detected bit label
  Widget remote_enabled_label;		// dsm control enabled bit label
  Widget heaters_on_label;		// heaters on status bit label
  Widget heaters_off_label;		// heaters off status bit label

  Widget preamp_on_text;		// preamp on status bit text
  Widget preamp_off_text;		// preamp off status bit text
  Widget rdoor_open_text;		// right door open status bit text
  Widget rdoor_closed_text;		// right door closed status bit text
  Widget ldoor_open_text;		// left door open status bit text
  Widget ldoor_closed_text;		// left door closed status bit text
  Widget dsm_active_text;		// dsm activity detected bit text
  Widget remote_enabled_text;		// dsm control enabled bit texe
  Widget heaters_on_text;		// heaters on status bit text
  Widget heaters_off_text;		// heaters off status bit text
};

#endif
