/*
-------------------------------------------------------------------------
OBJECT NAME:	hdr_decode.c

FULL NAME:	Decode Header

ENTRY POINTS:	DecodeHeader()
		DecodeHeader3()

DESCRIPTION:	Read header & add variables to appropriate table.  There
		are 2 major tables here:
			- Raw table (From analog & block probes)
			- Derived table for the derived variables.

		This module is the brunt end interface between an ADS tape
		and the rest of nimbus.

OUTPUT:		Variable vectors raw, derived
		(These globals are initialized in this file)

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2010
-------------------------------------------------------------------------
*/

#include <cctype>
#include <unistd.h>
#include <set>
#include <sys/types.h>
#include <sys/stat.h>
#include <float.h>


#include "nimbus.h"
#include "decode.h"
#include "gui.h"
#include "src/hdr_api/ctape.h"
#include <raf/pms.h>
#include <raf/vardb.hh>	// Variable DataBase
#include <raf/VarDBConverter.hh>
#include "mtp.h"

#include "sync_reader.hh"
#include <nidas/core/NidasApp.h>
#include <nidas/core/Project.h>

#include "amlib.h"

using nidas::dynld::raf::Aircraft;
using nidas::core::NidasApp;
using nidas::core::Project;

// ADS3 / nidas only
static nidas::dynld::raf::SyncRecordReader* syncRecReader = 0;
static nidas::core::Variable *nidas_var = 0;

VDBFile *vardb = 0; // Exports to cb_main.c and netcdf.c


typedef struct
  {
  const char	*name;
  int		type;	/* #defines used for switch's, in WINDS		*/
  size_t	VecLen;	/* VectorLength					*/
  size_t	pType;	/* probeType for 'ToggleProbe' menu, redundant	*/
  size_t	cnt;	/* Total # of these probes present.		*/
  } PMS;


static PMS	pms1_probes[] =
  {
  { FSSP_STR,	FSSP,	16,	PROBE_FSSP,	0 },
  { ASAS_STR,	ASAS,	16,	PROBE_PCASP,	0 },
  { F300_STR,	F300,	32,	PROBE_F300,	0 },
  { X200_STR,	X200,	16,	PROBE_200X,	0 },
  { Y200_STR,	Y200,	16,	PROBE_200Y,	0 },
  { X260_STR,	X260,	64,	PROBE_260X,	0 },
  { NULL,	0,	0,	0,		0 }
  };

static PMS	pms1v2_probes[] =
  {
  { FSSP_STR,	FSSP,	16,	PROBE_FSSP,	0 },
  { PCAS_STR,	ASAS,	16,	PROBE_PCASP,	0 },
  { F300_STR,	F300,	32,	PROBE_F300,	0 },
  { X200_STR,	X200,	16,	PROBE_200X,	0 },
  { Y200_STR,	Y200,	16,	PROBE_200Y,	0 },
  { X260_STR,	X260,	64,	PROBE_260X,	0 },
  { MASP_STR,	MASP,	32,	PROBE_200X,	0 },	// This only ever flew in INDOEX.
  { NULL,	0,	0,	0,		0 }
  };

static PMS	pms1v3_probes[] =
  {
  { SPP100_STR,	FSSP,	0,	PROBE_FSSP,	0 },	// These are the DMT versions.
  { SPP200_STR,	ASAS,	0,	PROBE_PCASP,	0 },
  { SPP300_STR,	F300,	0,	PROBE_F300,	0 },
  { NULL,	0,	0,	0,		0 }
  };

static PMS	pms2d_probes[] =
  {
  { P2D_C_STR,	0,	0,	PROBE_2DC,	0 },
  { P2D_P_STR,	0,	0,	PROBE_2DP,	0 },
  { P2D_G_STR,	0,	0,	PROBE_PMS2D,	0 },	// Greyscale.  Only flew this once.
  { P2D_H_STR,	0,	0,	PROBE_HVPS,	0 },
  { P2D_S_STR,	0,	0,	PROBE_2DS,	0 },
  { NULL,	0,	0,	0,		0 }
  };


static size_t	InertialSystemCount, GPScount, twoDcnt, NephCnt, GustCnt;
static size_t	probeCnt;
static size_t	probeType;
static int32_t	start, rate, length;
static std::string serialNumber;
static char	*item_type, location[NAMELEN];
static char	*derivedlist[MAX_DEFAULTS*4],	/* DeriveNames file	*/
		*rawlist[MAX_DEFAULTS*4];	/* RawNames file	*/


static RAWTBL	*initSDI_ADS3(const nidas::core::Variable* nidas_var, time_t startTime);
static RAWTBL	*add_name_to_RAWTBL(const char []);
static DERTBL	*add_name_to_DERTBL(const char []);

#ifdef notdef
static void	add_file_to_RAWTBL(const std::string&);
#endif

static void	add_file_to_DERTBL(const std::string&), initMTP(),
	initHDR(char vn[]), initSDI(char vn[]), initHoneywell(char vn[]),
	initOphir3(char vn[]), initPMS1D(char vn[]), initPMS1Dv2(char vn[]),
	addGustVariables(const char s[]), initGustCorrected(), initLitton51(char vn[]),
	add_derived_names(const char vn[]), initPMS2D(char vn[], int n),
	initPMS2Dhouse(char vn[]), add_raw_names(const char vn[]),
	initGreyHouse(char vn[]), initMASP(char vn[]), initPMS1Dv3(char vn[]),
	ReadProjectName(), initRDMA(char vn[]), initCLIMET(char vn[]),
	openVariableDatabase(), addUnitsAndLongName(var_base *vbp, const nidas::core::Variable *var),
	checkUnitsTitles();

static std::vector<NR_TYPE> getCalsForADS2(const char vn[]);
static int	locatePMS(const char target[], PMS list[]);


static const char *defaultQuality;

extern int32_t	INS_start;

char	*ExtractHeaderIntoFile(const char fn[]);
bool	Open2dFile(const char file[], int probeCnt);
void	ReadDespikeFile();
void	Add2DtoList(RAWTBL *varp), OpenLogFile(), GenerateProbeListADS3();


/* -------------------------------------------------------------------- */
bool VarCompareLT(const var_base *x, const var_base *y)
{
    return(strcmp(x->name, y->name) < 0);
}

/* -------------------------------------------------------------------- */
static void CommonPreInitialization()
{
  InertialSystemCount = GPScount = twoDcnt = NephCnt = 0;
  /* This used to init to 0, but AKY/WIY are not added via a full "GUST"
   * nameset.  So they need '0' in the swic() calc.  If they go away, then
   * this can go back to starting at zero.  swic() can have up to eight
   * wind/gust sets at once, so no worries if zero never used.
   *
   * Waiting to see how all the wind and angle of attack stuff shakes out on
   * the GV.  cjw 12/2018
   */
  GustCnt = 1;

  ReadProjectName();
  cfg.SetCoordTime("Time");

  if (cfg.ProductionRun())
    OpenLogFile();

  if (cfg.ProductionRun())
    defaultQuality = dataQuality[GOOD];
  else
    defaultQuality = dataQuality[PRELIMINARY];

  openVariableDatabase();

  ReadTextFile(RAWNAMES, rawlist);

  if (cfg.Aircraft() != Config::NOAA_G4)
    ReadTextFile(DERIVEDNAMES, derivedlist);
  else
    derivedlist[0] = 0;

  ReadDefaultsFile();

  MakeProjectFileName(buffer, PMS_SPEC_FILE);
  InitPMSspecs(buffer);

}	// END COMMONPREINITIALIZATION

/* -------------------------------------------------------------------- */
void AddProbeToList(const char name[], size_t type)
{
	// Stub function.  Previously in probe.c.  Refactored for ADS3.
}

/* -------------------------------------------------------------------- */
static void CommonPostInitialization()
{
  extern std::vector<RAWTBL *> decode; // raw variables where "xlate != 0" - for rec_decode.c

  /* Add the default derived variables.
   */
  rate = 1;

  if ( (cfg.ProjectName().compare("RAF_Lab") == 0) || (cfg.Aircraft() == Config::TADS) )
  {
    add_derived_names("RAF_LAB");
  }
  else
  if (cfg.Aircraft() != Config::SAILPLANE)
  {
    add_derived_names("ALWAYS");
    add_derived_names("HUMIDITY");
    AddProbeToList("Fluxes", (size_t)PROBE_FLUX);
  }
  else
  {
    add_derived_names("SAIL");
  }

  ReleasePMSspecs();

  if (cfg.Aircraft() != Config::NOAA_G4)
    FreeTextFile(derivedlist);

  FreeTextFile(rawlist);

  // Log any variables without units & titles.
  checkUnitsTitles();


  /* Add some final items to 'Toggle Probe' menu.
   */
  {
  int	cnt = 0;

  for (size_t i = 0; pms1_probes[i].name; ++i)
    cnt += pms1_probes[i].cnt;
  for (size_t i = 0; pms1v2_probes[i].name; ++i)
    cnt += pms1v2_probes[i].cnt;
  for (size_t i = 0; pms1v3_probes[i].name; ++i)
    cnt += pms1v3_probes[i].cnt;

  if (cnt > 1)
    AddProbeToList("All PMS1D's", (size_t)PROBE_PMS1D);

  cnt = 0;

  for (size_t i = 0; pms2d_probes[i].name; ++i)
    cnt += pms2d_probes[i].cnt;

  if (cnt > 1)
    AddProbeToList("All PMS2D's", (size_t)PROBE_PMS2D);
  }

  AddProbeToList("All On", (size_t)ALL_ON);
  AddProbeToList("All Off", (size_t)ALL_OFF);

  if (AccessProjectFile(USERNAMES, "r") == true)
    add_file_to_DERTBL(USERNAMES);

/*
for (size_t i = 0; i < raw.size(); ++i)
  printf("%-12s%5d\n", raw[i]->name, raw[i]->xlate);

for (size_t i = 0; i < derived.size(); ++i)
  printf("%-12s\n", derived[i]->name, derived[i]->compute);
*/

  std::sort(raw.begin(), raw.end(), VarCompareLT);
  std::sort(derived.begin(), derived.end(), VarCompareLT);

  ReadModuloVariables();
  ReadSumVariables();
  if (cfg.ProcessingMode() == Config::RealTime)
    ReadGroundVarsFile();

  for (size_t i = 0; i < raw.size(); ++i)
  {
    if (raw[i]->xlate)
      decode.push_back(raw[i]);

    if (cfg.ProcessingRate() == Config::SampleRate)
      raw[i]->OutputRate = raw[i]->SampleRate;
  }

  if (cfg.TimeShifting())
    ReadStaticLags();

  if (cfg.Despiking())
    ReadDespikeFile();

  ReadDefaultDerivedOutputRate();
  ReadDependencies();

}	// END COMMONPOSTINITIALIZATION

