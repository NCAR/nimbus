/*
-------------------------------------------------------------------------
OBJECT NAME:	hdr_decode.c

FULL NAME:	Decode Header

ENTRY POINTS:	DecodeHeader()

STATIC FNS:	initHDR(), initSDI(), initHoneywell(), initOphir3(),
		initPMS1D(), initPMS1Dv2(), locatePMS(), initLitton51(),
		add_file_to_???TBL(), add_name_to_???TBL(), add_derived_names(),
		check_cal_coes(), initPMS2D(), initMASP()

DESCRIPTION:	Read header & add variables to appropriate table.  There
		are 3 major tables here:
			- SDI table
			- Raw table (From block probes)
			- Derived table

		This module is the brunt end interface between an ADS tape
		and the rest of nimbus.

INPUT:		Header filename.

OUTPUT:		sdi, raw, derived
		(These globals are initialized in this file)

REFERENCES:	Header API (libhdr_api.a)
		OpenProjectFile(), AccessProjectFile()
		ReadTextFile(), FreeTextFile()
		SearchList(), SearchDERIVEFTNS()
		ReadStaticLags(), SetUpDependencies(), ReadDefaultsFile()

REFERENCED BY:	winput.c, nimbus.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-05
-------------------------------------------------------------------------
*/

#include <ctype.h>
#include <unistd.h>

#include "raf.h"
#include "pms.h"
#include "nimbus.h"
#include "decode.h"
#include "ctape.h"	/* ADS header API		*/
#include "amlib.h"

#ifdef RT
#include <SyncRecordReader.h>
extern dsm::SyncRecordReader* syncRecReader;
#endif

typedef struct
  {
  char		*name;
  int		type;	/* #defines used for switch's, in WINDS		*/
  size_t	VecLen;	/* VectorLength					*/
  unsigned long	pType;	/* probeType for 'ToggleProbe' menu, redundant	*/
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
  { MASP_STR,	MASP,	32,	PROBE_MASP,	0 },
  { NULL,	0,	0,	0,		0 }
  };

static PMS	pms1v3_probes[] =
  {
  { SPP100_STR,	FSSP,	0,	PROBE_FSSP,	0 },
  { SPP200_STR,	ASAS,	0,	PROBE_PCASP,	0 },
  { SPP300_STR,	F300,	0,	PROBE_F300,	0 },
  { NULL,	0,	0,	0,		0 }
  };

static PMS	pms2d_probes[] =
  {
  { P2D_C_STR,	0,	0,	PROBE_2DC,	0 },
  { P2D_P_STR,	0,	0,	PROBE_2DP,	0 },
  { P2D_G_STR,	0,	0,	PROBE_PMS2D,	0 },
  { P2D_H_STR,	0,	0,	PROBE_HVPS,	0 },
  { NULL,	0,	0,	0,		0 }
  };


static size_t	InertialSystemCount, GPScount, twoDcnt, NephCnt;
static size_t	probeCnt;
static unsigned long	probeType;
static long	start, rate, length;
static char	*item_type, location[NAMELEN];
static char	*derivedlist[MAX_DEFAULTS*4],	/* DeriveNames file	*/
		*rawlist[MAX_DEFAULTS*4];	/* RawNames file	*/


#ifdef RT
static SDITBL	*initSDI_ADS3(dsm::SyncRecordVariable* var);
#endif
static RAWTBL	*add_name_to_RAWTBL(const char []);
static DERTBL	*add_name_to_DERTBL(const char []);

static void add_file_to_RAWTBL(const char []), add_file_to_DERTBL(const char []),
	initHDR(char vn[]), initSDI(char vn[]), initHoneywell(char vn[]),
	initOphir3(char vn[]), initPMS1D(char vn[]), initPMS1Dv2(char vn[]),
	initGustCorrected(char vn[]), initLitton51(char vn[]),
	add_derived_names(const char vn[]), initPMS2D(char vn[], int n),
	initPMS2Dhouse(char vn[]), add_raw_names(const char vn[]),
	initGreyHouse(char vn[]), initMASP(char vn[]), initPMS1Dv3(char vn[]),
	ReadProjectName(), initRDMA(char vn[]), initCLIMET(char vn[]);

