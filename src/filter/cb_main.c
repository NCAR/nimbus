/*
-------------------------------------------------------------------------
OBJECT NAME:	cb_main.c

FULL NAME:	Command CallBacks

ENTRY POINTS:	CancelSetup()
		CreateListLineItem()
		FillListWidget()
		LoadSetup()
		LogMessage()
		PrintSetup()
		Proceed()
		Quit()
		SaveSetup()
		StartProcessing()
		ToggleOutput()
		ToggleRate()
		ToggleProbe()
		ValidateOutputFile()

STATIC FNS:	checkForProductionSetup()
		displaySetupWindow()
		readHeader()
		setOutputFileName()
		stopProcessing()
		validateInputFile()

DESCRIPTION:	Contains callbacks for the nimbus GUI main window & setup
		window.

INPUT:			

OUTPUT:		

REFERENCES:	Everything.

REFERENCED BY:	XtAppMainLoop()

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2005
-------------------------------------------------------------------------
*/

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/stat.h>

#include <Xm/List.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>

#include "nimbus.h"
#include "decode.h"
#include "ctape.h"
#include "gui.h"
#include "vardb.h"
#include "psql.h"
#include "version.h"


static char	ADSfileName[MAXPATHLEN];
char		OutputFileName[MAXPATHLEN];  /* Export to xlate/rdma.c */
static char	list1lineFrmt[] = "%-12s %c%c   %4d  %4d   %4d  %8.3f   %c";

extern FILE	*LogFile;

/* For wall clock timing	*/
static time_t	startWALL, finishWALL;
static clock_t	startCPU, finishCPU;

extern Widget	Shell001;
extern size_t	nDefaults;
extern DEFAULT	*Defaults[];

void	CloseSQL();
void	ValidateOutputFile(Widget w, XtPointer client, XtPointer call);
static int	validateInputFile();

static void	checkForProductionSetup(), displaySetupWindow(),
		setOutputFileName(), readHeader(), stopProcessing(),
		EngageSignals();

void	OpenLogFile(), InitAsyncModule(char fileName[]), RealTimeLoop(),
	CloseLogFile(), LogDespikeInfo(), InitAircraftDependencies(),
	CloseRemoveLogFile(), LogIRSerrors();


/* -------------------------------------------------------------------- */
void CancelSetup(Widget w, XtPointer client, XtPointer call)
{
  size_t	i;

  CloseADSfile();

  for (i = 0; i < sdi.size(); ++i)
    delete sdi[i];

  for (i = 0; i < raw.size(); ++i)
    delete raw[i];

  for (i = 0; i < derived.size(); ++i)
    delete derived[i];

  for (i = 0; i < nDefaults; ++i)
    {
    delete [] Defaults[i]->Value;
    delete Defaults[i];
    }

  FreeDataArrays();
  ReleaseFlightHeader();
  delete [] ProjectName;


  DismissEditWindow(NULL, NULL, NULL);
  DismissConfigWindow(NULL, NULL, NULL);

  XtPopdown(XtParent(SetupWindow));
  XtUnmanageChild(SetupWindow);

  XtSetSensitive(readHeaderButton, true);
  XtSetSensitive(aDSdataText, true);
  XtSetSensitive(outputFileText, true);

  ReleaseVarDB();
  ResetProbeList();
  Initialize();

}	/* END CANCELSETUP */

/* -------------------------------------------------------------------- */
void Proceed(Widget w, XtPointer client, XtPointer call)
{
  char	*p;

  if (w)
    {
    ExtractFileName(((XmFileSelectionBoxCallbackStruct *)call)->value, &p);
    strcpy(ADSfileName, p);
    XmTextFieldSetString(aDSdataText, p);

    if (validateInputFile() == OK)
      {
      FileCancel((Widget)NULL, (XtPointer)NULL, (XtPointer)NULL);
      readHeader();
      displaySetupWindow();
      }
    }
  else
    {
    p = XmTextFieldGetString(aDSdataText);
    strcpy(ADSfileName, p);
    XtFree(p);

    p = XmTextFieldGetString(outputFileText);
    strcpy(OutputFileName, p);
    XtFree(p);

    if (validateInputFile() == OK)
      {
      readHeader();
      ValidateOutputFile(NULL, NULL, NULL);
      }
    }

}	/* END PROCEED */

