/*
-------------------------------------------------------------------------
OBJECT NAME:	compute.c

FULL NAME:	Compute derived variables

ENTRY POINTS:	ComputeLowRateDerived()
		ComputeHighRateDerived()

DESCRIPTION:	Compute, via AMLIB functions, all derived variables.

REFERENCES:	AMLIB functions.

REFERENCED BY:	lrloop.c, hrloop.c, rtloop.c, winput.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "amlib.h"

extern NR_TYPE	*HighRateData, *AveragedData;

/* -------------------------------------------------------------------- */
void ComputeLowRateDerived()
{
  DERTBL	*dp;

  FeedBack = LOW_RATE_FEEDBACK;

  for (size_t i = 0; i < derived.size(); i++)
    if ((dp = ComputeOrder[i])->compute)
      (*dp->compute)(dp);
    else
      AveragedData[dp->LRstart] = floatNAN;

}	/* END COMPUTELOWRATEDERIVED */

/* -------------------------------------------------------------------- */
void ComputeHighRateDerived()
{
  DERTBL	*dp;

  FeedBack = HIGH_RATE_FEEDBACK;

  for (SampleOffset = 0; SampleOffset < (size_t)cfg.HRTRate(); ++SampleOffset)
    for (size_t i = 0; i < derived.size(); i++)
      if ((dp = ComputeOrder[i])->compute)
        (*dp->compute)(dp);
      else
        HighRateData[dp->HRstart + (SampleOffset * dp->Length)] = floatNAN;

}	/* END COMPUTEHIGHRATEDERIVED */
 
/* END COMPUTE.C */