/* -------------------------------------------------------------------- */
static void addSerialNumber(nidas::core::Variable *nidas_var, var_base *rp)
{
  const std::list<const nidas::core::Parameter *> parms = nidas_var->getParameters();

  std::list<const nidas::core::Parameter *>::const_iterator it;
  for (it = parms.begin(); it != parms.end(); ++it)
  {
    if ((*it)->getName() == "SerialNumber")
    {
      rp->SerialNumber = (*it)->getStringValue(0);
      printf("Setting serial number for [%s] to [%s]\n", rp->name, rp->SerialNumber.c_str());
    }
  }
}

/* -------------------------------------------------------------------- */
#include <sys/types.h>
#include <sys/param.h>
#include <dirent.h>
#include <libgen.h>
#include <set>

std::set<std::string>
GetADSFileList(const std::string& adsFileName)
{
  std::set<std::string> fileList;
  DIR *dir;
  char tmp_dir[MAXPATHLEN];

  strcpy(tmp_dir, adsFileName.c_str());
  char *directory = dirname(tmp_dir);

  if ((dir = opendir(directory)) == 0)
  {
    std::ostringstream oss;
    oss << "Failed to open directory " << directory;
    perror(oss.str().c_str());
    return fileList;
  }

  struct dirent *entry;

  // Read directory entries & get all files with matching flight number.
  while ( (entry = readdir(dir)) )
    if ( strstr(entry->d_name, cfg.FlightNumber().c_str()) &&
         strstr(entry->d_name, cfg.ADSfileExtension().c_str()))
    {
      std::string s(directory);
      s += "/";
      s += entry->d_name;
      fileList.insert(s);
    }
  closedir(dir);

  std::set<std::string>::iterator it;
  for (it = fileList.begin(); it != fileList.end(); )
  {
    if ((*it).compare(adsFileName) < 0)
      it = fileList.erase(it);
    else
      ++it;
  }

  for (it = fileList.begin(); it != fileList.end(); ++it)
    printf("%s\n", (*it).c_str());

  return fileList;
}


std::set<std::string>
get_header_files(const std::string& header_file)
{
  std::set<std::string> fileList;

  if (cfg.ProcessingMode() == Config::PostProcessing)
  {
    // Acquire list of all files which match the first 4 chars of this one
    // plus are greater in time.  Pass this list of file names into
    // launch_ss (which runs sync_server).
    fileList = GetADSFileList(header_file);
  }
  // File list is empty in real-time mode, meaning get header directly from
  // DSM server.
  return fileList;
}



/* -------------------------------------------------------------------- */
int DecodeHeader3(const char header_file[])
{
  bool postprocess = (cfg.ProcessingMode() == Config::PostProcessing);

  std::set<std::string> headerfiles = get_header_files(header_file);

  // At this point we have header files which specify the XML path, and we
  // may have an XML path specified in the NidasApp instance.  From these
  // we need to find and parse the right XML file into a Project and
  // retrieve all the metadata.

  // However, we need to start reading the data stream to get the header.
  // Therefore we rely on SyncServer to populate the application Project
  // instance and then retrieve a pointer to it here.

  NidasApp& napp = *NidasApp::getApplicationInstance();
  Project* project = napp.getProject();
  SetSyncXMLPath(napp.xmlHeaderFile());


  // After calling this, the SyncServer should have created the project and
  // initialized all the sensors.
  syncRecReader = StartSyncReader(headerfiles, postprocess);

  //  const std::list<Site*>& sites = project->getSites();
  //  const std::list<Site*>::iterator si;

  Aircraft* aircraft = Aircraft::getAircraft(project);
  if (!aircraft)
  {
    std::cerr << "No Aircraft site found in XML file, exiting!" << std::endl;
    exit(1);
  }
  cfg.SetProjectNumber(aircraft->getProject()->getName());
  cfg.SetTailNumber(aircraft->getTailNumber());
  if (cfg.FlightNumber().length() == 0)
    cfg.SetFlightNumber(syncRecReader->getFlightName());
  if (cfg.FlightNumber().length() == 0)
    cfg.SetFlightNumber("uncle");
  cfg.SetNIDASrevision(nidas::core::Version::getSoftwareVersion());

  if (0)
  {
    // Using the VarDBConverter to open either a binary or xml vardb makes
    // this script obsolete.
    snprintf(buffer, 8192, "update_depend %s", cfg.ProjectNumber().c_str());
    system(buffer);
  }

printf("ProjectName: %s\n", cfg.ProjectNumber().c_str());
printf("TailNumber: %s\n", cfg.TailNumber().c_str());
printf("FlightNumber: %s\n", cfg.FlightNumber().c_str());

  {
  time_t t = syncRecReader->getStartTime();
  struct tm *flDate = gmtime(&t);
  snprintf(buffer, 8192, "%02d/%02d/%04d", flDate->tm_mon+1, flDate->tm_mday, flDate->tm_year+1900);
  cfg.SetFlightDate(buffer);
  }

  cfg.SetCoordLAT("GGLAT");
  cfg.SetCoordLON("GGLON");
  cfg.SetCoordALT("GGALT");

  char const *p = &cfg.TailNumber()[0];

  while (*p && !isdigit(*p))
    ++p;
  cfg.SetAircraft(atoi(p));


  // Perform common (ADS2 & ADS3).
  CommonPreInitialization();

  // Add Time variables, hour, min, sec, year, mon, day.
  initHDR(0);

  bool gustPodAdded = false;

  std::list<const nidas::core::Variable*> vars;
  //  vars = syncRecReader->getVariables();
  vars = selectVariablesFromProject(project);

  // Main loop
  time_t startTime = syncRecReader->getStartTime();
  std::list<const nidas::core::Variable*>::const_iterator vi;

  for (vi = vars.begin(); vi != vars.end(); ++vi)
  {
    nidas_var = const_cast<nidas::core::Variable*>(*vi);

    char name_sans_location[64];
    strcpy(name_sans_location, nidas_var->getName().c_str());
    char * p = strrchr(name_sans_location, '_');
    if (p)
    {
      strcpy(location, p);
      *p = '\0';
    }
    else
      location[0] = '\0';

    rate = (int)ceil(nidas_var->getSampleRate());

    if (rate == 0)
    {
      char msg[128];

      snprintf(msg, 128, "hdr_decode.c: %s: Assertion `SampleRate > 0' failed.", nidas_var->getName().c_str());
      LogMessage(msg);
      quit();
    }

    length = nidas_var->getLength();

    serialNumber = getSerialNumber(nidas_var);

//printf("DecodeHeader3: adding %s, converter = %d, rate = %d, %s\n",
//  nidas_var->getName().c_str(), (size_t)nidas_var->getConverter(), (int)ceil(nidas_var->getSampleRate()), serialNumber.c_str());

    // Add Gust Pod derived (once).  This can be cleaned up after sync_server merge.
    if (!gustPodAdded && strcmp(location, "_GP") == 0)
    {
      addGustVariables("GUSTPOD");
      gustPodAdded = true;
    }

    // If a variable does not have a converter (ie, it has no calibration
    // and no analog conversion from volts to engineering units), then it
    // is considered a Raw variable.  Historically, I think this meant the
    // variable was extracted from the instrument data stream, typically
    // some binary block format, and taken as is.  So Analog does not
    // necessarily mean an Analog A2D channel.

    // Either way, the nidas sync header introduced an inconsistency in
    // this logic.  When a variables calfile does not exist, NIDAS gives it
    // NANs for coefficients.  These NANs were printed in the sync header
    // but then could not be parsed on the client end.  So even though a
    // variable had a converter defined in the XML, it would not have a
    // converter in the sync record client.  This was "fixed" when the
    // sync_server was merged into nimbus, since the variable converter is
    // parsed and attached even if the file itself does not exist.

    // To be consistent with the behavior prior to the sync_server merge,
    // the variable converter is checked for NAN coefficients.  If the
    // coefficients are NAN, then the converter is ignored for the purpose
    // of identifying Analog variables.  The converter is not removed
    // because it is still needed to maintain the past behavior of
    // replacing the variable with NANs.  Note that it is perfectly valid
    // for a NIDAS calibration to have NAN coefficients, and ISFS even uses
    // this in practice to replace periods of bad data.  However, NIMBUS
    // does not use this intenionally anywhere else.  So really this kludge
    // is just to maintain consistency with previous kludges, and this
    // comment in no way implies that the current behavior is correct, only
    // that it is consistent with past behavior.

    nidas::core::VariableConverter* converter =
      const_cast<nidas::core::VariableConverter*>(nidas_var->getConverter());
    if (converter)
    {
      nidas::core::Polynomial* poly =
	dynamic_cast<nidas::core::Polynomial*>(converter);
      nidas::core::Linear* linear =
	dynamic_cast<nidas::core::Linear*>(converter);
      if (linear)
      {
	if (std::isnan(linear->getIntercept()) || std::isnan(linear->getSlope()))
	{
	  converter = 0;
	}
      }
      else if (poly)
      {
	const std::vector<float>& dcoefs = poly->getCoefficients();
	if (dcoefs.size() == 0 || std::isnan(dcoefs[0]))
	{
	  converter = 0;
	}
      }
    }

    RAWTBL *rp;
    if (converter == 0)
    {
      rp = add_name_to_RAWTBL(name_sans_location);
      rp->TTindx = syncRecReader->getLagOffset(nidas_var);

      rp->dsmID = nidas_var->getSampleTag()->getDSMId();

      // Default real-time netCDF to SampleRate.
      if (cfg.ProcessingMode() == Config::RealTime)
        rp->OutputRate = rp->SampleRate;

      length = 1;
    }
    else
    {
      rp = initSDI_ADS3(nidas_var, startTime);

      // Default real-time netCDF to SampleRate.
      if (cfg.ProcessingMode() == Config::RealTime)
        rp->OutputRate = rp->SampleRate;
    }

    if (!strcmp(rp->name,"SCNT_MTP"))
    {
      initMTP();
    }

    rp->nidasLag = getLag(nidas_var);
    rp->SerialNumber = serialNumber;
    add_derived_names(name_sans_location);

    if (rp->Units().compare("count") == 0)
    {
      printf("Treating %s as a counter\n", rp->name);
      strcpy(rp->type, "C");
      rp->Average = rp->Length > 1 ? (void (*) (...))SumVector : (void (*) (...))Sum;
    }
    else
    {
      strcpy(rp->type, "A");
      rp->Average = rp->Length > 1 ? (void (*) (...))AverageVector : (void (*) (...))Average;
    }


    // CDP pbp needs needs to be converted from A2D counts to particle size.
    // Doing it by creating a derived variable, will not let us get the 2hz out.
    if (strncmp(rp->name, "PBPSZ", 5) == 0)
    {
      rp->xlate = xlpbpsz;
      rp->channelThresholds = getChannelThresholds(nidas_var);
    }

    /* Raw variables that are copied for HRT filtering purposes on the GV for
     * the Pitot-static.
     * These are redundant and useless in a LRT run.
     */
    if (cfg.Aircraft() == Config::HIAPER)
    {
      if (strcmp(rp->name, "ADIFR") == 0)
      {
        const char *adiff = "ADIFF";
        RAWTBL *rp1 = new RAWTBL(adiff);// Create new RAWTBL entry.
        *rp1 = *rp;			// Duplicate ADIFR
        strcpy(rp1->name, adiff);	// Correct the name
        rp1->xlate = xladiff;
        raw.push_back(rp1);
        add_derived_names(adiff);

        addGustVariables("GUSTALT");
      }

      if (strcmp(rp->name, "PSF") == 0)
      {
        const char *psff = "PSFF";
        RAWTBL *rp1 = new RAWTBL(psff);	// Create new RAWTBL entry.
        *rp1 = *rp;			// Duplicate PSF
        strcpy(rp1->name, psff);	// Correct the name
        rp1->xlate = xlpsff;
        raw.push_back(rp1);
        add_derived_names(psff);
      }
    }


    location[0] = '\0';
//    addSerialNumber(nidas_var, rp);
  }

  serialNumber = "";

  if (cfg.ProjectName().compare("RAF_Lab") && (cfg.Aircraft() != Config::TADS) )
  {
    addGustVariables("GUST");
    initGustCorrected();
    probeCnt = 0;
  }

  CommonPostInitialization();
  GenerateProbeListADS3();

  // in pms1d.c, temporary until we get sync_server merged in.
  void PMS1D_SetupForADS3();
  PMS1D_SetupForADS3();

  return OK;
}

