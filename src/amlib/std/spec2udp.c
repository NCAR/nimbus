/*
-------------------------------------------------------------------------
OBJECT NAME:	spec2d.c

FULL NAME:	Derived computations for the SPEC instruments, 2DS, HVPS

ENTRY POINTS:	sSpecUDPinit()
		sSpecUDP()
		sTwoDInitH()	HVPS
		sHVPS()		HVPS

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 2022
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <raf/pms.h>

#include <cassert>

static const size_t MAX_SPEC = 4;

static const size_t maxBins = 32;


static size_t	FIRST_BIN[MAX_SPEC], LAST_BIN[MAX_SPEC], conc50idx[MAX_SPEC],
		conc100idx[MAX_SPEC], conc150idx[MAX_SPEC];
static NR_TYPE  armDistance[MAX_SPEC], dof_const[MAX_SPEC],
		SampleRate[MAX_SPEC], basicSampleArea[MAX_SPEC];
static double   DENS[MAX_SPEC], PLWFAC[MAX_SPEC], DBZFAC[MAX_SPEC];

static NR_TYPE  total_concen[MAX_SPEC], dbar[MAX_SPEC], plwc[MAX_SPEC],
		disp[MAX_SPEC], dbz[MAX_SPEC], tact[MAX_SPEC];

static NR_TYPE  cell_size[MAX_SPEC][maxBins],
		cell_size2[MAX_SPEC][maxBins], cell_size3[MAX_SPEC][maxBins],
		sampleArea[MAX_SPEC][maxBins], reff2[MAX_SPEC], reff3[MAX_SPEC];


// Probe Count.
static size_t nProbes = 0;
extern void setProbeCount(const char * location, int count);

void addDOFtoAttrs(const var_base *varp, NR_TYPE eaw[], NR_TYPE dof[]);

/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
void sSpecUDPinit(var_base *varp)
{
  size_t	i, length, probeNum, nDiodes, minRange;
  char		*p;
  float		resolution;
  const char	*serialNumber;
  NR_TYPE	eaw[maxBins], dof[maxBins];

  if (varp->SerialNumber.length() == 0) {
    sprintf(buffer, "spec2d.c: %s has no serial number, fatal.", varp->name);
    HandleFatalError(buffer);
    }

  if (nProbes == MAX_SPEC) {
    HandleFatalError("spec2d.c: Exceeded maximum number of SPEC probes, change amlib.h.");
    }

  varp->SerialNumber.append("_UDP");

  for (i = 0; i < MAX_SPEC; ++i)
    SampleRate[i] = 1.0;

  setProbeCount(&varp->name[1], nProbes++);

  serialNumber = varp->SerialNumber.c_str();
  probeNum = varp->ProbeCount;

  MakeProjectFileName(buffer, PMS_SPEC_FILE);
  InitPMSspecs(buffer);

  if ((p = GetPMSparameter(serialNumber, "FIRST_BIN")) == NULL) {
    sprintf(buffer, "spec2d: serial number = [%s]: FIRST_BIN not found.", serialNumber);
    HandleFatalError(buffer);
    }
  FIRST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "LAST_BIN")) == NULL) {
    sprintf(buffer, "spec2d: serial number = [%s]: LAST_BIN not found.", serialNumber);
    HandleFatalError(buffer);
    }
  LAST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "RANGE_STEP")) == NULL) {
    sprintf(buffer, "spec2d: serial number = [%s]: RANGE_STEP not found.", serialNumber);
    HandleFatalError(buffer);
    }
  resolution = atof(p);

  if ((p = GetPMSparameter(serialNumber, "CELL_SIZE")) == NULL) {
    sprintf(buffer, "spec2d: serial number = [%s]: CELL_SIZE not found.", serialNumber);
    HandleFatalError(buffer);
    }

printf("%s : %d : %f\n", varp->name, varp->Length, resolution);
printf(" [%s]\n", p);
  strcpy(buffer, p);
  p = strtok(buffer, ", \t\n");
printf("cell_sizes=");
  for (i = 0; i < varp->Length; ++i)
  {
    cell_size[probeNum][i] = p ? atof(p) : 0.0;
    p = strtok(NULL, ", \t\n");
printf("%f, ", cell_size[probeNum][i]);
  }
printf("\n");

  if ((p = GetPMSparameter(serialNumber, "NDIODES")) == NULL) {
    sprintf(buffer, "spec2d: serial number = [%s]: NDIODES not found.", serialNumber);
    HandleFatalError(buffer);
    }
  nDiodes = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "DOF_CONST")) == NULL)
    dof_const[probeNum] = 5.13; // default for 2DS probes.
  else
    dof_const[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "ARM_DISTANCE")) == NULL) {
    sprintf(buffer, "spec2d: serial number = [%s]: ARM_DISTANCE not found.", serialNumber);
    HandleFatalError(buffer);
    }
  armDistance[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "DENS")) == NULL) {
    sprintf(buffer, "spec2d: serial number = [%s]: DENS not found.", serialNumber);
    HandleFatalError(buffer);
    }
  DENS[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "PLWFAC")) == NULL) {
    sprintf(buffer, "spec2d: serial number = [%s]: PLWFAC not found.", serialNumber);
    HandleFatalError(buffer);
    }
  PLWFAC[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "DBZFAC")) == NULL) {
    sprintf(buffer, "spec2d: serial number = [%s]: DBZFAC not found.", serialNumber);
    HandleFatalError(buffer);
    }
  DBZFAC[probeNum] = atof(p);

  ReleasePMSspecs();

  basicSampleArea[probeNum] = (armDistance[probeNum]/1000) * (resolution / 1000000) * nDiodes;

  /* 1DS has length 128, 2DS has length 256.
   */
  length = varp->Length;


  /* Precompute dia squared and cubed. */
