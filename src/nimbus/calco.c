/*
-------------------------------------------------------------------------
OBJECT NAME:	calco.c

FULL NAME:	Apply Calibration Coefficients

ENTRY POINTS:	ApplyCalCoes()

DESCRIPTION:	Perform application of calibration coefficients to the
		AveragedData or SampledData records.

COPYRIGHT:	University Corporation for Atmospheric Research, 1994-2012
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
    bool isAnalog = (sp->type[0] == 'A');
    bool applyPoly = (cfg.CalibrationsAppliedBy() == Config::NIMBUS && !sp->cof.empty());
    int nCoef = sp->cof.size();  // only used when applyPoly
    int corder = nCoef - 1;  // only used when applyPoly

    for (size_t j = 0; j < sp->SampleRate; ++j, ++pos)
    {
      if (isAnalog)
        SRTvolts[pos] = record[pos] =
		(record[pos] - sp->convertOffset) * sp->convertFactor;
      else
        SRTvolts[pos] = record[pos];

      if (applyPoly)
      {
        NR_TYPE out = sp->cof[corder];
        for (size_t k = 1; k < nCoef; k++)
          out = sp->cof[corder-k] + record[pos] * out;

        record[pos] = out;
      }
    }
    // Voltages for WINDS display.
    volts[sp->LRstart] = SRTvolts[sp->SRstart];
  }
}	/* END APPLYCALCOES */

/* END CALCO.C */
