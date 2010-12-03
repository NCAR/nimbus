/*
-------------------------------------------------------------------------
OBJECT NAME:	cb_main.c

FULL NAME:	Command CallBacks

ENTRY POINTS:	CancelSetup()
		Proceed()
		ReadHeader()
		StartProcessing()
		StopProcessing()
		CreateListLineItem()
		ToggleOutput()
		LogMessage()
		Quit()

STATIC FNS:	ValidateFileNames()
		FillListWidget()
		CondenseVariableList()

DESCRIPTION:	Contains callbacks for the nimbus GUI main window & setup
		window.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2005
-------------------------------------------------------------------------
*/

#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/stat.h>

#include <Xm/List.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleBG.h>

#include "define.h"


static char	InputFileName[MAXPATHLEN], OutputFileName[MAXPATHLEN];

/* For wall clock timing	*/
long	start, finish;


void		StopProcessing();
static void	FillListWidget(), CondenseVariableList();
static int	ValidateFileNames();

void	ReadFormatFile();
bool	NextTimeInterval(long *start, long *end);
int	LowRateLoop(long starttime, long endtime);
int	CreateOutputFile(char fileName[]);
int	ReadInputFile(char fileName[]);


/* -------------------------------------------------------------------- */
void CancelSetup(Widget w, XtPointer client, XtPointer call)
{
  close(InputFile);

  for (size_t i = 0; i < Variable.size(); ++i)
    delete Variable[i];

  Variable.clear();

  free(ProjectName);


  DismissTimeSliceWindow(NULL, NULL, NULL);

  XtPopdown(XtParent(SetupWindow));
  XtUnmanageChild(SetupWindow);

  XmListDeleteAllItems(list1);

  XtSetSensitive(readHeaderButton, true);
  XtSetSensitive(inputFileText, true);
  XtSetSensitive(outputFileText, true);

  Initialize();

}	/* END CANCELSETUP */

/* -------------------------------------------------------------------- */
void Proceed(Widget w, XtPointer client, XtPointer call)
{
  strcpy(InputFileName, XmTextFieldGetString(inputFileText));
  strcpy(OutputFileName, XmTextFieldGetString(outputFileText));

  if (ValidateFileNames() == OK)
    ReadHeader(NULL, NULL, NULL);

}	/* END PROCEED */

/* -------------------------------------------------------------------- */
void ReadHeader(Widget w, XtPointer client, XtPointer call)
{
  XtSetSensitive(readHeaderButton, false);
  XtSetSensitive(inputFileText, false);
  XtSetSensitive(outputFileText, false);

  XmUpdateDisplay(Shell001);

  if (ReadInputFile(InputFileName) == ERR)
    {
    CancelSetup(NULL, NULL, NULL);
    return;
    }


  if (!Interactive)
    {
    sprintf(buffer, "%s - %s, Flight %s\n",
		ProjectName, ProjectNumber, FlightNumber);

    LogMessage(buffer);
    }
  else
    {
    Arg		args[1];

    FillListWidget();

    sprintf(buffer, "%s - %s, Flight %s",
		ProjectName, ProjectNumber, FlightNumber);

    XtSetArg(args[0], XmNtitle, buffer);
    XtSetValues(Shell001, args, 1);

    XtManageChild(SetupWindow);
    XtPopup(XtParent(SetupWindow), XtGrabNone);
    }

}	/* END READHEADER */

/* -------------------------------------------------------------------- */
void StartProcessing(Widget w, XtPointer client, XtPointer call)
{
  XmString	label;
  Arg		args[1];
  int		rc;
  long		btim, etim;

  DismissTimeSliceWindow(NULL, NULL, NULL);
  XtSetSensitive(list1, false);
  XtSetSensitive(menuBar, false);

  GetUserTimeIntervals();
  if (Interactive)
    CondenseVariableList();
  CreateOutputFile(OutputFileName);
  ReadFormatFile();

  FlushXEvents();


  /* Turn "Go" button into "Pause" button.
  */
  XtRemoveAllCallbacks(goButton, XmNactivateCallback);
  label = XmStringCreate((char *)"Pause", XmFONTLIST_DEFAULT_TAG);
  XtSetArg(args[0], XmNlabelString, label);
  XtSetValues(goButton, args, 1);
  XmStringFree(label);
  XtAddCallback(goButton, XmNactivateCallback, PauseProcessing, NULL);

  FlushXEvents();

  start = time(NULL);

  while (NextTimeInterval(&btim, &etim))
    {
    rc = LowRateLoop(btim, etim);

    if (PauseWhatToDo == P_QUIT || rc == ERR)
      break;
    }

  StopProcessing();

}	/* END STARTPROCESSING */

