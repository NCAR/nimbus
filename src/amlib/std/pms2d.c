/*
-------------------------------------------------------------------------
OBJECT NAME:	pms2d.c

FULL NAME:	Derived computations for the PMS 2DC & 2DP probes.

ENTRY POINTS:	sTwodInit()
		sTwoD()
		sTwoDInitH()	HVPS
		sHVPS()		HVPS
		addDOFtoAttrs()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 2000-2022
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <raf/pms.h>

#include <cassert>

static const size_t MAX_PMS2D = 8;

// % of a diode which must be shadowed for the diode to trigger.
static const NR_TYPE shadowLevel = 0.55;

static const size_t maxBins = 130;


static size_t	FIRST_BIN[MAX_PMS2D], LAST_BIN[MAX_PMS2D], conc50idx[MAX_PMS2D],
		conc100idx[MAX_PMS2D], conc150idx[MAX_PMS2D];
static NR_TYPE  responseTime[MAX_PMS2D], armDistance[MAX_PMS2D], dof_const[MAX_PMS2D],
		DENS[MAX_PMS2D], SampleRate[MAX_PMS2D], basicSampleArea[MAX_PMS2D];
static double   PLWFAC[MAX_PMS2D], DBZFAC[MAX_PMS2D];

static NR_TYPE  total_concen[MAX_PMS2D], dbar[MAX_PMS2D], plwc[MAX_PMS2D],
		disp[MAX_PMS2D], dbz[MAX_PMS2D], tact[MAX_PMS2D];

static NR_TYPE  radius[MAX_PMS2D][maxBins], cell_size[MAX_PMS2D][maxBins],
		cell_size2[MAX_PMS2D][maxBins], cell_size3[MAX_PMS2D][maxBins],
		sampleArea[MAX_PMS2D][maxBins], reff2[MAX_PMS2D], reff3[MAX_PMS2D];

NR_TYPE         reff23[MAX_PMS2D], reff22[MAX_PMS2D];  /* For export to reff.c */

bool	thisIs2Dnot1D(const char * name);
void    ComputePMS1DParams(NR_TYPE radius[], NR_TYPE eaw[], NR_TYPE cell_size[],
	NR_TYPE dof[], float minRange, float resolution, size_t nDiodes, size_t
	length, float dof_const, size_t armDistance);

// Probe Count.
static size_t nProbes = 0;
extern void setProbeCount(const char * location, int count);


/* -------------------------------------------------------------------- */
void addDOFtoAttrs(const var_base *varp, NR_TYPE eaw[], NR_TYPE dof[])
{
  std::vector<float> dof_v, eaw_v;
  char name[64];
  strcpy(name, varp->name);
  name[0] = 'C';

  for (size_t i = 0; i < varp->Length; ++i)
    eaw_v.push_back(eaw[i]);
  AddToDefaults(name, "EffectiveAreaWidth", eaw_v);

  for (size_t i = 0; i < varp->Length; ++i)
    dof_v.push_back(dof[i]);
  AddToDefaults(name, "DepthOfField", dof_v);
}

