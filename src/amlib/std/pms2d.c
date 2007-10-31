/*
-------------------------------------------------------------------------
OBJECT NAME:	pms2d.c

FULL NAME:	Derived computations for the PMS2D C & P probes.

ENTRY POINTS:	sTwodInit()
		sTwoD()

STATIC FNS:	none

DESCRIPTION:	

DEPENDENCIES:	A2D?	- Actual 2D data
		TAS	- True Air Speed

OUTPUT:		C2D?	- Concentrations
		CONC2	- Total concentration
		DISP2
		DBAR2
		PLWC2	- Liquid Water Content

COPYRIGHT:	University Corporation for Atmospheric Research, 2000-2006
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <raf/pms.h>

static const NR_TYPE shadowLevel = 0.55;

// Use a fixed DOF, not what the manual specifies, until such time that a "research
// project" can be done.  Al Cooper, Jorgen Jenson 6/26/06

//static NR_TYPE DOF2dP[] = { 0.0, 145.203, 261.0, 261.0, 261.0,
static NR_TYPE DOF2dP[] = { 0.0, 261.0, 261.0, 261.0, 261.0,
	261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0,
	261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0,
	261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0,
	261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0,
	261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0,
	261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0, 261.0 };

//static NR_TYPE DOF2dC[] = { 0.0, 1.56, 6.25, 14.06, 25.0, 39.06, 56.25,
static NR_TYPE DOF2dC[] = { 0.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0,
	61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0,
	61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0,
	61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0,
	61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0,
	61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0, 61.0,
	61.0, 61.0, 61.0 };


static size_t	FIRST_BIN[MAX_PMS2D], LAST_BIN[MAX_PMS2D];
static NR_TYPE  responseTime[MAX_PMS2D], armDistance[MAX_PMS2D],
		DENS[MAX_PMS2D], resolution[MAX_PMS2D], SampleRate[MAX_PMS2D];
static double   PLWFAC[MAX_PMS2D], DBZFAC[MAX_PMS2D];

static NR_TYPE  total_concen[MAX_PMS2D], dbar[MAX_PMS2D], plwc[MAX_PMS2D],
                disp[MAX_PMS2D], dbz[MAX_PMS2D], tact[MAX_PMS2D];

static NR_TYPE  radius[MAX_PMS2D][BINS_64], cell_size[MAX_PMS2D][BINS_64],
		cell_size2[MAX_PMS2D][BINS_64], cell_size3[MAX_PMS2D][BINS_64],
                eaw[MAX_PMS2D][BINS_64], reff2[MAX_PMS2D], reff3[MAX_PMS2D];
                                                                                          
NR_TYPE         reff23[MAX_PMS2D], reff22[MAX_PMS2D];  /* For export to reff.c */

void    ComputePMS1DParams(NR_TYPE radius[], NR_TYPE eaw[], NR_TYPE cell_size[],
	float minRange, float resolution, size_t nDiodes, size_t length);


