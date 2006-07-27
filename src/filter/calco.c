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

COPYRIGHT:	University Corporation for Atmospheric Research, 1994-2005
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "amlib.h"

extern NR_TYPE *SRTvolts, *volts;

/* -------------------------------------------------------------------- */
void ApplyCalCoes(NR_TYPE *record)
{
  for (size_t i = 0; i < raw.size(); ++i)
  {
    // At this time the xlate function is responsible for applying cals
    // in ADS2.
    if (cfg.isADS2() && raw[i]->xlate != decodeADS2analog)
      continue;

    RAWTBL *sp = raw[i];
    int pos = sp->SRstart;

    for (size_t j = 0; j < sp->SampleRate; ++j, ++pos)
    {
if (strcmp(sp->name, "DPR") == 0 || strcmp(sp->name, "DPL") == 0)
{
  if (isnan(record[pos])) printf("%s pos=%d - nan\n", sp->name, j);
}

      if (sp->type[0] == 'A')
        SRTvolts[pos] = record[pos] =
		(record[pos] - sp->convertOffset) * sp->convertFactor;
      else
        SRTvolts[pos] = record[pos];

      if (sp->cof.size() > 0)
      {
        int corder = sp->cof.size() - 1;
        NR_TYPE out = sp->cof[corder];
        for (size_t k = 1; k < sp->cof.size(); k++)
          out = sp->cof[corder-k] + record[pos] * out;

        record[pos] = out;
      }
    }
    // Voltages for WINDS display.
    volts[sp->LRstart] = SRTvolts[sp->SRstart];
  }
}	/* END APPLYCALCOES */

/* END CALCO.C */
