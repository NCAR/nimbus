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

#include <Xm/List.h>
#include <Xm/RowColumn.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>

#include "nimbus.h"
#include "decode.h"
#include "gui.h"
#include "vardb.h"
#include "injectsd.h"


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

extern Widget slOpMenu,funcOpMenu;

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

      for (i = 0; i < sp->order; ++i)
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

      for (i = 0; i < rp->order; ++i)
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
  size_t	i;
  bool		output;
  int		outputRate, lag;
  char		*p, *dq;
  float		f;
  float		constsynthval;
  NR_TYPE	spike;
  Arg		args[3];
  XmString	newAttr;
  struct v	tempvar; 

  output = XmToggleButtonGetState(outputVarYes);

  if (XmToggleButtonGetState(outputLRbutton))
    outputRate = LOW_RATE;
  else
  if (XmToggleButtonGetState(outputSRbutton))
    outputRate = SAMPLE_RATE;
  else
  if (XmToggleButtonGetState(outputHRbutton))
    outputRate = HIGH_RATE;
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
      sp->order = 0;

      for (i = MAX_COF-1; i >= 0; --i)
        {
        p = XmTextFieldGetString(ev_text[i]);
        f = atof(p);
        XtFree(p);

        if (f == 0.0 && sp->order == 0)
          continue;

        sp->cof[i] = f;
        ++sp->order;
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

      if ((rp->OutputRate = outputRate) == SAMPLE_RATE)
        rp->OutputRate = rp->SampleRate;

      /* PMS1D don't go to 25hz, keep them back at 10. */
      if ((rp->ProbeType & PROBE_PMS1D) && outputRate == HIGH_RATE)
        rp->OutputRate = rp->SampleRate;

      rp->StaticLag = lag;
      rp->SpikeSlope = spike;
      rp->DataQuality = dq;

      for (i = 0; i < rp->order; ++i)
        {
        p = XmTextFieldGetString(ev_text[i]);
        rp->cof[i] = atof(p);
        XtFree(p);
        }

      newAttr = CreateListLineItem((void *)rp, RAW);
      break;

    case DERIVED:
      dp->Dirty = true;
      dp->Output = output;
      dp->OutputRate = outputRate;
      dp->DataQuality = dq;

      /* PMS1D don't go to 25hz, keep them back at 10. */
      if ((dp->ProbeType & PROBE_PMS1D) && outputRate == HIGH_RATE)
        dp->OutputRate = dp->Default_HR_OR;

      for (i = 0; i < dp->ndep; ++i)
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
    case LOW_RATE:
      XmToggleButtonSetState(outputLRbutton, true, false);
      break;

    case HIGH_RATE:
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

/* END CB_EDIT.C */
