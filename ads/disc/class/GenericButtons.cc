/* GenericButtons.cc
   GenericButtons control class.
 
   Original Author: Christopher J. Webster
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include "GenericButtons.h"
#include "TextFile.h"

void GenericButtonToggleButtonCallbackWrap (Widget w, XtPointer client,
                              XmToggleButtonCallbackStruct *cbs);


/* -------------------------------------------------------------------- */
GenericButtons::GenericButtons(Widget parent, DsmMessage &dm,
	void (*toggleButtonCallbackWrap)
                  (Widget, XtPointer, XmToggleButtonCallbackStruct*)) :
	dsm_msg (dm)
{
  int		i;
  Arg		args[8];
  Cardinal	n;

  if ((nButtons = readGenericButtonsFile()) == 0)
    return;


// Create the window frame and form.
  n = 0;
  frame = XmCreateFrame(parent, "genButtFrame", args, n);
  XtManageChild(frame);
 
  n = 0;
  title = XmCreateLabel(frame, "genButtTitle", args, n);
  XtManageChild(title);

  n = 0;
  RC = XmCreateRowColumn(frame, "genButtRC", args, n);
  XtManageChild(RC);

  for (i = 0; i < nButtons; ++i) {
    n = 0;
    button[i] = XmCreateToggleButton(RC, buttName[i], args, n);
    XtAddCallback(button[i], XmNvalueChangedCallback,
	(XtCallbackProc)GenericButtonToggleButtonCallbackWrap, (XtPointer)i);
  }

  XtManageChildren(button, nButtons);

}

/* -------------------------------------------------------------------- */
void GenericButtons::sendState()
{
  for (int i = 0; i < nButtons; ++i)
    if (invert[i])
      dsm_msg.sendDigOutMsg(1 - XmToggleButtonGetState(button[i]),
				dsmLocn[i], connector[i], channel[i], "");
    else
      dsm_msg.sendDigOutMsg(XmToggleButtonGetState(button[i]),
				dsmLocn[i], connector[i], channel[i], "");

}

/* -------------------------------------------------------------------- */
void GenericButtons::toggleButtonCallbackProc(Widget w, XtPointer client,
			XmToggleButtonCallbackStruct *cbs)
{
  int indx = (int)client;

  if (invert[indx])
    dsm_msg.sendDigOutMsg(1 - cbs->set, dsmLocn[indx], connector[indx],
		 channel[indx], "");
  else
    dsm_msg.sendDigOutMsg(cbs->set, dsmLocn[indx], connector[indx],
		 channel[indx], "");
}

/* -------------------------------------------------------------------- */
int GenericButtons::readGenericButtonsFile()
{
  int i;
  char *line[MAX_BUTTS], fileName[256], tstr[80], *p;
  TextFile digOuts;

  p = getenv("HOST");
  strcpy(tstr, p);
  if ((p = strchr(tstr, '.')))
    *p = '\0';

  sprintf(fileName, "%s/hosts/%s/DigOuts", getenv("PROJ_DIR"), tstr);
  nButtons = digOuts.readTextFile(fileName, line);

  for (i = 0; i < nButtons; ++i) {
    strcpy(dsmLocn[i], strtok(line[i], ",\t "));
    connector[i] = atoi(strtok(NULL, ",\t "));
    channel[i] = atoi(strtok(NULL, ",\t "));
    strcpy(buttName[i], strtok(NULL, "\n,\t "));
    p = strtok(NULL, "\n,\t ");
    if (p) invert[i] = atoi(p);
  }

  digOuts.freeTextFile(line);

  return(nButtons);

}

/* END GENERICBUTTONS.CC */