/* -------------------------------------------------------------------- */
void sTwodInit(var_base *varp)
{
  size_t	i, length, probeNum, nDiodes, minRange;
  char		*p;
  float		resolution;
  const char	*serialNumber;
  NR_TYPE	eaw[maxBins], dof[maxBins];

  if (varp->SerialNumber.length() == 0) {
    sprintf(buffer, "pms2d.c: %s has no serial number, fatal.", varp->name);
    HandleFatalError(buffer);
    }

  if (nProbes == MAX_PMS2D) {
    HandleFatalError("pms2d.c: Exceeded maximum number of PMS2D probes, change amlib.h.");
    }

  for (i = 0; i < MAX_PMS2D; ++i)
    SampleRate[i] = 1.0;

  setProbeCount(&varp->name[1], nProbes++);

  serialNumber = varp->SerialNumber.c_str();
  probeNum = varp->ProbeCount;

  for (i = 0; i < MAX_PMS2D; ++i)
    reff23[i] = reff22[i] = 0.0;

  MakeProjectFileName(buffer, PMS_SPEC_FILE);
  InitPMSspecs(buffer);

  if ((p = GetPMSparameter(serialNumber, "FIRST_BIN")) == NULL) {
    sprintf(buffer, "pms2d: serial number = [%s]: FIRST_BIN not found.", serialNumber);
    HandleFatalError(buffer);
    }
  FIRST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "LAST_BIN")) == NULL) {
    sprintf(buffer, "pms2d: serial number = [%s]: LAST_BIN not found.", serialNumber);
    HandleFatalError(buffer);
    }
  LAST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "MIN_RANGE")) == NULL) {
    sprintf(buffer, "pms2d: serial number = [%s]: MIN_RANGE not found.", serialNumber);
    HandleFatalError(buffer);
    }
  minRange = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "RANGE_STEP")) == NULL) {
    sprintf(buffer, "pms2d: serial number = [%s]: RANGE_STEP not found.", serialNumber);
    HandleFatalError(buffer);
    }
  resolution = atof(p);

  if ((p = GetPMSparameter(serialNumber, "NDIODES")) == NULL) {
    sprintf(buffer, "pms2d: serial number = [%s]: NDIODES not found.", serialNumber);
    HandleFatalError(buffer);
    }
  nDiodes = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "RESPONSE_TIME")) == NULL) {
    sprintf(buffer, "pms2d: serial number = [%s]: RESPONSE_TIME not found.", serialNumber);
    HandleFatalError(buffer);
    }
  responseTime[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "DOF_CONST")) == NULL)
    dof_const[probeNum] = 2.37; // default for PMS2 probes.
  else
    dof_const[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "ARM_DISTANCE")) == NULL) {
    sprintf(buffer, "pms2d: serial number = [%s]: ARM_DISTANCE not found.", serialNumber);
    HandleFatalError(buffer);
    }
  armDistance[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "DENS")) == NULL) {
    sprintf(buffer, "pms2d: serial number = [%s]: DENS not found.", serialNumber);
    HandleFatalError(buffer);
    }
  DENS[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "PLWFAC")) == NULL) {
    sprintf(buffer, "pms2d: serial number = [%s]: PLWFAC not found.", serialNumber);
    HandleFatalError(buffer);
    }
  PLWFAC[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "DBZFAC")) == NULL) {
    sprintf(buffer, "pms2d: serial number = [%s]: DBZFAC not found.", serialNumber);
    HandleFatalError(buffer);
    }
  DBZFAC[probeNum] = atof(p);

  ReleasePMSspecs();

  basicSampleArea[probeNum] = (armDistance[probeNum]/1000) * (resolution / 1000000) * nDiodes;

  /* 1DC/P has length 32, 2DC/P has length 64.
   */
  length = varp->Length;

  if (thisIs2Dnot1D(varp->name))	// Center-in & reconstruction has twice as many bins.
    LAST_BIN[probeNum] *= 2;

  ComputePMS1DParams(radius[probeNum], eaw, cell_size[probeNum], dof,
	minRange, resolution, nDiodes, length, dof_const[probeNum], armDistance[probeNum]);

  /* Precompute dia squared and cubed. */
  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    {
    cell_size2[probeNum][i] = cell_size[probeNum][i] * cell_size[probeNum][i];
    cell_size3[probeNum][i] = cell_size2[probeNum][i] * cell_size[probeNum][i];
    }

  // For center-in algorithm use this eaw, not one from ComputePMS1DParams()
  if (thisIs2Dnot1D(varp->name))  /* 2DC/P only (not 1DC/P). */
    {
    for (i = 0; i < length; ++i)
      eaw[i] = (resolution / 1000) * nDiodes;
    }

  addDOFtoAttrs(varp, eaw, dof);
  for (i = 0; i < length; ++i)
    sampleArea[probeNum][i] = eaw[i] * dof[i];

  conc50idx[probeNum] = (50 / (int)resolution) - 1 + cfg.ZeroBinOffset();
  conc100idx[probeNum] = (100 / (int)resolution) - 1 + cfg.ZeroBinOffset();
  conc150idx[probeNum] = (150 / (int)resolution) - 1 + cfg.ZeroBinOffset();

}	/* END STWODINIT */

