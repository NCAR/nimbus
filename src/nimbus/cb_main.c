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
		ToggleProbeOutput()
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
#include <sys/stat.h>

#include <Xm/List.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>

#include "nimbus.h"
#include "decode.h"
#include "timeseg.h"
#include "src/hdr_api/ctape.h"
#include "gui.h"
#include <raf/vardb.hh>
#include "NetCDF.h"
#include "psql.h"
#include "gitInfo.h"
#include "sync_reader.hh"

static char	ADSfileName[256];
char		OutputFileName[MAXPATHLEN];  /* Export to xlate/rdma.c */
static char	list1lineFrmt[] = "%-12s %c%c   %4d  %4d   %4d  %8.3f   %c";

extern FILE	*LogFile;
extern VDBFile  *vardb;

/* For wall clock timing	*/
static time_t	startWALL, finishWALL;
static clock_t	startCPU, finishCPU;

extern NetCDF	*ncFile;


void	CloseSQL(), ProcessFlightDate();
void	ValidateOutputFile(Widget w, XtPointer client, XtPointer call);
static int	validateInputFile();

static void	checkForProductionSetup(), displaySetupWindow(),
		setOutputFileName(), readHeader(), stopProcessing(),
		EngageSignals(), SetConfigGlobalAttributeVariables(),
		LogLagErrors();


//void	InitAsyncModule(char fileName[]);
void	RealTimeLoop(), RealTimeLoop3(), validateProjectDirectory(),
	CloseLogFile(), LogDespikeInfo(), InitAircraftDependencies(),
	CloseRemoveLogFile(), LogIRSerrors();


