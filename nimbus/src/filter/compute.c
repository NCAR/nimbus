/*
-------------------------------------------------------------------------
OBJECT NAME:	compute.c

FULL NAME:	Compute derived variables

ENTRY POINTS:	ComputeLowRateDerived()
		ComputeHighRateDerived()

DESCRIPTION:	Compute, via AMLIB functions, all derived variables.

INPUT:		NimbusRecord

OUTPUT:		none

REFERENCES:	AMLIB functions.

REFERENCED BY:	lrloop.c/hrloop.c, winput.c

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
      AveragedData[dp->LRstart] = MISSING_VALUE;

}	/* END COMPUTELOWRATEDERIVED */

/* -------------------------------------------------------------------- */
void ComputeHighRateDerived()
{
  DERTBL	*dp;

  FeedBack = HIGH_RATE_FEEDBACK;

  for (SampleOffset = 0; SampleOffset < Config::HighRate; ++SampleOffset)
    for (size_t i = 0; i < derived.size(); i++)
      if ((dp = ComputeOrder[i])->compute)
        (*dp->compute)(dp);
      else
        HighRateData[dp->HRstart + (SampleOffset * dp->Length)] = MISSING_VALUE;

}	/* END COMPUTEHIGHRATEDERIVED */
 
/* END COMPUTE.C */