static std::vector<float> getCalsForADS2(const char vn[]);
static int	check_cal_coes(int order, float *coef);
static int	locatePMS(const char target[], PMS list[]);


static char	*defaultQuality;

extern long	INS_start;
extern int	Aircraft;

char	*ExtractHeaderIntoFile(const char fn[]);
bool	Open2dFile(const char file[], int probeCnt);
void	AddProbeToList(const char name[], int type), ReadDespikeFile();
void	Add2DtoList(RAWTBL *varp);


/* -------------------------------------------------------------------- */
bool VarCompareLT(const var_base *x, const var_base *y)
{
    return(strcmp(x->name, y->name) < 0);
}

/* -------------------------------------------------------------------- */
static void CommonInitialization()
{

  ReadProjectName();

  if (cfg.ProductionRun())
    defaultQuality = "Good";
  else
    defaultQuality = "Preliminary";


  ReadTextFile(RAWNAMES, rawlist);

  if (Aircraft != NOAA_G4)
    ReadTextFile(DERIVEDNAMES, derivedlist);
  else
    derivedlist[0] = 0;

  ReadDefaultsFile();

  sprintf(buffer, PMS_SPEC_FILE, ProjectDirectory, ProjectNumber);
  InitPMSspecs(buffer);

}

/* -------------------------------------------------------------------- */
#ifdef RT
int DecodeHeader3(const char header_file[])
{
printf("DecodeHeader3\n");

  std::string proj_name = syncRecReader->getProjectName();
  ProjectNumber = new char(proj_name.size()+1);
  strcpy(ProjectNumber, proj_name.c_str());
ProjectNumber = "601";
printf("ProjectNumber = %s\n", ProjectNumber);

  std::string tail_number = syncRecReader->getTailNumber();
  char const *p = tail_number.c_str();

  while (*p && !isdigit(*p))
    ++p;
  Aircraft = atoi(p);


  // Perform common (ADS2 & ADS3).
  CommonInitialization();

  // Add Time variables, hour, min, sec, year, mon, day.
  initHDR(0);

  const std::list<const dsm::SyncRecordVariable*>& vars = syncRecReader->getVariables();

  std::list<const dsm::SyncRecordVariable*>::const_iterator vi;
  for (vi = vars.begin(); vi != vars.end(); ++vi)
  {
    dsm::SyncRecordVariable *var = const_cast<dsm::SyncRecordVariable*>(*vi);

    rate = (int)ceil(var->getSampleRate());
    length = var->getLength();

printf("DecodeHeader3: adding %s, converter = %d, rate = %d\n",
  var->getName().c_str(), (int)var->getConverter(), (int)ceil(var->getSampleRate()));

    if (var->getConverter() == 0)
      {
      RAWTBL *rp = add_name_to_RAWTBL(var->getName().c_str());
      // Default real-time netCDF to SAmpleRate.
      if (cfg.ProcessingMode() == Config::RealTime)
        rp->OutputRate = rp->SampleRate;
      }
    else
      {
      SDITBL *sp = initSDI_ADS3(var);
      // Default real-time netCDF to SAmpleRate.
      if (cfg.ProcessingMode() == Config::RealTime)
        sp->OutputRate = sp->SampleRate;
      }
  }

  return OK;
}

