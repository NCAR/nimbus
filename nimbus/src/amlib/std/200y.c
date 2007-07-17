/*
-------------------------------------------------------------------------
OBJECT NAME:	200y.c

FULL NAME:	Compute derived paramters for PMS1D 200Y probe.

ENTRY POINTS:	sc200y()

STATIC FNS:	none

DESCRIPTION:	

OUTPUT:		

REFERENCES:	pms1d.c (ComputePMS1DParams(), ComputeDOF(),
			 ComputeConcentrations())

REFERENCED BY:	Compute()

NOTES:		Calculations taken from Bulletin 24 dated 1/89.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <raf/pms.h>

#define MAX_200Y	1

static size_t	FIRST_BIN[MAX_200Y], LAST_BIN[MAX_200Y], SampleRate[MAX_200Y];
static NR_TYPE	responseTime, armDistance, DENS[MAX_200Y], resolution;
static double	PLWFAC[MAX_200Y];

static NR_TYPE	total_concen[MAX_200Y], dbar[MAX_200Y], plwc[MAX_200Y],
		disp[MAX_200Y], tact[MAX_200Y];

static NR_TYPE	radius[BINS_16],
		dia[BINS_16], dia2[BINS_16], dia3[BINS_16],
		eaw[BINS_16];	/* Effective Sample Width	*/

void    ComputePMS1DParams(NR_TYPE radius[], NR_TYPE eaw[], NR_TYPE cell_size[],
                float minRange, float resolution, size_t nDiodes, size_t length),

	ComputeDOF(NR_TYPE radius[], NR_TYPE tas, NR_TYPE dof[],
		size_t FirstBin, size_t LastBin, float RES, NR_TYPE RESPONSE_TIME);


/* -------------------------------------------------------------------- */
void c200yInit(var_base *varp)
{
  size_t	i, probeNum, nDiodes;
  char		*p;
  const char	*serialNumber;
  NR_TYPE	minRange;

  serialNumber = varp->SerialNumber.c_str();
  probeNum = varp->ProbeCount;

  MakeProjectFileName(buffer, PMS_SPEC_FILE);
  InitPMSspecs(buffer);

  if ((p = GetPMSparameter(serialNumber, "FIRST_BIN")) == NULL) {
    printf("%s: FIRST_BIN not found.\n", serialNumber); exit(1);
    }
  FIRST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "LAST_BIN")) == NULL) {
    printf("%s: LAST_BIN not found.\n", serialNumber); exit(1);
    }
  LAST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "NDIODES")) == NULL) {
    printf("%s: NDIODES not found.\n", serialNumber); exit(1);
    }
  nDiodes = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "MIN_RANGE")) == NULL) {
    printf("%s: MIN_RANGE not found.\n", serialNumber); exit(1);
    }
  minRange = atof(p);

  if ((p = GetPMSparameter(serialNumber, "RANGE_STEP")) == NULL) {
    printf("%s: RANGE_STEP not found.\n", serialNumber); exit(1);
    }
  resolution = atof(p);

  if ((p = GetPMSparameter(serialNumber, "RESPONSE_TIME")) == NULL) {
    printf("%s: RESPONSE_TIME not found.\n", serialNumber); exit(1);
    }
  responseTime = atof(p);

  if ((p = GetPMSparameter(serialNumber, "ARM_DISTANCE")) == NULL) {
    printf("%s: ARM_DISTANCE not found.\n", serialNumber); exit(1);
    }
  armDistance = atof(p);

  if ((p = GetPMSparameter(serialNumber, "DENS")) == NULL) {
    printf("%s: DENS not found.\n", serialNumber); exit(1);
    }
  DENS[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "PLWFAC")) == NULL) {
    printf("%s: PLWFAC not found.\n", serialNumber); exit(1);
    }
  PLWFAC[probeNum] = atof(p);

  ReleasePMSspecs();

  SampleRate[probeNum] = varp->SampleRate;

  ComputePMS1DParams(radius, eaw, dia, minRange, resolution, nDiodes, varp->Length);

  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    {
    dia2[i] = dia[i] * dia[i];
    dia3[i] = dia2[i] * dia[i];
    }

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void sc200y(DERTBL *varp)
{
  size_t	i, probeNum;
  NR_TYPE	*actual, *concentration;
  NR_TYPE	tas;		/* True Air Speed		*/
  NR_TYPE	dof[BINS_16],	/* Depth Of Field		*/
		sampleVolume[BINS_16];

  actual	= GetVector(varp, 0, BINS_16);
  tas		= GetSampleFor1D(varp, 1);
  probeNum	= varp->ProbeCount;


  ComputeDOF(radius, tas, dof, FIRST_BIN[probeNum], LAST_BIN[probeNum],
		resolution, responseTime);

  if (FeedBack == HIGH_RATE_FEEDBACK)
    {
    if (SampleOffset >= SampleRate[probeNum])
      return;

    tas /= SampleRate[probeNum];
    concentration = &HighRateData[varp->HRstart + (SampleOffset *varp->Length)];
    }
  else
    concentration = &AveragedData[varp->LRstart];

  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    {
    if (dof[i] > armDistance)
      dof[i] = armDistance;

    sampleVolume[i] = tas * (dof[i] * eaw[i]);
    }

#define PLWC

#include "pms1d_cv"

}	/* END SC200Y */

/* -------------------------------------------------------------------- */
void sconcy(DERTBL *varp)
{
  PutSample(varp, total_concen[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdispy(DERTBL *varp)
{
  PutSample(varp, disp[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void splwcy(DERTBL *varp)
{
  PutSample(varp, plwc[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdbary(DERTBL *varp)
{
  PutSample(varp, dbar[varp->ProbeCount]);
}

/* END 200Y.C */