/* -------------------------------------------------------------------- */
void CancelSetup(Widget w, XtPointer client, XtPointer call)
{
  void FreeDefaults();

  CloseADSfile();
  FreeDefaults();
  FreeDataArrays();
  ReleaseFlightHeader();

  ShutdownSyncServer();

  DismissEditWindow(NULL, NULL, NULL);
  DismissConfigWindow(NULL, NULL, NULL);

  XtPopdown(XtParent(SetupWindow));
  XtUnmanageChild(SetupWindow);

  XtSetSensitive(readHeaderButton, true);
  XtSetSensitive(aDSdataText, true);
  XtSetSensitive(outputFileText, true);

  delete vardb;
  vardb = 0;
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
  LogMessage(std::string("Repo branch: ") + REPO_BRANCH);

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

  snprintf(buffer, 8192, "%s - %s, Flight %s",
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

    CreateProbeOutputMenu();
    CreateProbeDataQualityMenu();
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

  validateProjectDirectory();

}	/* END READHEADER */

/* -------------------------------------------------------------------- */
static void runSecondPassPrograms()
{
  {
    LogMessage("Running netCDF file sanity checker (nc_sane)...\n");
    snprintf(buffer, 8192, "nc_sane %s", OutputFileName);
    system(buffer);
  }

  {
    LogMessage("Producing KML file (acTrack2kml)...\n");
    char kml_file[1024];
    strcpy(kml_file, OutputFileName);
    strcpy(strrchr(kml_file, '.'), ".kml");
    snprintf(buffer, 8192, "acTrack2kml %s %s", OutputFileName, kml_file);
    system(buffer);
  }
}	/* END RUNSECONDPASSPROGRAMS */

/* -------------------------------------------------------------------- */
void ConfigurationDump()
{
  LogMessage("\nConfiguration dump:");
  snprintf(buffer, 8192, "  Input file is %s.",
	cfg.isADS2() ? "ADS-2" : "ADS-3"); LogMessage(buffer);
  snprintf(buffer, 8192, "  %s mode.",
	cfg.ProcessingMode() == Config::RealTime ? "Real-time" : "Post-processing");
	LogMessage(buffer);
  snprintf(buffer, 8192, "  %s rate.",
	cfg.ProcessingRate() == Config::HighRate ? "High" : 
	  (cfg.ProcessingRate() == Config::LowRate ? "Low" : "Sample"));
	LogMessage(buffer);
  snprintf(buffer, 8192, "  %s run.",
	cfg.ProductionRun() ? "Production" : "Preliminary"); LogMessage(buffer);
  snprintf(buffer, 8192, "  De-spiking %s.",
	cfg.Despiking() ? "enabled" : "disabled"); LogMessage(buffer);
  snprintf(buffer, 8192, "  Time-shifting %s.",
	cfg.TimeShifting() ? "enabled" : "disabled"); LogMessage(buffer);
  snprintf(buffer, 8192, "  Blankouts %s.",
	cfg.BlankoutVariables() ? "enabled" : "disabled"); LogMessage(buffer);
  snprintf(buffer, 8192, "  Honeywell IRS cleanup %s.",
	cfg.HoneyWellCleanup() ? "enabled" : "disabled"); LogMessage(buffer);
  snprintf(buffer, 8192, "  Intertial time-shift %s.",
	cfg.InertialShift() ? "enabled" : "disabled"); LogMessage(buffer);
  snprintf(buffer, 8192, "  Output NetCDF: %s.",
	cfg.OutputNetCDF() ? "yes" : "no"); LogMessage(buffer);
  snprintf(buffer, 8192, "  Output SQL: %s.",
	cfg.OutputSQL() ? "yes" : "no"); LogMessage(buffer);
  snprintf(buffer, 8192, "  Size distribution legacy zero bin : %s.",
	cfg.ZeroBinOffset() ? "yes" : "no"); LogMessage(buffer);

  if (cfg.TransmitToGround())
    snprintf(buffer, 8192, "  Ground transmission every %d seconds.", cfg.GroundFeedDataRate());
  else
    snprintf(buffer, 8192, "  Ground transmission: no.");
  LogMessage(buffer);
}

/* -------------------------------------------------------------------- */
void StartProcessing(Widget w, XtPointer client, XtPointer call)
{
  XmString	label;
  Arg		args[1];
  int		rc = 0;
  time_t	btim, etim;

  ILOG(("StartProcessing"));

  DismissEditWindow(NULL, NULL, NULL);
  DismissConfigWindow(NULL, NULL, NULL);
  XtSetSensitive(list1, false);
  XtSetSensitive(menuBar, false);

  CleanOutUnwantedVariables();
  AllocateDataArrays();
  GenerateComputeOrder();	/* for Derived variables		*/
  GetUserTimeIntervals();
  ReadSetValues(BLANKVARS, true);
  ReadSetValues(SETVALVARS, false);

  if (cfg.ProductionRun())	/* Do it again in case Flight# changed	*/
    setOutputFileName();


  /* RunAMLIBinits before creating netCDF to setup defaults into
   * netCDF attributes.
   */
  ncFile = new NetCDF();
  ncFile->ProcessFlightDate();	// This needs to be called before CreateFile at this time.
  SetupDependencies();
  InitAircraftDependencies();
  RunAMLIBinitializers();
  SetConfigGlobalAttributeVariables();
  ncFile->CreateFile(OutputFileName, 0);
//  InitAsyncModule(OutputFileName);
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

    if (raw[i]->Output && vardb->search_var(raw[i]->name) == 0 && LogFile)
      fprintf(LogFile, "%s has no entry in the VarDB.\n", raw[i]->name);
  }

  for (size_t i = 0; i < derived.size(); ++i)
  {
    if (derived[i]->Output && vardb->search_var(derived[i]->name) == 0 && LogFile)
      fprintf(LogFile,"%s has no entry in the VarDB.\n", derived[i]->name);
  }

  FillListWidget();
  FlushXEvents();

  if (cfg.ProcessingRate() == Config::HighRate)
    InitMRFilters();

  /* Turn "Go" button into "Pause" button.
   */
  XtRemoveAllCallbacks(goButton, XmNactivateCallback);
  label = XmStringCreate((char *)"Pause", XmFONTLIST_DEFAULT_TAG);
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

  ncFile->SwitchToDataMode();
  ncFile->WriteCoordinateVariableData();

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

  stopProcessing();
  runSecondPassPrograms();

}	/* END STARTPROCESSING */

/* -------------------------------------------------------------------- */
void stopProcessing()
{
  XmString	label;
  Arg		args[1];
  float		x;

  ncFile->Close();

  LogDespikeInfo();
  LogIRSerrors();
  LogLagErrors();

  /* Log wall clock time.
   */
  x = (float)(finishWALL - startWALL) / 60.0;

  if (x > 60.0)
    snprintf(buffer, 8192, "Processing took %.2f hour(s).\n", x / 60.0);
  else
    snprintf(buffer, 8192, "Processing took %.1f minutes.\n", x);

  LogMessage(buffer);

/*
  x = (float)(finishCPU - startCPU) / CLOCKS_PER_SEC / 60.0;
  snprintf(buffer, 8192, "CPU time took %.1f minutes.\n", x);
  LogMessage(buffer);
*/

  CloseLogFile();
  CloseSyncReader();

  /* Turn "Pause" button back into "Go" button.
   */
  label = XmStringCreate((char *)"Quit", XmFONTLIST_DEFAULT_TAG);
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
  snprintf(buffer, 8192, "%s/nimbus/*", getenv("HOME"));
  QueryFile("Enter file name to load:", buffer, (XtCallbackProc)LoadSetup_OK);

}	/* END LOADSETUP */

