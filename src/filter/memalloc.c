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
extern NR_TYPE	*SRTvolts, *volts;
extern NR_TYPE	*SampledData, *AveragedData, *HighRateData;


/* -------------------------------------------------------------------- */
void AllocateDataArrays()
{
  int	nLRfloats, nSRfloats, nHRfloats;
  long	lrlen;

  get_lrlen(&lrlen);
  ADSrecord = new char[lrlen];


  if (AVAPS)
    for (int i = 0; i < MAX_AVAPS; ++i)
      AVAPSrecord[i] = new char[256];


  nFloats = 0;
  nLRfloats = nSRfloats = nHRfloats = 0;

  for (int i = 0; i < sdi.size(); ++i)
    {
    sdi[i]->LRstart = nLRfloats++;
    sdi[i]->SRstart = nFloats;
    sdi[i]->HRstart = nHRfloats;
    nFloats += sdi[i]->SampleRate;
    nHRfloats += 25;
    }

  int nVoltFloats = nFloats;

  for (int i = 0; i < raw.size(); ++i)
    {
    raw[i]->LRstart = nLRfloats;
    raw[i]->SRstart = nFloats;
    raw[i]->HRstart = nHRfloats;
    nLRfloats += raw[i]->Length;
    nFloats += (raw[i]->SampleRate * raw[i]->Length);
    nHRfloats += (25 * raw[i]->Length);
    }

  for (int i = 0; i < derived.size(); ++i)
    {
    derived[i]->LRstart = nLRfloats;
    derived[i]->HRstart = nHRfloats;
    nLRfloats += derived[i]->Length;
    nHRfloats += (25 * derived[i]->Length);
    }

  /* Reset dependIndices.
   */
  for (int i = 0; i < derived.size(); ++i)
    for (int j = 0; j < derived[i]->ndep; ++j)
      DependIndexLookup(derived[i], j);

  bits = new ushort[sdi.size()];
  volts = new NR_TYPE[sdi.size()];
  SRTvolts = new NR_TYPE[nVoltFloats];
  SampledData = new NR_TYPE[nFloats];
  AveragedData = new NR_TYPE[nLRfloats];

  if (ProcessingRate == HIGH_RATE)
    HighRateData = new NR_TYPE[nHRfloats];

  ArraysInitialized = true;

}	/* END ALLOCATEDATAARRAYS */

/* -------------------------------------------------------------------- */
void FreeDataArrays()
{
  if (ArraysInitialized)
    {
    delete [] bits;
    delete [] volts;
    delete [] ADSrecord;
    delete [] SRTvolts;
    delete [] SampledData;
    delete [] AveragedData;

    if (ProcessingRate == HIGH_RATE)
      delete [] HighRateData;

    if (AVAPS)
      for (int i = 0; i < MAX_AVAPS; ++i)
        delete [] AVAPSrecord[i];
    }

  ArraysInitialized = false;

}	/* END FREEDATAARRAYS */

/* END MEMALLOC.C */