/* -------------------------------------------------------------------- */
void sTwoD(DERTBL *varp)
{
  size_t	i, probeNum;
  NR_TYPE	*actual, *concentration, *dia, *dia2, *dia3;
  NR_TYPE	tas;		/* True Air Speed	*/
  NR_TYPE	sampleVolume[maxBins];
  NR_TYPE	deadTime = 0.0;

  assert(varp->Length > 1);

  actual	= GetVector(varp, 0);
  tas		= GetSampleFor1D(varp, 1);
  if (varp->nDependencies > 2)
  {
    deadTime = GetSample(varp, 2);
    if (deadTime > 1000.0)
      deadTime = 0.0;
  }

  probeNum	= varp->ProbeCount;
  dia		= cell_size[probeNum];
  dia2		= cell_size2[probeNum];
  dia3		= cell_size3[probeNum];
  concentration = &AveragedData[varp->LRstart];

  if (tas < 0.0) tas = 0.0;

  // Convert missing values to zero.  This will handle the asynchronous nature of
  // nidas::TwoD_USB only putting out histograms when there is data.
  {
  for (i = 0; i < varp->Length; ++i)
    if (std::isnan(actual[i]))
      actual[i] = 0;

  if (std::isnan(deadTime))
    deadTime = 0;
  }

  // During first 6 flights of PACDEX, 2DC probe was incorrectly decoding
  // true airspeed.
  if (cfg.ProjectName().compare("PACDEX") == 0
	&& cfg.FlightNumber().compare(0, 3, "rf0") == 0
	&& cfg.FlightNumber()[3] >= '1' && cfg.FlightNumber()[3] <= '6')
    {
    if (strstr(varp->name, "1DC"))
      {
      for (i = 0; i < varp->Length; ++i)
        actual[i] *= std::max(1.0, (tas / 50.0) / i);
      }
    else	// 2DC is unusable due to incorrect airspeed, zero it out.
      {
      for (i = 0; i < varp->Length; ++i)
        actual[i] = 0.0;
      }
    }

  if (deadTime > 1000.0)
    {
    sprintf(buffer, "%s: 2D Sample Volume negative, reason: deadTime == %f.", varp->name, deadTime);
    LogStdMsg(buffer);
    deadTime = 1000;
    }

  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    {
    sampleVolume[i] = tas * sampleArea[probeNum][i] * 0.001 *
		(((float)1000 - deadTime) / 1000);
    }

#define TACT
#define PLWC
#define DBZ
#define REFF

#include "pms1d_cv"

  if (strstr(varp->name, "1DC"))
    {
    /* For mixing with FSSP/CDP, no channels below 47 micron.
     */
    reff23[varp->ProbeCount] = reff22[varp->ProbeCount] = 0.0;
    for (i = 1 + cfg.ZeroBinOffset(); i < LAST_BIN[probeNum]; ++i)
      {
      reff23[varp->ProbeCount] += concentration[i] * dia3[i]; /* Export to reff.c */
      reff22[varp->ProbeCount] += concentration[i] * dia2[i];
      }
    }

}	/* END STWOD */