/* -------------------------------------------------------------------- */
static RAWTBL* initSDI_ADS3(const nidas::core::Variable* nidas_var, time_t startTime)
{
  RAWTBL *cp = new RAWTBL(nidas_var->getName().c_str());
  raw.push_back(cp);

  addUnitsAndLongName(cp, nidas_var);
//  cp->CategoryList.push_back("Housekeeping");

  cp->SampleRate   = rate;

  cp->TTindx = syncRecReader->getLagOffset(nidas_var);

  LoadCalibration(nidas_var, startTime, cp->cof);
  if (0)
  {
    printf("VAR %s cal coefficients: %f %f ...", nidas_var->getName().c_str(),
	   cp->cof[0], cp->cof[1]);
  }

  nidas::core::VariableConverter* converter =
    const_cast<nidas::core::VariableConverter*>(nidas_var->getConverter());
  if (converter)
  {
//    cp->AltUnits = nidas_var->getUnits();	// Do we really use AltUnits?  If I add to metadata, then it goes in the netCDF file regardless
    cp->addToMetadata("units", converter->getUnits());
  }

  return(cp);
}

/* -------------------------------------------------------------------- */
int DecodeHeader(const char header_file[])
{
  char	*vn;
  char	*loc, *p;

  for (probeCnt = 0; pms1_probes[probeCnt].name; ++probeCnt)
  {
    pms1_probes[probeCnt].cnt = 0;
    pms1v2_probes[probeCnt].cnt = 0;
    if (probeCnt < 3)
      pms1v3_probes[probeCnt].cnt = 0;
  }

  p = ExtractHeaderIntoFile(header_file);
  if (InitFlightHeader(p, CLOSE) == ERR)
  {
    snprintf(buffer, 8192, "Header decode failed, taperr = %d.", taperr);
    HandleError(buffer);
    unlink(p);
    free(p);
    return(ERR);
  }

  unlink(p);

  GetVersion(&p);
  HDRversion = atof(p);

  GetAircraft(&p);
  cfg.SetTailNumber(p);
  while (*p && !isdigit(*p))
    ++p;
  cfg.SetAircraft(atoi(p));

  if ((int)cfg.Aircraft() == 3)
    cfg.SetAircraft(Config::NRL_P3);

  GetFlightNumber(&p);
  cfg.SetFlightNumber(p);
  while (*p && !isdigit(*p))
    ++p;
  FlightNumberInt = atoi(p);

  GetProjectNumber(&p);
  cfg.SetProjectNumber(p);

  GetHeaderDate(&p);
  cfg.SetFlightDate(p);

  snprintf(buffer, 8192, "%s/%s", cfg.ProjectDirectory().c_str(), cfg.ProjectNumber().c_str());
  if (access(buffer, R_OK) == ERR)
  {
    snprintf(buffer, 8192, "No project directory for %s.", cfg.ProjectNumber().c_str());
    HandleError(buffer);
    return(ERR);
  }

  /* Old tapes don't set Aircraft field, so fudge it with proj_num.
  */
  if (cfg.Aircraft() == 0)
  {
    switch (cfg.ProjectNumber()[0])
    {
      case '2':
        cfg.SetAircraft(Config::KINGAIR);
        break;
      case '3':
        cfg.SetAircraft(Config::NRL_P3);
        break;
      case '6':
        cfg.SetAircraft(Config::TADS);
        break;
      case '7':
        cfg.SetAircraft(Config::SABRELINER);
        break;
      case '8':
        cfg.SetAircraft(Config::ELECTRA);
        break;
    }
  }

  // Perform common (ADS2 & ADS3).
  CommonPreInitialization();

  /* This is the main loop, loop through all variables in header
   */
  for (vn = GetFirst(); vn; vn = GetNext())
  {
    GetItemType(vn, &item_type);
    GetStart(vn, &start);

    if (GetRate(vn, &rate) == ERR)
      rate = 1;

    length = 1;
    serialNumber.clear();

    if (rate == 5000)
      continue;

    probeCnt = 0;
    probeType = 0;
    location[0] = '\0';
    SetLookupSuffix((char *)0);

    /* Initialize variable/probe into appropriate table(s).
    */
    if (!strcmp(item_type, SDI_STR) ||
        !strcmp(item_type, DIGOUT_STR) ||
        !strcmp(item_type, HSKP_STR))
    {
      initSDI(vn);
      add_derived_names(vn);
      continue;
    }


    /* Ok, it's not an analog/digital channel, must be a probe.
     */
    if (!strcmp(item_type, HDR_STR))
      initHDR(vn);
    else
    if (!strcmp(item_type, IRS_STR))
    {
      probeCnt = InertialSystemCount;
      probeType = PROBE_IRS;

      if (GetLocation(vn, &loc) ==  OK)
      {
        location[0] = '_';
        strcpy(&location[1], loc);
      }

      strcpy(buffer, item_type);
      strcat(buffer, location);
      AddProbeToList(buffer, probeType | probeCnt);

      initHoneywell(vn);
    }
    else
    if (!strcmp(item_type, PMS1D_STR))
    {
      probeType = PROBE_PMS1D;

      if (GetLocation(vn, &loc) ==  OK)
      {
        location[0] = '_';
        strcpy(&location[1], loc);
      }

      initPMS1D(vn);
    }
    else
    if (!strcmp(item_type, PMS1V2_STR))
    {
      probeType = PROBE_PMS1D;

      if (GetLocation(vn, &loc) ==  OK)
      {
        location[0] = '_';
        strcpy(&location[1], loc);
      }

      initPMS1Dv2(vn);
    }
    else
    if (!strcmp(item_type, PMS1V3_STR))
    {
      probeType = PROBE_PMS1D;

      if (GetLocation(vn, &loc) ==  OK)
      {
        location[0] = '_';
        strcpy(&location[1], loc);
      }

      initPMS1Dv3(vn);
    }
    else
    if (!strcmp(item_type, EVNT_STR))
    {
      if (GetLocation(vn, &loc) ==  OK)
      {
        location[0] = '_';
        strcpy(&location[1], loc);
      }

      add_raw_names(item_type);
    }
    else
    if (!strcmp(item_type, GPS_TRIM_STR) ||
        !strcmp(item_type, GPS_TANS2_STR) ||
        !strcmp(item_type, GPS_TANS3_STR) ||
        !strcmp(item_type, GPS_GARMIN_STR))
    {
      probeCnt = GPScount;
      probeType = PROBE_GPS;
      AddProbeToList(item_type, probeType);
      add_raw_names(item_type);
      add_derived_names(item_type);
      cfg.SetCoordLAT("GLAT");
      cfg.SetCoordLON("GLON");
      cfg.SetCoordALT("GALT");

      /* Force Garmin TRK & SPD to be depended upon, because of the way
       * we create GGVEW & GGVNS.
       */
      if (strcmp(item_type, GPS_GARMIN_STR) == 0)
      {
        int indx;

        if ((indx = SearchTable(raw, "GGTRK")) != ERR)
          raw[indx]->DependedUpon = true;
        if ((indx = SearchTable(raw, "GGSPD")) != ERR)
          raw[indx]->DependedUpon = true;

        cfg.SetCoordLAT("GGLAT");
        cfg.SetCoordLON("GGLON");
        cfg.SetCoordALT("GGALT");
      }

      initGustCorrected();
      ++GPScount;
    }
    else
    if (!strcmp(item_type, PMS2DH_STR))
    {
      probeType = PROBE_PMS2D;

      if (GetLocation(vn, &loc) ==  OK)
      {
        location[0] = '_';
        strcpy(&location[1], loc);
      }

      initPMS2Dhouse(vn);
    }
    else
    if (!strcmp(item_type, GREYVMEH_STR))
    {
      probeType = PROBE_PMS2D;

      if (GetLocation(vn, &loc) ==  OK)
      {
        location[0] = '_';
        strcpy(&location[1], loc);
      }

      initGreyHouse(vn);
    }
    else
    if (!strcmp(item_type, PPS_GPS_STR))
    {
      probeCnt = GPScount;
      probeType = PROBE_GPS;
      AddProbeToList(item_type, probeType);

      add_raw_names(item_type);

      ++GPScount;
    }
    else
    if (!strcmp(item_type, HW_GPS_STR))
    {
      probeCnt = GPScount;
      probeType = PROBE_GPS;

      if (GetLocation(vn, &loc) ==  OK)
      {
        location[0] = '_';
        strcpy(&location[1], loc);
      }

      AddProbeToList(item_type, probeType);
      add_raw_names(item_type);

      ++GPScount;
    }
    else
    if (!strcmp(item_type, NEPH903_STR))
    {
      snprintf(location, NAMELEN, "_%zu", ++NephCnt);
      add_raw_names(item_type);
    }
    else
    if (!strcmp(item_type, CMIGITS3_STR))
    {
      rate = 10;
      add_raw_names(item_type);
    }
    else
    if (!strcmp(item_type, DPRES_STR))
    {
      rate = 5;
      add_raw_names(item_type);
      add_derived_names(item_type);
    }
    else
    if (!strcmp(item_type, LHTDL_STR))
    {
      rate = 4;
      add_raw_names(item_type);
    }
    else
    if (!strcmp(item_type, MASP_STR))
    {
      probeType = PROBE_PMS1D;

      if (GetLocation(vn, &loc) == OK)
      {
        location[0] = '_';
        strcpy(&location[1], loc);
      }

      initMASP(item_type);
    }
    else
    if (!strcmp(item_type, OPHIR3_STR))
    {
      probeType = PROBE_OPHIR3;
      AddProbeToList(item_type, probeType);

      initOphir3(vn);
    }
    else
    if (!strcmp(item_type, UVHYG_STR))
    {
      probeType = PROBE_UVHYG;
      AddProbeToList(item_type, probeType);

      rate = sizeof(struct Uv_blk) / sizeof(struct Uv_smp);
      add_raw_names(item_type);
      add_derived_names(item_type);
    }
    else
    if (!strcmp(item_type, RDMA_STR))
    {
      probeType = PROBE_RDMA;
      strcpy(location, "_FCR");
      initRDMA(vn);
    }
    else


    /* Seldom used or retired instruments.
     */
    if (!strcmp(item_type, CLIMET_STR))
    {
      probeType = PROBE_CLMT;
      strcpy(location, "_C1");
      initCLIMET(vn);
    }
    else
    if (!strcmp(item_type, INS_STR))
    {
      probeCnt = InertialSystemCount;
      probeType = PROBE_IRS;

      AddProbeToList(item_type, probeType);

      initLitton51(vn);
    }
    else
    if (!strcmp(item_type, LRNC_STR) || !strcmp(item_type, DME_STR) ||
        !strcmp(item_type, NEPH_STR) || !strcmp(item_type, JPLTDL_STR))
    {
      add_raw_names(item_type);
      add_derived_names(item_type);
    }
    else
    if (!strcmp(item_type, GPS_MAG_STR))
    {
      LogMessage("Support for Magnavox GPS has been dropped.\n");
/*      probeCnt = GPScount;
      probeType = PROBE_GPS;
      AddProbeToList(item_type, probeType);

      add_file_to_RAWTBL(GPSMNAMES);
      add_derived_names(item_type);

      ++GPScount;
*/  }
    else


    /* else we're getting into some esoteric stuff.
     */
    if (!strcmp(item_type, AEROSOL_STR) ||  /* Ogren for INDOEX */
        !strcmp(item_type, MASSPEC_STR))    /* Mass-spectrometer for INDOEX */
    {
      add_raw_names(item_type);
    }
    else
    if (!strcmp(item_type, SER_STR))
      add_name_to_RAWTBL("SERIAL");
    else
    if (!strcmp(item_type, PMS2D_STR))
    {
//      if (Mode != REALTIME && Open2dFile(header_file, twoDcnt))
      if (Open2dFile(header_file, twoDcnt))
      {
        start = 0;
        rate = 1;

        strcpy(location, "_");
        if (GetLocation(vn, &loc) ==  OK)
          strcpy(&location[1], loc);
        strcat(location, "1");

        initPMS2D(vn, 0);
      }
    }
    else
    if (!strcmp(item_type, ASYNC_STR) && cfg.ProcessingMode() == Config::RealTime)
    {
      if (strcmp(vn, AVAPS_STR) == 0)
      {
        AVAPS = true;
        for (size_t i = 0; i < 4; ++i)
        {
          snprintf(location, NAMELEN, "_%02zu", i);
          add_raw_names(item_type);
        }
      }
      location[0] = '\0';
    }
    else
    {
      snprintf(buffer, 8192, "Unknown variable/probe %s encountered, ignoring & continuing.\n", item_type);
      LogMessage(buffer);
    }
  }


  probeType = 0;
  location[0] = '\0';

  if (cfg.ProcessingMode() == Config::RealTime)
  {
    for (probeCnt = 0; probeCnt < 3; ++probeCnt)
    {
      snprintf(location, NAMELEN, "_%zu", probeCnt);
      add_derived_names("PRCLTRK");
    }

    location[0] = '\0';
  }


  CommonPostInitialization();

  // For C130 check for AQRATIO.  No having produces bunk Pressure from the
  // radome since Al Schanot changed the PCOR in 2005.
  if (cfg.Aircraft() == Config::C130)
  {
    bool isDependedUpon(DERTBL *dp);

    int indx = SearchTable(derived, "AQRATIO");
    if (indx == ERR || isDependedUpon(derived[indx]) == false)
    {
      const char *msg = "\nC130 QC & PS variables now require AQRATIO.  Please fix DependTable.\n  Use a newer project as an example.  Older projects were not retro-fitted.\n";
      LogMessage(msg);
      fprintf(stderr, msg);
      XtSetSensitive(goButton, false);
    }
  }


  return(OK);

}	/* END DECODEHEADER */

