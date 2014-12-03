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

static const size_t MAX_ASAS = 3;
static const int MAX_BINS = 101;

static NR_TYPE USCAT_THRESHOLD = 3.95;

static size_t FIRST_BIN[MAX_ASAS], LAST_BIN[MAX_ASAS], SampleRate[MAX_ASAS];

static NR_TYPE	total_concen[MAX_ASAS], disp[MAX_ASAS], dbar[MAX_ASAS];
static NR_TYPE	aact[MAX_ASAS], pvol[MAX_ASAS], tact[MAX_ASAS];
static NR_TYPE	cell_size[MAX_ASAS][MAX_BINS];
static NR_TYPE	cell_size2[MAX_ASAS][MAX_BINS];
static NR_TYPE	cell_size3[MAX_ASAS][MAX_BINS];

static size_t concu100_start_bin = 1, concu500_start_bin = 1;

// Probe Count.
static size_t nProbes = 0;
extern void setProbeCount(const char * location, int count);

static void uhsasBinConsolidation(NR_TYPE *actual);

/* -------------------------------------------------------------------- */
void casasInit(var_base *varp)
{
  size_t	i, probeNum;
  const char	*serialNumber;
  char		*p;

  if (varp->SerialNumber.length() == 0) {
    sprintf(buffer, "asas.c: ASAS, PCASP, or UHSAS %s has no serial number, fatal.", varp->name);
    HandleFatalError(buffer);
    }

  if (nProbes == MAX_ASAS) {
    sprintf(buffer, "asas.c: Exceeded maximum number of ASAS/PCASP/UHSAS probes, change amlib.h.");
    HandleFatalError(buffer);
    }

  p = strrchr(varp->name, '_');
  setProbeCount(p, nProbes++);

  serialNumber = varp->SerialNumber.c_str();
  probeNum = varp->ProbeCount;

  MakeProjectFileName(buffer, PMS_SPEC_FILE);
  InitPMSspecs(buffer);

  if ((p = GetPMSparameter(serialNumber, "FIRST_BIN")) == NULL) {
    sprintf(buffer, "pcasp: serial number = [%s]: FIRST_BIN not found.", serialNumber);
    HandleFatalError(buffer);
    }
  FIRST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "LAST_BIN")) == NULL) {
    sprintf(buffer, "pcasp: serial number = [%s]: LAST_BIN not found.", serialNumber);
    HandleFatalError(buffer);
    }
  LAST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "CELL_SIZE")) == NULL)
    {
    /* ADS2 SPP probes mimiced old PMS1D interface and padded a useless
     * 0th bin (so 31 bins instead of 30).  ADS3 will not do this.  PMSspecs
     * files should now have FirstBin of 0 instead of 1.  Re: -1 vs. -0 below.
     */
    sprintf(buffer, "CELL_SIZE_%zu", varp->Length - 1);
    if ((p = GetPMSparameter(serialNumber, buffer)) == NULL) {
      char msg[128];
      sprintf(msg, "pcasp: serial number = [%s]: %s not found.", serialNumber, buffer);
      HandleFatalError(msg);
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

    // Locate start bin numbers for .1 and .5 nanometer variables (CONCU100 & CONCU500).
    if (cell_size[probeNum][i] >= 0.1)
      concu100_start_bin = i;

    if (cell_size[probeNum][i] >= 0.5)
      concu500_start_bin = i;
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
   * HIPPO phase 1 & 2 only.
   */
  if (cfg.ProjectName().compare("HIPPO-1") == 0 ||
      cfg.ProjectName().compare("HIPPO-2") == 0)
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


  // Check for valid USCAT (high USCAT indicates laser instability); if not, nan everything.
  if (varp->ndep > 2 && strncmp(varp->depend[2], "USCAT", 5) == 0)
  {
    NR_TYPE uscat = GetSample(varp, 2);

    if (uscat > USCAT_THRESHOLD)
    {
    for (i = 0; i < varp->Length; ++i)
      concentration[i] = floatNAN;

    total_concen[probeNum] = disp[probeNum] = dbar[probeNum] = pvol[probeNum] = floatNAN;
    return;
    }
  }

}	/* END SCS200 */

/* -------------------------------------------------------------------- */
void spflwc(DERTBL *varp)
{
  NR_TYPE	flow, psxc, atx, flowc;

  flow	= GetSample(varp, 0);
  psxc	= GetSample(varp, 1);
  atx	= GetSample(varp, 2);

  // Changed from 294.15 C to 298.15 C on 9/13/01 per DCRogers.
  flowc = flow * (StdPress / psxc) * (atx + Kelvin) / 298.15;

  PutSample(varp, flowc);
}

/* -------------------------------------------------------------------- */
void suflwc(DERTBL *varp)
{
  NR_TYPE flow, ups, psxc, atx, flowc;

  flow	= GetSample(varp, 0);
  ups	= GetSample(varp, 1);	// kPa; factor of 10 in calculations converts to hPa
  psxc	= GetSample(varp, 2);
  atx	= GetSample(varp, 3);

  /* In HIPPO-3 the UHSAS flow readout would periodically saturate the A/2 to
   * about 1.5 even though the flow was actually correct.  Hold flow to average
   * controlled value when this happens.  Dave Rogers says average is 0.72623 with
   * std dev of +-0.00023 sccs.  May 17 2010.
   */
//  if (cfg.ProjectName().compare("HIPPO-3") == 0) // Do it all the time, MikeReeves 5/1/2014
  if (flow > 1.0)
    flow = 0.72623;

  /* UHSAS flow algorithm changed to correct error.  09/29/2014, M Reeves.
   * Previously flow was interpreted as a mass flow referenced to standard
   * conditions.  It has been shown to be volumetric flow.  The old
   * correction from standard to ambient pressure and temperature is replaced
   * with a correction from instrument conditions to ambient.  This requires
   * UHSAS internal pressure UPRESS.
   */

  /* Check for valid UPRESS.  UPRESS readout at times fails, and the reported
   * value is too high.  UHSAS operates slightly below ambient pressure, so compare
   * with PSXC.  If UPRESS exceeds ambient pressure, replace it using a
   * parameterization in PSXC.  For the C-130, this is a linear relationship derived
   * from NOMADSS flight data, and for the GV it is a cubic polynomial derived
   * from CONTRAST flight data.  M Reeves, 09/29/2014.
   */
  if (ups*10.0 > 1.04*psxc)
  {
    if (cfg.Aircraft() == Config::C130)
    {
      ups = 0.1004 * psxc - 2.678;
    }

    if (cfg.Aircraft() == Config::HIAPER)
    {
      static const double a[] = { 7.52558263, 0.00782470, 0.00016456, -0.00000008 };
      ups = a[0] + a[1] * psxc + a[2] * psxc*psxc + a[3] * psxc*psxc*psxc;
    }
  }

  flowc = flow * (ups*10.0 / psxc) * (atx + Kelvin) / 305.0;

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

  for (size_t i = concu100_start_bin; i < 100; ++i)
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

  for (size_t i = concu500_start_bin; i < 100; ++i)
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
