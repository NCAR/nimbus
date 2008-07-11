/* StatusWin.h
   Creates and manages the project information window.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef STATUSWIN_H
#define STATUSWIN_H

#include <stdio.h>

#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>

#include "dsmctl.h"
#include "messageDefs.h"
 
#include "DsmConfig.h"
#include "DsmMessage.h"

/* 1 tape and 2 disks.
 */
#define NUM_DRVS	3

class StatusWin : private DsmConfig {

public:
  StatusWin(Widget, DsmMessage &tm,
	void (quitWrap)(Widget, XtPointer, XmPushButtonCallbackStruct*),
        void (analogWrap)(Widget, XtPointer, XmPushButtonCallbackStruct*),
        void (instrumentWrap)(Widget, XtPointer, XmPushButtonCallbackStruct*),
        void (recordWrap)(Widget, XtPointer, XmPushButtonCallbackStruct*)
);

  void displayProjectNumber(char *prj)
			{XmTextFieldSetString(projNumText, prj);}
  void displayFlightNumber(char *flt)
			{XmTextFieldSetString(fltNumText, flt);}
  void displayTime (int year, int month, int day, 
                    int hour, int minute, int second);	// display date/time

  void displayDSMTime(char *, int, int, int);
  void displayNetState(char*, int);


  void displayDriveState (int drive, int state);        // display drive state
  void recordCB(Widget w, XtPointer client,
                     XmPushButtonCallbackStruct *cbs);

  void displayMsg(char*);

  void Quit(Widget, XtPointer, XtPointer);

  Widget mainForm;


private:
  void CreateFlightInfoWin();
  void CreateTimeStampWin();
  void CreateNetworkStatusWin();
  void CreateRecordStatusWin();
  void CreateMessageWin();
  void CreateButtonWin();
//
  void updateDate(int year, int month, int day);	// update the date disp
  void updateTime(int hour, int minute, int second);	// update the time disp
//
//
  Widget projFrame, projNumText, fltNumText, tsFrame, dateText, timeText;
//
  Widget netFrame, netText[MAX_DSM];
//
  Widget rcdFrame, drvText[NUM_DRVS], rcd_radio_box, rcd_drv0_button,
	rcd_drv1_button, rcd_stop_button, unld_drv_button[1];
//
  Widget msgFrame, msgText;
//
  Widget buttonFrame, quitButton, analogButton, instrumentButton;
//
//
  DsmMessage &tape_msg;				// tape message class
  char tstr[20];				// temp string
};

#endif
