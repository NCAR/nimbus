/*
-------------------------------------------------------------------------
OBJECT NAME:	f300.c

FULL NAME:	Derived computation sfor the PMS1D FSSP-300 probe

ENTRY POINTS:	cf300Init()
		scf300()
		sconc3()
		sdbar3()

STATIC FNS:	none

DESCRIPTION:	

DEPENDANCIES:	AFSSP	- Actual FSSP data
		TAS	- True Air Speed
		ACT	- Activity
		FBMFR	- Beam Fraction

OUTPUT:		CF300	- Concentrations
		CONCF	- Total concentration
		DBARF

NOTES:		Calculations taken from Bulletin 24 dated 1/89.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <raf/pms.h>

static const size_t MAX_F300 = 1;

static size_t FIRST_BIN[MAX_F300], LAST_BIN[MAX_F300], SampleRate[MAX_F300];

static NR_TYPE	total_concen[MAX_F300], dbar[MAX_F300], disp[MAX_F300];
static NR_TYPE	cell_size[MAX_F300][BINS_40+1], pvol[MAX_F300], SAMPLE_AREA[MAX_F300],
		tact[MAX_F300], cell_size2[MAX_F300][BINS_40+1],
		cell_size3[MAX_F300][BINS_40+1], reff2[MAX_F300], reff3[MAX_F300];

// Probe Count.
static size_t nProbes = 0;
extern void setProbeCount(const char * location, int count);

/* -------------------------------------------------------------------- */
void cf300Init(var_base *varp)
{
  size_t	i, probeNum;
  char		*p;
  const char	*serialNumber;

  if (varp->SerialNumber.length() == 0) {
    sprintf(buffer, "f300.c: %s has no serial number, fatal.", varp->name);
    HandleFatalError(buffer);
    }

  if (nProbes == MAX_F300) {
    sprintf(buffer, "f300.c: Exceeded maximum number of F300 probes, change amlib.h.");
    HandleFatalError(buffer);
    }

  p = strrchr(varp->name, '_');
  setProbeCount(p, nProbes++);

  serialNumber = varp->SerialNumber.c_str();
  probeNum = varp->ProbeCount;

  MakeProjectFileName(buffer, PMS_SPEC_FILE);
  InitPMSspecs(buffer);

  if ((p = GetPMSparameter(serialNumber, "FIRST_BIN")) == NULL) {
    sprintf(buffer, "f300: serial number = [%s]: FIRST_BIN not found.", serialNumber);
    HandleFatalError(buffer);
    }
  FIRST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "LAST_BIN")) == NULL) {
    sprintf(buffer, "f300: serial number = [%s]: LAST_BIN not found.", serialNumber);
    HandleFatalError(buffer);
    }
  LAST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "SAMPLE_AREA")) == NULL)
    SAMPLE_AREA[probeNum] = 0.065;	/* Sample Area taken from cal in 1994 */
  else
    SAMPLE_AREA[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "CELL_SIZE")) == NULL)
    {
    /* ADS2 SPP probes mimiced old PMS1D interface and padded a useless
     * 0th bin (so 31 bins instead of 30).  ADS3 will not do this.  PMSspecs
     * files should now have FirstBin of 0 instead of 1.  Re: -1 vs. -0 below.
     */
    char s[32];
    sprintf(s, "CELL_SIZE_%zd", varp->Length - 1);
    if ((p = GetPMSparameter(serialNumber, s)) == NULL) {
      sprintf(buffer, "f300: serial number = [%s]: %s not found.", serialNumber, s);
      HandleFatalError(buffer);
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

}	/* END CF300INIT */

/* -------------------------------------------------------------------- */
void scf300(DERTBL *varp)
{
  size_t	i, probeNum;
  NR_TYPE	*concentration, *dia, *dia2, *dia3;
  NR_TYPE	tas, vol, sampleVolume[BINS_40+1];

  const NR_TYPE * actual = GetVector(varp, 0);
  tas		= GetSampleFor1D(varp, 1);
  probeNum	= varp->ProbeCount;
  dia		= cell_size[probeNum];
  dia2		= cell_size2[probeNum];
  dia3		= cell_size3[probeNum];

  if (tas < 0.0) tas = 0.0;

  if (FeedBack == HIGH_RATE_FEEDBACK)
    {
    if (SampleOffset >= SampleRate[probeNum])
      return;

    concentration = &HighRateData[varp->HRstart +(SampleOffset * varp->Length)];
    tas /= SampleRate[probeNum];
    }
  else
    concentration = &AveragedData[varp->LRstart];


  vol = tas * SAMPLE_AREA[probeNum];

  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    sampleVolume[i] = vol;

#define VOLUME
#define REFF
#define TACT

#include "pms1d_cv"

}	/* END SCF300 */

/* -------------------------------------------------------------------- */
void sconc3(DERTBL *varp)
{
  PutSample(varp, total_concen[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdbar3(DERTBL *varp)
{
  PutSample(varp, dbar[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void spvol3(DERTBL *varp)
{
  PutSample(varp, pvol[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void stcnt3(DERTBL *varp)
{
  PutSample(varp, tact[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sreff3(DERTBL *varp)       /* Effective Radius     */
{
  if (reff2[varp->ProbeCount] > 0.0)
    PutSample(varp, 0.5 * (reff3[varp->ProbeCount] / reff2[varp->ProbeCount]));
  else
    PutSample(varp, 0.0);
}

/* END F300.C */
