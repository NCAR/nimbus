/*
-------------------------------------------------------------------------
OBJECT NAME:	pcasp.c

FULL NAME:	Compute PMS1D ASAS/PCASP/SPP200 derived paramters

DESCRIPTION:	

NOTES:		Calculations taken from Bulletin 24 dated 1/89.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2018
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <raf/pms.h>

static const size_t MAX_ASAS = 2;
static const int MAX_BINS = 40;

// Changed from 294.15 C to 298.15 C on 9/13/01 per DCRogers.
static NR_TYPE StdTemperature = 298.15;

static size_t FIRST_BIN[MAX_ASAS], LAST_BIN[MAX_ASAS], SampleRate[MAX_ASAS];

static NR_TYPE	total_concen[MAX_ASAS], disp[MAX_ASAS], dbar[MAX_ASAS];
static NR_TYPE	pvol[MAX_ASAS], tact[MAX_ASAS];
static NR_TYPE	cell_size[MAX_ASAS][MAX_BINS];
static NR_TYPE	cell_size2[MAX_ASAS][MAX_BINS];
static NR_TYPE	cell_size3[MAX_ASAS][MAX_BINS];
static NR_TYPE	dt[MAX_ASAS];	// Average Particle Pulse Width - Coincidence corrrection

// Probe Count.
static size_t nProbes = 0;
extern void setProbeCount(const char * location, int count);

void	ReadPMSspecs(const char fileName[]);

/* -------------------------------------------------------------------- */
void casasInit(var_base *varp)
{
  size_t	i, probeNum;
  const char	*serialNumber;
  char		*p;

  if (varp->SerialNumber.length() == 0) {
    sprintf(buffer, "pcasp.c: PCASP %s has no serial number, fatal.", varp->name);
    HandleFatalError(buffer);
    }

  if (nProbes == MAX_ASAS) {
    sprintf(buffer, "pcasp.c: Exceeded maximum number of ASAS/PCASP probes, change amlib.h.");
    HandleFatalError(buffer);
    }

  p = strrchr(varp->name, '_');
  setProbeCount(p, nProbes++);

  serialNumber = varp->SerialNumber.c_str();
  probeNum = varp->ProbeCount;

  MakeProjectFileName(buffer, PMS_SPEC_FILE);
  ReadPMSspecs(buffer);

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

  // Flow cals and std temp changed in 2024 starting with CAESAR.
  if ((p = GetPMSparameter(serialNumber, "STD_TEMP")) != NULL)
    StdTemperature = atof(p);

  if ((p = GetPMSparameter(serialNumber, "PULSE_WIDTH")) == NULL)
    dt[probeNum] = 0.0;
  else
    dt[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "CELL_SIZE")) == NULL)
    {
    sprintf(buffer, "CELL_SIZE_%zu", varp->Length);
    if ((p = GetPMSparameter(serialNumber, buffer)) == NULL) {
      char msg[128];
      sprintf(msg, "pcasp: serial number = [%s]: %s not found.", serialNumber, buffer);
      HandleFatalError(msg);
      }
    }

  strcpy(buffer, p);
  p = strtok(buffer, ", \t\n");

  for (i = 0; i < varp->Length+1; ++i)
    {
    cell_size[probeNum][i] = p ? atof(p) : 0.0;
    p = strtok(NULL, ", \t\n");
    }

  for (i = 0; i < varp->Length; ++i)
    {
    // Compute mid-point.
    cell_size[probeNum][i] =
	(cell_size[probeNum][i] + cell_size[probeNum][i+1]) / 2;

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

  /* Following dummy tas added to keep system from complaining about
   * the code added to "pms1d_cv".  Ron Ruth 19 October 2001 
   */
  tas = 50.0;

#include "pms1d_cv"

}       /* END SCASAS */

/* -------------------------------------------------------------------- */
void scs200(DERTBL *varp)	// DMT Modified SPP200
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

  /* Following dummy tas added to keep system from complaining about
   * the code added to "pms1d_cv". Ron Ruth 19 October 2001.
   */
  tas = 50.0;

#include "pms1d_cv"

  // Apply coincidence correction.
  if (dt[probeNum] > 0.0)
  {
    NR_TYPE N = tact[probeNum];
    if (FeedBack == HIGH_RATE_FEEDBACK)
      N *= SampleRate[probeNum];

    NR_TYPE ccf = 1.0 / (1.0 - (dt[probeNum] * N));

    if (ccf > 0.0)
    {
      for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
        concentration[i] *= ccf;

      total_concen[probeNum] *= ccf;
#ifdef VOLUME
      pvol[probeNum] *= ccf;
#endif
    }
  }

}	/* END SCS200 */

/* -------------------------------------------------------------------- */
void spflwc(DERTBL *varp)	// PCASP / SPP-200 corrected flow.
{
  NR_TYPE	flow, psxc, atx, flowc;

  flow	= GetSample(varp, 0);
  psxc	= GetSample(varp, 1);
  atx	= GetSample(varp, 2);

  if (flow < 0.0)	// basic check.
    flow = 0.0;

  flowc = flow * (StdPress / psxc) * (atx + Kelvin) / StdTemperature;

  PutSample(varp, flowc);
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

/* END PCASP.C */
