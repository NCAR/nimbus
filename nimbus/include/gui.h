/*
-------------------------------------------------------------------------
OBJECT NAME:	gui.h

DESCRIPTION:	Header File declaring Variable and associated user
		interface.
-------------------------------------------------------------------------
*/

#ifndef GUI_H
#define GUI_H

#include <Xm/Xm.h>

/* Values for "VariableType"	*/
#define SDI	0
#define RAW	1
#define DERIVED	2


/* PauseWhatToDo values.	*/
#define P_QUIT		0
#define P_CONTINUE	1
#define P_XTV		2	/* Not used	*/


extern bool	Interactive;


/* Setup window widgets */
extern Widget	Shell001;
extern Widget	SetupWindow, EditWindow, TimeSliceWindow;
extern Widget	aDSdataText, outputFileText;
extern Widget	lowRateButton, highRateButton, timeDisplayText;
extern Widget	list1, goButton, menuBar, readHeaderButton;

/* Edit Variable window widgets */
extern Widget	varNameLabel;
extern Widget	outputVarYes, outputVarNo;
extern Widget	outputLRbutton, outputSRbutton, outputHRbutton;
extern Widget	lagText, spikeText, synthConstText, ev_text[], ts_text[];


/*	Callback prototypes
 */
void	ApplyVariableMods(Widget w, XtPointer client, XtPointer call),
	CancelSetup(Widget w, XtPointer client, XtPointer call),
	DismissDefaultsWindow(Widget w, XtPointer client, XtPointer call),
	DismissFlightWindow(Widget w, XtPointer client, XtPointer call),
	DismissEditWindow(Widget w, XtPointer client, XtPointer call),
	DismissTimeSliceWindow(Widget w, XtPointer client, XtPointer call),
	EditDefaultsFile(Widget w, XtPointer client, XtPointer call),
	EditFlightInfo(Widget w, XtPointer client, XtPointer call),
	EditTimeSlices(Widget w, XtPointer client, XtPointer call),
	EditVariable(Widget w, XtPointer client, XmListCallbackStruct *call),
	LoadSetup(Widget w, XtPointer client, XtPointer call),
	LoadSetup_OK(Widget w, XtPointer client, XmFileSelectionBoxCallbackStruct *call),
	LoadSynthetic_OK(Widget w, XtPointer client, XmFileSelectionBoxCallbackStruct *call),
	LoadSynthetic(Widget w, XtPointer client, XtPointer call),
	MapCalCof(Widget w, XtPointer client, XtPointer call),
	PauseProcessing(Widget w, XtPointer client, XtPointer call),
	PauseStop(Widget w, XtPointer client, XtPointer call),
	PauseContinue(Widget w, XtPointer client, XtPointer call),
	PrintSetup(Widget w, XtPointer client, XtPointer call),
	Proceed(Widget w, XtPointer client, XtPointer call),
	QueryOutputFile(Widget w, XtPointer client, XtPointer call),
	Quit(Widget w, XtPointer client, XtPointer call),
	SaveSetup(Widget w, XtPointer client, XtPointer call),
	SaveSetup_OK(Widget w, XtPointer client, XmFileSelectionBoxCallbackStruct *call),
	SetLowRate(Widget w, XtPointer client, XmToggleButtonCallbackStruct *call),
	SetHighRate(Widget w, XtPointer client, XmToggleButtonCallbackStruct *call),
	StartProcessing(Widget w, XtPointer client, XtPointer call),
	ToggleProbe(Widget w, XtPointer client, XtPointer call),
	ToggleOutput(Widget w, XtPointer client, XtPointer call),
	ToggleRate(Widget w, XtPointer client, XtPointer call),
	ValidateTime(Widget w, XtPointer client, XtPointer call),
	VerifyLagText(Widget w, XtPointer client, XmTextVerifyCallbackStruct *call);

XmString	CreateListLineItem(void *pp, int var_type);

void	CreateProbeMenu(),
	HandleWarning(char msg[], XtCallbackProc, XtCallbackProc);

#ifdef __cplusplus
	extern "C" {
#endif
void	FileCancel(Widget w, XtPointer clientData, XtPointer callData),
	WarnUser(char str[], XtCallbackProc, XtCallbackProc),
	ShowError(char str[]),
	QueryFile(char *prompt, char *directory, XtCallbackProc),
	ExtractFileName(XmString str, char **text);
#ifdef __cplusplus
	}
#endif

#endif

/* END GUI.H */