/* -------------------------------------------------------------------- */
static void initHDR(char vn[])
{
  /* These items must be added to the tables first (i.e. don't
   * add any variables above the main loop in the fn above).
   */
  start = 0; rate = 1; length = 1;
  add_raw_names("TIME");

  /* Long term we want to
   * depracate TimeInterval in the global attributes and remove
   * HOUR, MINUTE and SECOND from the netCDF file as they are just
   * file bloat.  As of this writing, ncplot requires TimeInterval
   * and HOUR, MINUTE & SECOND are used internally in nimbus. cjw 4/9/06
   */
  {
  RAWTBL *rp;

  rp = raw[SearchTable(raw, "YEAR")];
  rp->DependedUpon = true;
  rp->Output = false;

  rp = raw[SearchTable(raw, "MONTH")];
  rp->DependedUpon = true;
  rp->Output = false;

  rp = raw[SearchTable(raw, "DAY")];
  rp->DependedUpon = true;
  rp->Output = false;

  rp = raw[SearchTable(raw, "HOUR")];
  rp->DependedUpon = true;
  rp->Output = false;

  rp = raw[SearchTable(raw, "MINUTE")];
  rp->DependedUpon = true;
  rp->Output = false;

  rp = raw[SearchTable(raw, "SECOND")];
  rp->DependedUpon = true;
  rp->Output = false;
  }
}	/* END INITHDR */

