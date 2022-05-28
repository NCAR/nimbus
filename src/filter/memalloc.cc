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
#include "src/hdr_api/ctape.h"

#include "sync_reader.hh"

static bool	ArraysInitialized = false;

extern char	*ADSrecord, *AVAPSrecord[];
extern ushort	*bits;
extern NR_TYPE	*SRTvolts, *volts;
extern NR_TYPE	*SampledData, *AveragedData, *HighRateData;


/* -------------------------------------------------------------------- */
void AllocateDataArrays()
{
  int32_t lrlen;

  ILOG(("AllocateDataArrays"));
  nidas::dynld::raf::SyncRecordReader* syncRecReader = GetSyncReader();

  if (AVAPS)
    for (int i = 0; i < MAX_AVAPS; ++i)
      AVAPSrecord[i] = new char[256];


  nLRvalues = nSRvalues = nHRvalues = 0;

  // Set nSRvalues to the number already being sent in by SyncRecordReader.
  if (cfg.isADS3())
    nSRvalues = syncRecReader->getNumValues();

  const nidas::dynld::raf::SyncRecordVariable* var;

  for (size_t i = 0; i < raw.size(); ++i)
  {
    raw[i]->LRstart = nLRvalues;
    raw[i]->HRstart = nHRvalues;
    if (cfg.isADS3() && (var = syncRecReader->getVariable(raw[i]->name)) != 0)
      raw[i]->SRstart = var->getSyncRecOffset();
    else
    {
      raw[i]->SRstart = nSRvalues;
      nSRvalues += (raw[i]->SampleRate * raw[i]->Length);
    }

    if (raw[i]->Length < 1)	// Sanity check.
    {
      char msg[1000];
      sprintf(msg, "memalloc.c::AllocateDataArrays:: %s has Length of %zu!  Fatal.\n",
	raw[i]->name, raw[i]->Length);
      HandleFatalError(msg);
    }

    nLRvalues += raw[i]->Length;
    nHRvalues += (cfg.HRTRate() * raw[i]->Length);
  }

  for (size_t i = 0; i < derived.size(); ++i)
  {
    derived[i]->LRstart = nLRvalues;
    derived[i]->HRstart = nHRvalues;
    nLRvalues += derived[i]->Length;
    nHRvalues += (cfg.HRTRate() * derived[i]->Length);
  }

  /* Reset dependIndices.
   */
  for (size_t i = 0; i < derived.size(); ++i)
    for (size_t j = 0; j < derived[i]->nDependencies; ++j)
      DependIndexLookup(derived[i], j, false);

  int nVoltFloats = nSRvalues;

  bits = new ushort[nLRvalues];
  volts = new NR_TYPE[nLRvalues];
  SRTvolts = new NR_TYPE[nVoltFloats];
  SampledData = new NR_TYPE[nSRvalues];
  AveragedData = new NR_TYPE[nLRvalues];

  if (cfg.isADS2())
  {
    get_lrlen(&lrlen);
    ADSrecord = new char[lrlen];
  }
  else
  {
    // ADS3 will be put here, then copied out in rec_decode.c
    ADSrecord = new char[nSRvalues * sizeof(NR_TYPE)];
#ifdef DEBUG
    std::cerr << "Allocated ADSrecord with " << nSRvalues << " values, or "
	      << nSRvalues*sizeof(NR_TYPE) << " bytes." << std::endl;
#endif
  }

  if (cfg.ProcessingRate() == Config::HighRate)
    HighRateData = new NR_TYPE[nHRvalues];

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