/* -------------------------------------------------------------------- */
static void displaySetupWindow()
{
  XtManageChild(SetupWindow);
  XtPopup(XtParent(SetupWindow), XtGrabNone);

}	/* END DISPLAYSETUPWINDOW */

/* -------------------------------------------------------------------- */
static void readHeader()
{
  XtSetSensitive(readHeaderButton, false);
  XtSetSensitive(aDSdataText, false);
  XtSetSensitive(outputFileText, false);

  XmUpdateDisplay(Shell001);
  LogMessage(NIMBUS_VERSION);

  if (DecodeHeader(ADSfileName) == ERR) {
    CancelSetup(NULL, NULL, NULL);
    return;
    }

  if (FlightNumberInt == 0)
    HandleWarning("Flight Number is 0, a new one may be entered\nvia the 'Edit/Flight Info' menu item,\nor run fixFltNum on the ADS image and start nimbus again.", NULL, NULL);

  if (cfg.ProductionRun())
    OpenLogFile();

  sprintf(buffer, VARDB, ProjectDirectory, ProjectNumber);
  if (InitializeVarDB(buffer) == ERR) {
    LogMessage("InitializeVarDB for project specific failed, trying master file.\n");

    sprintf(buffer, VARDB, ProjectDirectory, "defaults");
    if (InitializeVarDB(buffer) == ERR) {
      fprintf(stderr, "InitializeVarDB for master file failed, this is fatal.\n");
      exit(1);
      }
    }

  if (cfg.Interactive())
    {
    Arg		args[1];

    CreateProbeMenu();
    FillListWidget();

    sprintf(buffer, "%s - %s, Flight %s",
		ProjectName, ProjectNumber, FlightNumber);

    XtSetArg(args[0], XmNtitle, buffer);
    XtSetValues(Shell001, args, 1);

    if (LogFile)
      {
      extern Widget logText;
      char	*p = XmTextGetString(logText);

      strcat(buffer, "\n");
      fprintf(LogFile, buffer);

      fprintf(LogFile, p);
      XtFree(p);
      }

    checkForProductionSetup();
    }
  else
    {
    if (cfg.ProcessingRate() == Config::HighRate)
      SetHighRate(NULL, NULL, NULL);

    sprintf(buffer, "%s - %s, Flight %s\n",
		ProjectName, ProjectNumber, FlightNumber);

    LogMessage(buffer);

    checkForProductionSetup();
    LoadSetup_OK(Shell001, NULL, NULL); /* Fake it with any widget name */
    }

  /* Scan VarDB for any non-existent variables.
   */
{
  FILE	*ofp = LogFile ? LogFile : stderr;
  size_t	i;

  for (i = 0; i < sdi.size(); ++i)
    if (VarDB_lookup(sdi[i]->name) == ERR)
      fprintf(ofp, "%s has no description or units.\n", sdi[i]->name);

  for (i = 0; i < raw.size(); ++i)
    if (VarDB_lookup(raw[i]->name) == ERR)
      fprintf(ofp, "%s has no description or units.\n", raw[i]->name);

  for (i = 0; i < derived.size(); ++i)
    if (VarDB_lookup(derived[i]->name) == ERR)
      fprintf(ofp, "%s has no description or units.\n", derived[i]->name);
}

}	/* END READHEADER */