/* -------------------------------------------------------------------- */
static void initSDI(char vn[])
{
  char	*type;
  int	indx;

  if (strcmp(vn, "DUMMY") == 0)
    return;

  /* Some words about PSFD.  In the original ADS, 2 variables were entered
   * into the Sample Table, PSFD & PSFD2.  They were guaranteed to be
   * adjacent to each other, and so I moved PSFD into the RAWTBL list (see
   * xlpsfd().  In the new DSM ADS2, PSFD was no longer inverted and they
   * wanted to see the raw output, to complicate things, PSFD1 & PSFD2 were
   * no longer guaranteed to be adjacent.  So the Sample Table will now have
   * PSFD1 and PSFD2.  PSFD is placed into the RAWTBL.  CJW - 2/96
   */

  /* Phase out of PSFD2, PSFD is a 17 bit SDI variable, contained
   * in 2 variables PSFD2 & PSFD.  To handle it easily, we are
   * putting it into the raw table.  xlpsfd() will pick up the
   * high order word correctly.  CJW - 5/93
   */
  GetADStype(&type);

  if (strcmp(vn, "PSFD2") == 0 && strcmp(type, ADS_TYPE_1_STR) == 0)
    return;

  /* Rename analog TEO3C to TEO3.  Naming mistake by ProjMgmt/Campos.
   */
  if (strcmp(vn, "TEO3C") == 0)
  {
    LogMessage("Analog TEO3C being renamed to TEO3.\n");
    strcpy(vn, "TEO3");
  }

  /* Certain analog's require special processing, locate them here and
   * give them their special xlate function.
   */
  if ((indx = SearchDERIVEFTNS(vn)) != ERR && deriveftns[indx].xlate)
  {
    RAWTBL	*rp;
    int32_t	offset;

    rp = add_name_to_RAWTBL(vn);

/* Not sure this is necessary, we've had several projects where things that
 * were derived, became analog signals....

    if (rp->xlate == 0)
    {
      fprintf(stderr, "DecodeHeader fatal error: A derived variable name has been entered into the Sample Table.\n");
      fprintf(stderr, "Please remove [%s] from the Sample Table.\n", rp->name);
      exit(1);
    }
*/

    if (GetConversionOffset(vn, &(rp->convertOffset)) == ERR ||
        atoi(cfg.ProjectNumber().c_str()) == 818)
      rp->convertOffset = 0;

    GetConversionFactor(vn, &(rp->convertFactor));

    rp->cof = getCalsForADS2(vn);

    GetSampleOffset(vn, &offset);
    rp->ADSoffset = offset >> 1;


    if (strcmp(rp->name, "HGM") == 0)
      rp->cof.push_back(0.0);

    if (strncmp(rp->name, "PSFD", 4) == 0 ||
        strcmp(rp->name, "HGM232") == 0 ||
        strcmp(rp->name, "CHGME") == 0)
      rp->DependedUpon = true;

    if (strcmp(rp->name, "HGM232") == 0) {	/* Status bit for APN-232 */
      rp = add_name_to_RAWTBL("HGM232S");
      rp->ADSoffset = offset >> 1;
    }

    if (strcmp(vn, "PSFD1") == 0) {
      rp->name[4] = '\0';
      add_derived_names("PSFD");
    }
    else
      return;
  }


  /* Ok, it's strictly nth order polynomial.
   */
  RAWTBL *cp = new RAWTBL(vn);
  raw.push_back(cp);

  addUnitsAndLongName(cp, 0);
  cp->CategoryList.push_back("Housekeeping");

  if (GetConversionOffset(vn, &(cp->convertOffset)) == ERR)
    cp->convertOffset = 0;

  GetConversionFactor(vn, &(cp->convertFactor));
  GetSampleOffset(vn, &(cp->ADSoffset));
  GetType(vn, &type);

  strcpy(cp->type, type);
  cp->SampleRate	= rate;
  cp->ADSstart		= start >> 1;
  cp->ADSoffset		>>= 1;
  cp->xlate		= decodeADS2analog;
  cp->Average		= (void (*) (...))(cp->type[0] == 'C' ? Sum : Average);

  if (strncmp(cp->name, "PSFD", 4) == 0)
    cp->DependedUpon = true;

  cp->cof = getCalsForADS2(vn);

}	/* END INITSDI */

/* -------------------------------------------------------------------- */
/* NOTE: this proc is assuming the primary IRS probe is stored in the
* header first.  Currently this is LEFT PIT.  xbuild.c is responsible
* for insuring this.
*/
static void initHoneywell(char vn[])
{
  RAWTBL	*rp;
  char		*names[50];
  char		name[NAMELEN];

  if (InertialSystemCount == 0)
    location[0] = '\0';

  /* This is how I resolv conflicting names between IRS & INS
   */
  SetLookupSuffix("_IRS");

  ReadTextFile(IRSNAMES, names);

  for (int i = 0; names[i]; ++i)
  {
    sscanf(names[i], "%s %d", name, &rate);

    if ((rp = add_name_to_RAWTBL(name)) == (RAWTBL *)ERR)
      continue;

    if (strncmp(rp->name, "LAG", 3) != 0)
      rp->DynamicLag = 1;	/* StartProcessing() will add to lag List */
  }


  FreeTextFile(names);

  if (InertialSystemCount++ == 0)
  {
    add_derived_names(item_type);
    SetLookupSuffix((char *)0);
    add_derived_names("GUST");
    ++GustCnt;
  }
  else
    SetLookupSuffix((char *)0);

}	/* END INITHONEYWELL */

/* -------------------------------------------------------------------- */
static void addGustVariables(const char name[])
{
  probeCnt = GustCnt++;
  add_derived_names(name);
  probeCnt = 0;

}

/* -------------------------------------------------------------------- */
static void initGustCorrected()
{
  /* We can only have 1 corrected winds, bail if already initialized.
   */
  if (GPScount > 0)
    return;

  if (cfg.Aircraft() != Config::SAILPLANE)
  {
    probeType = PROBE_GUSTC;
    AddProbeToList("Corrected Winds", (size_t)PROBE_GUSTC);

    /* ProbeCnt here relies on the fact that hdrbld puts inertials before
     * GPSs.
     */
    add_derived_names("POSNC");
    addGustVariables("GUSTC");
    cfg.SetCoordLAT("LATC");
    cfg.SetCoordLON("LONC");
  }

}	/* END INITGUSTCORRECTED */

/* -------------------------------------------------------------------- */
static void initLitton51(char vn[])
{
  char	*names[50];
  char	name[NAMELEN];

  /* Used by adsIO.c to lag the INS block 1 full second
   */
  LITTON51_present = true;
  LITTON51_start = start;

  /* This is how I resolve conflicting names between IRS & INS
   */
  SetLookupSuffix("_INS");

  ReadTextFile(INSNAMES, names);

  for (int i = 0; names[i]; ++i)
  {
    sscanf(names[i], "%s %d", name, &rate);
    add_name_to_RAWTBL(name);
  }

  FreeTextFile(names);

  if (InertialSystemCount++ == 0)
  {
    add_derived_names(item_type);
    SetLookupSuffix((char *)0);
    add_derived_names("GUST");
    ++GustCnt;
  }
  else
    SetLookupSuffix((char *)0);

}	/* END INITLITTON51 */

/* -------------------------------------------------------------------- */
static void initOphir3(char vn[])
{
  RAWTBL	*rp;
  char		*list[32];

  ReadTextFile(OPHIR3NAMES, list);

  for (size_t i = 0; list[i]; ++i)
  {
    if ((rp = add_name_to_RAWTBL(strtok(list[i], " \t"))) == (RAWTBL *)ERR)
      continue;

    rp->convertOffset	= 0;
    rp->convertFactor	= 1.0;

    size_t order = atoi(strtok((char *)NULL, " \t"));
    for (size_t j = 0; j < order; ++j)
      rp->cof.push_back((NR_TYPE)atof(strtok((char *)NULL, " \t")));
  }

  FreeTextFile(list);
  add_derived_names(item_type);

}	/* END IN_OPHIR3 */

/* -------------------------------------------------------------------- */
static void initMTP()
{
  int		nbins = NUM_CHANNELS*NUM_SCAN_ANGLES; // Constant set in mtp.h
  int		nbinsl = NUM_RETR_LVLS; // Constant set in mtp.h
  DERTBL	*dp;

  // Initialize brightness temperature
  dp = add_name_to_DERTBL("SCANBT");
  dp->Length = nbins;

  // Initialize physical temperature profile
  dp = add_name_to_DERTBL("TEMPC");
  dp->Length = nbinsl;
}
/* -------------------------------------------------------------------- */
static void initMASP(char vn[])
{
  int		indx, nbins = 1;
  RAWTBL	*rp;
  DERTBL	*dp;
  char		temp[NAMELEN];

  if ((indx = locatePMS(vn, pms1v2_probes)) == ERR)
  {
    char	msg[128];

    snprintf(msg, 128, "Unknown MASP probe: %s, continuing\n", vn);
    LogMessage(msg);
    return;
  }

  probeCnt	= pms1v2_probes[indx].cnt++;
  probeType	|= pms1v2_probes[indx].pType;

  strcpy(buffer, vn);
  strcat(buffer, location);
  AddProbeToList(buffer, pms1v2_probes[indx].pType | probeCnt);

  /* For real-time WINDS.  Calls pms1d.c for nimbus.
   */
  strcpy(temp, vn);
  strcat(temp, "_DEF");
  AddToPMS1DprobeList(vn, location, temp, pms1v2_probes[indx].type);


  /* Get raw NAMES.
   */
  strcpy(temp, "A");	/* Actual	*/
  strcat(temp, vn);

  serialNumber = vn;
  serialNumber += "_DEF";

  if ((rp = add_name_to_RAWTBL(temp)) == (RAWTBL *)ERR)
    return;

  rp->Length	= nbins = BINS_32;
  rp->Average	= (void (*) (...))SumVector;


{	/* Perform add_derived_names manually	*/
  char	*p;
  char	buff[128];

  temp[0] = 'C';		/* Concentration	*/

  if ( (p = SearchList(derivedlist, vn)) )
  {
    strcpy(buff, p);
    p = strtok(buff, " \t");

    while ( (p = strtok((char *)NULL, " \t")) )
    {
      if ((dp = add_name_to_DERTBL(p)) == (DERTBL *)ERR)
        continue;

      dp->Default_HR_OR = rate;

      /* Make sure CMASP gets proper space alloc
       */
      if (strcmp(p, temp) == 0)
        dp->Length = BINS_32;
    }
  }
}
  probeCnt = 0;

}	/* END INITMASP */

