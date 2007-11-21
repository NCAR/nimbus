/*
-------------------------------------------------------------------------
OBJECT NAME:	cb_main.c

FULL NAME:	Command CallBacks

ENTRY POINTS:	CancelSetup()
		CreateListLineItem()
		FillListWidget()
		LoadSetup()
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
		runSecondPassPrograms()
		setOutputFileName()
		stopProcessing()
		validateInputFile()
		LogLagErrors()

DESCRIPTION:	Contains callbacks for the nimbus GUI main window & setup
		window.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2007
-------------------------------------------------------------------------
*/

#include <cerrno>
#include <fcntl.h>
#include <csignal>
#include <ctime>
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <Xm/List.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>

#include "nimbus.h"
#include "decode.h"
#include <raf/ctape.h>
#include "gui.h"
#include <raf/vardb.h>
#include "psql.h"
#include "svnInfo.h"


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

void	CloseSQL(), ProcessFlightDate();
void	ValidateOutputFile(Widget w, XtPointer client, XtPointer call);
static int	validateInputFile();

static void	checkForProductionSetup(), displaySetupWindow(),
		setOutputFileName(), readHeader(), stopProcessing(),
		EngageSignals(), SetConfigGlobalAttributeVariables(),
		LogLagErrors();


void	InitAsyncModule(char fileName[]), RealTimeLoop(),
	CloseLogFile(), LogDespikeInfo(), InitAircraftDependencies(),
	CloseRemoveLogFile(), LogIRSerrors(), RealTimeLoop3();


/* -------------------------------------------------------------------- */
void CancelSetup(Widget w, XtPointer client, XtPointer call)
{
  size_t	i;

  CloseADSfile();

  for (i = 0; i < raw.size(); ++i)
    delete raw[i];

  for (i = 0; i < derived.size(); ++i)
    delete derived[i];

  for (i = 0; i < nDefaults; ++i)
    delete Defaults[i];

  FreeDataArrays();
  ReleaseFlightHeader();


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
  int	rc;

  if (w)
    {
    ExtractFileName(((XmFileSelectionBoxCallbackStruct *)call)->value, &p);
    strcpy(ADSfileName, p);
    XmTextFieldSetString(aDSdataText, p);

    if ((rc = validateInputFile()) == OK)
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

    if ((rc = validateInputFile()) == OK)
      {
      readHeader();
      ValidateOutputFile(NULL, NULL, NULL);
      }
    }

  if (rc == OK && cfg.isADS2() && FlightNumberInt == 0)
    HandleWarning("Flight Number is 0, a new one may be entered\nvia the 'Edit/Flight Info' menu item,\nor run fixFltNum on the ADS image and start nimbus again.", NULL, NULL);

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
  LogMessage(SVNREVISION);

  int rc = ERR;

  if (cfg.isADS2())
    rc = DecodeHeader(ADSfileName);
  else
  if (cfg.isADS3())
    rc = DecodeHeader3(ADSfileName);

  if (rc == ERR)
  {
    CancelSetup(NULL, NULL, NULL);
    return;
  }

  sprintf(buffer, "%s - %s, Flight %s",
	cfg.ProjectName().c_str(), cfg.ProjectNumber().c_str(),
	cfg.FlightNumber().c_str());

  if (cfg.Interactive())
    {
    Arg		args[1];

    XtSetArg(args[0], XmNtitle, buffer);
    XtSetValues(Shell001, args, 1);

    if (LogFile)
      {
      extern Widget logText;
      char *p = XmTextGetString(logText);

      strcat(buffer, "\n");
      fprintf(LogFile, buffer);

      fprintf(LogFile, p);
      XtFree(p);
      }

    CreateProbeMenu();
    FillListWidget();
    checkForProductionSetup();
    }
  else
    {
    if (cfg.ProcessingRate() == Config::HighRate)
      SetHighRate(NULL, (void *)cfg.HRTRate(), NULL);

    LogMessage(buffer);

    checkForProductionSetup();
    LoadSetup_OK(Shell001, NULL, NULL); /* Fake it with any widget name */
    }

}	/* END READHEADER */