/* -------------------------------------------------------------------- */
void StartProcessing(Widget w, XtPointer client, XtPointer call)
{
  XmString	label;
  Arg		args[1];
  int		rc = 0;
  long		btim, etim;

  DismissEditWindow(NULL, NULL, NULL);
  DismissConfigWindow(NULL, NULL, NULL);
  XtSetSensitive(list1, false);
  XtSetSensitive(menuBar, false);

  CleanOutUnwantedVariables();
  AllocateDataArrays();
  GenerateComputeOrder();	/* for Derived variables		*/
  GetUserTimeIntervals();

  if (cfg.ProductionRun())	/* Do it again in case Flight# changed	*/
    setOutputFileName();


  /* RunAMLIBinits before creating netCDF to setup defaults into
   * netCDF attributes.
   */
  RunAMLIBinitializers();
  CreateNetCDF(OutputFileName);
  InitAsyncModule(OutputFileName);

  /* This needs to be after CreateNetCDF, so that FlightDate is initialized.
   */
  InitAircraftDependencies();

  {
  size_t i;
  bool	firstSpike = true;

  for (i = 0; i < sdi.size(); ++i) {
    if (sdi[i]->StaticLag != 0)
      AddVariableToSDIlagList(sdi[i]);

    if (sdi[i]->SpikeSlope > 0) {
      AddVariableToSDIdespikeList(sdi[i]);

      if (firstSpike) {
        LogMessage("Despiking enabled.\n");
        firstSpike = false;
        }
      }

    if (sdi[i]->Output && VarDB_lookup(sdi[i]->name) == ERR && LogFile)
      fprintf(LogFile, "%s has no entry in the VarDB.\n", sdi[i]->name);
    }

  for (i = 0; i < raw.size(); ++i) {
    if (raw[i]->StaticLag != 0 || raw[i]->DynamicLag != 0)
      AddVariableToRAWlagList(raw[i]);

    if (raw[i]->SpikeSlope > 0) {
      AddVariableToRAWdespikeList(raw[i]);

      if (firstSpike) {
        LogMessage("Despiking enabled.\n");
        firstSpike = false;
        }
      }

    if (raw[i]->Output && VarDB_lookup(raw[i]->name) == ERR && LogFile)
      fprintf(LogFile, "%s has no entry in the VarDB.\n", raw[i]->name);
    }

  for (i = 0; i < derived.size(); ++i) {
    if (derived[i]->Output && VarDB_lookup(derived[i]->name) == ERR && LogFile)
      fprintf(LogFile,"%s has no entry in the VarDB.\n", derived[i]->name);
    }
  }

  FlushXEvents();

  if (cfg.ProcessingRate() == Config::HighRate)
    InitMRFilters();

  /* Turn "Go" button into "Pause" button.
   */
  XtRemoveAllCallbacks(goButton, XmNactivateCallback);
  label = XmStringCreate("Pause", XmFONTLIST_DEFAULT_TAG);
  XtSetArg(args[0], XmNlabelString, label);
  XtSetValues(goButton, args, 1);
  XmStringFree(label);
  XtAddCallback(goButton, XmNactivateCallback, PauseProcessing, NULL);

  FlushXEvents();

  EngageSignals();

  if (cfg.ProcessingMode() == Config::RealTime)
    {
    NextTimeInterval(&btim, &etim);
    RealTimeLoop();	/* Never to return	*/
    exit(0);
    }


  startWALL	= time(NULL);
  startCPU	= clock();

  while (NextTimeInterval(&btim, &etim))
    {
    switch (cfg.ProcessingRate())
      {
      case Config::LowRate:
        rc = LowRateLoop(btim, etim);
        break;

      case Config::HighRate:
        rc = HighRateLoop(btim, etim);
        break;
      }

    if (PauseWhatToDo == P_QUIT || rc == ERR)
      break;
    }

  finishWALL = time(NULL);
  finishCPU  = clock();

  BlankOutBadData();
  stopProcessing();

}	/* END STARTPROCESSING */

/* -------------------------------------------------------------------- */
void stopProcessing()
{
  XmString	label;
  Arg		args[1];
  float		x;

  CloseNetCDF();

  LogDespikeInfo();
  LogIRSerrors();

  /* Log wall clock time.
   */
  x = (float)(finishWALL - startWALL) / 60.0;

  if (x > 60.0)
    sprintf(buffer, "Processing took %.2f hour(s).\n", x / 60.0);
  else
    sprintf(buffer, "Processing took %.1f minutes.\n", x);

  LogMessage(buffer);

/*
  x = (float)(finishCPU - startCPU) / CLOCKS_PER_SEC / 60.0;
  sprintf(buffer, "CPU time took %.1f minutes.\n", x);
  LogMessage(buffer);
*/

  CloseLogFile();

  /* Turn "Pause" button back into "Go" button.
   */
  label = XmStringCreate("Quit", XmFONTLIST_DEFAULT_TAG);
  XtSetArg(args[0], XmNlabelString, label);
  XtSetValues(goButton, args, 1);
  XmStringFree(label);
  XtRemoveAllCallbacks(goButton, XmNactivateCallback);
  XtAddCallback(goButton, XmNactivateCallback, Quit, NULL);
  XtSetSensitive(goButton, true);

  XtSetSensitive(menuBar, true);
  XtSetSensitive(list1, true);

}	/* END STOPPROCESSING */

