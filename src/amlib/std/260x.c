/*
-------------------------------------------------------------------------
OBJECT NAME:	260x.c

FULL NAME:	Compute derived paramters for PMS1D 260X probe.

ENTRY POINTS:	sc260x()

STATIC FNS:	none

DESCRIPTION:	

REFERENCES:	pms1d.c (ComputePMS1DParams(), ComputeDOF(),
			ComputeConcentrations())

NOTES:		Calculations taken from Bulletin 24 dated 1/89.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2006
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <raf/pms.h>

static size_t	FIRST_BIN[MAX_260X], LAST_BIN[MAX_260X], SampleRate[MAX_260X];
static NR_TYPE	responseTime[MAX_260X], armDistance[MAX_260X], DENS[MAX_260X],
		resolution[MAX_260X], tact[MAX_260X];
static double	PLWFAC[MAX_260X], DBZFAC[MAX_260X];

static NR_TYPE	total_concen[MAX_260X], dbar[MAX_260X], plwc[MAX_260X],
		disp[MAX_260X], dbz[MAX_260X], reff2[MAX_260X], reff3[MAX_260X];

static NR_TYPE	radius[MAX_260X][BINS_64], cell_size[MAX_260X][BINS_64],
		cell_size2[MAX_260X][BINS_64], cell_size3[MAX_260X][BINS_64],
		eaw[MAX_260X][BINS_64];	/* Effective Sample Width	*/

NR_TYPE         reff63[MAX_260X], reff62[MAX_260X];  /* For export to reff.c */

void    ComputePMS1DParams(NR_TYPE radius[], NR_TYPE eaw[], NR_TYPE cell_size[],
	NR_TYPE dof[], float minRange, float resolution, size_t nDiodes,
	size_t length, size_t armDistance),

	ComputeDOF(NR_TYPE radius[], NR_TYPE tas, NR_TYPE dof[],
	size_t FirstBin, size_t LastBin, float RES, NR_TYPE RESPONSE_TIME);

// Probe Count.
static int nProbes = 0;
extern void setProbeCount(const char * location, int count);

