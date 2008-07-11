/* FlightWin.cc
   Provides a popup panel for the flight number entry.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <FlightWin.h>


/* -------------------------------------------------------------------- */
FlightWin::FlightWin(
	Widget topPane,
	void (callbackWrap)(Widget, XtPointer, XmSelectionBoxCallbackStruct*),
	char *proj_num)
{
  TextFile	fns;
  Widget	w;
  XmString	label;
  char	localBuff[256];
  Arg	args[2];
  int	n;

  flight_number = NULL;

 
  // Read "flightNums" file to determine last flight number.
  sprintf(localBuff, "%s/%s/flightNums", getenv("PROJ_DIR"), proj_num);
  fns.readTextFile(localBuff, flightNums);


  // Create the flight dialog box.

  strcpy(localBuff, "Enter Flight Number:");
  if (flightNums[0])
    {
    strcat(localBuff, "\n(");

    for (n = 0; flightNums[n]; ++n)
      {
      strcat(localBuff, flightNums[n]);
      if (flightNums[n+1])
        strcat(localBuff, ", ");
      }

    strcat(localBuff, ")");
    }

  n = 0;
  label = XmStringCreateLtoR(localBuff, XmSTRING_DEFAULT_CHARSET);
  XtSetArg(args[0], XmNselectionLabelString, label); ++n;
  XtSetArg(args[1], XmNautoUnmanage, FALSE); ++n;
  flightDialog = XmCreatePromptDialog(topPane, "flightNum", args, n);
  XmStringFree(label);

  XtVaSetValues(flightDialog,
                XmNdialogStyle,
                XmDIALOG_FULL_APPLICATION_MODAL,
                NULL);

  XtUnmanageChild(XmSelectionBoxGetChild(flightDialog, XmDIALOG_CANCEL_BUTTON));
  XtUnmanageChild(XmSelectionBoxGetChild(flightDialog, XmDIALOG_HELP_BUTTON));


  if (flightNums[0]) {
    strcpy(localBuff, flightNums[0]);
    sprintf(&localBuff[2], "%02d", atoi(&localBuff[2]) + 1);
    w = XmSelectionBoxGetChild(flightDialog, XmDIALOG_TEXT);
    XmTextSetString(w, localBuff);
  }

  XtAddCallback(flightDialog, XmNokCallback, 
                (XtCallbackProc)callbackWrap, NULL);
  XtManageChild(flightDialog);
  XtPopup(XtParent(flightDialog), XtGrabExclusive);

}

/* -------------------------------------------------------------------- */
int FlightWin::readFlightNumber(Widget w, XtPointer client,
                              XmSelectionBoxCallbackStruct *cbs)
 
// Reads the user flight number entry.
{
  int valid = FALSE;

  XmStringGetLtoR(cbs->value, XmSTRING_DEFAULT_CHARSET, &flight_number);

  if (strlen(flight_number)) {
    if (strcmp(flight_number, "0") == 0)
      valid = TRUE;
    else {
      strlower(flight_number);

      if ((strncmp(flight_number, "rf", 2) == 0 || 
           strncmp(flight_number, "tf", 2) == 0 ||
           strncmp(flight_number, "ff", 2) == 0 ||
           strncmp(flight_number, "gt", 2) == 0) &&
           atoi(&flight_number[2]) > 0) {
        sprintf(&flight_number[2], "%02d", atoi(&flight_number[2]));
        valid = TRUE;
      }
    }
  }

  if (valid)
    XtDestroyWidget(w);
  else
    flight_number = NULL;

  return(valid);
}

/* -------------------------------------------------------------------- */
void FlightWin::updateFlightNums(char *proj_num)
{
  int	i;
  FILE	*fp;
  char	localBuff[256];
  static int firstTime = TRUE;

  if (firstTime) {
    sprintf(localBuff, "%s/%s/flightNums", getenv("PROJ_DIR"), proj_num);
    if ( (fp = fopen(localBuff, "w+")) ) {

      fprintf(fp, "%s\n", flight_number);  // write current first

      for (i = 0; flightNums[i]; ++i)
        if (strncmp(flight_number, flightNums[i], 2))
          fprintf(fp, "%s\n", flightNums[i]);

      fclose(fp);
    }
  }

}

/* -------------------------------------------------------------------- */
void FlightWin::popupFlightWin()
{
  XtPopup(XtParent(flightDialog), XtGrabExclusive);
}

/* -------------------------------------------------------------------- */
#include <ctype.h>
 
char *FlightWin::strlower(char s[])
{
        char   *p;
 
        for (p = s; *p; ++p)
                *p |= 0x20;
 
        return(s);
 
}
