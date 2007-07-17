/*
-------------------------------------------------------------------------
OBJECT NAME:	masp.c

FULL NAME:	Compute MASP derived paramters

ENTRY POINTS:	maspInit()
		scmasp()

STATIC FNS:	none

DESCRIPTION:	

DEPENDANCIES:	none

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	Compute()

COPYRIGHT:	University Corporation for Atmospheric Research, 1998
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <raf/pms.h>

static const size_t	MAX_MASP = 2;

static size_t FIRST_BIN[MAX_MASP], LAST_BIN[MAX_MASP], SampleRate[MAX_MASP];

static NR_TYPE	total_concen[MAX_MASP], dbar[MAX_MASP], pvol[MAX_MASP];
static NR_TYPE	cell_size[MAX_MASP][BINS_32], disp[MAX_MASP],
		cell_size3[MAX_MASP][BINS_32], cell_size2[MAX_MASP][BINS_32];


/* -------------------------------------------------------------------- */
void maspInit(var_base *varp)
{
  size_t   i, probeNum;
  char  *p, *serialNumber;
 
//  serialNumber = varp->SerialNumber;
serialNumber = "MASP_DEF";
  probeNum = varp->ProbeCount;
 
  sprintf(buffer, PMS_SPEC_FILE, cfg.ProjectDirectory().c_str(), cfg.ProjectNumber().c_str());
  InitPMSspecs(buffer);
 
  if ((p = GetPMSparameter(serialNumber, "FIRST_BIN")) == NULL) {
    printf("%s: FIRST_BIN not found.\n", serialNumber); exit(1);
    }
  FIRST_BIN[probeNum] = atoi(p);
 
  if ((p = GetPMSparameter(serialNumber, "LAST_BIN")) == NULL) {
    printf("%s: LAST_BIN not found.\n", serialNumber); exit(1);
    }
  LAST_BIN[probeNum] = atoi(p);
 
  if ((p = GetPMSparameter(serialNumber, "CELL_SIZE")) == NULL) {
    printf("%s: CELL_SIZE not found.\n", serialNumber); exit(1);
    }
 
  strcpy(buffer, p);
  p = strtok(buffer, ", \t\n");
 
  for (i = 0; i < BINS_32; ++i)
    {
    cell_size[probeNum][i] = p ? atof(p) : 0.0;
    p = strtok(NULL, ", \t\n");
    }
 
  for (i = BINS_32-1; i > 0; --i)
    {
    cell_size[probeNum][i] =
        (cell_size[probeNum][i] + cell_size[probeNum][i-1]) / 2;

    cell_size2[probeNum][i] = cell_size[probeNum][i] * cell_size[probeNum][i];
    cell_size3[probeNum][i] = cell_size2[probeNum][i] * cell_size[probeNum][i];
    }

  ReleasePMSspecs();
 
  SampleRate[probeNum] = varp->SampleRate;

}	/* END MASPINIT */

/* -------------------------------------------------------------------- */
void scmasp(DERTBL *varp)
{
  size_t	i, probeNum;
  NR_TYPE	*actual, *concentration, *dia, *dia2, *dia3;
  NR_TYPE	tas, vol, sampleVolume[BINS_32];

  actual	= GetVector(varp, 0, BINS_32);
  tas		= GetSample(varp, 1);
  probeNum	= varp->ProbeCount;
  dia		= cell_size[probeNum];
  dia2		= cell_size2[probeNum];
  dia3		= cell_size3[probeNum];

  if (FeedBack == HIGH_RATE_FEEDBACK)
    {
    if (SampleOffset >= SampleRate[probeNum])
      return;
 
    concentration = &HighRateData[varp->HRstart +(SampleOffset * varp->Length)];
    tas /= SampleRate[probeNum];
    }
  else
    concentration = &AveragedData[varp->LRstart];

  vol = tas * 0.05;
 
  for (i = FIRST_BIN[probeNum]; i < LAST_BIN[probeNum]; ++i)
    sampleVolume[i] = vol;

#define VOLUME

#include "../std/pms1d_cv"

}	/* END SCMASP */

/* -------------------------------------------------------------------- */
void sconcm(DERTBL *varp)
{
  PutSample(varp, total_concen[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sdbarm(DERTBL *varp)
{
  PutSample(varp, dbar[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void spvolm(DERTBL *varp)
{
  PutSample(varp, pvol[varp->ProbeCount]);
}

/* END MASP.C */