/* -------------------------------------------------------------------- */
void c260xInit(var_base *varp)
{
  size_t	i, probeNum, nDiodes;
  char		*p;
  const char	*serialNumber;
  float		minRange;
  NR_TYPE	dof[BINS_64];

  p = strchr(varp->name, '_');
  setProbeCount(p, nProbes++);

  serialNumber = varp->SerialNumber.c_str();
  probeNum = varp->ProbeCount;

  for (i = 0; i < MAX_260X; ++i)
    reff63[i] = reff62[i] = 0.0;

  MakeProjectFileName(buffer, PMS_SPEC_FILE);
  InitPMSspecs(buffer);

  if ((p = GetPMSparameter(serialNumber, "FIRST_BIN")) == NULL) {
    fprintf(stderr, "260x: serial # = [%s]: FIRST_BIN not found.\n", serialNumber); exit(1);
    }
  FIRST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "LAST_BIN")) == NULL) {
    fprintf(stderr, "260x: serial # = [%s]: LAST_BIN not found.\n", serialNumber); exit(1);
    }
  LAST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "NDIODES")) == NULL) {
    fprintf(stderr, "260x: serial # = [%s]: NDIODES not found.\n", serialNumber); exit(1);
    }
  nDiodes = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "MIN_RANGE")) == NULL) {
    fprintf(stderr, "260x: serial # = [%s]: MIN_RANGE not found.\n", serialNumber); exit(1);
    }
  minRange = atof(p);

  if ((p = GetPMSparameter(serialNumber, "RANGE_STEP")) == NULL) {
    fprintf(stderr, "260x: serial # = [%s]: RANGE_STEP not found.\n", serialNumber); exit(1);
    }
  resolution[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "RESPONSE_TIME")) == NULL) {
    fprintf(stderr, "260x: serial # = [%s]: RESPONSE_TIME not found.\n", serialNumber); exit(1);
    }
  responseTime[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "ARM_DISTANCE")) == NULL) {
    fprintf(stderr, "260x: serial # = [%s]: ARM_DISTANCE not found.\n", serialNumber); exit(1);
    }
  armDistance[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "DENS")) == NULL) {
    fprintf(stderr, "260x: serial # = [%s]: DENS not found.\n", serialNumber); exit(1);
    }
  DENS[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "PLWFAC")) == NULL) {
    fprintf(stderr, "260x: serial # = [%s]: PLWFAC not found.\n", serialNumber); exit(1);
    }
  PLWFAC[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "DBZFAC")) == NULL) {
    fprintf(stderr, "260x: serial # = [%s]: DBZFAC not found.\n", serialNumber); exit(1);
    }
  DBZFAC[probeNum] = atof(p);

  ReleasePMSspecs();

  SampleRate[probeNum] = varp->SampleRate;

  ComputePMS1DParams(radius[probeNum], eaw[probeNum], cell_size[probeNum],
	dof, minRange, resolution[probeNum], nDiodes, varp->Length, armDistance[probeNum]);

  /* Precompute dia squared and cubed. */
  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    {
    cell_size2[probeNum][i] = cell_size[probeNum][i] * cell_size[probeNum][i];
    cell_size3[probeNum][i] = cell_size2[probeNum][i] * cell_size[probeNum][i];
    }

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void sc260x(DERTBL *varp)
{
  size_t	i, probeNum;
  NR_TYPE	tas, *concentration, *dia, *dia2, *dia3;
  NR_TYPE	dof[BINS_64],	/* Depth Of Field	*/
		sampleVolume[BINS_64];

  const NR_TYPE * actual = GetVector(varp, 0);
  tas		= GetSampleFor1D(varp, 1);
  probeNum	= varp->ProbeCount;
  dia		= cell_size[probeNum];
  dia2		= cell_size2[probeNum];
  dia3		= cell_size3[probeNum];

  if (tas < 0.0) tas = 0.0;

  ComputeDOF(radius[probeNum], tas, dof, FIRST_BIN[probeNum],
	LAST_BIN[probeNum], resolution[probeNum], responseTime[probeNum]);

  if (FeedBack == HIGH_RATE_FEEDBACK)
    {
    if (SampleOffset >= SampleRate[probeNum])
      return;

    tas /= SampleRate[probeNum];
    concentration = &HighRateData[varp->HRstart + (SampleOffset*varp->Length)];
    }
  else
    concentration = &AveragedData[varp->LRstart];

  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    {
    if (dof[i] > armDistance[probeNum])
      dof[i] = armDistance[probeNum];

    sampleVolume[i] = tas * (dof[i] * eaw[probeNum][i]) * 0.001;
    }

#define PLWC
#define DBZ
#define REFF
#define TACT

#include "pms1d_cv"

  /* For mixing with FSSP, no channels below 47 micron.
   */
  reff63[varp->ProbeCount] = reff62[varp->ProbeCount] = 0.0;
  for (i = 4; i < LAST_BIN[probeNum]; ++i)
    {
    reff63[varp->ProbeCount] += concentration[i] * dia3[i]; /* Export to reff.c */
    reff62[varp->ProbeCount] += concentration[i] * dia2[i];
    }

}	/* END SC260X */

/* -------------------------------------------------------------------- */
void sconc6(DERTBL *varp)
{
  PutSample(varp, total_concen[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void stcnt6(DERTBL *varp)
{
  PutSample(varp, tact[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdisp6(DERTBL *varp)
{
  PutSample(varp, disp[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void splwc6(DERTBL *varp)
{
  PutSample(varp, plwc[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdbz6(DERTBL *varp)
{
  PutSample(varp, dbz[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdbar6(DERTBL *varp)
{
  PutSample(varp, dbar[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sreff6(DERTBL *varp)       /* Effective Radius     */
{
  if (reff2[varp->ProbeCount] > 0.0)
    PutSample(varp, 0.5 * (reff3[varp->ProbeCount] / reff2[varp->ProbeCount]));
  else
    PutSample(varp, 0.0);

}

/* END 260X.C */
