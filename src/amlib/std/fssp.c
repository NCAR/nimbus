/*
-------------------------------------------------------------------------
OBJECT NAME:	fssp.c

FULL NAME:	Derived computations for the PMS1D FSSP-100 probe

ENTRY POINTS:	cfsspInit()
		scs100()	DMT F100
		scfssp()	Original F100
		sfact()
		sfbmfr()

STATIC FNS:	none

DESCRIPTION:	

DEPENDANCIES:	AFSSP	- Actual FSSP data
		TAS	- True Air Speed
		ACT	- Activity
		FBMFR	- Beam Fraction

OUTPUT:		CFSSP	- Concentrations
		CONCF	- Total concentration
		DISPF
		DBARF
		PLWCF	- Liquid Water Content
		TCNT	- Total raw counts
		REFFF	- Effective Radius

REFERENCES:	pms1d.c

REFERENCED BY:	Compute()

NOTES:		Calculations taken from Bulletin 24 dated 1/89.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2003
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <raf/pms.h>

static const size_t MAX_FSSP = 4;

static size_t	FIRST_BIN[MAX_FSSP], LAST_BIN[MAX_FSSP], SampleRate[MAX_FSSP];
static double	PLWFAC[MAX_FSSP], DBZFAC[MAX_FSSP];
static NR_TYPE	sa[MAX_FSSP], tau1[MAX_FSSP], tau2[MAX_FSSP], vol[MAX_FSSP],
		fssp_csiz[MAX_FSSP][BINS_40*4+4], DENS[MAX_FSSP],
		tact[MAX_FSSP], fssp_csiz2[MAX_FSSP][BINS_40*4+4],
		fssp_csiz3[MAX_FSSP][BINS_40*4+4];

static NR_TYPE	total_concen[MAX_FSSP], dbar[MAX_FSSP], plwc[MAX_FSSP],
		disp[MAX_FSSP], dbz[MAX_FSSP], reff2[MAX_FSSP], reff3[MAX_FSSP];

NR_TYPE		refff3[MAX_FSSP], refff2[MAX_FSSP];  /* For export to reff.c */

// Probe Count.
static size_t nProbes = 0;
extern void setProbeCount(const char * location, int count);

/* -------------------------------------------------------------------- */
void cfsspInit(var_base *varp)
{
  size_t	i, probeNum;
  char		*p;
  const char	*serialNumber;
  NR_TYPE	DOF, beamDiameter;

  if (varp->SerialNumber.length() == 0) {
    sprintf(buffer, "fssp.c: %s has no serial number, fatal.", varp->name);
    HandleFatalError(buffer);
    }

  if (nProbes == MAX_FSSP) {
    sprintf(buffer, "fssp.c: Exceeded maximum number of FSSP probes, change amlib.h.");
    HandleFatalError(buffer);
    }

  p = strrchr(varp->name, '_');
  setProbeCount(p, nProbes++);

  serialNumber = varp->SerialNumber.c_str();
  probeNum = varp->ProbeCount;

  for (i = 0; i < MAX_FSSP; ++i)
    refff3[i] = refff2[i] = 0.0;

  MakeProjectFileName(buffer, PMS_SPEC_FILE);
  InitPMSspecs(buffer);

  if ((p = GetPMSparameter(serialNumber, "FIRST_BIN")) == NULL) {
    sprintf(buffer, "fssp: serial number = [%s]: FIRST_BIN not found.", serialNumber);
    HandleFatalError(buffer);
    }
  FIRST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "LAST_BIN")) == NULL) {
    sprintf(buffer, "fssp: serial number = [%s]: LAST_BIN not found.", serialNumber);
    HandleFatalError(buffer);
    }
  LAST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "DOF")) == NULL) {
    sprintf(buffer, "fssp: serial number = [%s]: DOF not found.", serialNumber);
    HandleFatalError(buffer);
    }
  DOF = atof(p);

  if ((p = GetPMSparameter(serialNumber, "BEAM_DIAM")) == NULL) {
    sprintf(buffer, "fssp: serial number = [%s]: BEAM_DIAM not found.", serialNumber);
    HandleFatalError(buffer);
    }
  beamDiameter = atof(p);

  if ((p = GetPMSparameter(serialNumber, "DENS")) == NULL) {
    sprintf(buffer, "fssp: serial number = [%s]: DENS not found.", serialNumber);
    HandleFatalError(buffer);
    }
  DENS[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "PLWFAC")) == NULL) {
    sprintf(buffer, "fssp: serial number = [%s]: PLWFAC not found.", serialNumber);
    HandleFatalError(buffer);
    }
  PLWFAC[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "DBZFAC")) == NULL) {
    sprintf(buffer, "fssp: serial number = [%s]: DBZFAC not found.", serialNumber);
    HandleFatalError(buffer);
    }
  DBZFAC[probeNum] = atof(p);

  // Don't acquire for SPP100 & CDP, FSSP100 only.
  if (varp->name[1] == 'F')
    {
    if ((p = GetPMSparameter(serialNumber, "TAU1")) == NULL) {
      sprintf(buffer, "fssp: serial number = [%s]: TAU1 not found.", serialNumber);
      HandleFatalError(buffer);
      }
    tau1[probeNum] = atof(p);

    if ((p = GetPMSparameter(serialNumber, "TAU2")) == NULL) {
      sprintf(buffer, "fssp: serial number = [%s]: TAU2 not found.", serialNumber);
      HandleFatalError(buffer);
      }
    tau2[probeNum] = atof(p);
    }


  if ((p = GetPMSparameter(serialNumber, "CELL_SIZE")) == NULL)
    {
    /* ADS2 SPP probes mimiced old PMS1D interface and padded a useless
     * 0th bin (so 31 bins instead of 30).  ADS3 will not do this.  PMSspecs
     * files should now have FirstBin of 0 instead of 1.  Re: -1 vs. -0 below.
     */
    char s[32];
    sprintf(s, "CELL_SIZE_%d", varp->Length - 1);
    if ((p = GetPMSparameter(serialNumber, s)) == NULL) {
      sprintf(buffer, "fssp: serial number = [%s]: %s not found.", serialNumber, s);
      }
    }

  strcpy(buffer, p);
  p = strtok(buffer, ", \t\n");

  for (i = 0; i < varp->Length*4; ++i)	/* 4 "ranges"	*/
    {       
    fssp_csiz[probeNum][i] = p ? atof(p) : 0.0;
    p = strtok(NULL, ", \t\n");
    }       

  for (i = varp->Length*4-1; i > 0; --i)
    {
    fssp_csiz[probeNum][i] =
        (fssp_csiz[probeNum][i] + fssp_csiz[probeNum][i-1]) / 2;

    fssp_csiz2[probeNum][i] = fssp_csiz[probeNum][i] * fssp_csiz[probeNum][i];
    fssp_csiz3[probeNum][i] = fssp_csiz2[probeNum][i] * fssp_csiz[probeNum][i];
    }
 
  ReleasePMSspecs();

  SampleRate[probeNum] = varp->SampleRate;
  sa[probeNum] = DOF * beamDiameter;

  if (strncmp(varp->name, "AS100", 5) == 0 &&
      varp->SerialNumber.compare("FSSP128") == 0)
  {
    char msg[200];
    sprintf(msg, "fssp.c: <<< WARNING >>> %s: Serial # %s, TAS will be altered as follows:\n  tas = -0.0466 + 0.95171 * tas;", varp->name, varp->SerialNumber.c_str());
    LogMessage(msg);
  }

}	/* END CFSSPINIT */

