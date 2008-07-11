/* Pms2dWin.h
   Creates and manages the Pms 1d control window.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef PMS2DWIN_H
#define PMS2DWIN_H

#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#include <Xm/Xm.h>

#include <dsmctl.h>
#include <header.h>
#include <DsmMessage.h>

#define PMS2_TAS_BUTTON_VALUES 		{20, 34, 80, 120, 140, TAS2D_AUTO} 
#define PMS2_RATE_BUTTON_VALUES		{RATE2D_OFF, 1, 2, 4, 8, RATE2D_MAX}
#define PMS2_TAS_NUM_BUTTONS		6		// num tas buttons
#define PMS2_RATE_NUM_BUTTONS		6		// num rate buttons

class Pms2dWin {
public:
  Pms2dWin (Widget parent, DsmMessage &dm, 
    void(*tasCallbackWrap)(Widget, XtPointer, XmToggleButtonCallbackStruct*),
    void(*rateCallbackWrap)(Widget, XtPointer, XmToggleButtonCallbackStruct*));	
  void createProbe (char *probe_name, char *probe_locn, char *dsm_locn,
    void (*probeCallbackWrap)(Widget, XtPointer,XmToggleButtonCallbackStruct*));

  void tasCallbackProc (Widget w, XtPointer client,
                        XmToggleButtonCallbackStruct *cbs);
  void rateCallbackProc (Widget w, XtPointer client,
                         XmToggleButtonCallbackStruct *cbs);
  void probeCallbackProc (Widget w, XtPointer client,
                         XmToggleButtonCallbackStruct *cbs);

  void sendState();


private:
  DsmMessage &dsm_msg;                  // message handler class

  Widget frame;
  Widget title;
  Widget form;
  Widget tas_radio_box;
  Widget tas_label;
  Widget tas_button[PMS2_TAS_NUM_BUTTONS];
  Widget rate_radio_box;
  Widget rate_label;
  Widget rate_button[PMS2_RATE_NUM_BUTTONS];
  Widget probe_check_box;
  Widget probe_label;
  Widget probe_button[MAX_PMS2];
  str12 name[MAX_PMS2];			// probe names
  str12 location[MAX_PMS2];		// probe locations

  int p_cnt;				// number of p probes
  int c_cnt;				// number of c probes
  int g_cnt;				// number of grey probes
  int h_cnt;				// number of hvps probes
  int tot_cnt;				// total number of probes

};

#endif