/* -------------------------------------------------------------------- */
static SDITBL* initSDI_ADS3(dsm::SyncRecordVariable* var)
{
  SDITBL *cp = new SDITBL(var->getName().c_str());
  sdi.push_back(cp);

  // In ADS3, we don't get a/d counts.  They give us voltages.  These are
  // not necessary.
  cp->convertOffset = 0;
  cp->convertFactor = 1.0;

  cp->SampleRate   = rate;
  cp->DependedUpon = false;
  cp->Modulo       = 0;

  switch (var->getType())
  {
    case  dsm::Variable::CONTINUOUS:
      strcpy(cp->type, "A");
      break;
    case  dsm::Variable::COUNTER:
      strcpy(cp->type, "C");
      break;
    default:
      LogMessage("hdr_decode:initSDI_ADS3: Unsupported type from Variable->getType()\n");
  }

  cp->Average = (void (*) (NR_TYPE *, NR_TYPE *, void *))(cp->type[0] == 'C' ? SumSDI : AverageSDI);

  dsm::VariableConverter* converter =
		const_cast<dsm::VariableConverter*>(var->getConverter());

  dsm::Polynomial* poly;
  dsm::Linear* linear = dynamic_cast<dsm::Linear*>(converter);
  if (linear)
  {
     cp->cof.push_back(linear->getIntercept());
     cp->cof.push_back(linear->getSlope());
  }
  else
  if ((poly = dynamic_cast<dsm::Polynomial*>(converter)))
  {
    const std::vector<float>& coefs = poly->getCoefficients();
    cp->cof = coefs;
  } 

  return(cp);

}
#endif

/* -------------------------------------------------------------------- */
int DecodeHeader(const char header_file[])
{
  char	*vn;
  char	*loc, *p;

  InertialSystemCount = GPScount = twoDcnt = NephCnt = 0;

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
    sprintf(buffer, "Header decode failed, taperr = %d.", taperr);
    HandleError(buffer);
    unlink(p);
    free(p);
    return(ERR);
    }

  unlink(p);

  GetVersion(&p);
  HDRversion = atof(p);

  GetAircraft(&p);
  while (*p && !isdigit(*p))
    ++p;
  Aircraft = atoi(p);

  if (Aircraft == 3)
    Aircraft = NRL_P3;

  GetFlightNumber(&p);
  strcpy(FlightNumber, p);
  while (*p && !isdigit(*p))
    ++p;
  FlightNumberInt = atoi(p);

  GetProjectNumber(&ProjectNumber);

  sprintf(buffer, "%s/%s", ProjectDirectory, ProjectNumber);
  if (access(buffer, R_OK) == ERR)
    {
    sprintf(buffer, "No project directory for %s.", ProjectNumber);
    HandleError(buffer);
    return(ERR);
    }

  /* Old tapes don't set Aircraft field, so fudge it with proj_num.
  */
  if (Aircraft == 0)
    {
    switch (ProjectNumber[0])
      {
      case '2':
        Aircraft = KINGAIR;
        break;
      case '3':
        Aircraft = NRL_P3;
        break;
      case '6':
        Aircraft = 600;
        break;
      case '7':
        Aircraft = SABRELINER;
        break;
      case '8':
        Aircraft = ELECTRA;
        break;
      }
    }

  // Perform common (ADS2 & ADS3).
  CommonInitialization();

  /* This is the main loop, loop through all variables in header
   */
  for (vn = GetFirst(); vn; vn = GetNext())
    {
    GetItemType(vn, &item_type);
    GetStart(vn, &start);

    if (GetRate(vn, &rate) == ERR)
      rate = 1;

    length = 1;

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
        }

      initGustCorrected(vn);
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
      add_file_to_RAWTBL(GPSHWNAMES);

      ++GPScount;
      }
    else
    if (!strcmp(item_type, NEPH903_STR))
      {
      sprintf(location, "_%d", ++NephCnt);
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
*/    }
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
          sprintf(location, "_%02d", i);
          add_raw_names(item_type);
          }
        }
      location[0] = '\0';
      }
    else
      {
      sprintf(buffer, "Unknown variable/probe %s encountered, ignoring & continuing.\n", item_type);
      LogMessage(buffer);
      }
    }


  probeType = 0;
  location[0] = '\0';


  /* Add the default derived variables.
   */
  rate = 1;

  if (Aircraft != SAILPLANE)
    {
    add_derived_names("ALWAYS");
/*
probeCnt = 0;
    add_name_to_DERTBL("XISCAI");
probeCnt = 1;
    add_name_to_DERTBL("XISTAS");
probeCnt = 2;
    add_name_to_DERTBL("XISNAS");
probeCnt = 0;
*/
    AddProbeToList("Fluxes", (unsigned long)PROBE_FLUX);
    }
  else
    add_derived_names("SAIL");

  if (cfg.ProcessingMode() == Config::RealTime)
    {
    for (probeCnt = 0; probeCnt < 3; ++probeCnt)
      {
      sprintf(location, "_%d", probeCnt);
      add_derived_names("PRCLTRK");
      }

    location[0] = '\0';
    }


  ReleasePMSspecs();

  if (Aircraft != NOAA_G4)
    FreeTextFile(derivedlist);

  FreeTextFile(rawlist);

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
    AddProbeToList("All PMS1D's", (unsigned long)PROBE_PMS1D);

  cnt = 0;

  for (size_t i = 0; pms2d_probes[i].name; ++i)
    cnt += pms2d_probes[i].cnt;

  if (cnt > 1)
    AddProbeToList("All PMS2D's", (unsigned long)PROBE_PMS2D);
  }

  AddProbeToList("All On", (unsigned long)ALL_ON);
  AddProbeToList("All Off", (unsigned long)ALL_OFF);

  if (AccessProjectFile(USERNAMES, "r") == true)
    add_file_to_DERTBL(USERNAMES);

