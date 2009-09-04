/*
-------------------------------------------------------------------------
OBJECT NAME:	asas.c

FULL NAME:	Compute PMS1D ASAS/PCASP/UHSAS derived paramters

DESCRIPTION:	

NOTES:		Calculations taken from Bulletin 24 dated 1/89.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2009
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <raf/pms.h>

static const int MAX_BINS = 101;

static size_t FIRST_BIN[MAX_ASAS], LAST_BIN[MAX_ASAS], SampleRate[MAX_ASAS];

static NR_TYPE	total_concen[MAX_ASAS], disp[MAX_ASAS], dbar[MAX_ASAS];
static NR_TYPE	aact[MAX_ASAS], pvol[MAX_ASAS], tact[MAX_ASAS];
static NR_TYPE	cell_size[MAX_ASAS][MAX_BINS];
static NR_TYPE	cell_size2[MAX_ASAS][MAX_BINS];
static NR_TYPE	cell_size3[MAX_ASAS][MAX_BINS];

// Probe Count.
static int nProbes = 0;
extern void setProbeCount(const char * location, int count);

static void uhsasBinConsolidation(NR_TYPE *actual);

/* -------------------------------------------------------------------- */
void casasInit(var_base *varp)
{
  size_t	i, probeNum;
  const char	*serialNumber;
  char		*p;

  p = strchr(varp->name, '_');
  setProbeCount(p, nProbes++);

  serialNumber = varp->SerialNumber.c_str();
  probeNum = varp->ProbeCount;

  MakeProjectFileName(buffer, PMS_SPEC_FILE);
  InitPMSspecs(buffer);

  if ((p = GetPMSparameter(serialNumber, "FIRST_BIN")) == NULL) {
    fprintf(stderr, "pcasp: serial number = [%s]: FIRST_BIN not found.\n", serialNumber); exit(1);
    }
  FIRST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "LAST_BIN")) == NULL) {
    fprintf(stderr, "pcasp: serial number = [%s]: LAST_BIN not found.\n", serialNumber); exit(1);
    }
  LAST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "CELL_SIZE")) == NULL)
    {
    /* ADS2 SPP probes mimiced old PMS1D interface and padded a useless
     * 0th bin (so 31 bins instead of 30).  ADS3 will not do this.  PMSspecs
     * files should now have FirstBin of 0 instead of 1.  Re: -1 vs. -0 below.
     */
    sprintf(buffer, "CELL_SIZE_%d", varp->Length - 1);
    if ((p = GetPMSparameter(serialNumber, buffer)) == NULL) {
      fprintf(stderr, "pcasp: serial number = [%s]: %s not found.\n", serialNumber, buffer); exit(1);
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
    // Compute mid-point.
    cell_size[probeNum][i] =
	(cell_size[probeNum][i] + cell_size[probeNum][i-1]) / 2;

    // Squared and cube'd.
    cell_size2[probeNum][i] = cell_size[probeNum][i] * cell_size[probeNum][i];
    cell_size3[probeNum][i] = cell_size2[probeNum][i] * cell_size[probeNum][i];
    }

  ReleasePMSspecs();

  SampleRate[probeNum] = varp->SampleRate;

}	/* END CASASINIT */

/* -------------------------------------------------------------------- */
void scasas(DERTBL *varp)	// Original PMS ASAS/PCASP probes.
{
  size_t	i, probeNum;
  NR_TYPE	*concentration, activity, *dia, *dia2, *dia3;
  NR_TYPE	flow;           /* PCAS Flow Rate       */
  NR_TYPE	sampleVolume[MAX_BINS];
  NR_TYPE	tas = 0.0;

  const NR_TYPE * actual = GetVector(varp, 0);
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

    concentration = &HighRateData[varp->HRstart +(SampleOffset * varp->Length)];
    flow /= SampleRate[probeNum];
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
void scs200(DERTBL *varp)	// DMT Modified SPP200 & UHSAS.
{
  size_t	i, probeNum;
  NR_TYPE	*actual, *concentration, *dia, *dia2, *dia3;
  NR_TYPE	flow;
  NR_TYPE	sampleVolume[MAX_BINS];
  NR_TYPE	tas;

  actual	= GetVector(varp, 0);
  flow		= GetSample(varp, 1);
  probeNum	= varp->ProbeCount;
  dia		= cell_size[probeNum];
  dia2		= cell_size2[probeNum];
  dia3		= cell_size3[probeNum];

  /* Fix overlapping gain stages.  The probe should be dynamically configured,
   * but there is no documentation to that at this time, so a fix gain stage
   * setting is used.  There are four gain stages, combine 8 bins into one bin
   * to smooth out discontinuities at each gain stage (at bin 19, bin 44, and 77).
   */
  if (cfg.ProjectName().compare(0, 5, "HIPPO") == 0)
    uhsasBinConsolidation(actual);

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
   *   the code added to "pms1d_cv". Ron Ruth 19 October 2001.
   */
  tas = 50.0;

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
  flowc = flow * (1013.25 / psx) * (atx + Kelvin) / 298.15;

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
  NR_TYPE * concentration = GetVector(varp, 0);

  if (SampleOffset >= SampleRate[0])
    return;

  for (size_t i = 11; i < 100; ++i)
    concu100 += concentration[i];

  PutSample(varp, concu100);
}

/* -------------------------------------------------------------------- */
void sconcu500(DERTBL *varp)
{
  NR_TYPE concu500 = 0.0;
  NR_TYPE * concentration = GetVector(varp, 0);

  if (SampleOffset >= SampleRate[0])
    return;

  for (size_t i = 73; i < 100; ++i)
    concu500 += concentration[i];

  PutSample(varp, concu500);
}

/* -------------------------------------------------------------------- */
static void uhsasBinConsolidation(NR_TYPE *actual)
{
  /* Fix overlapping gain stages.  The probe should be dynamically configured,
   * but there is no documentation to that at this time, so a fix gain stage
   * setting is used.  There are four gain stages, combine 8 bins into one bin
   * to smooth out discontinuities at each gain stage (at bin 19, bin 44, and 77).
   */
  NR_TYPE uhsas_raw[MAX_BINS];	// Used only for correcting PACDEX-HIPPO.
  int src = 0, dst = 0;
  memset(uhsas_raw, 0, sizeof(uhsas_raw));

  while (src < 16)
    uhsas_raw[dst++] = actual[src++];

  while (src < 24)
    uhsas_raw[dst] += actual[src++];
  ++dst;

  while (src < 41)
    uhsas_raw[dst++] = actual[src++];

  while (src < 49)
    uhsas_raw[dst] += actual[src++];
  ++dst;

  while (src < 74)
    uhsas_raw[dst++] = actual[src++];

  while (src < 82)
    uhsas_raw[dst] += actual[src++];
  ++dst;

  while (src < 100)
    uhsas_raw[dst++] = actual[src++];

  while (dst < 100)
    uhsas_raw[dst++] = 0.0;

  memcpy(actual, uhsas_raw, 100 * 4);
}

/* END ASAS.C */
