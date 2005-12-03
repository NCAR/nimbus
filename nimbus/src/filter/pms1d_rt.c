/*
-------------------------------------------------------------------------
OBJECT NAME:	pms1d_rt.c

FULL NAME:	Real Time PMS1D routines

ENTRY POINTS:	WritePMSdataToSHMEM()	winput.c
		AddToPMS1DprobeList()	hdr_decode.c (see also fake1d.c)
		SetUpPMS1D_SHMEM()	winput.c
		SetPMS1DnetCDF()	winput.c (for post-processing)

STATIC FNS:	none

DESCRIPTION:	Shared memory interface with WINDS, for PMS1D probes.

REFERENCED BY:	winput.c

NOTE:		This file has "FRNG" hard-wired into it, and also the
		names of the various PMS1D probes (e.g. "200X", "FSSP")

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "/jnet/local/winds/include/pms1d.h"

#define RANGE_NAME	"FRNG"

static char	*probe_name[MAX1DPROBES];
static char	*serialNums[MAX1DPROBES];
static int	probe_type[MAX1DPROBES];	/* Used for switch case	*/
static int	pcnt = 0;
static RAWTBL	*actual[MAX1DPROBES];
static RAWTBL	*range[MAX1DPROBES];		/* FSSP only		*/
static DERTBL	*conc[MAX1DPROBES];


/* -------------------------------------------------------------------- */
void WritePMS1DdataToSHMEM(NR_TYPE data[])
{
  int	i;

  for (i = 0; i < NumberPMS1Dprobes(); ++i)
    {
    memcpy( (char *)PMS1Dprobe[i].actual,
            (char *)&data[actual[i]->LRstart],
            sizeof(float) * actual[i]->Length);

    memcpy( (char *)PMS1Dprobe[i].conc,
            (char *)&data[conc[i]->LRstart],
            sizeof(float) * conc[i]->Length);

/*  if (PMS1Dprobe[i].type == FSSP)
      PMS1Dprobe[i].range = data[range[i]->LRstart];
*/  }

}	/* END WRITEPMS1DDATATOSHMEM */

/* -------------------------------------------------------------------- */
void SetUpPMS1D_SHMEM()
{
  int	indx;
  char	target[NAMELEN];

  for (int i = 0; i < pcnt; ++i)
    {
    strcpy((char *)PMS1Dprobe[i].name, probe_name[i]);
    PMS1Dprobe[i].SerialNumber = serialNums[i];

    strcpy(target, "A");
    strcat(target, probe_name[i]);
    indx = SearchTable(raw, target);
    actual[i] = raw[indx];

    target[0] = 'C';
    indx = SearchTable(derived, target);
    conc[i] = derived[indx];

    PMS1Dprobe[i].type	= probe_type[i];
    PMS1Dprobe[i].nbins	= actual[i]->Length;

    if (PMS1Dprobe[i].type == FSSP)
      {
      strcpy(target, RANGE_NAME);
      strcat(target, strchr((char *)PMS1Dprobe[i].name, '_'));
      strcpy((char *)PMS1Dprobe[i].rangeName, target);

/*    range[i] = raw[SearchTable(raw, target)]; */
      }
    else
      range[i] = NULL;

    delete [] probe_name[i];
    delete [] serialNums[i];
    }

  *nPMS1Dprobes = pcnt;

}	/* END SETUPPMS1D_SHMEM */

/* -------------------------------------------------------------------- */
void AddToPMS1DprobeList(char probe[], char location[], char serial_num[], int type)
{
  strcpy((probe_name[pcnt] = new char[NAMELEN]), probe);
  strcat(probe_name[pcnt], location);
  strcpy((serialNums[pcnt] = new char[NAMELEN]), serial_num);
  probe_type[pcnt] = type;
  probe_name[++pcnt] = NULL;

}	/* END ADDTOPMS1DPROBELIST */

extern "C" {
/* -------------------------------------------------------------------- */
float *SetPMS1DnetCDF(char name[], int length)
{
  int	indx;

  for (indx = 0; indx < pcnt; ++indx)
    if (strcmp(PMS1Dprobe[indx].name, &name[1]) == 0)
      break;


  /* It's not in the list yet, add it.
   */
  if (indx == pcnt)
    {
    strcpy((char *)PMS1Dprobe[pcnt].name, &name[1]);
    PMS1Dprobe[pcnt].range	= 0;
    PMS1Dprobe[pcnt].nbins	= length;

    if (memcmp(&name[1], "FSSP", 4) == 0)
      {
      PMS1Dprobe[pcnt].type = FSSP;
      PMS1Dprobe[pcnt].SerialNumber = "FSSP_DEF";
      strcpy(PMS1Dprobe[pcnt].rangeName, RANGE_NAME);
      strcat(PMS1Dprobe[pcnt].rangeName, strchr(name, '_'));
      }
    else
    if (memcmp(&name[1], "F300", 4) == 0)
      {
      PMS1Dprobe[pcnt].SerialNumber = "F300_DEF";
      PMS1Dprobe[pcnt].type = F300;
      }
    else
    if (memcmp(&name[1], "200X", 4) == 0)
      {
      PMS1Dprobe[pcnt].SerialNumber = "200X_DEF";
      PMS1Dprobe[pcnt].type = X200;
      }
    else
    if (memcmp(&name[1], "200Y", 4) == 0)
      {
      PMS1Dprobe[pcnt].SerialNumber = "200Y_DEF";
      PMS1Dprobe[pcnt].type = Y200;
      }
    else
    if (memcmp(&name[1], "260X", 4) == 0)
      {
      PMS1Dprobe[pcnt].SerialNumber = "260X_DEF";
      PMS1Dprobe[pcnt].type = X260;
      }
    else
    if (memcmp(&name[1], "MASP", 4) == 0)
      {
      PMS1Dprobe[pcnt].SerialNumber = "MASP_DEF";
      PMS1Dprobe[pcnt].type = MASP;
      }
    else
      {
      PMS1Dprobe[pcnt].SerialNumber = "ASAS_DEF";
      PMS1Dprobe[pcnt].type = ASAS;
      }

    *nPMS1Dprobes = ++pcnt;
    }


  switch (name[0])
    {
    case 'A':
      return((float *)PMS1Dprobe[indx].actual);

    case 'C':
      return((float *)PMS1Dprobe[indx].conc);
    }

  return((float *)NULL);

}	/* END SETPMS1DNETCDF */

}

#include "header.h"

bool Open2dFile(const char file[], int probeCnt)
{
  return(FALSE);
}

bool Next2dRecord(P2d_rec *record, int probeCnt, short id)
{
  return(FALSE);
}

/* END PMS1D.C */
