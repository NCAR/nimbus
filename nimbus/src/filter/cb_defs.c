/*
-------------------------------------------------------------------------
OBJECT NAME:	cb_defs.c

FULL NAME:	Defaults Object

ENTRY POINTS:	SaveDefaults()
		SetDefaultsValue()
		CreateEditDefaultsWindow()
		EditDefaultsFile()
		DismissDefaultsWindow()

STATIC FNS:	set_defaultText()
		MarkDirty()
		VerifyDefault()
		ResetDefaults()

DESCRIPTION:	The Defaults file contains constants for various probe
		calculations.

REFERENCES:	OpenProjectFile()

REFERENCED BY:	hdr_decode.c, cb_main(), various AMLIB fn's.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2005
-------------------------------------------------------------------------
*/

#include <stdio.h>

#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/TextF.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>

#include "nimbus.h"
#include "decode.h"
#include "gui.h"

static Widget	Shell004, EditDefaultsWindow, defaultText[MAX_DEFAULTS];

extern size_t	nDefaults;
extern DEFAULT	*Defaults[];

static void	set_defaultText(int);

static void MarkDirty(Widget w, XtPointer indx, XtPointer call);
static void VerifyDefault(Widget w, int indx, XtPointer call);


/* -------------------------------------------------------------------- */
void SaveDefaults(FILE *fp)	/* Save modified defaults into "Setup" file */
{
  for (size_t i = 0; i < nDefaults; ++i)
    if (Defaults[i]->Dirty)
      {
      fprintf(fp, "DEFAULT=%s %d", Defaults[i]->Name, Defaults[i]->nValues);

      for (size_t j = 0; j < Defaults[i]->nValues; ++j)
        fprintf(fp, " %e", Defaults[i]->Value[j]);

      fprintf(fp, "\n");
      }

}	/* END SAVEDEFAULTS */

/* -------------------------------------------------------------------- */
void SetDefaultsValue(char target[], NR_TYPE *new_value)
{
  for (size_t i = 0; i < nDefaults; ++i)
    if (strcmp(Defaults[i]->Name, target) == 0)
      {
      Defaults[i]->Dirty = true;

      for (size_t j = 0; j < Defaults[i]->nValues; ++j)
        Defaults[i]->Value[j] = new_value[j];

      return;
      }

  sprintf(buffer, "Request to set non-existent default, %s.\n", target);
  LogMessage(buffer);

}	/* END SETDEFAULTSVALUE */

/* -------------------------------------------------------------------- */
void ResetDefaults(Widget w, XtPointer client, XtPointer call)
{
  for (size_t i = 0; i < nDefaults; ++i)
    {
    delete [] Defaults[i]->Value;
    delete Defaults[i];
    }

  nDefaults = 0;

  ReadDefaultsFile();

  for (size_t i = 0; i < nDefaults; ++i)
    set_defaultText(i);

}	/* END RESETDEFAULTS */