/* -------------------------------------------------------------------- */
void LoadSetup(Widget w, XtPointer client, XtPointer call)
{
  sprintf(buffer, "%s/nimbus/*", getenv("HOME"));
  QueryFile("Enter file name to load:", buffer, (XtCallbackProc)LoadSetup_OK);

}	/* END LOADSETUP */

/* -------------------------------------------------------------------- */
void  LoadSynthetic(Widget w, XtPointer client, XtPointer call)
{
  sprintf(buffer, "%s/nimbus/*", getenv("HOME"));
  QueryFile("Enter synthetic data file name to load:", buffer, (XtCallbackProc)LoadSynthetic_OK);

}	/* END LOADSYNTHETIC */

/* -------------------------------------------------------------------- */
static void checkForProductionSetup()
{
  int	i, revision2 = False;
  char	*group[256];

  if (cfg.LoadProductionSetup() == false)
    return;


  /* Check for Flight Groups
   */
  ReadTextFile(GROUPS, group);

  for (i = 0; group[i]; ++i)
    {
    if (strlen(group[i]) < (size_t)4)
      continue;

    if (strcmp(group[i], "Revision 2") == 0)
      {
      revision2 = True;
      continue;
      }

    if (!revision2)
      {
      if (atoi(group[i]) == FlightNumberInt)
        {
        sprintf(buffer, "%s/%s/Production/Flight_%d", ProjectDirectory,
		ProjectNumber, atoi(strrchr(group[i], '=')+1));

        LoadSetup_OK(NULL, NULL, NULL);

        break;
        }
      }
    else
      {
      if (strncmp(group[i], FlightNumber, 4) == 0)
        {
        sprintf(buffer, "%s/%s/Production/Flight_%s", ProjectDirectory,
		ProjectNumber, strrchr(group[i], '=')+2);

        LoadSetup_OK(NULL, NULL, NULL);

        break;
        }
      }
    }

  FreeTextFile(group);


  /* Check for Production File
   */
  if (!revision2)
    sprintf(buffer, "%s/%s/Production/Flight_%d",
		ProjectDirectory, ProjectNumber, FlightNumberInt);
  else
    sprintf(buffer, "%s/%s/Production/Flight_%s",
		ProjectDirectory, ProjectNumber, FlightNumber);

  LoadSetup_OK(NULL, NULL, NULL);

}	/* END CHECKFORPRODUCTIONSETUP */

/* -------------------------------------------------------------------- */
void SaveSetup(Widget w, XtPointer client, XtPointer call)
{
  if (cfg.ProductionRun())
    {
    sprintf(buffer, "%s/%s/Production", ProjectDirectory, ProjectNumber);

    mkdir(buffer, 0775);

    sprintf(buffer, "%s/%s/Production/Flight_%s",
			ProjectDirectory, ProjectNumber, FlightNumber);

    SaveSetup_OK(NULL, NULL, NULL);
    }
  else
    {
    sprintf(buffer, "%s/nimbus/*", getenv("HOME"));
    QueryFile("Enter file name to save setup to:", buffer,
		    (XtCallbackProc)SaveSetup_OK);
    }

}	/* END SAVESETUP */