/*
for (size_t i = 0; i < sdi.size(); ++i)
  printf("%-12s%5d, %d\n", sdi[i]->name, sdi[i]->SampleRate, sdi[i]->ADSstart);

for (size_t i = 0; i < raw.size(); ++i)
  printf("%-12s%5d\n", raw[i]->name, raw[i]->xlate);

for (size_t i = 0; i < derived.size(); ++i)
  printf("%-12s\n", derived[i]->name, derived[i]->compute);
*/

  std::sort(sdi.begin(), sdi.end(), VarCompareLT);
  std::sort(raw.begin(), raw.end(), VarCompareLT);
  std::sort(derived.begin(), derived.end(), VarCompareLT);

  ReadModuloVariables();
  ReadSumVariables();

  if (cfg.TimeShifting())
    ReadStaticLags();

  if (cfg.Despiking())
    ReadDespikeFile();

  ReadDefaultDerivedOutputRate();
  SetUpDependencies();

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

}	/* END INITHDR */

/* -------------------------------------------------------------------- */
static void initSDI(char vn[])
{
  char		*type;
  int		indx;

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

  /* By placing an SDI variable into the 'deriveftns[]' list
   * (ninc/amlib.fns) the variable will automatically be placed
   * into the 'raw' list instead of 'sdi' (i.e. if you have an SDI
   * variable that requires special processing).
   */
  if ((indx = SearchDERIVEFTNS(vn)) != ERR && deriveftns[indx].xlate)
    {
    RAWTBL	*rp;
    long	offset;

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
        atoi(ProjectNumber) == 818)
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


  /* Ok, it's strictly nth order polynomial, put it in the SDI table.
   */
  SDITBL *cp = new SDITBL(vn);
  sdi.push_back(cp);

  if (GetConversionOffset(vn, &(cp->convertOffset)) == ERR)
    cp->convertOffset = 0;

  GetConversionFactor(vn, &(cp->convertFactor));
  GetSampleOffset(vn, &(cp->ADSoffset));
  GetType(vn, &type);

  strcpy(cp->type, type);
  cp->SampleRate	= rate;
  cp->ADSstart		= start >> 1;
  cp->ADSoffset		>>= 1;
  cp->Average		= (void (*) (NR_TYPE *, NR_TYPE *, void *))(cp->type[0] == 'C' ? SumSDI : AverageSDI);
  cp->Modulo		= 0;

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
    sscanf(names[i], "%s %ld", name, &rate);

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
    }
  else
    SetLookupSuffix((char *)0);

}	/* END INITHONEYWELL */