/* -------------------------------------------------------------------- */
static void runSecondPassPrograms()
{
  LogMessage("Running netCDF file sanity checker (nc_sane)...\n");
  sprintf(buffer, "nc_sane %s", OutputFileName);
  system(buffer);

  if (cfg.ProcessingRate() == Config::LowRate)
  {
    LogMessage("Producing KML file (rt_kml)...\n");
    strcpy(buffer, OutputFileName);
    strcpy(strrchr(buffer, '.'), ".kml");
    sprintf(buffer, "rt_kml %s %s", OutputFileName, buffer);
    system(buffer);
  }
}	/* END RUNSECONDPASSPROGRAMS */

/* -------------------------------------------------------------------- */
void ConfigurationDump()
{
  LogMessage("\nConfiguration dump:");
  sprintf(buffer, "  Input file is %s.",
	cfg.isADS2() ? "ADS-2" : "ADS-3"); LogMessage(buffer);
  sprintf(buffer, "  %s mode.",
	cfg.ProcessingMode() == Config::RealTime ? "Real-time" : "Post-processing");
	LogMessage(buffer);
  sprintf(buffer, "  %s run.",
	cfg.ProductionRun() ? "Production" : "Preliminary"); LogMessage(buffer);
  sprintf(buffer, "  De-spiking %s.",
	cfg.Despiking() ? "enabled" : "disabled"); LogMessage(buffer);
  sprintf(buffer, "  Time-shifting %s.",
	cfg.TimeShifting() ? "enabled" : "disabled"); LogMessage(buffer);
  sprintf(buffer, "  Honeywell IRS cleanup %s.",
	cfg.HoneyWellCleanup() ? "enabled" : "disabled"); LogMessage(buffer);
  sprintf(buffer, "  Intertial time-shift %s.",
	cfg.InertialShift() ? "enabled" : "disabled"); LogMessage(buffer);
  sprintf(buffer, "  Output NetCDF: %s.",
	cfg.OutputNetCDF() ? "yes" : "no"); LogMessage(buffer);
  sprintf(buffer, "  Output SQL: %s.",
	cfg.OutputSQL() ? "yes" : "no"); LogMessage(buffer);
  sprintf(buffer, "  Ground transmission: %s.",
	cfg.TransmitToGround() ? "yes" : "no"); LogMessage(buffer);
}

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
  ReadBlankOuts();
  AllocateDataArrays();
  GenerateComputeOrder();	/* for Derived variables		*/
  GetUserTimeIntervals();

  if (cfg.ProductionRun())	/* Do it again in case Flight# changed	*/
    setOutputFileName();


  /* RunAMLIBinits before creating netCDF to setup defaults into
   * netCDF attributes.
   */
  ProcessFlightDate();
  InitAircraftDependencies();
  RunAMLIBinitializers();
  SetConfigGlobalAttributeVariables();
  CreateNetCDF(OutputFileName);
  InitAsyncModule(OutputFileName);
  ConfigurationDump();


  // Do some clean-up/preperation.
  for (size_t i = 0; i < raw.size(); ++i)
  {
    if (!cfg.TimeShifting())
      raw[i]->StaticLag = raw[i]->DynamicLag = 0;

    if (!cfg.Despiking())
      raw[i]->SpikeSlope = 0.0;

    if (raw[i]->StaticLag != 0 || raw[i]->DynamicLag != 0)
      AddVariableToRAWlagList(raw[i]);

    if (raw[i]->SpikeSlope > 0.0)
      AddVariableToRAWdespikeList(raw[i]);

    if (raw[i]->Output && VarDB_lookup(raw[i]->name) == ERR && LogFile)
      fprintf(LogFile, "%s has no entry in the VarDB.\n", raw[i]->name);
  }

  for (size_t i = 0; i < derived.size(); ++i)
  {
    if (derived[i]->Output && VarDB_lookup(derived[i]->name) == ERR && LogFile)
      fprintf(LogFile,"%s has no entry in the VarDB.\n", derived[i]->name);
  }

  FillListWidget();
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

  if (cfg.isADS2())
  {
    FindFirstLogicalRecord = FindFirstLogicalADS2;
    FindNextLogicalRecord = FindNextLogicalADS2;
  }
  else
  if (cfg.isADS3())
  {
    FindFirstLogicalRecord = FindFirstLogicalADS3;
    FindNextLogicalRecord = FindNextLogicalADS3;
  }
  else
  {
    FindFirstLogicalRecord = 0;
    FindNextLogicalRecord = 0;
  }

  void SwitchNetCDFtoDataMode();
  SwitchNetCDFtoDataMode();

  if (cfg.ProcessingMode() == Config::RealTime)
    {
    NextTimeInterval(&btim, &etim);

    if (cfg.isADS2())
      RealTimeLoop();	/* Never to return	*/
    else
      RealTimeLoop3();

    quit();
    }


  startWALL	= time(NULL);
  startCPU	= clock();

  while (NextTimeInterval(&btim, &etim))
    {
    switch (cfg.ProcessingRate())
      {
      case Config::LowRate:
      case Config::SampleRate:
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
  runSecondPassPrograms();

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
  LogLagErrors();

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

  extern void closeSyncRecordReader();
  closeSyncRecordReader();

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
        char t[32];
        MakeProjectFileName(buffer, "%s/%s/%s/Production/Flight_");
        sprintf(t, "%d", atoi(strrchr(group[i], '=')+1));
        strcat(buffer, t);

        LoadSetup_OK(NULL, NULL, NULL);

        break;
        }
      }
    else
      {
      if (strncmp(group[i], &cfg.FlightNumber()[0], 4) == 0)
        {
        char t[32];
        MakeProjectFileName(buffer, "%s/%s/%s/Production/Flight_");
        sprintf(t, "%d", atoi(strrchr(group[i], '=')+2));
        strcat(buffer, t);

        LoadSetup_OK(NULL, NULL, NULL);

        break;
        }
      }
    }

  FreeTextFile(group);


  /* Check for Production File
   */
  MakeProjectFileName(buffer, "%s/%s/%s/Production/Flight_");
  if (cfg.isADS3())
    revision2 = true;

  if (!revision2)
  {
    char t[32];
    sprintf(t, "%d", FlightNumberInt);
    strcat(buffer, t);
  }
  else
    strcat(buffer, cfg.FlightNumber().c_str());

  LoadSetup_OK(NULL, NULL, NULL);

}	/* END CHECKFORPRODUCTIONSETUP */

