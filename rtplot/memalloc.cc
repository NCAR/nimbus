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


/* -------------------------------------------------------------------- */
void AllocateDataArrays()
{
  ADSrecord = new char[file->hdr->lrLength()];

  nFloats = 0;

  for (size_t i = 0; i < sdi.size(); ++i)
    {
    sdi[i]->SRstart = nFloats;
    nFloats += sdi[i]->SampleRate;
    }

  for (size_t i = 0; i < raw.size(); ++i)
    {
    raw[i]->SRstart = nFloats;
    nFloats += (raw[i]->SampleRate * raw[i]->Length);
    }


  bits = new float[nFloats];
  volts = new float[nFloats];
  SampledData = new float[nFloats];

  memset((void *)bits, 0, sizeof(long) * nFloats);
  memset((void *)volts, 0, sizeof(NR_TYPE) * nFloats);

}	/* END ALLOCATEDATAARRAYS */

/* -------------------------------------------------------------------- */
void FreeDataArrays()
{
  delete [] ADSrecord;
  delete [] SampledData;
  delete [] bits;
  delete [] volts;

}	/* END FREEDATAARRAYS */

/* END MEMALLOC.C */
