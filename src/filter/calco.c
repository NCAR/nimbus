/*
-------------------------------------------------------------------------
OBJECT NAME:	calco.c

FULL NAME:	Apply Calibration Coefficients

ENTRY POINTS:	ApplyCalCoes()

STATIC FNS:	none

DESCRIPTION:	Perform application of calibration coeffiecients to the
		AveragedData or SampledData records.

INPUT:		data, int LOW_RATE/SAMPLE_RATE

OUTPUT:		modified input stream

REFERENCES:	none

REFERENCED BY:	LowRateLoop(), HighRateLoop(), winputops.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1994-7
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"


/* -------------------------------------------------------------------- */
void ApplyCalCoes(NR_TYPE *record)
{
  int		corder, pos;

  for (int i = 0; i < sdi.size(); ++i)
  {
    SDITBL *sp = sdi[i];
    pos = sp->SRstart;

    for (int j = 0; j < sp->SampleRate; ++j, ++pos)
    {
      corder  = sp->order - 1;

      NR_TYPE out     = sp->cof[corder];

      if (sp->type[0] == 'A')
        record[pos] = (record[pos] - sp->convertOffset) * sp->convertFactor;

      for (int k = 1; k < sp->order; k++)
        out = sp->cof[corder-k] + record[pos] * out;

      record[pos] = out;
    }
  }

}	/* END APPLYCALCOES */

/* END CALCO.C */
