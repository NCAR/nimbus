/*
-------------------------------------------------------------------------
OBJECT NAME:	DataSource.cc
 
FULL NAME:	Data source & type widgets
 
DESCRIPTION:	X window part for selecting file, probe & type (counts vs.
		concentrations).
 
COPYRIGHT:	University Corporation for Atmospheric Research, 1997
-------------------------------------------------------------------------
*/

#include "DataSource.h"
#include "FileMgr.h"
#include "SetMgr.h"

#include <Xm/ToggleB.h>

extern FileManager	fileMgr;
extern SetManager	setMgr;


/* -------------------------------------------------------------------- */
void DataSource::CreateDataSourceWidgets(Widget fileParent, Widget probeParent, Widget typeParent, Widget computeParent)
{
  Widget	compute[2];

  /* File Toggle Buttons.
   */
  for (int i = 0; i < MAX_DATAFILES; ++i)
    {
    _fileB[i] = XmCreateToggleButton(fileParent, (char *)"none", NULL, 0);
    XtAddCallback(_fileB[i], XmNvalueChangedCallback,
                  (XtCallbackProc)SetCurrentFile, (XtPointer)i);
    }
 
  XtManageChildren(_fileB, MAX_DATAFILES);
 
 
  /* Probe Toggle Buttons.
   */
  for (int i = 0; i < MAX_PROBES; ++i)
    {
    _probeB[i] = XmCreateToggleButton(probeParent, (char *)"none        ", NULL, 0);
    XtAddCallback(_probeB[i], XmNvalueChangedCallback,
                  (XtCallbackProc)ModifyActiveProbes, (XtPointer)i);
    }
 
  XtManageChildren(_probeB, MAX_PROBES);


  /* Accum vs. Conc, etc
   */
  _dataType[0] = XmCreateToggleButton(typeParent, (char *)"Raw Counts", NULL, 0);
  _dataType[1] = XmCreateToggleButton(typeParent, (char *)"Concentrations", NULL, 0);
  _dataType[2] = XmCreateToggleButton(typeParent, (char *)"Surface area", NULL, 0);
  _dataType[3] = XmCreateToggleButton(typeParent, (char *)"Volume", NULL, 0);
  XtManageChildren(_dataType, 4);
  XmToggleButtonSetState(_dataType[1], True, True);

  for (int i = 0; i < 4; ++i)
    XtAddCallback(_dataType[i], XmNvalueChangedCallback,
          (XtCallbackProc)ModifyActiveDataTypes, NULL);
 
  compute[0] = XmCreateToggleButton(computeParent, (char *)"From file     ", NULL, 0);
  compute[1] = XmCreateToggleButton(computeParent, (char *)"Computed", NULL, 0);
  XtManageChildren(compute, 2);

  XtAddCallback(compute[0], XmNvalueChangedCallback,
          (XtCallbackProc)ToggleCompute, (XtPointer)False);
  XtAddCallback(compute[1], XmNvalueChangedCallback,
          (XtCallbackProc)ToggleCompute, (XtPointer)True);

  XmToggleButtonSetState(compute[0], True, False);

}	/* END CREATEWIDGETS */

/* -------------------------------------------------------------------- */
void DataSource::SetFileNames()
{
  int		i;
  XmString	label;
  Arg		args[2];

  for (i = 0; i < fileMgr.NumberOfFiles(); ++i)
    {
    XtSetSensitive(_fileB[i], True);
 
    label = XmStringCreate((char *)fileMgr._dataFile[i]->FileName().c_str(),
                           XmFONTLIST_DEFAULT_TAG);
    XtSetArg(args[0], XmNlabelString, label);
    XtSetValues(_fileB[i], args, 1);
    XmStringFree(label);
 
    if (fileMgr._dataFile[i] == fileMgr.CurrentFile())
      XmToggleButtonSetState(_fileB[i], True, True);
    }
 
  for (; i < MAX_DATAFILES; ++i)
    {
    XtSetSensitive(_fileB[i], False);
 
    label = XmStringCreate((char *)"none", XmFONTLIST_DEFAULT_TAG);
    XtSetArg(args[0], XmNlabelString, label);
    XtSetValues(_fileB[i], args, 1);
    XmStringFree(label);
    }

}	/* END SETFILENAMES */

/* -------------------------------------------------------------------- */
void DataSource::SetProbeNames()
{
  int		i = 0;
  XmString	label;
  Arg		args[2];
  DataSet	*set;

  if (fileMgr.CurrentFile())
    {
    DataFile	*currFile = fileMgr.CurrentFile();

    for (i = 0; i < currFile->NumberOfProbes(); ++i)
      {
      XtSetSensitive(_probeB[i], True);

      label = XmStringCreate((char *)currFile->probe[i]->Name().c_str(),
                             XmFONTLIST_DEFAULT_TAG);
      XtSetArg(args[0], XmNlabelString, label);
      XtSetValues(_probeB[i], args, 1);
      XmStringFree(label);

      XmToggleButtonSetState(_probeB[i], False, False);

      for (set = setMgr.FirstSet(); set; set = setMgr.NextSet())
        if (*(set->file()) == *currFile && set->probe() == currFile->probe[i])
          XmToggleButtonSetState(_probeB[i], True, False);
      }
    }
 
  for (; i < MAX_PROBES; ++i)
    {
    XtSetSensitive(_probeB[i], False);
 
    label = XmStringCreate((char *)"none", XmFONTLIST_DEFAULT_TAG);
    XtSetArg(args[0], XmNlabelString, label);
    XtSetValues(_probeB[i], args, 1);
    XmStringFree(label);

    XmToggleButtonSetState(_probeB[i], False, False);
    }

}	/* END SETPROBENAMES */

/* -------------------------------------------------------------------- */
DataType DataSource::ActiveDataTypes()
{
  int	i;
  DataType dt = 0;

  for (i = 0; i < 4; ++i)
    if (XmToggleButtonGetState(_dataType[i]))
      dt |= (DataType)(0x01 << i);

  return(dt);

}	/* END ACTIVEDATATYPES */

/* END DATASOURCE.CC */