/* -------------------------------------------------------------------- */
static void initGustCorrected(char vn[])
{
  /* We can only have 1 corrected winds, bail if already initialized.
   */
  if (GPScount > 0)
    return;

  if (Aircraft != SAILPLANE)
    {
    probeType = PROBE_GUSTC;
    AddProbeToList("Corrected Winds", (unsigned long)PROBE_GUSTC);

    /* ProbeCnt here relies on the fact that xbuild puts inertials before
     * GPSs.
     */
    probeCnt = InertialSystemCount;
    add_derived_names("POSNC");
    add_derived_names("GUSTC");
    }

}	/* END INITTANS */

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
    sscanf(names[i], "%s %ld", name, &rate);
    add_name_to_RAWTBL(name);
    }

  FreeTextFile(names);

  if (InertialSystemCount++ == 0)
    {
    add_derived_names(item_type);
    SetLookupSuffix((char *)0);
    add_derived_names("GUST");
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
      rp->cof.push_back((float)atof(strtok((char *)NULL, " \t")));
    }

  FreeTextFile(list);
  add_derived_names(item_type);

}	/* END IN_OPHIR3 */

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

    sprintf(msg, "Unknown MASP probe: %s, continuing\n", vn);
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

  if ((rp = add_name_to_RAWTBL(temp)) == (RAWTBL *)ERR)
    return;

  rp->Length	= nbins = BINS_32;
  rp->Average	= (void (*) (...))SumVector;

  strcpy(rp->SerialNumber, vn);
  strcat(rp->SerialNumber, "_DEF");


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

  add_raw_names(vn);
  add_derived_names(vn);

  strcpy(buffer, "ACLMT"); strcat(buffer, location);
  if ((indx = SearchTable(raw, buffer)) == ERR)
    {
    fprintf(stderr, "ACLMT not found, fatal, update $PROJ_DIR/defaults/RawNames\n");
    exit(1);
    }

  raw[indx]->Length = 7;
  strcpy(raw[indx]->SerialNumber, "CLIMET1");


  strcpy(buffer, "CCLMT"); strcat(buffer, location);
  if ((indx = SearchTable(derived, buffer)) == ERR)
    {
    fprintf(stderr, "CCLMT not found, fatal, update $PROJ_DIR/defaults/DerivedNames\n");
    exit(1);
    }

  derived[indx]->Length = 7;

  AddProbeToList(vn, probeType);
  AddToPMS1DprobeList("CLMT", location, "CLIMET1", PROBE_CLMT);

}