/* -------------------------------------------------------------------- */
void ToggleRate(Widget w, XtPointer client, XtPointer call)
{
  int		*pos_list, pos_cnt = 0;
  XmString	item;

  if (XmListGetSelectedPos(list1, &pos_list, &pos_cnt) == false)
    return;

  for (size_t i = 0; i < (size_t)pos_cnt; ++i)
    {
    size_t indx = pos_list[i] - 1;

    if (indx >= sdi.size()+raw.size())
      {
      DERTBL	*dp = derived[indx-(sdi.size()+raw.size())];
      dp->Dirty = true;

      switch (dp->OutputRate)
        {
        case Config::LowRate:
          if (cfg.ProcessingRate() == Config::LowRate)
            dp->OutputRate = Config::LowRate;
          break;

        case Config::HighRate:
          dp->OutputRate = Config::LowRate;
          break;
        }

      item = CreateListLineItem(dp, DERIVED);
      }
    else
    if (indx >= sdi.size())
      {
      RAWTBL	*rp = raw[indx-sdi.size()];

      if (strcmp(rp->name, "HOUR") != 0 && strcmp(rp->name, "MINUTE") != 0
				&& strcmp(rp->name, "SECOND") != 0)
        {
        rp->Dirty = true;

        switch (rp->OutputRate)
          {
          case Config::LowRate:
            if (rp->OutputRate != rp->SampleRate)
              rp->OutputRate = rp->SampleRate;
            else
            if (cfg.ProcessingRate() == Config::HighRate)
              rp->OutputRate = Config::HighRate;
            break;

          case Config::HighRate:
            rp->OutputRate = Config::LowRate;
            break;

          default:
            if (cfg.ProcessingRate() == Config::HighRate)
              rp->OutputRate = Config::HighRate;
            else
              rp->OutputRate = Config::LowRate;
          }
        }

      item = CreateListLineItem(rp, RAW);
      }
    else
      {
      SDITBL	*sp = sdi[indx];
      sp->Dirty = true;

      switch (sp->OutputRate)
        {
        case Config::LowRate:
          sp->OutputRate = sp->SampleRate;
          break;

        case Config::HighRate:
          sp->OutputRate = Config::LowRate;
          break;

        default:
          if (cfg.ProcessingRate() == Config::HighRate)
            sp->OutputRate = Config::HighRate;
          else
            sp->OutputRate = Config::LowRate;
        }

      item = CreateListLineItem(sp, SDI);
      }

    XmListReplaceItemsPos(list1, &item, 1, pos_list[i]);
    XmStringFree(item);
    }

  if (pos_cnt == 1)
    XmListSelectPos(list1, pos_list[0], false);

  XtFree((char *)pos_list);

}	/* END TOGGLERATE */

/* -------------------------------------------------------------------- */
void ToggleOutput(Widget w, XtPointer client, XtPointer call)
{
  int		*pos_list, pos_cnt = 0;
  XmString	item;

  if (XmListGetSelectedPos(list1, &pos_list, &pos_cnt) == false)
    return;

  for (size_t i = 0; i < (size_t)pos_cnt; ++i)
    {
    size_t indx = pos_list[i] - 1;

    if (indx >= sdi.size()+raw.size())
      {
      DERTBL	*dp = derived[indx-(sdi.size()+raw.size())];
      dp->Dirty = true;
      dp->Output = 1 - dp->Output;

      item = CreateListLineItem(dp, DERIVED);
      }
    else
    if (indx >= sdi.size())
      {
      RAWTBL	*rp = raw[indx-sdi.size()];

      if (strcmp(rp->name, "HOUR") != 0 && strcmp(rp->name, "MINUTE") != 0
				&& strcmp(rp->name, "SECOND") != 0)
        {
        rp->Dirty = true;
        rp->Output = 1 - rp->Output;
        }

      item = CreateListLineItem(rp, RAW);
      }
    else
      {
      SDITBL	*sp = sdi[indx];
      sp->Dirty = true;
      sp->Output = 1 - sp->Output;

      item = CreateListLineItem(sp, SDI);
      }

    XmListReplaceItemsPos(list1, &item, 1, pos_list[i]);
    XmStringFree(item);
    }

  if (pos_cnt == 1)
    XmListSelectPos(list1, pos_list[0], false);

  XtFree((char *)pos_list);

}	/* END TOGGLEOUTPUT */

/* -------------------------------------------------------------------- */
void Quit(Widget w, XtPointer client, XtPointer call)
{
  CloseNetCDF();
  CloseRemoveLogFile();
  exit(0);
}

