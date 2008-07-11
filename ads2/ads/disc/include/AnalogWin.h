/* AnalogWin.h
   Creates and manages the analog control window.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef ANALOGWIN_H
#define ANALOGWIN_H

#include <Xm/CascadeB.h>
#include <Xm/DialogS.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/MessageB.h>
#include <Xm/PanedW.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>

#include <dsmctl.h>
#include <header.h>
#include <messageDefs.h>
#include <DsmConfig.h>
#include <DsmHeaders.h>
#include <DsmMessage.h>

#define ANALOG_BUTTON_VALUE_MASK	0x00ff	// mask to read button val

#define ANALOG_DSM_BUTTON_BASE		0x0000 	// dsm selection base value
#define ANALOG_CHAN_BUTTON_BASE		0x0100 	// channel selection base value
#define ANALOG_VOLT_BUTTON_BASE		0x0200 	// voltage selection base value
#define ANALOG_GAIN_BUTTON_BASE		0x0300 	// gain selection base value
#define ANALOG_OFFSET_BUTTON_BASE	0x0400 	// gain selection base value
#define ANALOG_REPORT_BUTTON_BASE	0x0500 	// report popup base value

#define ANALOG_CONVERT_OFFSET_MIN	-250	// convert_offset lower limit
#define ANALOG_CONVERT_OFFSET_MAX	 250	// convert_offset upper limit
#define ANALOG_CONVERT_MIN		.000350	// convert lower limit
#define ANALOG_CONVERT_MAX		.000450	// convert upper limit

class AnalogWin : private DsmConfig {
public:
  AnalogWin (Widget parent, DsmMessage &dm, DsmHeaders &dh,
             void (*toggleButtonCallbackWrap)(Widget, XtPointer, 
                                              XmToggleButtonCallbackStruct*),
             void (*pushButtonCallbackWrap)(Widget, XtPointer, 
                                            XmPushButtonCallbackStruct*),
             void (*questionCallbackWrap)(Widget, XtPointer, XtPointer));

  void toggleButtonCallbackProc (Widget w, XtPointer client,
                                 XmToggleButtonCallbackStruct *cbs);
  void pushButtonCallbackProc (Widget w, XtPointer client,
                               XmPushButtonCallbackStruct *cbs);
  void warningCallbackProc (Widget button, XtPointer client_data,
                        XtPointer call_data);
  void displayState (int action, char *locn);	// set & display analog state
  void popUp(Widget, XtPointer, XmPushButtonCallbackStruct *);

private:
  int calComplete();				// TRUE if cal is complete
  int QCcomplete();				// TRUE if QC is complete
  void mergeHeaders();				// merge updated dsm headers
  void setToggleButtonStates();			// set toggle button states
  void displayWarningDialog(char *message);	// popup message dialog
  void displayMessageDialog(char *message);	// popup message dialog
  void writeReportMessage(char *message);	// write message to report dia
  void writeQCreport();				// write QC report to window.

  int inRunMode() 	{return mode == ANALOG_RUN_MODE;}
  int inCalMode() 	{return mode == ANALOG_CAL_MODE;}
  int inQCmode() 	{return mode == ANALOG_QC_MODE;}
  int inDiagMode() 	{return mode == ANALOG_DIAG_MODE;}

  DsmHeaders &dsm_headers;              // message handler class
  DsmMessage &dsm_msg;                  // message handler class

  Widget mainForm, modeFrame, modeTitle, modeRadioBox;
  Widget runButton;
  Widget calButton;
  Widget qcButton;
  Widget diagButton;

  Widget diagFrame, diagTitle, diagRC;

  Widget buttonFrame, buttonRC;

  Widget dsmMenu, chanMenu, voltMenu, gainMenu, offsetMenu;
  Widget dsmCascade, chanCascade, voltCascade, gainCascade, offsetCascade;

  Widget dismissButt, reportButt;

  Widget warningDialog;			// warning dialog popup
  Widget messageDialog;			// message dialog popup

  Widget reportDialog;			// report dialog shell
  Widget reportForm;			// report dialog PanedWindow
  Widget reportText;			// report dialog PanedWindow
  Widget reportFrame;			// report dialog PanedWindow
  Widget reportRC;			// report dialog PanedWindow
  Widget report_ok_button;		// dismiss report window button

  int mode;				// current analog mode
  int confMode;				// new mode to be confirmed
  int diagChan;				// current diagnostic channel selection
  int diagVolt;				// current diagnostic voltage selection
  int diagGain;				// current diagnostic gain selection
  int diagOffset;			// current diagnostic offset selection
  int dsmMode[MAX_DSM];			// current dsm analog mode
  str12 diagLocn;			// current diagnostic dsm location

  char msg[DSM_MSG_STRING_SIZE];
};

#endif
