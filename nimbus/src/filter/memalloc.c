/*
-------------------------------------------------------------------------
OBJECT NAME:	memalloc.c

FULL NAME:	Memory Allocation

ENTRY POINTS:	AllocateDataArrays()
		FreeDataArrays()

STATIC FNS:	none

DESCRIPTION:	Allocate storage for pointers & data array.  This is
		analogous to shmem.c for winput.c

INPUT:		none

OUTPUT:		none

REFERENCES:	none

REFERENCED BY:	cb_main.c
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "ctape.h"

static bool	ArraysInitialized = false;

extern char	*ADSrecord, *AVAPSrecord[];
extern ushort	*bits;
extern NR_TYPE	*volts;
extern NR_TYPE	*SampledData, *AveragedData, *HighRateData;


/* -------------------------------------------------------------------- */
void AllocateDataArrays()
{
  int	i, j, nLRfloats, nSRfloats, nHRfloats;
  long	lrlen;

  get_lrlen(&lrlen);
  ADSrecord = (char *)GetMemory((unsigned)lrlen);


  if (AVAPS)
    for (i = 0; i < MAX_AVAPS; ++i)
      AVAPSrecord[i] = (char *)GetMemory(256);


  nFloats = 0;
  nLRfloats = nSRfloats = nHRfloats = 0;

  for (i = 0; i < nsdi; ++i)
    {
    sdi[i]->LRstart = nLRfloats++;
    sdi[i]->SRstart = nFloats;
    sdi[i]->HRstart = nHRfloats;
    nFloats += sdi[i]->SampleRate;
    nHRfloats += 25;
    }

  for (i = 0; i < nraw; ++i)
    {
    raw[i]->LRstart = nLRfloats;
    raw[i]->SRstart = nFloats;
    raw[i]->HRstart = nHRfloats;
    nLRfloats += raw[i]->Length;
    nFloats += (raw[i]->SampleRate * raw[i]->Length);
    nHRfloats += (25 * raw[i]->Length);
    }

  for (i = 0; i < nderive; ++i)
    {
    derived[i]->LRstart = nLRfloats;
    derived[i]->HRstart = nHRfloats;
    nLRfloats += derived[i]->Length;
    nHRfloats += (25 * derived[i]->Length);
    }

  /* Reset dependIndices.
   */
  for (i = 0; i < nderive; ++i)
    for (j = 0; j < derived[i]->ndep; ++j)
      DependIndexLookup(derived[i], j);

  bits = (ushort *)GetMemory((unsigned)sizeof(ushort) * nsdi);
  volts = (NR_TYPE *)GetMemory((unsigned)sizeof(NR_TYPE) * nsdi);
  SampledData = (NR_TYPE *)GetMemory((unsigned)sizeof(NR_TYPE) * nFloats);
  AveragedData = (NR_TYPE *)GetMemory((unsigned)sizeof(NR_TYPE) * nLRfloats);

  if (ProcessingRate == HIGH_RATE)
    HighRateData =(NR_TYPE *)GetMemory((unsigned)sizeof(NR_TYPE)*nHRfloats);

  ArraysInitialized = true;

}	/* END ALLOCATEDATAARRAYS */

/* -------------------------------------------------------------------- */
void FreeDataArrays()
{
  if (ArraysInitialized)
    {
    free((char *)bits);
    free((char *)volts);
    free((char *)ADSrecord);
    free((char *)SampledData);
    free((char *)AveragedData);

    if (ProcessingRate == HIGH_RATE)
      free((char *)HighRateData);
    }

  ArraysInitialized = false;

}	/* END FREEDATAARRAYS */

/* END MEMALLOC.C */