/* -------------------------------------------------------------------- */
static int validateInputFile()
{
  if (cfg.ProcessingMode() == Config::RealTime)
    return(OK);

  if (strlen(ADSfileName) == 0 || access(ADSfileName, R_OK) == ERR)
    {
    HandleError("Non-existent input file.");
    return(ERR);
    }

  return(OK);

}	/* END VALIDATEINPUTFILE */

/* -------------------------------------------------------------------- */
void ValidateOutputFile(Widget w, XtPointer client, XtPointer call)
{
  char	*p;
  struct stat statBuf;

  if (w)
    {
    ExtractFileName(((XmFileSelectionBoxCallbackStruct *)call)->value, &p);
    FileCancel((Widget)NULL, (XtPointer)NULL, (XtPointer)NULL);
    strcpy(OutputFileName, p);
    XmTextFieldSetString(outputFileText, p);
    }


  if (strlen(OutputFileName) == 0 || cfg.ProductionRun())
    setOutputFileName();


  /* If it's a directory, then gen up our own name.
   */
  if (stat(OutputFileName, &statBuf) != ERR && S_ISDIR(statBuf.st_mode))
    setOutputFileName();


  /* Make sure we have a .nc extension.
   */
  if (strcmp(&OutputFileName[strlen(OutputFileName)-3], ".nc") != 0)
    {
    strcat(OutputFileName, ".nc");
    XmTextFieldSetString(outputFileText, OutputFileName);
    }


  /* If it doesn't exist, then go away happy.
   */
  if (access(OutputFileName, F_OK) == ERR && errno == ENOENT)
    {
    StartProcessing(NULL, NULL, NULL);
    return;
    }

  /* If we can't access it for writing, generate perm denied.
   */
  if (access(OutputFileName, W_OK) == ERR && errno == EACCES)
    {
    HandleError("Permission denied on output file.");
    QueryOutputFile(NULL, NULL, NULL);
    return;
    }

  /* Else if it exists, warn user.
   */
  HandleWarning("Output file exists.", StartProcessing, CancelSetup);

}	/* END VALIDATEOUTPUTFILE */

/* -------------------------------------------------------------------- */
static void setOutputFileName()
{
  char	*s;
  char	*p = cfg.ProcessingRate() == Config::LowRate ? (char *)"%s%s.nc" : (char *)"%s%sh.nc";

  if (cfg.ProductionRun())
    s = getenv("PROD_DATA");
  else
    s = getenv("DATA_DIR");


  if (s) {
    strcpy(OutputFileName, s);
    strcat(OutputFileName, "/");
    }

  if (cfg.ProductionRun())
    {
    sprintf(buffer, p, ProjectNumber, FlightNumber);
    strcat(OutputFileName, buffer);
    }
  else
    {
    strcat(OutputFileName, getenv("USER"));
    strcat(OutputFileName, ".nc");
    }

  XmTextFieldSetString(outputFileText, OutputFileName);

}	/* END SETOUTPUTFILENAME */

/* -------------------------------------------------------------------- */
XmString CreateListLineItem(void *pp, int var_type)
{
  SDITBL	*sp;
  RAWTBL	*rp;
  DERTBL	*dp;
  char		tmp[16];

  switch (var_type)
    {
    case SDI:
      sp = (SDITBL *)pp;

      sprintf(buffer, list1lineFrmt, sp->name,
		sp->Dirty ? '*' : ' ', sp->Output ? 'Y' : 'N',
		sp->SampleRate, sp->OutputRate,
		sp->StaticLag, sp->SpikeSlope,
		sp->DataQuality[0]);

      for (size_t i = 0; i < sp->order; ++i)
        {
        sprintf(tmp, "%10.4f", sp->cof[i]);
        strcat(buffer, tmp);
        }

      break;

    case RAW:
      rp = (RAWTBL *)pp;

      sprintf(buffer, list1lineFrmt, rp->name,
		rp->Dirty ? '*' : ' ', rp->Output ? 'Y' : 'N',
		rp->SampleRate, rp->OutputRate,
		rp->StaticLag, rp->SpikeSlope,
		rp->DataQuality[0]);

      for (size_t i = 0; i < rp->order; ++i)
        {
        sprintf(tmp, "%10.4f", rp->cof[i]);
        strcat(buffer, tmp);
        }

      break;

    case DERIVED:
      dp = (DERTBL *)pp;

      sprintf(buffer, list1lineFrmt, dp->name,
		dp->Dirty ? '*' : ' ', dp->Output ? 'Y' : 'N',
		0, dp->OutputRate, 0, 0.0, dp->DataQuality[0]);

      buffer[20] = 'N'; buffer[21] = 'A';
      buffer[33] = 'N'; buffer[34] = 'A';
      memcpy(&buffer[40], "NA    ", 5);

      for (size_t i = 0; i < dp->ndep; ++i)
        {
        if (i > 0)
          strcat(buffer, ",");

        sprintf(tmp, " %s", dp->depend[i]);
        strcat(buffer, tmp);
        }

      break;
    }

  return(XmStringCreateLocalized(buffer));

}	/* END CREATELISTLINEITEM */

