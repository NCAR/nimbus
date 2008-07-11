/* Pms1dWin.h
   Creates and manages the Pms 1d control window.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef PMS1DWIN_H
#define PMS1DWIN_H


#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>
#include <Xm/ToggleB.h>

#include <dsmctl.h>
#include <header.h>
#include <DsmMessage.h>

#define PMS1_BUTTON0_VALUE     	RANGE_0 	// 0 button value
#define PMS1_BUTTON1_VALUE     	RANGE_1  	// 1 button value
#define PMS1_BUTTON2_VALUE     	RANGE_2  	// 2 button value
#define PMS1_BUTTON3_VALUE     	RANGE_3		// 3 button value
#define PMS1_NUM_BUTTONS	4	 // max number of buttons per probe

class Pms1dWin {
public:
  Pms1dWin(Widget parent, DsmMessage &dm);	// constructor
  void createFssp (char *name, char *probe_locn, char *dsm_locn, 
                   void(*)(Widget, XtPointer, XmToggleButtonCallbackStruct*)); 
                  
  void createAsas (char *name, char *probe_locn, char *dsm_locn, 
                   void(*)(Widget, XtPointer, XmToggleButtonCallbackStruct*)); 

  void callbackProc (Widget w, XtPointer client,
                     XmToggleButtonCallbackStruct *cbs);

  void sendState();


private:
  inline int buttonOffset (int c) 	{return PMS1_NUM_BUTTONS * c;}
  inline int buttonValue (int c) 	{return c % PMS1_NUM_BUTTONS;}
  inline int buttonIndex (int c) 	{return c / PMS1_NUM_BUTTONS;}
  void makePms1Name (char *name, char *locn);	// make composite name


  DsmMessage &dsm_msg;                  // message handler class

  Widget pms1_frame;
  Widget pms1_title;
  Widget pms1_form;
  Widget pms1_label[MAX_PMS1];
  Widget pms1_radio_box[MAX_PMS1];
  Widget button0[MAX_PMS1];
  Widget button1[MAX_PMS1];
  Widget button2[MAX_PMS1];
  Widget button3[MAX_PMS1];
  str12 pms1_name[MAX_PMS1];		// probe names
  str12 location[MAX_PMS1];		// probe locations

  int count;				// number of probes
};

#endif
