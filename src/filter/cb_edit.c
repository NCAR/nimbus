/*
-------------------------------------------------------------------------
OBJECT NAME:	cb_edit.c

FULL NAME:	Edit Window CallBacks

ENTRY POINTS:	EditVariable()
		GetNewEditVariable()
		ApplyVariableMods()
		DismissEditWindow()
		VerifyLagText()
		MapCalCoe()


STATIC FNS:	set_edit_window_data()

DESCRIPTION:	Contains callbacks for the nimbus GUI.

INPUT:			

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	XtAppMainLoop()

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2000
-------------------------------------------------------------------------
*/

#include <sys/param.h>
#include <ctype.h>

#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/List.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>

#include "nimbus.h"
#include "decode.h"
#include "gui.h"
#include "vardb.h"
#include "injectsd.h"


static Widget   EditShell = 0, EditWindow = 0;

static int	VariableType, ListPosition;
static SDITBL	*sp;
static RAWTBL	*rp;
static DERTBL	*dp;

static void set_edit_window_data(
		char	name[],
		bool	output,
		int	outputRate,
		int	lag,
		NR_TYPE	spike,
		char	*dq);

extern char *dataQuality[];

extern Widget slOpMenu, funcOpMenu;

extern  vars tempvar;
extern SyntheticData sd;
extern char * func[19];

/* -------------------------------------------------------------------- */
void EditVariable(Widget w, XtPointer client, XmListCallbackStruct *call)
{
  size_t i, indx;

  if (w == list1)
    {
    indx = (ListPosition = call->item_position) - 1;

    if (indx < sdi.size())
      {
      sp = sdi[indx];
      VariableType = SDI;
      }
    else
    if ((indx -= sdi.size()) < raw.size())
      {
      rp = raw[indx];
      VariableType = RAW;
      
      }
    else
      {
      dp = derived[indx - raw.size()];
      VariableType = DERIVED;
      }
    }


  switch (VariableType)
    {
    case SDI:
      set_edit_window_data(sp->name, sp->Output, sp->OutputRate,
                           sp->StaticLag, sp->SpikeSlope, sp->DataQuality);

      for (i = 0; i < sp->cof.size(); ++i)
        {
        sprintf(buffer, "%e", sp->cof[i]);
        XmTextFieldSetString(ev_text[i], buffer);
        XtSetSensitive(ev_text[i], true);
        }

      for (; i < MAXDEPEND; ++i)
        {
        XmTextFieldSetString(ev_text[i], "");
        XtSetSensitive(ev_text[i], true);
        }

      break;

    case RAW:
      set_edit_window_data(rp->name, rp->Output, rp->OutputRate,
                           rp->StaticLag, rp->SpikeSlope, rp->DataQuality);

      for (i = 0; i < rp->cof.size(); ++i)
        {
        sprintf(buffer, "%e", rp->cof[i]);
        XmTextFieldSetString(ev_text[i], buffer);
        XtSetSensitive(ev_text[i], true);
        }

      for (; i < MAXDEPEND; ++i)
        {
        XmTextFieldSetString(ev_text[i], "");
        XtSetSensitive(ev_text[i], false);
        }

      break;

    case DERIVED:
      set_edit_window_data(dp->name, dp->Output, dp->OutputRate,
			   ERR, 0.0, dp->DataQuality);

      for (i = 0; i < dp->ndep; ++i)
        {
        XmTextFieldSetString(ev_text[i], dp->depend[i]);
        XtSetSensitive(ev_text[i], true);
        }

      for (; i < MAXDEPEND; ++i)
        {
        XmTextFieldSetString(ev_text[i], "");
        XtSetSensitive(ev_text[i], false);
        }
      break;
      }


  XtManageChild(EditWindow);
  XtPopup(XtParent(EditWindow), XtGrabNone);

}	/* END EDITVARIABLE */