/* -------------------------------------------------------------------- */
void sTwodInit(var_base *varp)
{
  size_t	i, j, length, probeNum, nDiodes, minRange;
  char		*p;
  const char	*serialNumber;

  for (i = 0; i < MAX_PMS2D; ++i)
    SampleRate[i] = 1.0;

  /* This function unfortunatly only gets called once, yet we need to
   * initialaize two probes (sort of) the 1D version and the 2D version.
   * This function is called from the A1D? xlTwodInit, not A2D?...
   */

  serialNumber = varp->SerialNumber.c_str();
  probeNum = varp->ProbeCount;

  for (i = 0; i < MAX_PMS2D; ++i)
    reff23[i] = reff22[i] = 0.0;

  MakeProjectFileName(buffer, PMS_SPEC_FILE);
  InitPMSspecs(buffer);

  /* Perform twice, once for 1DC, and again for 2DC.
   */
  for (j = 0; j < 2; ++j, ++probeNum)
    {
    if ((p = GetPMSparameter(serialNumber, "FIRST_BIN")) == NULL) {
      fprintf(stderr, "pms2d: serial number = [%s]: FIRST_BIN not found.\n", serialNumber); exit(1);
      }
    FIRST_BIN[probeNum] = atoi(p);

    if ((p = GetPMSparameter(serialNumber, "LAST_BIN")) == NULL) {
      fprintf(stderr, "pms2d: serial number = [%s]: LAST_BIN not found.\n", serialNumber); exit(1);
      }
    LAST_BIN[probeNum] = atoi(p);

    if ((p = GetPMSparameter(serialNumber, "MIN_RANGE")) == NULL) {
      fprintf(stderr, "pms2d: serial number = [%s]: MIN_RANGE not found.\n", serialNumber); exit(1);
      }
    minRange = atoi(p);

    if ((p = GetPMSparameter(serialNumber, "RANGE_STEP")) == NULL) {
      fprintf(stderr, "pms2d: serial number = [%s]: RANGE_STEP not found.\n", serialNumber); exit(1);
      }
    resolution[probeNum] = atof(p);

    if ((p = GetPMSparameter(serialNumber, "NDIODES")) == NULL) {
      fprintf(stderr, "pms2d: serial number = [%s]: NDIODES not found.\n", serialNumber); exit(1);
      }
    nDiodes = atoi(p);

    if ((p = GetPMSparameter(serialNumber, "RESPONSE_TIME")) == NULL) {
      fprintf(stderr, "pms2d: serial number = [%s]: RESPONSE_TIME not found.\n", serialNumber); exit(1);
      }
    responseTime[probeNum] = atof(p);

    if ((p = GetPMSparameter(serialNumber, "ARM_DISTANCE")) == NULL) {
      fprintf(stderr, "pms2d: serial number = [%s]: ARM_DISTANCE not found.\n", serialNumber); exit(1);
      }
    armDistance[probeNum] = atof(p);

    if ((p = GetPMSparameter(serialNumber, "DENS")) == NULL) {
      fprintf(stderr, "pms2d: serial number = [%s]: DENS not found.\n", serialNumber); exit(1);
      }
    DENS[probeNum] = atof(p);

    if ((p = GetPMSparameter(serialNumber, "PLWFAC")) == NULL) {
      fprintf(stderr, "pms2d: serial number = [%s]: PLWFAC not found.\n", serialNumber); exit(1);
      }
    PLWFAC[probeNum] = atof(p);

    if ((p = GetPMSparameter(serialNumber, "DBZFAC")) == NULL) {
      fprintf(stderr, "pms2d: serial number = [%s]: DBZFAC not found.\n", serialNumber); exit(1);
      }
    DBZFAC[probeNum] = atof(p);

    /* 1DC/P has length 32, 2DC/P has length 64.
     */
    length = varp->Length;

    if (j > 0)
      {
      LAST_BIN[probeNum] += 32;
      length += 32;
      nDiodes += 32;
      }

    ComputePMS1DParams(radius[probeNum], eaw[probeNum], cell_size[probeNum],
        minRange, resolution[probeNum], nDiodes, length);

    /* Precompute dia squared and cubed. */
    for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
      {
      cell_size2[probeNum][i] = cell_size[probeNum][i] * cell_size[probeNum][i];
      cell_size3[probeNum][i] = cell_size2[probeNum][i] * cell_size[probeNum][i];
      }

    if (j > 0)  /* 2DC only (not 1DC). */
      {
      if (varp->name[3] == 'C')		/* EAW is fixed .8 for 2DC */
        for (i = 0; i < length; ++i)
          eaw[probeNum][i] = 0.8;
      else
      if (varp->name[3] == 'P')		/* EAW is fixed 6.4 for 2DP */
        for (i = 0; i < length; ++i)
          eaw[probeNum][i] = 6.4;
      }
    }

  ReleasePMSspecs();

}	/* END STWODINIT */

/* -------------------------------------------------------------------- */
void sTwodInitH(var_base *varp)
{
  size_t	i, length, probeNum, nDiodes = 0, minRange;
  char		*p;
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
    printf("%s: FIRST_BIN not found.\n", serialNumber); exit(1);
    }
  FIRST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "LAST_BIN")) == NULL) {
    printf("%s: LAST_BIN not found.\n", serialNumber); exit(1);
    }
  LAST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "MIN_RANGE")) == NULL) {
    printf("%s: MIN_RANGE not found.\n", serialNumber); exit(1);
    }
  minRange = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "RANGE_STEP")) == NULL) {
    printf("%s: RANGE_STEP not found.\n", serialNumber); exit(1);
    }
  resolution[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "DENS")) == NULL) {
    printf("%s: DENS not found.\n", serialNumber); exit(1);
    }
  DENS[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "PLWFAC")) == NULL) {
    printf("%s: PLWFAC not found.\n", serialNumber); exit(1);
    }
  PLWFAC[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "DBZFAC")) == NULL) {
    printf("%s: DBZFAC not found.\n", serialNumber); exit(1);
    }
  DBZFAC[probeNum] = atof(p);

  /* 1DC/P has length 32, 2DC/P has length 64.
   */
  length = varp->Length;
                                                                                          
  ComputePMS1DParams(radius[probeNum], eaw[probeNum], cell_size[probeNum],
      minRange, resolution[probeNum], nDiodes, length);

  /* Precompute dia squared and cubed. */
  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    {
    cell_size2[probeNum][i] = cell_size[probeNum][i] * cell_size[probeNum][i];
    cell_size3[probeNum][i] = cell_size2[probeNum][i] * cell_size[probeNum][i];
    }

  ReleasePMSspecs();

}       /* END STWODINITH */