/* -------------------------------------------------------------------- */
void FillListWidget()
{
  size_t	i, cnt;
  XmString	items[MAX_VARIABLES];

  static int	firstTime = true;

  if (firstTime)
    XmListDeleteAllItems(list1);

  cnt = 0;

  for (i = 0; i < sdi.size(); ++i)
    items[cnt++] = CreateListLineItem(sdi[i], SDI);

  for (i = 0; i < raw.size(); ++i)
    items[cnt++] = CreateListLineItem(raw[i], RAW);

  for (i = 0; i < derived.size(); ++i)
    items[cnt++] = CreateListLineItem(derived[i], DERIVED);


  if (firstTime)
    XmListAddItems(list1, items, cnt, 1);
  else
    XmListReplaceItemsPos(list1, items, cnt, 1);


  for (i = 0; i < cnt; ++i)
    XmStringFree(items[i]);

  if (firstTime)
    {
    XmListSelectPos(list1, 1, false);
    firstTime = false;
    }

}	/* END FILLLISTWIDGET */

/* -------------------------------------------------------------------- */
void PrintSetup(Widget w, XtPointer client, XtPointer call)
{
  FILE	*fp;

  if ((fp = popen("lpr", "w")) == NULL)
    {
    HandleError("PrintList: can't open pipe to 'lpr'");
    return;
    }


  fprintf(fp, "%s - %s, Flight %s\n\n", ProjectName, ProjectNumber, FlightNumber);

  fprintf(fp, "Name       Output  SR    OR     Lag   Spike Slope\n");
  fprintf(fp, "--------------------------------------------------------------------------------\n");

  for (size_t i = 0; i < sdi.size(); ++i)
    {
    SDITBL *sp = sdi[i];

    fprintf(fp, list1lineFrmt,
			sp->name,
			sp->Dirty ? '*' : ' ',
			sp->Output ? 'Y' : 'N',
			sp->SampleRate,
			sp->OutputRate,
			sp->StaticLag,
			sp->SpikeSlope);

    for (size_t j = 0; j < sp->order; ++j)
      fprintf(fp, "%14e", sp->cof[j]);

    fprintf(fp, "\n");
    }


  for (size_t i = 0; i < raw.size(); ++i)
    {
    RAWTBL *rp = raw[i];

    fprintf(fp, list1lineFrmt,
			rp->name,
			rp->Dirty ? '*' : ' ',
			rp->Output ? 'Y' : 'N',
			rp->SampleRate,
			rp->OutputRate,
			rp->StaticLag,
			rp->SpikeSlope);

    for (size_t j = 0; j < rp->order; ++j)
      fprintf(fp, "%14e", rp->cof[j]);

    fprintf(fp, "\n");
    }


  for (size_t i = 0; i < derived.size(); ++i)
    {
    DERTBL *dp = derived[i];

    sprintf(buffer, list1lineFrmt,
			dp->name,
			dp->Dirty ? '*' : ' ',
			dp->Output ? 'Y' : 'N',
			0,
			dp->OutputRate,
			0, 0.0);

    buffer[33] = 'N'; buffer[34] = 'A';
    fprintf(fp, buffer);

    for (size_t j = 0; j < dp->ndep; ++j)
      {
      if (j > 0)
        {
        fprintf(fp, ", ");

        if (j % 5 == 0)
          fprintf(fp, "\n\t\t\t\t\t ");
        }

      fprintf(fp, "%s", dp->depend[j]);
      }

    fprintf(fp, "\n");
    }

  fprintf(fp, "\n\nDefaults:\n\n");

  for (size_t i = 0; i < nDefaults; ++i)
    {
    if (Defaults[i]->Used == false)
      continue;

    fprintf(fp, "%s :", Defaults[i]->Name);

    if (Defaults[i]->nValues > 5)
      fprintf(fp, "\n\t");

    for (size_t j = 0; j < Defaults[i]->nValues; ++j)
      {
      if (j > 0 && j % 5 == 0)
        fprintf(fp, "\n\t");

      fprintf(fp, "%14e", Defaults[i]->Value[j]);
      }

    fprintf(fp, "\n");
    }


  pclose(fp);

}	/* END PRINTSETUP */

