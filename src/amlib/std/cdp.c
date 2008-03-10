/*
-------------------------------------------------------------------------
OBJECT NAME:	cdp.c

FULL NAME:	Derived computations for the PMS1D FSSP-100 probe

ENTRY POINTS:	ccdpInit()
		scdp()	DMT F100

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

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2007
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <raf/pms.h>

static size_t	FIRST_BIN[MAX_FSSP], LAST_BIN[MAX_FSSP], SampleRate[MAX_FSSP];
static double	PLWFAC[MAX_FSSP], DBZFAC[MAX_FSSP];
static NR_TYPE	SAMPLE_AREA[MAX_FSSP], vol[MAX_FSSP],
		cell_size[MAX_FSSP][BINS_40+1], DENS[MAX_FSSP],
		tact[MAX_FSSP], cell_size2[MAX_FSSP][BINS_40+1],
		cell_size3[MAX_FSSP][BINS_40+1];

static NR_TYPE	total_concen[MAX_FSSP], dbar[MAX_FSSP], plwc[MAX_FSSP],
		disp[MAX_FSSP], dbz[MAX_FSSP], reff2[MAX_FSSP], reff3[MAX_FSSP];

NR_TYPE		reffd3[MAX_FSSP], reffd2[MAX_FSSP];  /* For export to reff.c */

/* -------------------------------------------------------------------- */
void ccdpInit(var_base *varp)
{
  size_t	i, probeNum;
  char		*p;
  const char	*serialNumber;
  NR_TYPE	dof, beam_dia;

  serialNumber = varp->SerialNumber.c_str();
  probeNum = varp->ProbeCount;

  for (i = 0; i < MAX_FSSP; ++i)
    reffd3[i] = reffd2[i] = 0.0;

  MakeProjectFileName(buffer, PMS_SPEC_FILE);
  InitPMSspecs(buffer);

  if ((p = GetPMSparameter(serialNumber, "FIRST_BIN")) == NULL) {
    fprintf(stderr, "cdp: serial number = [%s]: FIRST_BIN not found.\n", serialNumber); exit(1);
    }
  FIRST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "LAST_BIN")) == NULL) {
    fprintf(stderr, "cdp: serial number = [%s]: LAST_BIN not found.\n", serialNumber); exit(1);
    }
  LAST_BIN[probeNum] = atoi(p);

  if ((p = GetPMSparameter(serialNumber, "DOF")) == NULL) {
    fprintf(stderr, "cdp: serial number = [%s]: DOF not found.\n", serialNumber); exit(1);
  }
  dof = atof(p);

  if ((p = GetPMSparameter(serialNumber, "BEAM_DIAM")) == NULL) {
    fprintf(stderr, "cdp: serial number = [%s]: BEAM_DIAM not found.\n", serialNumber); exit(1);
  }
  beam_dia = atof(p);

  SAMPLE_AREA[probeNum] = dof * beam_dia;


  if ((p = GetPMSparameter(serialNumber, "DENS")) == NULL) {
    fprintf(stderr, "cdp: serial number = [%s]: DENS not found.\n", serialNumber); exit(1);
  }
  DENS[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "PLWFAC")) == NULL) {
    fprintf(stderr, "cdp: serial number = [%s]: PLWFAC not found.\n", serialNumber); exit(1);
  }
  PLWFAC[probeNum] = atof(p);

  if ((p = GetPMSparameter(serialNumber, "DBZFAC")) == NULL) {
    fprintf(stderr, "cdp: serial number = [%s]: DBZFAC not found.\n", serialNumber); exit(1);
  }
  DBZFAC[probeNum] = atof(p);


  if ((p = GetPMSparameter(serialNumber, "CELL_SIZE")) == NULL)
  {
    /* ADS2 SPP probes mimiced old PMS1D interface and padded a useless
     * 0th bin (so 31 bins instead of 30).  ADS3 will not do this.  PMSspecs
     * files should now have FirstBin of 0 instead of 1.  Re: -1 vs. -0 below.
     */
    sprintf(buffer, "CELL_SIZE_%d", varp->Length - 1);
    if ((p = GetPMSparameter(serialNumber, buffer)) == NULL) {
      fprintf(stderr, "cdp: serial number = [%s]: %s not found.\n", serialNumber, buffer); exit(1);
    }
  }

  strcpy(buffer, p);
  p = strtok(buffer, ", \t\n");

  for (i = 0; i < varp->Length; ++i)	/* 4 "ranges"	*/
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

}	/* END CCDPINIT */

/* -------------------------------------------------------------------- */
void scdp(DERTBL *varp)
{
  size_t	i, probeNum;
  NR_TYPE       *actual, *concentration, *dia, *dia2, *dia3;
  NR_TYPE       tas;		/* True Air Speed		*/
  NR_TYPE       sampleVolume[BINS_40+1];

  actual	= GetVector(varp, 0, varp->Length);
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

    concentration = &HighRateData[varp->HRstart + (SampleOffset * varp->Length)];
    tas /= SampleRate[probeNum];
  }
  else
  {
    concentration = &AveragedData[varp->LRstart];
  }

  tact[probeNum] = 0.0;
  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    tact[probeNum] += actual[i];

  vol[probeNum] = tas * SAMPLE_AREA[probeNum];

  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    sampleVolume[i] = vol[probeNum];

#define PLWC
#define DBZ
#define REFF
#define TACT

#include "pms1d_cv"

}       /* END SCDP */

/* -------------------------------------------------------------------- */
void sconcd(DERTBL *varp)	/* Concentration	*/
{
  PutSample(varp, total_concen[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdispd(DERTBL *varp)
{
  PutSample(varp, disp[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void splwcd(DERTBL *varp)	/* Liquid Water		*/
{
  PutSample(varp, plwc[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdbzd(DERTBL *varp)
{
  PutSample(varp, dbz[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdbard(DERTBL *varp)
{
  PutSample(varp, dbar[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void ssvold(DERTBL *varp)	/* Sample Volume (not what the volume from PCAS) */
{
  PutSample(varp, vol[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void stcntd(DERTBL *varp)	/* Total raw counts	*/
{
  PutSample(varp, tact[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sreffd(DERTBL *varp)	/* Effective Radius	*/
{
  reffd3[varp->ProbeCount] = reff3[varp->ProbeCount]; /* Export to reff.c */
  reffd2[varp->ProbeCount] = reff2[varp->ProbeCount];

  if (reff2[varp->ProbeCount] > 0.0)
    PutSample(varp, 0.5 * (reff3[varp->ProbeCount] / reff2[varp->ProbeCount]));
  else
    PutSample(varp, 0.0);
}

/* END CDP.C */