/* -------------------------------------------------------------------- */
void scfssp(DERTBL *varp)
{
  size_t	i, probeNum;
  NR_TYPE	*concentration, *dia, *dia2, *dia3;
  NR_TYPE	tas;		/* True Air Speed	*/
  NR_TYPE	activity;	/* Activity		*/
  NR_TYPE	fbmfr;		/* Beam Fraction	*/
  int		frange;
  NR_TYPE	sampleVolume[BINS_16];

  const NR_TYPE * actual = GetVector(varp, 0);
  tas		= GetSampleFor1D(varp, 1);
  activity	= GetSample(varp, 2);
  fbmfr		= GetSample(varp, 3);
  frange	= (int)GetSample(varp, 4);
  probeNum	= varp->ProbeCount;

  if (tas < 0.0) tas = 0.0;

  if (FeedBack == HIGH_RATE_FEEDBACK)
    {
    if (SampleOffset >= SampleRate[probeNum])
      return;

    concentration = &HighRateData[varp->HRstart +(SampleOffset * varp->Length)];
    tas /= SampleRate[probeNum];
    activity *= SampleRate[probeNum];
    }
  else
    {
    concentration = &AveragedData[varp->LRstart];
    }

  vol[probeNum] = tas * sa[probeNum] * fbmfr * (1.0 - (activity * 0.71));

  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    sampleVolume[i] = vol[probeNum];

  dia = &fssp_csiz[probeNum][frange * varp->Length];
  dia2 = &fssp_csiz2[probeNum][frange * varp->Length];
  dia3 = &fssp_csiz3[probeNum][frange * varp->Length];

#define PLWC
#define DBZ
#define TACT
#define REFF

#include "pms1d_cv"

#undef PLWC
#undef DBZ
#undef TACT
#undef REFF

}	/* END SCFSSP */