/* -------------------------------------------------------------------- */
void ApplyVariableMods(Widget w, XtPointer client, XtPointer call)
{
  bool		output;
  int		outputRate = Config::LowRate, lag;
  char		*p, *dq;
  float		f;
  float		constsynthval;
  NR_TYPE	spike;
  Arg		args[3];
  XmString	newAttr;
  struct v	tempvar; 

  output = XmToggleButtonGetState(outputVarYes);

  if (XmToggleButtonGetState(outputLRbutton))
    outputRate = Config::LowRate;
  else
  if (XmToggleButtonGetState(outputSRbutton))
    outputRate = Config::SampleRate;
  else
  if (XmToggleButtonGetState(outputHRbutton))
    outputRate = Config::HighRate;
  else
    HandleError("Impossible, no outputRate set.");

  p = XmTextFieldGetString(lagText);
  lag = atoi(p);
  XtFree(p);

  p = XmTextFieldGetString(spikeText);
  spike = atof(p);
  XtFree(p);

  p=XmTextFieldGetString(synthConstText);
  constsynthval=atof(p);
  XtFree(p);


  /* Extract "Data Quality" */
  XtSetArg(args[0], XmNlabelString, &newAttr);
  XtGetValues(XmOptionButtonGadget(slOpMenu), args, 1);
  XmStringGetLtoR(newAttr, XmSTRING_DEFAULT_CHARSET, &p);
  dq = SearchDataQuality(p);

  /*Extract Function */
  XtSetArg(args[0],XmNlabelString,&newAttr);
  XtGetValues(XmOptionButtonGadget(funcOpMenu),args,1);
  XmStringGetLtoR(newAttr,XmSTRING_DEFAULT_CHARSET, &p);


  switch (VariableType)
    {
    case SDI:
      /*synthetic data modifications*/
       if (strcmp(dq, "synthetic") == 0)
         {
         if (constsynthval)
           {
           tempvar.type='s';
           tempvar.name=sp->name;
           tempvar.value=constsynthval;
           sd.registervar(tempvar);
           }
         else if(!(p=="none"))
           {
           tempvar.type='s';
           tempvar.name=sp->name;
           tempvar.function=p;
           sd.registerfunc(tempvar); 
          }
        }

      sp->Dirty = true;
      sp->Output = output;

      if ((sp->OutputRate = outputRate) == 0)
        sp->OutputRate = sp->SampleRate;

      sp->StaticLag = lag;
      sp->SpikeSlope = spike;
      sp->DataQuality = dq;
      sp->cof.clear();

      for (int i = MAX_COF-1; i >= 0; --i)
        {
        p = XmTextFieldGetString(ev_text[i]);
        f = atof(p);
        XtFree(p);

        if (f == 0.0 && sp->cof.size() == 0)
          continue;

        sp->cof.insert(sp->cof.begin(), f);
        }

      newAttr = CreateListLineItem((void *)sp, SDI);
      break;

    case RAW:
       /*synthetic data modifications*/
       if (strcmp(dq, "synthetic") == 0)
         {
         if (constsynthval)
           {
           tempvar.type='r';
           tempvar.name=rp->name;
           tempvar.value=constsynthval;
           sd.registervar(tempvar);
           }
         else if(!(p=="none"))
           {
           tempvar.type='r';
           tempvar.name=rp->name;
           tempvar.function=p;
           sd.registerfunc(tempvar);
           }
         }

      rp->Dirty = true;
      rp->Output = output;

      if ((rp->OutputRate = outputRate) == Config::SampleRate)
        rp->OutputRate = rp->SampleRate;

      /* PMS1D don't go to 25hz, keep them back at 10. */
      if ((rp->ProbeType & PROBE_PMS1D) && outputRate == Config::HighRate)
        rp->OutputRate = rp->SampleRate;

      rp->StaticLag = lag;
      rp->SpikeSlope = spike;
      rp->DataQuality = dq;
      rp->cof.clear();

      for (int i = MAX_COF-1; i >= 0; --i)
        {
        p = XmTextFieldGetString(ev_text[i]);
        f = atof(p);
        XtFree(p);

        if (f == 0.0 && rp->cof.size() == 0)
          continue;

        rp->cof.insert(rp->cof.begin(), f);
        }

      newAttr = CreateListLineItem((void *)rp, RAW);
      break;

    case DERIVED:
      dp->Dirty = true;
      dp->Output = output;
      dp->OutputRate = outputRate;
      dp->DataQuality = dq;

      /* PMS1D don't go to 25hz, keep them back at 10. */
      if ((dp->ProbeType & PROBE_PMS1D) && outputRate == Config::HighRate)
        dp->OutputRate = dp->Default_HR_OR;

      for (size_t i = 0; i < dp->ndep; ++i)
        {
        p = XmTextFieldGetString(ev_text[i]);
        strcpy(dp->depend[i], p);
        DependIndexLookup(dp, i);
        XtFree(p);
        }

      newAttr = CreateListLineItem((void *)dp, DERIVED);
      break;
    }

  XmListReplaceItemsPos(list1, &newAttr, 1, ListPosition);
  XmListSelectPos(list1, ListPosition, false);

}	/* END APPLYVARIABLEMODS */

/* -------------------------------------------------------------------- */
void DismissEditWindow(Widget w, XtPointer client, XtPointer call)
{
  XtPopdown(XtParent(EditWindow));
  XtUnmanageChild(EditWindow);
}