/* -------------------------------------------------------------------- */
void  LoadSynthetic(Widget w, XtPointer client, XtPointer call)
{
  snprintf(buffer, 8192, "%s/nimbus/*", getenv("HOME"));
  QueryFile("Enter synthetic data file name to load:", buffer, (XtCallbackProc)LoadSynthetic_OK);

}	/* END LOADSYNTHETIC */

/* -------------------------------------------------------------------- */
static void checkForProductionSetup()
{
  int	i, revision2 = False;
  char	*group[256];

  if (cfg.LoadProductionSetup() == false)
    return;

  /* Check for three files:
   *  - Flight_All
   *  - FlightGroups
   *  - Flight_rf##; this flight number
   */


  /* Check for Flight_All
   */
  MakeProjectFileName(buffer, "%s/%s/%s/Production/Flight_All");
  LoadSetup_OK(NULL, NULL, NULL);


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
        snprintf(t, 32, "%d", atoi(strrchr(group[i], '=')+1));
        strcat(buffer, t);

        LoadSetup_OK(NULL, NULL, NULL);

        break;
      }
    }
    else
    {
      if (cfg.FlightNumber().compare(0, 4, group[i], 4) == 0)
      {
        char *src;
        MakeProjectFileName(buffer, "%s/%s/%s/Production/Flight_");
        src = strtok(group[i], " \t\n:=");
        src = strtok(NULL, " \t\n:=");
        strcat(buffer, src);

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
    snprintf(t, 32, "%d", FlightNumberInt);
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
    snprintf(buffer, 8192, "%s/nimbus/*", getenv("HOME"));
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
//      dp->Dirty = true;
      dp->Output = 1 - dp->Output;

      item = CreateListLineItem(dp, DERIVED);
    }
    else
    {
      RAWTBL	*rp = raw[indx];
//      rp->Dirty = true;
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
  if (ncFile)
    ncFile->Close();

  extern PostgreSQL *psql;
  if (psql)
    psql->closeSQL();

  CloseRemoveLogFile();
  CancelSetup(NULL, NULL, NULL);
  exit(0);
}

/* -------------------------------------------------------------------- */
static int determineInputFileVersion()
{
  FILE *fp = fopen64(ADSfileName, "r");

  if (fp == 0)
  {
    char msg[512];
    snprintf(msg, 128, "Failed to open input file [%s], errno = %d.\n", ADSfileName, errno);
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

    // Attempt to determine flight number from filename.
    strcpy(tmp, ADSfileName);
    if ((p = strrchr(tmp, '/')) == 0)
      p = tmp;

    if ( (p = strtok(p+1, "_")) )
      if ( (p = strtok(NULL, "_")) )
        p = strtok(NULL, "_.");

    if (p)
      cfg.SetFlightNumber(p);
    else
      LogMessage("Unable to determine FlightNumber from file name.\n");
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
    char msg[512];
    snprintf(msg, 128, "Non-existent input file [%s].\n", ADSfileName);
    HandleError(msg);
    return(ERR);
  }

  struct stat st_buf;
  stat(ADSfileName, &st_buf);
  if ( S_ISDIR( st_buf.st_mode ) )
  {
    char msg[512];
    snprintf(msg, 128, "Input file [%s] is a directory.\n", ADSfileName);
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
  snprintf(buffer, 8192, "Output file [%s] exists.", OutputFileName);
  HandleWarning(buffer, StartProcessing, CancelSetup);

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
    snprintf(buffer, 8192, p, cfg.ProjectNumber().c_str(), cfg.FlightNumber().c_str());
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
XmString CreateListLineItem(var_base *pp, int var_type)
{
  RAWTBL	*rp;
  DERTBL	*dp;
  char		tmp[100];

  switch (var_type)
  {
    case RAW:
      rp = (RAWTBL *)pp;

      snprintf(buffer, 8192, list1lineFrmt, rp->name,
		rp->Dirty ? '*' : ' ', rp->Output ? 'Y' : 'N',
		rp->SampleRate, rp->OutputRate,
		rp->StaticLag, rp->SpikeSlope,
		rp->DataQuality[0]);

      for (size_t i = 0; i < rp->cof.size(); ++i)
      {
        snprintf(tmp, 100, "%10.4lf", rp->cof[i]);
        strcat(buffer, tmp);
      }

      break;

    case DERIVED:
      dp = (DERTBL *)pp;

      snprintf(buffer, 8192, list1lineFrmt, dp->name,
		dp->Dirty ? '*' : ' ', dp->Output ? 'Y' : 'N',
		0, dp->OutputRate, 0, 0.0, dp->DataQuality[0]);

      buffer[20] = 'N'; buffer[21] = 'A';
      buffer[33] = 'N'; buffer[34] = 'A';
      memcpy(&buffer[40], "NA    ", 5);

      for (size_t i = 0; i < dp->nDependencies; ++i)
      {
        if (i > 0)
          strcat(buffer, ",");

        snprintf(tmp, 100, " %s", dp->depend[i]);
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

  if (cfg.Interactive() == false)
    return;

  size_t cnt = 0;
  XmString items[MAX_VARIABLES];

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

  extern size_t	nDefaults;
  extern DEFAULT	*Defaults[];

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

    snprintf(buffer, 8192, list1lineFrmt,
			dp->name,
			dp->Dirty ? '*' : ' ',
			dp->Output ? 'Y' : 'N',
			0,
			dp->OutputRate,
			0, 0.0);

    buffer[33] = 'N'; buffer[34] = 'A';
    fprintf(fp, buffer);

    for (size_t j = 0; j < dp->nDependencies; ++j)
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
void ToggleAllOn(Widget w, XtPointer client, XtPointer call)
{
  for (size_t i = 0; i < raw.size(); ++i)
    raw[i]->Output = true;

  for (size_t i = 0; i < derived.size(); ++i)
    derived[i]->Output = true;

  FillListWidget();
}

/* -------------------------------------------------------------------- */
void ToggleAllOff(Widget w, XtPointer client, XtPointer call)
{
  for (size_t i = 0; i < raw.size(); ++i) {
    if (strcmp(raw[i]->name, "HOUR") == 0 ||
        strcmp(raw[i]->name, "MINUTE") == 0 ||
        strcmp(raw[i]->name, "SECOND") == 0)
      continue;

    raw[i]->Dirty = true;
    raw[i]->Output = false;
  }

  for (size_t i = 0; i < derived.size(); ++i) {
    derived[i]->Dirty = true;
    derived[i]->Output = false;
  }

  FillListWidget();
}

/* -------------------------------------------------------------------- */
void ToggleAllDerivedOff(Widget w, XtPointer client, XtPointer call)
{
  for (size_t i = 0; i < derived.size(); ++i) {
    derived[i]->Dirty = true;
    derived[i]->Output = false;
  }

  FillListWidget();
}

/* -------------------------------------------------------------------- */
void ToggleProbeOutput(Widget w, XtPointer client, XtPointer call)
{
  const char *suffix = (char *)client;

  for (size_t i = 0; i < raw.size(); ++i)
    if (strstr(raw[i]->name, suffix)) {
      raw[i]->Dirty = true;
      raw[i]->Output = !raw[i]->Output;
    }

  for (size_t i = 0; i < derived.size(); ++i)
    if (strstr(derived[i]->name, suffix)) {
      derived[i]->Dirty = true;
      derived[i]->Output = !derived[i]->Output;
    }

  FillListWidget();

}	/* END TOGGLEPROBEOUTPUT */

/* -------------------------------------------------------------------- */
void ToggleProbeDataQuality(Widget w, XtPointer client, XtPointer call)
{
  const char *suffix = (char *)client;
  const char *good = dataQuality[PRELIMINARY];

  if (cfg.ProductionRun())
    good = dataQuality[GOOD];

  for (size_t i = 0; i < raw.size(); ++i)
    if (strstr(raw[i]->name, suffix)) {
      raw[i]->Dirty = true;
      if (strcmp(raw[i]->DataQuality, dataQuality[BAD]))
        raw[i]->DataQuality = dataQuality[BAD];
      else
        raw[i]->DataQuality = good;
    }

  for (size_t i = 0; i < derived.size(); ++i)
    if (strstr(derived[i]->name, suffix)) {
      derived[i]->Dirty = true;
      if (strcmp(derived[i]->DataQuality, dataQuality[BAD]))
        derived[i]->DataQuality = dataQuality[BAD];
      else
        derived[i]->DataQuality = good;
    }

  FillListWidget();

}	/* END TOGGLEPROBEDATAQUALITY */

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
  fprintf(stderr, "SigHandler: signal=%s cleaning up netCDF file.\n",strsignal(s));
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


  if (SearchTable(derived, "WIX") != ERR)
    cfg.SetWindVertical("WIX");
  else
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
      snprintf(buffer, 8192, "%s: %zu bad Lags.\n", raw[i]->name, raw[i]->badLagCntr);
      LogMessage(buffer);
    }
  }
}

/* -------------------------------------------------------------------- */
static void EngageSignals()
{
  ILOG(("EngageSignals"));
  signal(SIGINT, sighandler);
  signal(SIGFPE, sighandler);
  signal(SIGTERM, sighandler);
  signal(SIGSEGV, sighandler);

}	/* END ENGAGESIGNALS */

/* END CB_MAIN.C */