/* -------------------------------------------------------------------- */
static void initCLIMET(char vn[])
{
  int		indx;

  serialNumber = "CLIMET1";
  add_raw_names(vn);
  add_derived_names(vn);

  strcpy(buffer, "ACLMT"); strcat(buffer, location);
  if ((indx = SearchTable(raw, buffer)) == ERR)
    HandleFatalError("ACLMT not found, fatal, update $PROJ_DIR/Configuration/RawNames");

  raw[indx]->Length = 7;

  strcpy(buffer, "CCLMT"); strcat(buffer, location);
  if ((indx = SearchTable(derived, buffer)) == ERR)
    HandleFatalError("CCLMT not found, fatal, update $PROJ_DIR/Configuration/DerivedNames");

  derived[indx]->Length = 7;

  AddProbeToList(vn, probeType);
  AddToPMS1DprobeList("CLMT", location, "CLIMET1", PROBE_CLMT);

}

/* -------------------------------------------------------------------- */
static void initRDMA(char vn[])
{
  int		indx;

  serialNumber = "RDMA1";
  add_raw_names(vn);
  add_derived_names(vn);

  strcpy(buffer, "ARDMA"); strcat(buffer, location);
  if ((indx = SearchTable(raw, buffer)) == ERR)
    HandleFatalError("ARDMA not found, fatal, update $PROJ_DIR/Configuration/RawNames");

  raw[indx]->Length = 64;

  strcpy(buffer, "CRDMA"); strcat(buffer, location);
  if ((indx = SearchTable(derived, buffer)) == ERR)
    HandleFatalError("CRDMA not found, fatal, update $PROJ_DIR/Configuration/DerivedNames");

  derived[indx]->Length = 64;

  for (size_t i = 0; i < derived.size(); ++i)
    if (derived[i]->ProbeType == PROBE_RDMA)
      derived[i]->Default_HR_OR = rate;

  AddProbeToList(vn, probeType);

}	/* END INITRDMA */

/* -------------------------------------------------------------------- */
static void initPMS1D(char vn[])
{
  int		indx;
  RAWTBL	*rp;
  DERTBL	*dp;
  char		*probe, temp[NAMELEN], *p, buff[128];


  GetName(vn, &probe);

  if ((indx = locatePMS(probe, pms1_probes)) == ERR)
  {
    char	msg[128];

    snprintf(msg, 128, "Unknown pms1d probe: %s, continuing\n", probe);
    LogMessage(msg);
    return;
  }

  probeCnt	= pms1_probes[indx].cnt++ + pms1v2_probes[indx].cnt;
  probeType	|= pms1_probes[indx].pType;

  if (indx < 3)
    probeCnt += pms1v3_probes[indx].cnt;

  strcpy(buffer, probe);
  strcat(buffer, location);
  AddProbeToList(buffer, pms1_probes[indx].pType | probeCnt);

  /* For real-time WINDS.  Calls pms1d.c for nimbus.
   */
  strcpy(temp, probe);
  strcat(temp, "_DEF");
  AddToPMS1DprobeList(probe, location, temp, pms1_probes[indx].type);


  /* Get raw NAMES.
   */
  SetLookupSuffix("_OLD");

  strcpy(temp, "A");	/* Actual	*/
  strcat(temp, probe);
  serialNumber = probe;
  serialNumber += "_DEF";

  if ( (p = SearchList(rawlist, probe)) )
  {
    strcpy(buff, p);
    p = strtok(buff, " \t");

    while ( (p = strtok((char *)NULL, " \t")) )
      {
      if ((rp = add_name_to_RAWTBL(p)) == (RAWTBL *)ERR)
        continue;

      if (strcmp(p, temp) == 0)
      {
        rp->Length	= pms1_probes[indx].VecLen;
        rp->Average	= (void (*) (...))SumVector;
      }
    }
  }

  SetLookupSuffix((char *)0);


  /* Perform add_derived_names manually	*/

  temp[0] = 'C';		/* Concentration	*/

  if ( (p = SearchList(derivedlist, probe)) )
  {
    strcpy(buff, p);
    p = strtok(buff, " \t");

    while ( (p = strtok((char *)NULL, " \t")) )
    {
      if ((dp = add_name_to_DERTBL(p)) == (DERTBL *)ERR)
        continue;

      dp->Default_HR_OR = rate;

      /* Make sure CFSSP, C200X, etc, get proper space alloc
       */
      if (strcmp(p, temp) == 0)
        dp->Length = pms1_probes[indx].VecLen;
    }
  }

  probeCnt = 0;

}	/* END INITPMS1D */

/* -------------------------------------------------------------------- */
static void initPMS1Dv2(char vn[])
{
  int		indx;
  RAWTBL	*rp;
  DERTBL	*dp;
  strnam	*hsk_name;
  char		*probe, temp[NAMELEN], *p, buff[128], name[32];
  float		*cals;
  int32_t	ps_start, ps_length;

  GetName(vn, &probe);

  if ((indx = locatePMS(probe, pms1v2_probes)) == ERR)
  {
    char	msg[128];

    snprintf(msg, 128, "Unknown pms1d probe: %s, continuing\n", probe);
    LogMessage(msg);
    return;
  }

  probeCnt   = pms1_probes[indx].cnt + pms1v2_probes[indx].cnt++;
  probeType |= pms1v2_probes[indx].pType;

  if (indx < 3)
    probeCnt += pms1v3_probes[indx].cnt;

  strcpy(buffer, probe);
  strcat(buffer, location);
  AddProbeToList(buffer, pms1_probes[indx].pType | probeCnt);

  if (GetSerialNumber(vn, &p) != ERR)
    serialNumber = p;
  else
  {
    serialNumber = probe;
    serialNumber += "_DEF";
  }

  AddToPMS1DprobeList(probe, location, serialNumber.c_str(), pms1v2_probes[indx].type);


  /* Add Housekeeping variables to RAWTBL
   */
  GetHouseKeepingNames(vn, (char **)&hsk_name);
  GetCalCoeff(vn, &cals);

  for (size_t i = 0; i < P1DV2_AUX; ++i)
  {
    if (strcmp(hsk_name[i], "DUMMY") == 0 || strcmp(hsk_name[i], "") == 0)
      continue;

    strcpy(name, hsk_name[i]);

    /* F300 has same names as F100, so tack a '3' on
     */
    if (pms1_probes[indx].pType == F300)
      strcat(name, "3");

    if ((rp = add_name_to_RAWTBL(name)) == (RAWTBL *)ERR)
      continue;

    size_t order = P1DV2_COF;

    for (size_t j = 0; j < order; ++j)
      rp->cof.push_back(cals[i*P1DV2_COF+j]);
  }


  /* Read raw variable names from *.names
   */
  strcpy(name, probe);
  if (strcmp(probe, "FSSP") == 0)
    strcat(name, "V2");

  if ( (p = SearchList(rawlist, name)) )
  {
    strcpy(temp, "A");	/* Actual	*/
    strcat(temp, probe);

    strcpy(buff, p);
    p = strtok(buff, " \t");

    while ( (p = strtok((char *)NULL, " \t")) )
    {
      strcpy(name, p);
      if ((rp = add_name_to_RAWTBL(name)) == (RAWTBL *)ERR)
        continue;

      if (strcmp(name, temp) == 0)
        {
        rp->Length	= pms1v2_probes[indx].VecLen;
        rp->Average	= (void (*) (...))SumVector;
      }
    }
  }


  /* Check for Particle Spacing and add.
   */
  GetParticleSpacingStart(vn, &ps_start);
  GetParticleSpacingLength(vn, &ps_length);

  if (ps_start > 0 && ps_length > 0)
  {
    temp[0] = 'P';

    if ((rp = add_name_to_RAWTBL(temp)) != (RAWTBL *)ERR)
    {
      rp->ADSstart	= ps_start >> 1;
      rp->Length	= (ps_length / rate) >> 1;
      rp->Average	= (void (*) (...))SumVector;
    }
  }


  /* Read in derived names
   */
  strcpy(name, probe);
  if (strcmp(probe, "FSSP") == 0)
    strcat(name, "V2");


  /* Perform add_derived_names manually	*/

  temp[0] = 'C';

  if ( (p = SearchList(derivedlist, name)) )
  {
    strcpy(buff, p);
    p = strtok(buff, " \t");

    while ( (p = strtok((char *)NULL, " \t")) )
    {
      if ((dp = add_name_to_DERTBL(p)) == (DERTBL *)ERR)
        continue;

      dp->Default_HR_OR = rate;

      /* Make sure CFSSP, C200X, etc, get proper space alloc
       */
      if (strcmp(p, temp) == 0)
        dp->Length = pms1v2_probes[indx].VecLen;
    }
  }

  probeCnt = 0;

}	/* END INITPMS1DV2 */