/* -------------------------------------------------------------------- */
static void initRDMA(char vn[])
{
  int		indx;

  add_raw_names(vn);
  add_derived_names(vn);

  strcpy(buffer, "ARDMA"); strcat(buffer, location);
  if ((indx = SearchTable(raw, buffer)) == ERR)
    {
    fprintf(stderr, "ARDMA not found, fatal, update $PROJ_DIR/defaults/RawNames\n");
    exit(1);
    }

  raw[indx]->Length = 64;
  strcpy(raw[indx]->SerialNumber, "RDMA1");


  strcpy(buffer, "CRDMA"); strcat(buffer, location);
  if ((indx = SearchTable(derived, buffer)) == ERR)
    {
    fprintf(stderr, "CRDMA not found, fatal, update $PROJ_DIR/defaults/DerivedNames\n");
    exit(1);
    }

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

    sprintf(msg, "Unknown pms1d probe: %s, continuing\n", probe);
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

      strcpy(rp->SerialNumber, probe);
      strcat(rp->SerialNumber, "_DEF");
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
  long		ps_start, ps_length;

  GetName(vn, &probe);

  if ((indx = locatePMS(probe, pms1v2_probes)) == ERR)
    {
    char	msg[128];

    sprintf(msg, "Unknown pms1d probe: %s, continuing\n", probe);
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

  GetSerialNumber(vn, &p);
  AddToPMS1DprobeList(probe, location, p, pms1v2_probes[indx].type);


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

      if (GetSerialNumber(vn, &p) != ERR)
        strcpy(rp->SerialNumber, p);
      else
        {
        strcpy(rp->SerialNumber, probe);
        strcat(rp->SerialNumber, "_DEF");
        }

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
  long		nbins;
  RAWTBL	*rp;
  DERTBL	*dp;
  char		*p, *probe, temp[NAMELEN], *serialNum;


  GetName(vn, &probe);

  if ((indx = locatePMS(probe, pms1v3_probes)) == ERR)
    {
    char	msg[128];

    sprintf(msg, "Unknown pms1d probe: %s, continuing\n", probe);
    LogMessage(msg);
    return;
    }

  probeCnt	= pms1_probes[indx].cnt++ + pms1v2_probes[indx].cnt +
		pms1v3_probes[indx].cnt;
  probeType	|= pms1v3_probes[indx].pType;

  strcpy(buffer, probe);
  strcat(buffer, location);
  GetSerialNumber(vn, &p);
  AddProbeToList(buffer, pms1_probes[indx].pType | probeCnt);

  /* For real-time WINDS.  Calls pms1d.c for nimbus.
   */
  GetSerialNumber(vn, &serialNum);
  AddToPMS1DprobeList(probe, location, serialNum, pms1_probes[indx].type);


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

  strcpy(rp->SerialNumber, serialNum);
  SetLookupSuffix("_V3");

  /* Housekeeping names and cals from PMSspecs */
  for (int i = 0; i < 8; ++i)
    {
    int j;

    sprintf(buffer, "HSKP%d", i);
    p = GetPMSparameter(serialNum, buffer);
    
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
      strcpy(dp->SerialNumber, serialNum);

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

    sprintf(msg, "Unknown pms2d probe: %s, continuing\n", vn);
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

    rp = add_name_to_RAWTBL(buffer);
    rp->Average	= (void (*) (...))SumVector;
    rp->Length	= nBins;

    if (GetSerialNumber(vn, &p) != ERR)
      strcpy(rp->SerialNumber, p);
    else
      {
      strcpy(rp->SerialNumber, buffer);
      strcat(rp->SerialNumber, "_DEF");
      }

    if (order == 0)
      Add2DtoList(rp);

    sprintf(name, "DT%s", &buffer[1]);
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
{
          dp->Length = nBins;
//printf("initPMS2D: dp = %s %d\n", buffer, probeCnt);
}
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
static void add_file_to_DERTBL(const char filename[])
{
  FILE	*fp;

  fp = OpenProjectFile(filename, "r", EXIT);

  while (fscanf(fp, "%s", buffer) != EOF)
    if (buffer[0] != COMMENT)
      add_name_to_DERTBL(buffer);

  fclose(fp);

}	/* END ADD_FILE_TO_DERTBL */

/* -------------------------------------------------------------------- */
static void add_file_to_RAWTBL(const char filename[])
{
  FILE	*fp;

  fp = OpenProjectFile(filename, "r", EXIT);

  while (fscanf(fp, "%s", buffer) != EOF)
    if (buffer[0] != COMMENT)
      add_name_to_RAWTBL(buffer);

  fclose(fp);

}	/* END ADD_FILE_TO_RAWTBL */

/* -------------------------------------------------------------------- */
static RAWTBL *add_name_to_RAWTBL(const char name[])
{
  int indx = SearchDERIVEFTNS(name);

  if (indx == ERR && (cfg.isADS2() || syncRecReader->getVariable(name) == 0))
    {
    char msg[128];

    sprintf(msg, "Throwing away %s, has no decode function.\n", name);
    LogMessage(msg);
    return((RAWTBL *)ERR);
    }

  RAWTBL *rp = new RAWTBL(name);
  raw.push_back(rp);

  /* For ADS2 we decode raw/block/struct data.  ADS3 hands us everything in
   * float format, so decode fn's not required.
   */
  if (indx != ERR)
    {
    rp->Initializer = deriveftns[indx].constructor;
    rp->xlate = (void (*) (void *, void *, float *))deriveftns[indx].xlate;
    }
  else
    {
    rp->Initializer = 0;
    rp->xlate = 0;
    }

  if (*location)
    strcat(rp->name, location);

  strcpy(rp->SerialNumber, "");


  rp->ADSstart		= start >> 1;
  rp->ADSoffset		= 1;
  rp->SampleRate	= rate;
  rp->Length		= length;
  rp->convertOffset	= 0;
  rp->convertFactor	= 1.0;

  rp->Average		= (void (*) (...))Average;
  rp->Modulo		= 0;
  rp->ProbeType		= probeType;
  rp->ProbeCount	= probeCnt;

  return(rp);

}	/* END ADD_NAME_TO_RAWTBL */

/* -------------------------------------------------------------------- */
static DERTBL *add_name_to_DERTBL(const char name[])
{
  int		indx;

  if ((indx = SearchDERIVEFTNS(name)) == ERR)
  {
    char	msg[128];

    sprintf(msg, "Throwing away %s, has no compute function.\n", name);
    LogMessage(msg);
    return((DERTBL *)ERR);
  }

  if (SearchTable(derived, name) != ERR)
  {
    char	msg[128];

    sprintf(msg, "%s already added to derived list, ignoring duplicate, check dependencies.\n", name);
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

  if (*location)
    strcat(dp->name, location);

  dp->Initializer	= deriveftns[indx].constructor;
  dp->compute		= (void (*) (void *))deriveftns[indx].compute;

  dp->Default_HR_OR	= Config::HighRate;
  dp->Length		= length;
  dp->ProbeType		= probeType;
  dp->ProbeCount	= probeCnt;

  dp->Modulo		= 0;

  /* As a kludge, .xlate field used as ProbeCount for FLUX variables.
   */
  if (strncmp(name, "FLX", 3) == 0)
    {
    dp->Output		= false;
    dp->ProbeCount	= (int)deriveftns[indx].xlate;
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
  if ((fp = OpenProjectFile("%s/%s/ProjectName", "r", RETURN)) != NULL)
    {
    fgets(buffer, 512, fp);

    if (buffer[strlen(buffer)-1] == '\n')
      buffer[strlen(buffer)-1] = '\0'; 
    ProjectName = new char[strlen(buffer)+1];
    strcpy(ProjectName, buffer);
    fclose(fp);
    }
  else
    ProjectName = "";
}

/* -------------------------------------------------------------------- */
static std::vector<float> getCalsForADS2(const char vn[])
{
  long order;
  float *f;

  GetOrder(vn, &order);
  GetCalCoeff(vn, &f);

  // Remove trailing zeroes.
  order = check_cal_coes(order, f);

  std::vector<float> cals;

  for (long i = 0; i < order; ++i)
    cals.push_back(f[i]);

  return cals;

}

/* -------------------------------------------------------------------- */
/* Strip out trailing 0 cal coe's
 */
static int check_cal_coes(int order, float *coef)
{
  do
    {
    --order;
    }
  while (order >= 0 && coef[order] == 0.0);

  return(order + 1);

}	/* END CHECK_CAL_COES */

/* -------------------------------------------------------------------- */
var_base::var_base(const char s[])
{
  strcpy(name, s);
  varid = 0;
  LRstart = SRstart = HRstart = 0;

  SampleRate = 0;
  Length = 1;

  OutputRate = Config::LowRate;

  Dirty = false;
  Output = true;
  DependedUpon = false;
  Broadcast = cfg.ProcessingMode() == Config::RealTime ? true : false;
  Transmit = cfg.TransmitToGround();

  DataQuality	= defaultQuality;

}

SDITBL::SDITBL(const char s[]) : var_base(s)
{
  StaticLag = 0;
  SpikeSlope = 0.0;

}

RAWTBL::RAWTBL(const char s[]) : var_base(s)
{
  StaticLag = 0;
  DynamicLag = 0;
  SpikeSlope = 0.0;
  ProbeCount = 0;
}

DERTBL::DERTBL(const char s[]) : var_base(s)
{
  ndep = 0;
  ProbeCount = 0;
}

/* END HDR_DECODE.C */