/* -------------------------------------------------------------------- */
void scs100(DERTBL *varp)
{
  size_t	i, probeNum;
  NR_TYPE       *concentration, *dia, *dia2, *dia3;
  NR_TYPE       tas;		/* True Air Speed		*/
  NR_TYPE       rejAT;		/* Rejected, Avg Transit	*/
  NR_TYPE       oflow;		/* Lost counts to overflow	*/
  int	       frange;
  NR_TYPE       sampleVolume[BINS_40+1];

  const NR_TYPE * actual = GetVector(varp, 0);
  tas		= GetSampleFor1D(varp, 1);
  rejAT		= GetSample(varp, 2);
  oflow		= GetSample(varp, 3);
  frange	= (int)GetSample(varp, 4);
  probeNum	= varp->ProbeCount;

  if (tas < 0.0) tas = 0.0;

  if (varp->SerialNumber.compare("FSSP128") == 0)
    tas = -0.0466 + 0.95171 * tas;

  if (FeedBack == HIGH_RATE_FEEDBACK)
    {
    if (SampleOffset >= SampleRate[probeNum])
      return;

    concentration = &HighRateData[varp->HRstart + (SampleOffset * varp->Length)];
    tas /= SampleRate[probeNum];
    }
  else
    {
    concentration = &AveragedData[varp->LRstart];
    }

  tact[probeNum] = 0.0;
  for (i = 1; i < varp->Length; ++i)
    tact[probeNum] += actual[i];

  vol[probeNum] = tas * sa[probeNum];

  if (rejAT > 0 && tact[probeNum] > 0)
    vol[probeNum] *= (float)(tact[probeNum]) / (float)(tact[probeNum] + rejAT);

  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    sampleVolume[i] = vol[probeNum];

  if (frange < 0 || frange > 3)
    frange = 0;
  dia = &fssp_csiz[probeNum][frange * varp->Length];
  dia2 = &fssp_csiz2[probeNum][frange * varp->Length];
  dia3 = &fssp_csiz3[probeNum][frange * varp->Length];

#define PLWC
#define DBZ
#define REFF

#include "pms1d_cv"

  if (oflow > 0 && tact[probeNum] > 0 && oflow < 5000)
    {
    NR_TYPE ccc = (tact[probeNum]+oflow) / tact[probeNum];

    for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
      concentration[i] *= ccc;

    total_concen[probeNum] *= ccc;
    }

}       /* END SCS100 */

/* -------------------------------------------------------------------- */
void sfact(DERTBL *varp)
{
  NR_TYPE	fstrob, freset;

  /* This function is for the old interface card only.  Activity is
   * a housekeeping param on the new interface card.
   */
  if (FeedBack == HIGH_RATE_FEEDBACK && SampleOffset >= varp->OutputRate)
    return;

  freset = GetSample(varp, 0);
  fstrob = GetSample(varp, 1);

  PutSample(varp, (fstrob * tau1[varp->ProbeCount] +
                   freset * tau2[varp->ProbeCount]));

}	/* END SFACT */

/* -------------------------------------------------------------------- */
void sfbmfr(DERTBL *varp)
{
  size_t	i;
  NR_TYPE	*afssp;
  NR_TYPE	fstrob, fbmfr = 0.0, total_actual = 0.0;

  static NR_TYPE prev_fbmfr = 0.4;

  if (FeedBack == HIGH_RATE_FEEDBACK && SampleOffset >= varp->OutputRate)
    return;

  afssp  = GetVector(varp, 0);
  fstrob = GetSample(varp, 1);

  for (i = FIRST_BIN[varp->ProbeCount]; i < LAST_BIN[varp->ProbeCount]; ++i)
    total_actual += afssp[i];

  if (fstrob != 0.0)
    fbmfr = total_actual / fstrob;

  if (fstrob < 1.0 || total_actual <= 0.0)
    fbmfr = 0.4;

  if (fbmfr < .2 || fbmfr > 1.0) /* If it's out of range, repeat previous */
    fbmfr = prev_fbmfr;

  if (fstrob < 0.0)
    fbmfr = prev_fbmfr;

  prev_fbmfr = fbmfr;

  PutSample(varp, fbmfr);

}	/* END SFBMFR */

/* -------------------------------------------------------------------- */
void sconcf(DERTBL *varp)	/* Concentration	*/
{
  PutSample(varp, total_concen[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdispf(DERTBL *varp)
{
  PutSample(varp, disp[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void splwcf(DERTBL *varp)	/* Liquid Water		*/
{
  PutSample(varp, plwc[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdbzf(DERTBL *varp)
{
  PutSample(varp, dbz[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdbarf(DERTBL *varp)
{
  PutSample(varp, dbar[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void ssvolf(DERTBL *varp)	/* Sample Volume (not what the volume from PCAS) */
{
  PutSample(varp, vol[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void stcntf(DERTBL *varp)	/* Total raw counts	*/
{
  PutSample(varp, tact[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void srefff(DERTBL *varp)	/* Effective Radius	*/
{
  refff3[varp->ProbeCount] = reff3[varp->ProbeCount]; /* Export to reff.c */
  refff2[varp->ProbeCount] = reff2[varp->ProbeCount];

  if (reff2[varp->ProbeCount] > 0.0)
    PutSample(varp, 0.5 * (reff3[varp->ProbeCount] / reff2[varp->ProbeCount]));
  else
    PutSample(varp, 0.0);
}

/* END FSSP.C */