/* -------------------------------------------------------------------- */
static void initPMS1Dv3(char vn[])
{
  int		indx;
  int		nbins;
  RAWTBL	*rp;
  DERTBL	*dp;
  char		*p, *probe, temp[NAMELEN];


  GetName(vn, &probe);

  if ((indx = locatePMS(probe, pms1v3_probes)) == ERR)
  {
    char	msg[128];

    snprintf(msg, 128, "Unknown pms1d probe: %s, continuing\n", probe);
    LogMessage(msg);
    return;
  }

  probeCnt	= pms1_probes[indx].cnt++ + pms1v2_probes[indx].cnt +
		pms1v3_probes[indx].cnt;
  probeType	|= pms1v3_probes[indx].pType;

  strcpy(buffer, probe);
  strcat(buffer, location);

  if (GetSerialNumber(vn, &p) != ERR)
    serialNumber = p;
  else
  {
    serialNumber = probe;
    serialNumber += "_DEF";
  }

  AddProbeToList(buffer, pms1_probes[indx].pType | probeCnt);

  /* For real-time WINDS.  Calls pms1d.c for nimbus.
   */
  AddToPMS1DprobeList(probe, location, serialNumber.c_str(), pms1_probes[indx].type);


  /* Get raw NAMES.
   */
  strcpy(temp, "A");	/* Actual	*/
  strcat(temp, probe);

  if ((rp = add_name_to_RAWTBL(temp)) != (RAWTBL *)ERR)
  {
    GetNumberBins(vn, &nbins);
    ++nbins;		/* DMT uses bin 0, mimick old style by adding an extra bin. */
    rp->Length	= nbins;
    rp->Average	= (void (*) (...))SumVector;
  }

  SetLookupSuffix("_V3");

  /* Housekeeping names and cals from PMSspecs */
  for (int i = 0; i < 8; ++i)
  {
    int j;

    snprintf(buffer, 8192, "HSKP%d", i);
    p = GetPMSparameter(serialNumber.c_str(), buffer);

    if (strncmp(p, "DUMMY", 5) == 0)
      continue;

    strcpy(buffer, p);
    p = strtok(buffer, " \t\n");

    rp = add_name_to_RAWTBL(p);

    for (j = 0; (p = strtok(NULL, " \t\n")); ++j)
      rp->cof.push_back(atof(p));
  }

  if (strncmp(temp, "AS100", 5) == 0)
  {
    add_name_to_RAWTBL("FRNG");
    add_name_to_RAWTBL("REJDOF");
    add_name_to_RAWTBL("REJAT");
    add_name_to_RAWTBL("OVFLW");
  }

  SetLookupSuffix((char *)0);


{	/* Perform add_derived_names manually	*/
  char	*p;
  char	buff[128];

  temp[0] = 'C';		/* Concentration	*/

  if ( (p = SearchList(derivedlist, probe)) )
  {
    strcpy(buff, p);
    p = strtok(buff, " \t");

    while ( (p = strtok((char *)NULL, " \t")) )
    {
      if ((dp = add_name_to_DERTBL(p)) == (DERTBL *)ERR)
        continue;

      dp->Default_HR_OR = rate;

      /* Make sure CFSSP, C200X, etc, get proper space alloc
       */
      if (strcmp(p, temp) == 0)
        dp->Length = nbins;
    }
  }
}
  probeCnt = 0;

}	/* END INITPMS1DV3 */

/* -------------------------------------------------------------------- */
static void initGreyHouse(char vn[])
{
  strnam	*hsk_name;
  float		*cals;
  RAWTBL	*rp;


  /* Add Housekeeping variables to RAWTBL
   */
  GetHouseKeepingNames(vn, (char **)&hsk_name);
  GetCalCoeff(vn, &cals);

  for (int i = 0; i < GREY_HOUSE; ++i)
  {
    if (strcmp(hsk_name[i], "DUMMY") == 0 || strcmp(hsk_name[i], "") == 0)
      continue;

    if ((rp = add_name_to_RAWTBL(hsk_name[i])) == (RAWTBL *)ERR)
      continue;

    size_t order = P1DV2_COF;

    for (size_t j = 0; j < order; ++j)
      rp->cof.push_back(cals[i*P1DV2_COF+j]);

    add_derived_names(hsk_name[i]);
  }

}	/* END INITGREYHOUSE */

/* -------------------------------------------------------------------- */
static void initPMS2Dhouse(char vn[])
{
  strnam	*hsk_name;
  float		*cals;
  RAWTBL	*rp;
  char		name[64];


  /* Add Housekeeping variables to RAWTBL
   */
  GetHouseKeepingNames(vn, (char **)&hsk_name);
  GetCalCoeff(vn, &cals);

  for (int i = 0; i < P2D_HOUSE; ++i)
  {
    if (strcmp(hsk_name[i], "DUMMY") == 0 || strcmp(hsk_name[i], "") == 0)
      continue;

    if (isdigit(hsk_name[i][0]))
    {
      name[0] = 'H';
      strcpy(&name[1], hsk_name[i]);
    }
    else
      strcpy(name, hsk_name[i]);


    if ((rp = add_name_to_RAWTBL(name)) == (RAWTBL *)ERR)
      continue;

    size_t order = P1DV2_COF;

    for (size_t j = 0; j < order; ++j)
      rp->cof.push_back(cals[i*P1DV2_COF+j]);

    add_derived_names(name);
  }

}	/* END INITPMS2DHOUSE */

/* -------------------------------------------------------------------- */
static void initPMS2D(char vn[], int order)
{
  int		indx, nBins;
  char		*p, name[32], buff[128];
  RAWTBL	*rp;
  DERTBL	*dp;

  if ((indx = locatePMS(vn, pms2d_probes)) == ERR)
  {
    char	msg[128];

    snprintf(msg, 128, "Unknown pms2d probe: %s, continuing\n", vn);
    LogMessage(msg);
    return;
  }

  pms2d_probes[indx].cnt++;

/*  Async pointers into seperate binary file.  <<<<<<<<<<<<<<<<<<<<
    rp = add_name_to_RAWTBL(vn);
    rp->Output	= false;
    rp->Average	= SumVector;
    rp->Length	= 3;*/  /* (Offset, ItemLength, nItems) */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

  /* Create 2 sets of arrays.  Set one [A1DC/C1DC] is edge rejected, height
   * only.  Set two (A2DC/C2DC) is for particle reconstruction.
   */
  probeCnt = (twoDcnt << 1);

  for (order = 0; order < 2; ++order)
  {
    if (order == 1 && strcmp(vn, "2D-H") == 0)
      break;

    probeCnt += order;

    if (order == 0)
    {
      if (strcmp(vn, "2D-H") == 0)
      {
        nBins = 256;
        strcpy(buffer, "AHVPS");
      }
      else
      {
        nBins = BINS_32;
        strcpy(buffer, "A1D");
      }
    }
    else
    {
      nBins = BINS_64;
      strcpy(buffer, "A2D");
    }

    if (strcmp(vn, "2D-H")) {
      buffer[3] = vn[3]; buffer[4] = '\0';
    }

    probeType = PROBE_PMS2D | pms2d_probes[indx].pType;
    AddProbeToList(buffer, pms2d_probes[indx].pType | probeCnt);
//printf("initPMS2D: rp = %s %d\n", buffer, probeCnt);

    if (GetSerialNumber(vn, &p) != ERR)
      serialNumber = p;
    else
    {
      serialNumber = buffer;
      serialNumber += "_DEF";
    }

    rp = add_name_to_RAWTBL(buffer);
    rp->Average	= (void (*) (...))SumVector;
    rp->Length	= nBins;

    if (order == 0)
      Add2DtoList(rp);

    snprintf(name, 32, "DT%s", &buffer[1]);
    rp = add_name_to_RAWTBL(name);

    /* Perform add_derived_names manually   */

    buffer[0] = 'C';

    if ( (p = SearchList(derivedlist, &buffer[1])) )
    {
      strcpy(buff, p);
      p = strtok(buff, " \t");

      while ( (p = strtok((char *)NULL, " \t")) )
      {
        if ((dp = add_name_to_DERTBL(p)) == (DERTBL *)ERR)
          continue;

        dp->Default_HR_OR = rate;

        /* Make sure CFSSP, C200X, etc, get proper space alloc
         */
        if (strcmp(p, buffer) == 0)
          dp->Length = nBins;
      }
    }

    ++location[strlen(location)-1];
  }

  probeCnt = 0;
  ++twoDcnt;

}	/* END INITPMS2D */


/* -------------------------------------------------------------------- */
/* Header Decode Atomic functions					*/
/* -------------------------------------------------------------------- */
static void add_raw_names(const char name[])
{
  char  *p;
  char  buff[512];

  /* Find variable in raw list and add ALL associated names to raw table.
   */
  if ( (p = SearchList(rawlist, name)) )
  {
    strcpy(buff, p);
    p = strtok(buff, " \t");

    while ( (p = strtok((char *)NULL, " \t")) )
      add_name_to_RAWTBL(p);
  }

}       /* END ADD_RAW_NAMES */

/* -------------------------------------------------------------------- */
static void add_derived_names(const char name[])
{
  char	*p;
  char	buff[512];

  /* Find variable in derived list and add ALL associated names to
   * derived table.
   */
  if ( (p = SearchList(derivedlist, name)) )
  {
    strcpy(buff, p);
    p = strtok(buff, " \t");

    while ( (p = strtok((char *)NULL, " \t")) )
      add_name_to_DERTBL(p);
  }

}	/* END ADD_DERIVED_NAMES */

/* -------------------------------------------------------------------- */
static void add_file_to_DERTBL(const std::string& filename)
{
  FILE	*fp;

  fp = OpenProjectFile(filename, "r", EXIT);

  while (fscanf(fp, "%s", buffer) != EOF)
    if (buffer[0] != COMMENT)
      add_name_to_DERTBL(buffer);

  fclose(fp);

}	/* END ADD_FILE_TO_DERTBL */

#ifdef notdef
/* -------------------------------------------------------------------- */
static void add_file_to_RAWTBL(const std::string& filename)
{
  FILE	*fp;

  fp = OpenProjectFile(filename, "r", EXIT);

  while (fscanf(fp, "%s", buffer) != EOF)
    if (buffer[0] != COMMENT)
      add_name_to_RAWTBL(buffer);

  fclose(fp);

}	/* END ADD_FILE_TO_RAWTBL */
#endif

