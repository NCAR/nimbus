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
  int		i, j, k, corder, pos;
  SDITBL	*sp;
  NR_TYPE	out;

  for (i = 0; (sp = sdi[i]); ++i)
    {
    pos = sp->SRstart;

    for (j = 0; j < sp->SampleRate; ++j, ++pos)
      {
      corder  = sp->order - 1;
      out     = sp->cof[corder];

      if (sp->type[0] == 'A')
        record[pos] = (record[pos] - sp->convertOffset) * sp->convertFactor;

      for (k = 1; k < sp->order; k++)
        out = sp->cof[corder-k] + record[pos] * out;

      record[pos] = out;
      }
    }

}	/* END APPLYCALCOES */

/* END CALCO.C */