/* -------------------------------------------------------------------- */
void CreateEditDefaultsWindow()
{
  Arg		args[16];
  Cardinal	n;
  XmString	labelString;
  Widget	edFrame, edRC, b[2], defWin, defRC,
		form[MAX_DEFAULTS], label[MAX_DEFAULTS];

  extern Widget	Shell001, AppShell;

  n = 0;
  XtSetArg(args[n], XmNtitle, "Edit Defaults"); n++;
  XtSetArg(args[n], XmNtransientFor, Shell001); n++;
  Shell004 = XtCreatePopupShell("editDefaultsShell",
		transientShellWidgetClass, AppShell, args, n);

  n = 0;
  EditDefaultsWindow = XmCreateForm(Shell004, "editDefForm", args, n);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;
  defWin = XmCreateScrolledWindow(EditDefaultsWindow, "defWin", args, n);
  XtManageChild(defWin);

  n = 0;
  defRC = XmCreateRowColumn(defWin, "defRC", args, n);
  XtManageChild(defRC);

  for (size_t i = 0; i < nDefaults; ++i)
    {
    n = 0;
    form[i] = XmCreateForm(defRC, "frm", args, n);

    labelString = XmStringCreate(Defaults[i]->Name, XmFONTLIST_DEFAULT_TAG);

    n = 0;
    XtSetArg(args[n], XmNlabelString, labelString); ++n;
    label[i] = XmCreateLabel(form[i], "lbl", args, n);
    XtManageChild(label[i]);
    XmStringFree(labelString);

    n = 0;
    XtSetArg(args[n], XmNcolumns, 14 * Defaults[i]->nValues); n++;
    XtSetArg(args[n], XmNmaxLength, 14 * Defaults[i]->nValues); n++;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNleftWidget, label[i]); n++;
    defaultText[i] = XmCreateTextField(form[i], "txt", args, n);
    XtAddCallback(defaultText[i], XmNlosingFocusCallback,
				(XtCallbackProc)VerifyDefault, (XtPointer)i);
    XtAddCallback(defaultText[i], XmNvalueChangedCallback,
				(XtCallbackProc)MarkDirty, (XtPointer)i);
    XtManageChild(defaultText[i]);

    set_defaultText(i);
    }

  XtManageChildren(form, nDefaults);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, defWin); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  edFrame = XmCreateFrame(EditDefaultsWindow, "buttonFrame", args, n);
  XtManageChild(edFrame);

  n = 0;
  edRC = XmCreateRowColumn(edFrame, "edRC", args, n);
  XtManageChild(edRC);


  n = 0;
  b[0] = XmCreatePushButton(edRC, "resetButton", args, n);
  XtAddCallback(b[0], XmNactivateCallback, ResetDefaults, NULL);

  n = 0;
  b[1] = XmCreatePushButton(edRC, "dismissButton", args, n);
  XtAddCallback(b[1], XmNactivateCallback, DismissDefaultsWindow, NULL);
  XtManageChildren(b, 2);

}	/* END CREATEEDITDEFAULTSWINDOW */

/* -------------------------------------------------------------------- */
void EditDefaultsFile(Widget w, XtPointer client, XtPointer call)
{
  static int	firstTime = true;

  if (firstTime)
    {
    CreateEditDefaultsWindow();
    firstTime = false;
    }

  XtManageChild(EditDefaultsWindow);
  XtPopup(XtParent(EditDefaultsWindow), XtGrabNone);

}	/* END EDITDEFAULTSFILE */

/* -------------------------------------------------------------------- */
void DismissDefaultsWindow(Widget w, XtPointer client, XtPointer call)
{
  XtPopdown(XtParent(EditDefaultsWindow));
  XtUnmanageChild(EditDefaultsWindow);

}	/* END DISMISSDEFAULTSWINDOW */

/* -------------------------------------------------------------------- */
static void MarkDirty(Widget w, XtPointer indx, XtPointer call)
{
  Defaults[(int)indx]->Dirty = true;

}	/* END MARKDIRTY */

/* -------------------------------------------------------------------- */
static void VerifyDefault(Widget w, int indx, XtPointer call)
{
  char	*p, *p1;
  NR_TYPE	f[128];

  if (Defaults[indx]->Dirty == false)
    return;


  p = XmTextFieldGetString(defaultText[indx]);

  /* Verify values
   */
  p1 = strtok(p, ", ");

  for (size_t i = 0; i < Defaults[indx]->nValues; ++i)
    {
    f[i] = p1 ? (NR_TYPE)atof(p1) : 0.0;

    p1 = strtok(NULL, ", ");
    }

  XtFree(p);


  /* Set values & reprint text widget
   */
  for (size_t i = 0; i < Defaults[indx]->nValues; ++i)
    Defaults[indx]->Value[i] = f[i];

  set_defaultText(indx);

}	/* END VERIFYDEFAULT */

/* -------------------------------------------------------------------- */
static void set_defaultText(int indx)
{
  char	temp[64];

  buffer[0] = '\0';

  for (size_t i = 0; i < Defaults[indx]->nValues; ++i)
    {
    if (i > 0)
      strcat(buffer, ", ");

    sprintf(temp, "%e", Defaults[indx]->Value[i]);
    strcat(buffer, temp);
    }

  XtRemoveCallback(	defaultText[indx], XmNvalueChangedCallback,
			MarkDirty, (XtPointer)indx);

  XmTextFieldSetString(defaultText[indx], buffer);

  XtAddCallback(	defaultText[indx], XmNvalueChangedCallback,
			MarkDirty, (XtPointer)indx);

}	/* END SETDEFAULTTEXT */

/* END CB_DEFS.C */