/* -------------------------------------------------------------------- */
void sconc2(DERTBL *varp)
{
  PutSample(varp, total_concen[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdisp2(DERTBL *varp)
{
  PutSample(varp, disp[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void stcnt2(DERTBL *varp)
{
  PutSample(varp, tact[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void splwc2(DERTBL *varp)
{
  PutSample(varp, plwc[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void siwc2(DERTBL *varp)
{
  NR_TYPE area	= GetSample(varp, 0);
  NR_TYPE tas	= GetSample(varp, 1);

  NR_TYPE mass = 0.115 * pow(area, 1.218) * 0.001;	// grams
  NR_TYPE iwc = mass / (tas * basicSampleArea[varp->ProbeCount]);
  PutSample(varp, iwc);
}

/* -------------------------------------------------------------------- */
void sdbz2(DERTBL *varp)
{
  PutSample(varp, dbz[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdbar2(DERTBL *varp)
{
  PutSample(varp, dbar[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sreff2(DERTBL *varp)       /* Effective Radius     */
{
  if (reff2[varp->ProbeCount] > 0.0)
    PutSample(varp, 0.5 * (reff3[varp->ProbeCount] / reff2[varp->ProbeCount]));
  else
    PutSample(varp, 0.0);
}

/* -------------------------------------------------------------------- */
void sconc2dc050(DERTBL *varp)
{
  NR_TYPE conc = 0.0;
  NR_TYPE * concentration = GetVector(varp, 0);
  size_t n = 128;

  if (strstr(varp->depend[0], "1DC"))
    n = 64;

  // 50 micron and bigger.
  for (size_t i = conc50idx[varp->ProbeCount]; i < n; ++i)
    conc += concentration[i];

  PutSample(varp, conc);
}

/* -------------------------------------------------------------------- */
void sconc2dc100(DERTBL *varp)
{
  NR_TYPE conc = 0.0;
  NR_TYPE * concentration = GetVector(varp, 0);
  size_t n = 128;

  if (strstr(varp->depend[0], "1DC"))
    n = 64;

  // 100 micron and bigger.
  for (size_t i = conc100idx[varp->ProbeCount]; i < n; ++i)
    conc += concentration[i];

  PutSample(varp, conc);
}

/* -------------------------------------------------------------------- */
void sconc2dc150(DERTBL *varp)
{
  NR_TYPE conc = 0.0;
  NR_TYPE * concentration = GetVector(varp, 0);
  size_t n = 128;

  if (strstr(varp->depend[0], "1DC"))
    n = 64;

  // 150 micron and bigger.
  for (size_t i = conc150idx[varp->ProbeCount]; i < n; ++i)
    conc += concentration[i];

  PutSample(varp, conc);
}


/* -------------------------------------------------------------------- *
 * HVPS Below here.
 * -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
void sTwodInitH(var_base *varp)
{
  size_t	i, probeNum, minRange;
  char		*p;
  float		resolution;
  const char	*serialNumber;

  for (i = 0; i < MAX_PMS2D; ++i)
    SampleRate[i] = 1.0;

  /* This function unfortunatly only gets called once, yet we need to
   * initialaize two probes (sort of) the 1D version and the 2D version.
   * This function is called from the A1D? xlTwodInit, not A2D?...
   */

  serialNumber = varp->SerialNumber.c_str();
  probeNum = varp->ProbeCount;

  MakeProjectFileName(buffer, PMS_SPEC_FILE);
  InitPMSspecs(buffer);

  /* Perform twice, once for 1DC, and again for 2DC.
   */
  if ((p = GetPMSparameter(serialNumber, "FIRST_BIN")) == NULL) {
    sprintf(buffer, "%s: FIRST_BIN not found.", serialNumber);
    HandleFatalError(buffer);
    }
  FIRST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "LAST_BIN")) == NULL) {
    sprintf(buffer, "%s: LAST_BIN not found.", serialNumber);
    HandleFatalError(buffer);
    }
  LAST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "MIN_RANGE")) == NULL) {
    sprintf(buffer, "%s: MIN_RANGE not found.", serialNumber);
    HandleFatalError(buffer);
    }
  minRange = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "RANGE_STEP")) == NULL) {
    sprintf(buffer, "%s: RANGE_STEP not found.", serialNumber);
    HandleFatalError(buffer);
    }
  resolution = atof(p);

  if ((p = GetPMSparameter(serialNumber, "DENS")) == NULL) {
    sprintf(buffer, "%s: DENS not found.", serialNumber);
    HandleFatalError(buffer);
    }
  DENS[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "PLWFAC")) == NULL) {
    sprintf(buffer, "%s: PLWFAC not found.", serialNumber);
    HandleFatalError(buffer);
    }
  PLWFAC[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "DBZFAC")) == NULL) {
    sprintf(buffer, "%s: DBZFAC not found.", serialNumber);
    HandleFatalError(buffer);
    }
  DBZFAC[probeNum] = atof(p);

  /* Precompute dia squared and cubed. */
  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    {
    cell_size2[probeNum][i] = cell_size[probeNum][i] * cell_size[probeNum][i];
    cell_size3[probeNum][i] = cell_size2[probeNum][i] * cell_size[probeNum][i];
    }

  ReleasePMSspecs();

  for (i = 0; i < varp->Length; ++i)
    {
    // 256 diodes, minus 80 we mask off.
    sampleArea[probeNum][i] = 203.0 * 200.0 * (256-80) * 1.0e-6;
    }

}       /* END STWODINITH */

/* -------------------------------------------------------------------- */
void sHVPS(DERTBL *varp)
{
  size_t	i, probeNum;
  NR_TYPE	*concentration, *dia, *dia2, *dia3;
  NR_TYPE	tas;		/* True Air Speed	*/
  NR_TYPE	sampleVolume[256];
  NR_TYPE	deadTime = 0.0;

  const NR_TYPE * actual = GetVector(varp, 0);
  tas		= GetSampleFor1D(varp, 1);
  deadTime	= GetSample(varp, 2);

  probeNum	= varp->ProbeCount;
  dia		= cell_size[probeNum];
  dia2		= cell_size2[probeNum];
  dia3		= cell_size3[probeNum];
  concentration = &AveragedData[varp->LRstart];


  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    {
    sampleVolume[i] = tas * sampleArea[probeNum][i] * (((float)1000 - deadTime) / 1000);

    if (sampleVolume[i] < 0.0)
      {
      printf("HVPS Sample Volume went negative, %f\n", sampleVolume[i]);
      sampleVolume[i] = 0.0;
      }
    }

#define PLWC
#define DBZ
#define REFF

#include "pms1d_cv"

}	/* END SHVPS */

/* END PMS2D.C */