/* -------------------------------------------------------------------- */
void SaveSetup(Widget w, XtPointer client, XtPointer call)
{
  if (cfg.ProductionRun())
  {
    MakeProjectFileName(buffer, "%s/%s/%s/Production");
    mkdir(buffer, 0775);

    strcat(buffer, "/Flight_");
    strcat(buffer, cfg.FlightNumber().c_str());

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

    if (indx >= raw.size())
      {
      DERTBL	*dp = derived[indx-raw.size()];
      dp->Dirty = true;

      if (cfg.ProcessingRate() == Config::LowRate)
        dp->OutputRate = Config::LowRate;
      else
        switch (dp->OutputRate)
          {
          case Config::LowRate:
            /* PMS Probes HRT is SampleRate.  Period.
             */
            if ((dp->ProbeType & PROBE_PMS2D) || (dp->ProbeType & PROBE_PMS1D))
              dp->OutputRate = dp->Default_HR_OR;
            else
              dp->OutputRate = cfg.HRTRate();
            break;

          default:
            dp->OutputRate = Config::LowRate;
            break;
          }

      item = CreateListLineItem(dp, DERIVED);
      }
    else
      {
      RAWTBL	*rp = raw[indx];

      if (strcmp(rp->name, "HOUR") != 0 && strcmp(rp->name, "MINUTE") != 0
				&& strcmp(rp->name, "SECOND") != 0)
        {
        rp->Dirty = true;

        switch (rp->OutputRate)
          {
          case Config::LowRate:
            if (rp->OutputRate != rp->SampleRate ||
               (rp->ProbeType & PROBE_PMS2D) || (rp->ProbeType & PROBE_PMS1D))
              rp->OutputRate = rp->SampleRate;
            else
            if (cfg.ProcessingRate() == Config::HighRate)
              rp->OutputRate = cfg.HRTRate();
            break;

          case Config::HighRate:
            rp->OutputRate = Config::LowRate;
            break;

          default:
            if (cfg.ProcessingRate() != Config::HighRate ||
               (rp->ProbeType & PROBE_PMS2D) || (rp->ProbeType & PROBE_PMS1D))
              rp->OutputRate = Config::LowRate;
            else
              rp->OutputRate = cfg.HRTRate();
          }
        }

      item = CreateListLineItem(rp, RAW);
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

    if (indx >= raw.size())
      {
      DERTBL	*dp = derived[indx-raw.size()];
      dp->Dirty = true;
      dp->Output = 1 - dp->Output;

      item = CreateListLineItem(dp, DERIVED);
      }
    else
      {
      RAWTBL	*rp = raw[indx];
      rp->Dirty = true;
      rp->Output = 1 - rp->Output;

      item = CreateListLineItem(rp, RAW);
      }

    XmListReplaceItemsPos(list1, &item, 1, pos_list[i]);
    XmStringFree(item);
    }

  if (pos_cnt == 1)
    XmListSelectPos(list1, pos_list[0], false);

  XtFree((char *)pos_list);

}	/* END TOGGLEOUTPUT */

/* -------------------------------------------------------------------- */
void quit()
{
  Quit(NULL, NULL, NULL);
}

/* -------------------------------------------------------------------- */
void Quit(Widget w, XtPointer client, XtPointer call)
{
  CloseNetCDF();

  extern PostgreSQL *psql;
  if (psql)
    psql->closeSQL();

  CloseRemoveLogFile();

  // kill sync_server or any other processes.
  extern pid_t syncPID;
  if (syncPID > 0)
  {
    kill(syncPID, SIGTERM);
  }

  if (strlen(sync_server_pipe))
    unlink(sync_server_pipe);

  exit(0);
}

/* -------------------------------------------------------------------- */
static int determineInputFileVersion()
{
  FILE *fp = fopen64(ADSfileName, "r");

  if (fp == 0)
  {
    char msg[128];
    sprintf(msg, "Failed to open input file [%s], errno = %d.\n", ADSfileName, errno);
    perror(msg);
    HandleError(msg);
    return ERR;
  }

  fread(buffer, 20, 1, fp);
  fclose(fp);
  buffer[20] = '\0';

  if ( strstr(buffer, "NIDAS") )
  {
    char tmp[512], *p;
    cfg.SetADSVersion(Config::ADS_3);

    strcpy(tmp, ADSfileName);
    p = strrchr(tmp, '/');
    p = strtok(p+1, "_");
//    cfg.SetProjectNumber(p);
    p = strtok(NULL, "_");
    p = strtok(NULL, "_.");
    if (p)
      cfg.SetFlightNumber(p);
    else
      LogMessage("Unable to determine FlightNumber from file name.\n");

// Needs to be retreived from "system_name".
    cfg.SetTailNumber("N677F");
  }

  return OK;

}	/* END DETERMINEINPUTFILEVERSION */

/* -------------------------------------------------------------------- */
static int validateInputFile()
{
  if (cfg.ProcessingMode() == Config::RealTime)
    return(OK);

  if (strlen(ADSfileName) == 0 || access(ADSfileName, R_OK) == ERR)
    {
    char msg[128];
    sprintf(msg, "Non-existent input file [%s].\n", ADSfileName);
    HandleError(msg);
    return(ERR);
    }

  struct stat st_buf;
  stat(ADSfileName, &st_buf);
  if ( S_ISDIR( st_buf.st_mode ) )
    {
    char msg[128];
    sprintf(msg, "Input file [%s] is a directory.\n", ADSfileName);
    HandleError(msg);
    return(ERR);
    }

  return determineInputFileVersion();

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
    sprintf(buffer, p, cfg.ProjectNumber().c_str(), cfg.FlightNumber().c_str());
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
  RAWTBL	*rp;
  DERTBL	*dp;
  char		tmp[16];

  switch (var_type)
    {
    case RAW:
      rp = (RAWTBL *)pp;

      sprintf(buffer, list1lineFrmt, rp->name,
		rp->Dirty ? '*' : ' ', rp->Output ? 'Y' : 'N',
		rp->SampleRate, rp->OutputRate,
		rp->StaticLag, rp->SpikeSlope,
		rp->DataQuality[0]);

      for (size_t i = 0; i < rp->cof.size(); ++i)
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
  static int	firstTime = true;
  XmString	items[MAX_VARIABLES];

  size_t cnt = 0;

  for (size_t i = 0; i < raw.size(); ++i)
    items[cnt++] = CreateListLineItem(raw[i], RAW);

  for (size_t i = 0; i < derived.size(); ++i)
    items[cnt++] = CreateListLineItem(derived[i], DERIVED);

  XmListDeleteAllItems(list1);
  XmListAddItems(list1, items, cnt, 1);

  for (size_t i = 0; i < cnt; ++i)
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


  fprintf(fp, "%s - %s, Flight %s\n\n", cfg.ProjectName().c_str(), cfg.ProjectNumber().c_str(), cfg.FlightNumber().c_str());

  fprintf(fp, "Name       Output  SR    OR     Lag   Spike Slope\n");
  fprintf(fp, "--------------------------------------------------------------------------------\n");


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

    for (size_t j = 0; j < rp->cof.size(); ++j)
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

    if (Defaults[i]->Values.size() > 5)
      fprintf(fp, "\n\t");

    for (size_t j = 0; j < Defaults[i]->Values.size(); ++j)
      {
      if (j > 0 && j % 5 == 0)
        fprintf(fp, "\n\t");

      fprintf(fp, "%14e", Defaults[i]->Values[j]);
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
  printf("SigHandler: cleaning up netCDF file.\n");
  Quit(NULL, NULL, NULL);

}	/* END SIGHANDLER */

/* -------------------------------------------------------------------- */
static void SetConfigGlobalAttributeVariables()
{
  if (SearchTable(derived, "LATC") != ERR)
    cfg.SetCoordLAT("LATC");
  else
  if (SearchTable(raw, "GGLAT") != ERR)
    cfg.SetCoordLAT("GGLAT");
  else
  if (SearchTable(raw, "GLAT") != ERR)
    cfg.SetCoordLAT("GLAT");
  else
  if (SearchTable(raw, "LAT") != ERR)
    cfg.SetCoordLAT("LAT");


  if (SearchTable(derived, "LONC") != ERR)
    cfg.SetCoordLON("LONC");
  else
  if (SearchTable(raw, "GGLON") != ERR)
    cfg.SetCoordLON("GGLON");
  else
  if (SearchTable(raw, "GLON") != ERR)
    cfg.SetCoordLON("GLON");
  else
  if (SearchTable(raw, "LON") != ERR)
    cfg.SetCoordLON("LON");


  if (SearchTable(derived, "WSC") != ERR)
    cfg.SetWindSpeed("WSC");
  else
  if (SearchTable(derived, "WS") != ERR)
    cfg.SetWindSpeed("WS");


  if (SearchTable(derived, "WDC") != ERR)
    cfg.SetWindDirection("WDC");
  else
  if (SearchTable(derived, "WD") != ERR)
    cfg.SetWindDirection("WD");


  if (SearchTable(derived, "WIC") != ERR)
    cfg.SetWindVertical("WIC");
  else
  if (SearchTable(derived, "WI") != ERR)
    cfg.SetWindVertical("WI");
}

/* -------------------------------------------------------------------- */
static void LogLagErrors()
{
  if (cfg.LagReporting() == false)
    return;

  for (size_t i = 0; i < raw.size(); ++i)
  {
    if (raw[i]->badLagCntr > 0)
    {
      sprintf(buffer, "%s: %d bad Lags.\n", raw[i]->name, raw[i]->badLagCntr);
      LogMessage(buffer);
    }
  }
}

/* -------------------------------------------------------------------- */
static void EngageSignals()
{
  signal(SIGINT, sighandler);
  signal(SIGKILL, sighandler);
  signal(SIGFPE, sighandler);
  signal(SIGTERM, sighandler);
  signal(SIGSEGV, sighandler);

}	/* END ENGAGESIGNALS */

/* END CB_MAIN.C */