/* -------------------------------------------------------------------- */
void ToggleProbe(Widget w, XtPointer client, XtPointer call)
{
  size_t i;
  unsigned	cat  = (int)client & 0xf0000000,
		type = (int)client & 0x0ffffff0,
		cnt  = (int)client & 0x0000000f;

  if ((unsigned)client == ALL_ON || (unsigned)client == ALL_OFF)
    {
    bool	value = (unsigned)client == ALL_ON ? true : false;

    for (i = 0; i < sdi.size(); ++i) {
      sdi[i]->Dirty = true;
      sdi[i]->Output = value;
      }

    for (i = 0; i < raw.size(); ++i) {
      if (strcmp(raw[i]->name, "HOUR") == 0 ||
	  strcmp(raw[i]->name, "MINUTE") == 0 ||
	  strcmp(raw[i]->name, "SECOND") == 0)
        continue;

      raw[i]->Dirty = true;
      raw[i]->Output = value;
      }

    for (i = 0; i < derived.size(); ++i) {
      derived[i]->Dirty = true;
      derived[i]->Output = value;
      }
    }
  else
    {
    for (i = 0; i < raw.size(); ++i)
      if ((cat && raw[i]->ProbeType & cat) || (raw[i]->ProbeType & type &&
		raw[i]->ProbeCount == cnt))
        {
        raw[i]->Dirty = true;
        raw[i]->Output = 1 - raw[i]->Output;
        }

    for (i = 0; i < derived.size(); ++i)
      if ((cat && derived[i]->ProbeType & cat) ||
	(derived[i]->ProbeType & type && derived[i]->ProbeCount == cnt))
        {
        derived[i]->Dirty = true;
        derived[i]->Output = 1 - derived[i]->Output;
        }
    }

  FillListWidget();

}	/* END TOGGLEPROBE */

/* -------------------------------------------------------------------- */
void QueryOutputFile(Widget w, XtPointer client, XtPointer call)
{
  if (!cfg.ProductionRun())
    {
    GetDataDirectory(buffer);
    strcat(buffer, "*.nc");

    QueryFile("Enter netCDF file name:", buffer, ValidateOutputFile);
    }
  else
    ValidateOutputFile(NULL, NULL, NULL);

}

/* -------------------------------------------------------------------- */
void sighandler(int s)
{
  extern PostgreSQL *psql;

  printf("SigHandler: cleaning up netCDF file.\n");
  CloseNetCDF();

  if (psql)
    psql->closeSQL();

  exit(0);

}	/* END SIGHANDLER */

/* -------------------------------------------------------------------- */
static void EngageSignals()
{
  signal(SIGINT, sighandler);
  signal(SIGKILL, sighandler);
  signal(SIGFPE, sighandler);
  signal(SIGTERM, sighandler);

}	/* END ENGAGESIGNALS */

/* -------------------------------------------------------------------- */
void LogMessage(char msg[])
{
  XmTextPosition	position;
  extern Widget	logText;

  if (cfg.Interactive())
    {
    position = XmTextGetInsertionPosition(logText);
    XmTextInsert(logText, position, msg);

    position += strlen(msg);
    XmTextShowPosition(logText, position);
    XmTextSetInsertionPosition(logText, position);
    }
  else
    fprintf(stderr, msg);

  if (LogFile)
    fprintf(LogFile, msg);

}	/* END LOGMESSAGE */

/* END CB_MAIN.C */