//  for (i = length-1; i >= 1; --i)
  for (i = 0; i < varp->Length;  ++i)
  {
    float size = cell_size[probeNum][i] =
        (cell_size[probeNum][i] + cell_size[probeNum][i+1]) / 2;

    cell_size2[probeNum][i] = size * size;
    cell_size3[probeNum][i] = cell_size2[probeNum][i] * size * size;

    eaw[i]      = (resolution / 1000) * nDiodes;
    dof[i]      = dof_const[probeNum] * size * size / 1000.0;
    if (dof[i] > armDistance[probeNum])
      dof[i] = (NR_TYPE)armDistance[probeNum];
  }

printf("cell_sizes=");
for (i = 0; i < length; ++i)
  printf("%f, ", cell_size[probeNum][i]);
printf("\n");

//  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
//    {
//    cell_size2[probeNum][i] = cell_size[probeNum][i] * cell_size[probeNum][i];
//    cell_size3[probeNum][i] = cell_size2[probeNum][i] * cell_size[probeNum][i];
//    }

  addDOFtoAttrs(varp, eaw, dof);
  for (i = 0; i < length; ++i)
    sampleArea[probeNum][i] = eaw[i] * dof[i];

  conc50idx[probeNum] = 50 / (int)resolution;
  conc100idx[probeNum] = 100 / (int)resolution;
  conc150idx[probeNum] = 150 / (int)resolution;

}	/* END STWODINIT */

/* -------------------------------------------------------------------- */
void s2DS(DERTBL *varp)
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

  if (deadTime > 1000.0)
    {
    sprintf(buffer, "%s: 2D Sample Volume negative, reason: deadTime == %f.", varp->name, deadTime);
    LogStdMsg(buffer);
    deadTime = 1000;
    }

if (actual[0])
  printf("sv: ");
  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    {
    sampleVolume[i] = tas * sampleArea[probeNum][i] * 0.001 *
		(((float)1000 - deadTime) / 1000);
if (actual[0])
  printf("%f, ", sampleVolume[i]);
    }
if (actual[0])
  printf("\n");

#define TACT
#define PLWC
#define DBZ
#define REFF

#include "pms1d_cv"

}	/* END STWOD */

/* -------------------------------------------------------------------- */
void sconc2s(DERTBL *varp)
{
  PutSample(varp, total_concen[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdisp2s(DERTBL *varp)
{
  PutSample(varp, disp[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void stcnt2s(DERTBL *varp)
{
  PutSample(varp, tact[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void splwc2s(DERTBL *varp)
{
  PutSample(varp, plwc[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void siwc2s(DERTBL *varp)
{
  NR_TYPE area	= GetSample(varp, 0);
  NR_TYPE tas	= GetSample(varp, 1);

  NR_TYPE mass = 0.115 * pow(area, 1.218) * 0.001;	// grams
  NR_TYPE iwc = mass / (tas * basicSampleArea[varp->ProbeCount]);
  PutSample(varp, iwc);
}

/* -------------------------------------------------------------------- */
void sdbz2s(DERTBL *varp)
{
  PutSample(varp, dbz[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdbar2s(DERTBL *varp)
{
  PutSample(varp, dbar[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sconc2ds050(DERTBL *varp)
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
void sconc2ds100(DERTBL *varp)
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
void sconc2ds150(DERTBL *varp)
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