/* -------------------------------------------------------------------- */
void StopProcessing()
{
  XmString	label;
  Arg		args[1];
  float		x;

  CloseNetCDF();


  /* Log wall clock time.
  */
  finish = time(NULL);
  x = (float)(finish - start) / 60.0;

  if (x > 60.0)
    sprintf(buffer, "Processing took %.2f hour(s).\n", x / 60.0);
  else
    sprintf(buffer, "Processing took %.1f minutes.\n", x);

  LogMessage(buffer);


  /* Turn "Pause" button back into "Go" button.
  */
  label = XmStringCreate((char *)"Go", XmFONTLIST_DEFAULT_TAG);
  XtSetArg(args[0], XmNlabelString, label);
  XtSetValues(goButton, args, 1);
  XmStringFree(label);
  XtRemoveAllCallbacks(goButton, XmNactivateCallback);
  XtAddCallback(goButton, XmNactivateCallback, StartProcessing, NULL);

  XtSetSensitive(menuBar, true);
  XtSetSensitive(list1, true);

}	/* END STOPPROCESSING */

/* -------------------------------------------------------------------- */
void Quit(Widget w, XtPointer client, XtPointer call)
{
  exit(0);
}

/* -------------------------------------------------------------------- */
static int ValidateFileNames()
{
  if (strcmp(&InputFileName[strlen(InputFileName)-3], ".nc") != 0)
    {
    strcat(InputFileName, ".nc");
    XmTextFieldSetString(inputFileText, InputFileName);
    }

  if (access(InputFileName, R_OK) == ERR || strlen(InputFileName) == 0)
    {
    HandleError("Non-existent input file.");
    return(ERR);
    }


  if (strlen(OutputFileName) == 0)
    {
    HandleError("No output file specified.");
    return(ERR);
    }

  if (strcmp(&OutputFileName[strlen(OutputFileName)-4], ".asc") != 0)
    {
    strcat(OutputFileName, ".asc");
    XmTextFieldSetString(outputFileText, OutputFileName);
    }

  if (access(OutputFileName, R_OK) == ERR)
    if (errno == ENOENT)
      return(OK);
    else
      {
      HandleError("Permission denied on output file.");
      return(ERR);
      }
  else
    {
    HandleWarning("Output file exists.", ReadHeader);
    return(ERR);
    }

  return(OK);

}	/* END VALIDATEFILENAMES */

/* -------------------------------------------------------------------- */
XmString CreateListLineItem(VARTBL *vp)
{
  static const char *list1lineFrmt = "%-13s  %c   %4d    %4d";

  sprintf(buffer, list1lineFrmt,
		vp->name.c_str(),
		vp->Output ? 'Y' : 'N',
		vp->SampleRate,
		vp->OutputRate);

  return(XmStringCreateLocalized(buffer));

}	/* END CREATELISTLINEITEM */

/* -------------------------------------------------------------------- */
static void FillListWidget()
{
  size_t	cnt;
  XmString	items[MAX_VARIABLES];

  XmListDeleteAllItems(list1);

  cnt = 0;

  for (size_t i = 0; i < Variable.size(); ++i)
    items[cnt++] = CreateListLineItem(Variable[i]);


  XmListAddItems(list1, items, cnt, 1);

  for (size_t i = 0; i < cnt; ++i)
    XmStringFree(items[i]);

}	/* END FILLLISTWIDGET */

/* -------------------------------------------------------------------- */
void ToggleOutput(Widget w, XtPointer client, XtPointer call)
{
  int		*pos_list, pos_cnt = 0;
  int		i, indx;
  XmString	s;

  XmListGetSelectedPos(list1, &pos_list, &pos_cnt);

  for (i = 0; i < pos_cnt; ++i)
  {
    indx = pos_list[i] - 1;

    Variable[indx]->Output = 1 - Variable[indx]->Output;

    s = CreateListLineItem(Variable[indx]);

    XmListReplaceItemsPos(list1, &s, 1, pos_list[i]);
    XmStringFree(s);
  }

}	/* END TOGGLEOUTPUT */

/* -------------------------------------------------------------------- */
void LogMessage(const char msg[])
{
  XmTextPosition	position;
  extern Widget	logText;

  if (Interactive)
    {
    position = XmTextGetInsertionPosition(logText);
    XmTextInsert(logText, position, (char *)msg);

    position += strlen(msg);
    XmTextShowPosition(logText, position);
    XmTextSetInsertionPosition(logText, position);
    }
  else
    fprintf(stderr, msg);

}	/* END LOGMESSAGE */

/* -------------------------------------------------------------------- */
static void CondenseVariableList()
{
  for (size_t i = 0; i < Variable.size(); ++i)
    if (Variable[i]->Output)
      outputList.push_back(Variable[i]);

}	/* END CONDENSEVARIABLELIST */

/* END CB_MAIN.C */