/* -------------------------------------------------------------------- */
void VerifyLagText(Widget w, XtPointer client, XmTextVerifyCallbackStruct *call)
{
  for (int i = 0; i < call->text->length; ++i)
    if (!isdigit(call->text->ptr[i]))
      {
      call->doit = false;
      return;
      }

}	/* END VERIFYLAGTEXT */

/* -------------------------------------------------------------------- */
static void set_edit_window_data(
		char	name[],
		bool	output,
		int	outputRate,
		int	lag,
		NR_TYPE	spike,
		char	*dq)
{
  int		pos, n;
  Arg		args[3],sarg[1];
  XmString	ns;

  strcpy(buffer, name);
  strcat(buffer, " - ");
  strcat(buffer, VarDB_GetTitle(name));
  ns = XmStringCreateLocalized(buffer);

  n = 0;
  XtSetArg(args[n], XmNlabelString, ns); n++;
  XtSetValues(varNameLabel, args, n);
  XmStringFree(ns);
 



  XmToggleButtonSetState(outputVarYes, output, false);
  XmToggleButtonSetState(outputVarNo, !output, false);

  XmToggleButtonSetState(outputLRbutton, false, false);
  XmToggleButtonSetState(outputSRbutton, false, false);
  XmToggleButtonSetState(outputHRbutton, false, false);

  switch (outputRate)
    {
    case Config::LowRate:
      XmToggleButtonSetState(outputLRbutton, true, false);
      break;

    case Config::HighRate:
      XmToggleButtonSetState(outputHRbutton, true, false);
      break;

    default:
      /* Cheesy hack to determnine if derived or raw.  Derived don't have
       * SampleRate....
       */
      if (lag == ERR)
        XmToggleButtonSetState(outputHRbutton, true, false);
      else
        XmToggleButtonSetState(outputSRbutton, true, false);
      break;
    }


  ns = XmStringCreateLocalized(SearchDataQuality(dq));
  XtSetArg(args[0], XmNlabelString, ns);
  XtSetValues(XmOptionButtonGadget(slOpMenu), args, 1);
  XmStringFree(ns);
 
  ns=XmStringCreateLocalized(func[0]);   //sets the function to none
  XtSetArg(sarg[0],XmNlabelString,ns);
  XtSetValues(XmOptionButtonGadget(funcOpMenu),sarg,1);
  XmStringFree(ns);

  XmTextFieldSetString(synthConstText, "");     //display nothing in this field each time editvariable is clicked


  if (VariableType == DERIVED)
    {
    XmTextFieldSetString(lagText, "NA");
    XmTextFieldSetString(spikeText, "NA");
    XtSetSensitive(lagText, false);
    XtSetSensitive(spikeText, false);
    XtSetSensitive(outputSRbutton, false);
    XmTextFieldSetString(synthConstText,"NA");
    XtSetSensitive(synthConstText,false);
    XtSetSensitive(funcOpMenu,false);
    }
  else
    {
    XtSetSensitive(lagText, true);
    XtSetSensitive(spikeText, true);
    XtSetSensitive(outputSRbutton, true);
    XtSetSensitive(funcOpMenu,true);    //for some reason a segmentation error occures here on some occasions
    XtSetSensitive(synthConstText,true);
   
    sprintf(buffer, "%d", lag);
    XmTextFieldSetString(lagText, buffer);
    pos = XmTextFieldGetLastPosition(lagText);
    XmTextFieldSetInsertionPosition(lagText, pos);

    sprintf(buffer, "%.2e", spike);
    XmTextFieldSetString(spikeText, buffer);
    pos = XmTextFieldGetLastPosition(spikeText);
    XmTextFieldSetInsertionPosition(spikeText, pos);
    }

}	/* END SETEDITVAR */

/* -------------------------------------------------------------------- */
void MapCalCof(Widget w, XtPointer client, XtPointer call)
{
  char	*p;

  p = XmTextFieldGetString(w);

  if (VariableType == DERIVED)
    {
    strupr(p);
    XmTextFieldSetString(w, p);
    }
  else
    {
    sprintf(buffer, "%e", atof(p));
    XmTextFieldSetString(w, buffer);
    }

  XtFree(p);

}	/* END MAPCALCOE */