/* -------------------------------------------------------------------- */
static RAWTBL *add_name_to_RAWTBL(const char name[])
{
  int indx = SearchDERIVEFTNS(name);
  char fullName[64];
  strcpy(fullName, name);
  strcat(fullName, location);

  if (indx == ERR && (cfg.isADS2() || nidas_var == 0))
  {
    char msg[128];

    snprintf(msg, 128, "add_name_to_RAWTBL: Throwing away %s, has no decode function.\n", fullName);
    LogMessage(msg);
    return((RAWTBL *)ERR);
  }

  if (cfg.isADS3())
    indx = ERR;

  if (SearchTable(derived, name) != ERR)
  {
    char	msg[128];

    snprintf(msg, 128, "Will not add %s to raw table, it already exists in derived table.\n", name);
    LogMessage(msg);
    return((RAWTBL *)ERR);
  }

  RAWTBL *rp = new RAWTBL(fullName);
  raw.push_back(rp);
  addUnitsAndLongName(rp, nidas_var);
  rp->CategoryList.push_back("None");

  /* For ADS2 we decode raw/block/struct data.  ADS3 hands us everything in
   * float format, so decode fn's not required.
   */
  if (indx != ERR)
  {
    rp->Initializer = deriveftns[indx].constructor;
    rp->xlate = deriveftns[indx].xlate;
  }

  assert(length > 0);

  rp->ADSstart		= start >> 1;
  rp->ADSoffset		= 1;
  rp->SampleRate	= rate;
  rp->Length		= length;
  rp->convertOffset	= 0;
  rp->convertFactor	= 1.0;

  if (length > 1)
    rp->Average		= (void (*) (...))AverageVector;
  else
    rp->Average		= (void (*) (...))Average;

  rp->SerialNumber	= serialNumber;
  rp->ProbeType		= probeType;
  rp->ProbeCount	= probeCnt;

  return(rp);

}	/* END ADD_NAME_TO_RAWTBL */

/* -------------------------------------------------------------------- */
static DERTBL *add_name_to_DERTBL(const char name_sans_location[])
{
  int	indx;

  if ((indx = SearchDERIVEFTNS(name_sans_location)) == ERR)
  {
    char	msg[128];

    snprintf(msg, 128, "add_name_to_DERTBL: Throwing away %s, has no compute function.\n", name_sans_location);
    LogMessage(msg);
    return((DERTBL *)ERR);
  }

  char name[512];
  strcpy(name, name_sans_location);

  if (*location)
    strcat(name, location);

  if (SearchTable(raw, name) != ERR)
  {
    char	msg[128];

    snprintf(msg, 128, "Will not add %s to derived table, it already exists in raw table.\n", name);
    LogMessage(msg);
    return((DERTBL *)ERR);
  }

  if (SearchTable(derived, name) != ERR)
  {
    char	msg[128];

    snprintf(msg, 128, "%s already added to derived list, ignoring duplicate, check dependencies.\n", name);
    LogMessage(msg);
    return((DERTBL *)ERR);
  }


  /* Eliminate chemistry vars that are computed via second pass program.
   * (Can be used for any variable).
   */
  if (cfg.ProcessingMode() == Config::RealTime &&
      deriveftns[indx].compute == smissval)
    return((DERTBL *)ERR);


  DERTBL *dp = new DERTBL(name);
  derived.push_back(dp);
  addUnitsAndLongName(dp, 0);
  dp->CategoryList.push_back("None");

  dp->Initializer	= deriveftns[indx].constructor;
  dp->compute		= (void (*) (void *))deriveftns[indx].compute;

  dp->Default_HR_OR	= Config::HighRate;
  dp->Length		= length;
  dp->SerialNumber	= serialNumber;
  dp->ProbeType		= probeType;
  dp->ProbeCount	= probeCnt;

  /* As a kludge, .xlate field used as ProbeCount for FLUX variables.
   */
  if (strncmp(name, "FLX", 3) == 0)
  {
    dp->Output		= false;
    dp->ProbeCount	= (size_t)deriveftns[indx].xlate;
    dp->ProbeType	= PROBE_FLUX;
  }

  return(dp);

}	/* END ADD_NAME_TO_DERTBL */

/* -------------------------------------------------------------------- */
static int locatePMS(const char target[], PMS list[])
{
  int	i;

  for (i = 0; list[i].name; ++i)
    if (strcmp(target, list[i].name) == 0)
      return(i);

  return(ERR);

}	/* END LOCATEPMS */

/* -------------------------------------------------------------------- */
static void ReadProjectName()
{
  FILE *fp;

  /* Extract ProjectName
   */
  if ((fp = OpenProjectFile(PROJ_NAME, "r", RETURN)) != NULL)
  {
    fgets(buffer, 512, fp);
    fclose(fp);

    if (buffer[strlen(buffer)-1] == '\n')
      buffer[strlen(buffer)-1] = '\0';
    cfg.SetProjectName(buffer);
  }
}

/* -------------------------------------------------------------------- */
/* Strip out trailing 0 cal coe's
 */
static int
check_cal_coes(int order, float *coef)
{
  do
  {
    --order;
  }
  while (order >= 0 && coef[order] == 0.0);

  return(order + 1);

}	/* END CHECK_CAL_COES */

/* -------------------------------------------------------------------- */
static std::vector<NR_TYPE> getCalsForADS2(const char vn[])
{
  int32_t order;
  float *f;

  GetOrder(vn, &order);
  GetCalCoeff(vn, &f);

  // Remove trailing zeroes.
  order = check_cal_coes(order, f);

  std::vector<NR_TYPE> cals;

  for (int32_t i = 0; i < order; ++i)
    cals.push_back((NR_TYPE)f[i]);

  return cals;
}

/* -------------------------------------------------------------------- */
static void
addUnitsAndLongName(var_base *vbp, const nidas::core::Variable *nidas_var)
{
  VDBVar *vdb_var = vardb->search_var(vbp->name);
  std::string cat;

  float miss_val = (float)MISSING_VALUE;
  vbp->addToMetadata("_FillValue", miss_val);

  // If this is a nidas raw variable, then get units/long_name from here.
  // null will be passed in if the variable is derived, or from ADS2.
  if (nidas_var)
  {
    vbp->addToMetadata("units", nidas_var->getUnits());
    vbp->addToMetadata("long_name", nidas_var->getLongName());
  }

  /* Check if units and long_name have already been added (from above),
   * if they are empty, try and locate them in the VarDB
   */
  if (vdb_var)
  {
    if (vbp->Units().length() == 0)
      vbp->addToMetadata("units", vdb_var->get_attribute(VDBVar::UNITS));
    if (vbp->LongName().length() == 0)
      vbp->addToMetadata("long_name", vdb_var->get_attribute(VDBVar::LONG_NAME));

    cat = vdb_var->get_attribute(VDBVar::CATEGORY);
    if ( cat.size() > 0 )
      vbp->CategoryList.push_back(cat);
  }

  /* Order of precedence for Category is VarDB, XML, nimbus
   * VarDB is checked 3 lines above, XML is checked here, nimbus will add default
   * if this is empty.  Move this above the "if (vdb_var)" 10 lines above to
   * change the order of precedence.
   */
  cat = getSensorStringParameter(nidas_var, "Category");
  if ( cat.size() > 0 )
    vbp->CategoryList.push_back(cat);
}

/* -------------------------------------------------------------------- */
static void
testUnitsTitles(var_base *var)
{
  extern FILE *LogFile;
  FILE  *ofp = LogFile ? LogFile : stderr;

  bool units = false, title = false;

  if (var->Units().length() == 0)
  {
    fprintf(ofp, "%s has no units", var->name);
    units = true;
  }

  if (var->LongName().length() == 0)
  {
    if (units)
      fprintf(ofp, " or title/long_name");
    else
      fprintf(ofp, "%s has no title/long_name", var->name);
    title = true;
  }

  if (units || title)
    fprintf(ofp, ".\n");
}

static void
checkUnitsTitles()
{
  for (size_t i = 0; i < raw.size(); ++i)
    testUnitsTitles(raw[i]);

  for (size_t i = 0; i < derived.size(); ++i)
    testUnitsTitles(derived[i]);
}

/* -------------------------------------------------------------------- */
static void
openVariableDatabase()
{
  MakeProjectFileName(buffer, VARDB);
  std::string vardbfile(buffer);

  /*
   * Use the vardb library to handle reading either a VarDB binary file or
   * an XML file.  For now we expect the VarDB file only, but update the
   * vardb.xml file same as update_depend would have done.
   */
  VarDBConverter vdbc;
  vardb = vdbc.open(new VDBFile(), vardbfile);

  if (! vardb->is_valid())
  {
    LogMessage("InitializeVarDB for project specific failed, "
	       "trying master file.\n");
    snprintf(buffer, 8192, VARDB.c_str(), cfg.ProjectDirectory().c_str(),
	    "Configuration/", "");
    vardb->open(buffer);
    if (! vardb->is_valid())
    {
      std::ostringstream oss;
      oss << "InitializeVarDB for master file ("
	  << buffer << ") failed, this is fatal.";
      HandleFatalError(oss.str().c_str());
    }
  }
  else if (vdbc.defaultOutputPath() != vardbfile)
  {
    // Successfully opened a binary file, so update the xml file.
    ILOG(("Updating ")
	 << vdbc.defaultOutputPath() << " from " << vardbfile);
    vardb->save(vdbc.defaultOutputPath());
  }
}

/* -------------------------------------------------------------------- */
var_base::var_base(const char s[])
{
  strcpy(name, s);
  varid = -1;
  LRstart = SRstart = HRstart = -1;

  SampleRate = 0;
  Length = 1;
  ProbeCount = 0;
  ProbeType = 0;

  OutputRate = Config::LowRate;

  Dirty = false;
  Output = true;
  DependedUpon = false;
  Transmit = false;

  Modulo = 0;

  DataQuality	= defaultQuality;

  min = FLT_MAX;
  max = -FLT_MAX;
}

var_base::~var_base()
{
#ifdef DEBUG
  if (Modulo)
  {
    std::cerr << "deleting modulo for variable " << name << std::endl;
  }
#endif
  delete Modulo;
}

RAWTBL::RAWTBL(const char s[]) : var_base(s)
{
  type[0] = '\0';
  Initializer = 0;
  xlate = 0;
  Average = 0;

  convertOffset = 0;
  convertFactor = 1.0;

  TTindx = -1;
  badLagCntr = 0;

  nidasLag = 0;
  StaticLag = 0;
  DynamicLag = 0;
  SpikeSlope = 0.0;
}

DERTBL::DERTBL(const char s[]) : var_base(s)
{
  Initializer = 0;
  compute = 0;
  nDependencies = 0;
}

/* END HDR_DECODE.C */
