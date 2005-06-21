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

#include "define.h"
#include "decode.h"
#include "adsIOrt.h"

extern ADS_rtFile       *file;

extern NR_TYPE *SRTvolts;

/* -------------------------------------------------------------------- */
void AllocateDataArrays()
{
  ADSrecord = new char[file->hdr->lrLength()];

  nSRfloats = 0;

  for (size_t i = 0; i < sdi.size(); ++i)
    {
    sdi[i]->SRstart = nSRfloats;
    nSRfloats += sdi[i]->SampleRate;
    }

  for (size_t i = 0; i < raw.size(); ++i)
    {
    raw[i]->SRstart = nSRfloats;
    nSRfloats += (raw[i]->SampleRate * raw[i]->Length);
    }


  bits = new float[nSRfloats];
  volts = new float[sdi.size()];
  SRTvolts = new float[nSRfloats];
  SampledData = new float[nSRfloats];

  memset((void *)bits, 0, sizeof(long) * nSRfloats);
  memset((void *)SRTvolts, 0, sizeof(NR_TYPE) * nSRfloats);

}	/* END ALLOCATEDATAARRAYS */

/* -------------------------------------------------------------------- */
void FreeDataArrays()
{
  delete [] ADSrecord;
  delete [] SampledData;
  delete [] bits;
  delete [] SRTvolts;
  delete [] volts;

}	/* END FREEDATAARRAYS */

/* END MEMALLOC.C */