/* -------------------------------------------------------------------- */
void CreateEditWindow()
{
  int	i;
  Arg           args[16];
  Cardinal      n;
  Widget        frame[12];
  Widget        obRC, radioBox;
  Widget        form, label;
  Widget        cbRC, cmdBtn[3];
  Widget        evRC[2];
  Widget        slPD, slButts[15],funcPD,funcButts[19];
  XmString      name;

  extern Widget AppShell, Shell001;

  //**************Definition of FuncPD******************//
  func[0]="none";
  func[1]="abs";
  func[2]="acos";
  func[3]="asin";
  func[4]="atan";
  func[5]="ceil";
  func[6]="cos";
  func[7]="cosh";
  func[8]="exp";
  func[9]="fabs";
  func[10]="floor";
  func[11]="ln";
  func[12]="log";
  func[13]="pow";
  func[14]="sin";
  func[15]="sinh";
  func[16]="sqrt";
  func[17]="tan";
  func[18]="tanh";
 //***************************************************//


  n = 0;
  XtSetArg(args[n], XmNtitle, "Edit Variable"); n++;
  XtSetArg(args[n], XmNiconName, "VarEdit"); n++;
  XtSetArg(args[n], XmNtransientFor, Shell001); n++;
  EditShell = XtCreatePopupShell("editVariableShell",
              transientShellWidgetClass, AppShell, args, n);

  n = 0;
  EditWindow = XmCreateForm(EditShell, "editForm", args, n);

  /* Create all frame's at once.
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  frame[0] = XmCreateFrame(EditWindow, "varNameFrame", args, n);
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopWidget, frame[0]); n++;
  frame[1] = XmCreateFrame(EditWindow, "outputFrame", args, n);
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopWidget, frame[1]); n++;
  frame[2] = XmCreateFrame(EditWindow, "outRateFrame", args, n);
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopWidget, frame[2]); n++;
  frame[3] = XmCreateFrame(EditWindow, "lagFrame", args, n);
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopWidget, frame[3]); n++;
  frame[4] = XmCreateFrame(EditWindow, "spikeFrame", args, n);
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopWidget, frame[4]); n++;
  frame[8] = XmCreateFrame(EditWindow, "synthConstFrame", args, n);
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopWidget, frame[8]); n++;
  frame[5] = XmCreateFrame(EditWindow, "buttonFrame", args, n);
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, frame[0]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, frame[1]); n++;
  frame[6] = XmCreateFrame(EditWindow, "evTextFrame1", args, n);
  n = 0;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, frame[6]); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNbottomWidget, frame[8]); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, frame[4]); n++;
  frame[7] = XmCreateFrame(EditWindow, "dqFrame", args, n);
   n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget,frame[8]); n++;
  XtSetArg(args[n], XmNtopWidget, frame[4]); n++;
  frame[9] = XmCreateFrame(EditWindow, "funcmenuFrame", args, n);

  XtManageChildren(frame, 10);

  /* Var Name TF.
   */
  n = 0;
  varNameLabel = XmCreateLabel(frame[0], "varNameLabel", args, n);
  XtManageChild(varNameLabel);


  /* Output Box.
   */
  n = 0;
  obRC = XmCreateRowColumn(frame[1], "outputRC", args, n);
  XtManageChild(obRC);

  n = 0;
  label = XmCreateLabel(obRC, "outputLabel", args, n);
  XtManageChild(label);

  n = 0;
  radioBox = XmCreateRadioBox(obRC, "outputRB", args, n);
  XtManageChild(radioBox);

  n = 0;
  outputVarYes = XmCreateToggleButton(radioBox, "outputVarYes", args,n);
  XtManageChild(outputVarYes);

  n = 0;
  outputVarNo = XmCreateToggleButton(radioBox, "outputVarNo", args, n);
  XtManageChild(outputVarNo);


  /* OutputRate Box.
   */
  n = 0;
  obRC = XmCreateRowColumn(frame[2], "outRateRC", args, n);
  XtManageChild(obRC);

  n = 0;
  label = XmCreateLabel(obRC, "outRateLabel", args, n);
  XtManageChild(label);

  n = 0;
  radioBox = XmCreateRadioBox(obRC, "outRateRB", args, n);
  XtManageChild(radioBox);

  n = 0;
  outputLRbutton = XmCreateToggleButton(radioBox, "outputLR", args, n);
  XtManageChild(outputLRbutton);

  n = 0;
  outputSRbutton = XmCreateToggleButton(radioBox, "outputSR", args, n);
  XtManageChild(outputSRbutton);

  n = 0;
  outputHRbutton = XmCreateToggleButton(radioBox, "outputHR", args, n);
  XtManageChild(outputHRbutton);


  /* Lag Text.
   */
  n = 0;
  form = XmCreateForm(frame[3], "lagForm", args, n);
  XtManageChild(form);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  label = XmCreateLabel(form, "lagLabel", args, n);
  XtManageChild(label);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, label); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  lagText = XmCreateTextField(form, "lagText", args, n);
  XtManageChild(lagText);


  /* Spike Slope Text.
   */
  n = 0;
  form = XmCreateForm(frame[4], "spikeForm", args, n);
  XtManageChild(form);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  label = XmCreateLabel(form, "spikeLabel", args, n);
  XtManageChild(label);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, label); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  spikeText = XmCreateTextField(form, "spikeText", args, n);
  XtManageChild(spikeText);


  /* Synthetic Constant Text
   */
  n = 0;
  form = XmCreateForm(frame[8], "synthConstForm", args, n);
  XtManageChild(form);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  label = XmCreateLabel(form, "Constant Value", args, n);
  XtManageChild(label);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, label); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  synthConstText = XmCreateTextField(form, "synthConstText", args, n);
  XtManageChild(synthConstText);


  /* Command buttons.
   */
  n = 0;
  cbRC = XmCreateRowColumn(frame[5], "buttonRC", args, n);
  XtManageChild(cbRC);

  n = 0;
  cmdBtn[0] = XmCreatePushButton(cbRC, "applyButton", args, n);
  cmdBtn[1] = XmCreatePushButton(cbRC, "resetButton", args, n);
  cmdBtn[2] = XmCreatePushButton(cbRC, "dismissButton", args, n);

  XtAddCallback(cmdBtn[0], XmNactivateCallback, ApplyVariableMods, NULL);
  XtAddCallback(cmdBtn[1], XmNactivateCallback, (XtCallbackProc)EditVariable,NULL);
  XtAddCallback(cmdBtn[2], XmNactivateCallback, DismissEditWindow, NULL);
  XtManageChildren(cmdBtn, 3);


  n = 0;
  label = XmCreateLabel(frame[6], "evTextTitle", args, n);
  XtManageChild(label);

  n = 0;
  evRC[0] = XmCreateRowColumn(frame[6], "evTextRC1", args, n);
  XtManageChild(evRC[0]);

  n = 0;

  for (i = 0; i < MAXDEPEND; ++i)
    {
    sprintf(buffer, "ev_text%d", i);
    ev_text[i] = XmCreateTextField(evRC[0], buffer, args, n);

    XtAddCallback(ev_text[i], XmNlosingFocusCallback, MapCalCof, NULL);
    XtAddCallback(ev_text[i], XmNactivateCallback, (XtCallbackProc)NextWidget,(XtPointer)i);
    }

  XtManageChildren(ev_text, MAXDEPEND);


  /* Data Quality Op Menu.
   */
  n = 0;
  slPD = XmCreatePulldownMenu(frame[7], "slPullDown", args, n);

  name = XmStringCreateLocalized("Data Quality");
  n = 0;
  XtSetArg(args[n], XmNlabelString, name); ++n;
  XtSetArg(args[n], XmNsubMenuId, slPD); ++n;
  slOpMenu = XmCreateOptionMenu(frame[7], "slOpMenu", args, n);
  XtManageChild(slOpMenu);
  XmStringFree(name);


  for (i = 0; dataQuality[i]; ++i)
    {
    name = XmStringCreateLocalized(dataQuality[i]);

    n = 0;
    XtSetArg(args[n], XmNlabelString, name); ++n;
    slButts[i] = XmCreatePushButton(slPD, "opMenB", args, n);
/*
    XtAddCallback(slButts[i], XmNactivateCallback, SetDataQuality,
                  (XtPointer)dataQuality[i]);
*/
    XmStringFree(name);
    }

  XtManageChildren(slButts, i);


  /* Functions menu*/

  n=0;
  funcPD=XmCreatePulldownMenu(frame[9],"funcPD",args,n);

  name=XmStringCreateLocalized("Functions");
  n=0;
  XtSetArg(args[n],XmNlabelString,name);++n;
  XtSetArg(args[n],XmNsubMenuId,funcPD);++n;
  funcOpMenu=XmCreateOptionMenu(frame[9],"Function Menu",args,n);
  XtManageChild(funcOpMenu);
  XmStringFree(name);

  for (i = 0; i < 18; ++i)
  {
    name=XmStringCreateLocalized(func[i]);

    n=0;
    XtSetArg(args[n],XmNlabelString,name);++n;
    funcButts[i]=XmCreatePushButton(funcPD,"Function Menu",args,n);

    XmStringFree(name);
  }

  XtManageChildren(funcButts,i);

}       /* END CREATEEDITWINDOW */

/* END CB_EDIT.C */
