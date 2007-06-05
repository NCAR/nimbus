/*
-------------------------------------------------------------------------
OBJECT NAME:	asas.c

FULL NAME:	Compute PMS1D ASAS/PCAS/UHSAS derived paramters

DESCRIPTION:	

NOTES:		Calculations taken from Bulletin 24 dated 1/89.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2007
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include "pms.h"

static const int MAX_BINS = 101;

static size_t FIRST_BIN[MAX_ASAS], LAST_BIN[MAX_ASAS], SampleRate[MAX_ASAS];

static NR_TYPE	total_concen[MAX_ASAS], disp[MAX_ASAS], dbar[MAX_ASAS];
static NR_TYPE	aact[MAX_ASAS], pvol[MAX_ASAS], tact[MAX_ASAS];
static NR_TYPE	cell_size[MAX_ASAS][MAX_BINS];
static NR_TYPE	cell_size2[MAX_ASAS][MAX_BINS];
static NR_TYPE	cell_size3[MAX_ASAS][MAX_BINS];

/* -------------------------------------------------------------------- */
void casasInit(var_base *varp)
{
  size_t	i, probeNum;
  const char	*serialNumber;
  char		*p;

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

  if ((p = GetPMSparameter(serialNumber, "CELL_SIZE")) == NULL)
    {
    /* ADS2 SPP probes mimiced old PMS1D interface and padded a useless
     * 0th bin (so 31 bins instead of 30).  ADS3 will not do this.  PMSspecs
     * files should now have FirstBin of 0 instead of 1.  Re: -1 vs. -0 below.
     */
    sprintf(buffer, "CELL_SIZE_%d", varp->Length - (cfg.isADS2() ? 1 : 0));
    if ((p = GetPMSparameter(serialNumber, buffer)) == NULL) {
      printf("%s: CELL_SIZE not found.\n", serialNumber); exit(1);
      }
    }

  strcpy(buffer, p);
  p = strtok(buffer, ", \t\n");

  for (i = 0; i < varp->Length; ++i)
    {
    cell_size[probeNum][i] = p ? atof(p) : 0.0;
    p = strtok(NULL, ", \t\n");
    }

  for (i = varp->Length-1; i > 0; --i)
    {
    cell_size[probeNum][i] =
	(cell_size[probeNum][i] + cell_size[probeNum][i-1]) / 2;

    cell_size2[probeNum][i] = cell_size[probeNum][i] * cell_size[probeNum][i];
    cell_size3[probeNum][i] = cell_size2[probeNum][i] * cell_size[probeNum][i];
    }

  ReleasePMSspecs();

  SampleRate[probeNum] = varp->SampleRate;

}	/* END CASASINIT */

/* -------------------------------------------------------------------- */
void scasas(DERTBL *varp)
{
  size_t	i, probeNum;
  NR_TYPE	*actual, *concentration, activity, *dia, *dia2, *dia3;
  NR_TYPE	flow;           /* PCAS Flow Rate       */
  NR_TYPE	sampleVolume[MAX_BINS];
  NR_TYPE	tas;

  actual	= GetVector(varp, 0, varp->Length);
  activity	= GetSample(varp, 1);
  flow		= GetSample(varp, 2);
  probeNum	= varp->ProbeCount;
  dia		= cell_size[probeNum];
  dia2		= cell_size2[probeNum];
  dia3		= cell_size3[probeNum];

  if (FeedBack == HIGH_RATE_FEEDBACK)
    {
    if (SampleOffset >= SampleRate[probeNum])
      return;

    concentration = &HighRateData[varp->HRstart +(SampleOffset * varp->Length)];    flow /= SampleRate[probeNum];
    activity *= SampleRate[probeNum];
    }
  else
    concentration = &AveragedData[varp->LRstart];


  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    sampleVolume[i] = (1.0 - activity) * flow;

#define VOLUME

/*   Following dummy tas added to keep system from complaining about
       the code added to "pms1d_cv"
         Ron Ruth 19 October 2001  */
    tas = 50.;

#include "pms1d_cv"

}       /* END SCASAS */

/* -------------------------------------------------------------------- */
void scs200(DERTBL *varp)
{
  size_t	i, probeNum;
  NR_TYPE	*actual, *concentration, *dia, *dia2, *dia3;
  NR_TYPE	flow;
  NR_TYPE	sampleVolume[MAX_BINS];
  NR_TYPE	tas;

  actual	= GetVector(varp, 0, varp->Length);
  flow		= GetSample(varp, 1);
  probeNum	= varp->ProbeCount;
  dia		= cell_size[probeNum];
  dia2		= cell_size2[probeNum];
  dia3		= cell_size3[probeNum];

  if (FeedBack == HIGH_RATE_FEEDBACK)
    {
    if (SampleOffset >= SampleRate[probeNum])
      return;

    concentration = &HighRateData[varp->HRstart +(SampleOffset * varp->Length)];
    flow /= SampleRate[probeNum];
    }
  else
    concentration = &AveragedData[varp->LRstart];

  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    sampleVolume[i] = flow;

#define VOLUME
#define TACT

/*   Following dummy tas added to keep system from complaining about
       the code added to "pms1d_cv"
         Ron Ruth 19 October 2001  */
    tas = 50.;

#include "pms1d_cv"

}	/* END SCASAS */

/* -------------------------------------------------------------------- */
void spflwc(DERTBL *varp)
{
  NR_TYPE	flow, psx, atx, flowc;

  flow	= GetSample(varp, 0);
  psx	= GetSample(varp, 1);
  atx	= GetSample(varp, 2);

  // Changed from 294.15 C to 298.15 C on 9/13/01 per DCRogers.
  flowc = flow * (1013.25 / psx) * (atx + 273.15) / 298.15;

  PutSample(varp, flowc);

}

/* -------------------------------------------------------------------- */
void saact(DERTBL *varp)
{
  PutSample(varp, aact[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sconca(DERTBL *varp)
{
  PutSample(varp, total_concen[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdispa(DERTBL *varp)
{
  PutSample(varp, disp[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdbara(DERTBL *varp)
{
  PutSample(varp, dbar[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void spvolp(DERTBL *varp)
{
  PutSample(varp, pvol[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void stcntp(DERTBL *varp)
{
  PutSample(varp, tact[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sconcu100(DERTBL *varp)
{
  NR_TYPE concu100 = 0.0;
  NR_TYPE * concentration = GetVector(varp, 0, varp->Length);

  for (size_t i = 11; i < 99; ++i)
    concu100 += concentration[i];

  PutSample(varp, concu100);
}

/* -------------------------------------------------------------------- */
void sconcu500(DERTBL *varp)
{
  NR_TYPE concu500 = 0.0;
  NR_TYPE * concentration = GetVector(varp, 0, varp->Length);

  for (size_t i = 56; i < 99; ++i)
    concu500 += concentration[i];

  PutSample(varp, concu500);
}

/* END ASAS.C */
