/*
-------------------------------------------------------------------------
OBJECT NAME:	memalloc.cc

FULL NAME:	Memory Allocation

ENTRY POINTS:	AllocateDataArrays()
		FreeDataArrays()

STATIC FNS:	none

DESCRIPTION:	Allocate storage for pointers & data array.  This is
		analogous to shmem.c for winput.c

COPYRIGHT:      University Corporation for Atmospheric Research, 1992-05
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "ctape.h"

#include <nidas/dynld/raf/SyncRecordReader.h>
extern nidas::dynld::raf::SyncRecordReader* syncRecReader;

static bool	ArraysInitialized = false;

extern char	*ADSrecord, *AVAPSrecord[];
extern ushort	*bits;
extern NR_TYPE	*SRTvolts, *volts;
extern NR_TYPE	*SampledData, *AveragedData, *HighRateData;


/* -------------------------------------------------------------------- */
void AllocateDataArrays()
{
  long	lrlen;

  if (AVAPS)
    for (int i = 0; i < MAX_AVAPS; ++i)
      AVAPSrecord[i] = new char[256];


  nLRfloats = nSRfloats = nHRfloats = 0;

  // Set nSRfloats to the number already being sent in by SyncRecordReader.
  if (cfg.isADS3())
    nSRfloats = syncRecReader->getNumFloats();

  const nidas::dynld::raf::SyncRecordVariable* var;

  for (size_t i = 0; i < raw.size(); ++i)
  {
    raw[i]->LRstart = nLRfloats;
    raw[i]->HRstart = nHRfloats;
    if (cfg.isADS3() && (var = syncRecReader->getVariable(raw[i]->name)) != 0)
      raw[i]->SRstart = var->getSyncRecOffset();
    else
    {
      raw[i]->SRstart = nSRfloats;
      nSRfloats += (raw[i]->SampleRate * raw[i]->Length);
    }

    nLRfloats += raw[i]->Length;
    nHRfloats += (cfg.HRTRate() * raw[i]->Length);
  }

  for (size_t i = 0; i < derived.size(); ++i)
  {
    derived[i]->LRstart = nLRfloats;
    derived[i]->HRstart = nHRfloats;
    nLRfloats += derived[i]->Length;
    nHRfloats += (cfg.HRTRate() * derived[i]->Length);
  }

  /* Reset dependIndices.
   */
  for (size_t i = 0; i < derived.size(); ++i)
    for (size_t j = 0; j < derived[i]->ndep; ++j)
      DependIndexLookup(derived[i], j);

  int nVoltFloats = nSRfloats;

  bits = new ushort[nLRfloats];
  volts = new NR_TYPE[nLRfloats];
  SRTvolts = new NR_TYPE[nVoltFloats];
  SampledData = new NR_TYPE[nSRfloats];
  AveragedData = new NR_TYPE[nLRfloats];

  if (cfg.isADS2())
  {
    get_lrlen(&lrlen);
    ADSrecord = new char[lrlen];
  }
  else // ADS3 will be put here, then copied out in rec_decode.c
    ADSrecord = new char[nSRfloats * sizeof(NR_TYPE)];

  if (cfg.ProcessingRate() == Config::HighRate)
    HighRateData = new NR_TYPE[nHRfloats];

  ArraysInitialized = true;

}	// END ALLOCATEDATAARRAYS

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

    if (cfg.ProcessingRate() == Config::HighRate)
      delete [] HighRateData;

    if (AVAPS)
      for (size_t i = 0; i < MAX_AVAPS; ++i)
        delete [] AVAPSrecord[i];
  }

  ArraysInitialized = false;

}	// END FREEDATAARRAYS

// END MEMALLOC.CC