/* -------------------------------------------------------------------- */
void sTwoD(DERTBL *varp)
{
  size_t	i, probeNum;
  NR_TYPE	*actual, *concentration, *dia, *dia2, *dia3;
  NR_TYPE	tas;		/* True Air Speed	*/
  NR_TYPE	sampleVolume[BINS_64], sampleArea;
  NR_TYPE	*dof, deadTime;

  actual	= GetVector(varp, 0, varp->Length);
  tas		= GetSampleFor1D(varp, 1);
  deadTime	= GetSample(varp, 2);

  probeNum	= varp->ProbeCount;
  dia		= cell_size[probeNum];
  dia2		= cell_size2[probeNum];
  dia3		= cell_size3[probeNum];
  concentration = &AveragedData[varp->LRstart];

  if (varp->name[3] == 'P')
    dof = DOF2dP;
  else
    dof = DOF2dC;

//if (tas > 100 && varp->name[3] == 'C')
//  printf("%s %d %f %f -------\n", varp->name, varp->ProbeCount, deadTime, (float)((1000 - (int)deadTime) / 1000));

  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    {
    sampleArea = dof[i] * eaw[probeNum][i];
/*
    if (varp->name[3] == 'C')
      if (dia[i] < 160)
        sampleArea = tas * (7.58e-7*resolution[probeNum]*dia[i]*dia[i]+1.71e-8*dia[i]*dia[i]*dia[i]);
      else
        sampleArea = tas * (1.94e-2*resolution[probeNum]+4.39e-4*dia[i]);
*/
    sampleVolume[i] = tas * sampleArea * 0.001 *
		(((float)1000 - deadTime) / 1000);

    if (sampleVolume[i] < 0.0)
      {
      sprintf(buffer, "2D Sample Volume went negative, %f", sampleVolume[i]);
      LogStdMsg(buffer);
      sampleVolume[i] = 0.0;
      }

//if (tas > 100 && varp->name[3] == 'C')
//  printf("%d %d %f %f\n", i, (int)dia[i], sampleArea, dof[i] * eaw[probeNum][i]);
    }

//if (tas > 100 && varp->name[3] == 'C') exit(1);

#define PLWC
#define DBZ
#define REFF

//printf("tact=%f, deadTime=%f\n", tact[probeNum], deadTime);

#include "pms1d_cv"

//printf("cnt: %f %f %f %f %f\n", actual[1], actual[2], actual[3], actual[4], actual[5]);
//printf("sv: %f %f %f %f %f\n", sampleVolume[1], sampleVolume[2], sampleVolume[3], sampleVolume[4], sampleVolume[5]);
//printf("cnc: %f %f %f %f %f\n", concentration[1], concentration[2], concentration[3], concentration[4], concentration[5]);

  if (strstr(varp->name, "1DC"))
    {
    /* For mixing with FSSP, no channels below 47 micron.
     */
    reff23[varp->ProbeCount] = reff22[varp->ProbeCount] = 0.0;
    for (i = 2; i < LAST_BIN[probeNum]; ++i)
      {
      reff23[varp->ProbeCount] += concentration[i] * dia3[i]; /* Export to reff.c */
      reff22[varp->ProbeCount] += concentration[i] * dia2[i];
      }
    }

}	/* END STWOD */

/* -------------------------------------------------------------------- */
void sHVPS(DERTBL *varp)
{
  size_t	i, probeNum;
  NR_TYPE	*actual, *concentration, *dia, *dia2, *dia3;
  NR_TYPE	tas;		/* True Air Speed	*/
  NR_TYPE	sampleVolume[256], sampleArea;
  NR_TYPE	deadTime;

  actual	= GetVector(varp, 0, varp->Length);
  tas		= GetSampleFor1D(varp, 1);
  deadTime	= GetSample(varp, 2);

  probeNum	= varp->ProbeCount;
  dia		= cell_size[probeNum];
  dia2		= cell_size2[probeNum];
  dia3		= cell_size3[probeNum];
  concentration = &AveragedData[varp->LRstart];

  // 256 diodes, minus 80 we mask off.
  sampleArea = 203.0 * 200.0 * (256-80) * 1.0e-6;

  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    {
    sampleVolume[i] = tas * sampleArea * (((float)1000 - deadTime) / 1000);

    if (sampleVolume[i] < 0.0)
      {
      printf("HVPS Sample Volume went negative, %f\n", sampleVolume[i]);
      sampleVolume[i] = 0.0;
      }
    }

#define PLWC
#define DBZ
#define REFF

//printf("tact=%f, deadTime=%f\n", tact[probeNum], deadTime);

#include "pms1d_cv"

}	/* END SHVPS */

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

/* END PMS2D.C */
