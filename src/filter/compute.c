/*
-------------------------------------------------------------------------
OBJECT NAME:	compute.c

FULL NAME:	Compute derived variables

ENTRY POINTS:	ComputeLowRateDerived()
		ComputeHighRateDerived()

DESCRIPTION:	Loop over derived variable list, in compute order, and
		call AMLIB compute functions.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2011
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "amlib.h"

extern NR_TYPE	*HighRateData, *AveragedData;

bool blankOutThisValue(var_base * var, time_t thistime);

// Why was this here?  It is shadowed by the thisTime parameter to all the
// functions in this module, so perhaps it is a leftover global variable...
//
// static int thisTime;

#ifdef DEBUG
#include <nidas/util/UTime.h>
using nidas::util::UTime;

void
compute_break()
{}
#endif

/* -------------------------------------------------------------------- */
void ComputeLowRateDerived(time_t thisTime)
{
  FeedBack = LOW_RATE_FEEDBACK;

  for (size_t i = 0; i < derived.size(); i++)
  {
    DERTBL *dp = ComputeOrder[i];

#ifdef DEBUG
    if (strcmp(dp->name, "EW_DPR") == 0 || 
	strcmp(dp->name, "EWX") == 0 ||
	strcmp(dp->name, "DP_DPR") == 0)
      compute_break();
#endif
    if (dp->compute)
      (*dp->compute)(dp);
#ifdef DEBUG
    {
      static std::string tfmt("At %Y/%m/%d;%H:%M:%S.%3f, computed: ");
      std::cerr << UTime(thisTime).format(tfmt)
		<< dp->name << " = " << AveragedData[dp->LRstart] << std::endl;
    }
#endif

    if (blankOutThisValue(dp, thisTime) == true || dp->compute == 0)
    {
      for (size_t j = 0; j < dp->Length; ++j)
        AveragedData[dp->LRstart+j] = floatNAN;
    }
  }
}	/* END COMPUTELOWRATEDERIVED */

/* -------------------------------------------------------------------- */
void ComputeHighRateDerived(time_t thisTime)
{
  FeedBack = HIGH_RATE_FEEDBACK;

  for (SampleOffset = 0; SampleOffset < (size_t)cfg.HRTRate(); ++SampleOffset)
  {
    for (size_t i = 0; i < derived.size(); i++)
    {
      DERTBL *dp = ComputeOrder[i];

      if ((dp = ComputeOrder[i])->compute)
        (*dp->compute)(dp);

      if (blankOutThisValue(dp, thisTime) == true || dp->compute == 0)
      {
        size_t n = cfg.ProcessingRate() * dp->Length;
        for (size_t j = 0; j < n; ++j)
          HighRateData[dp->HRstart+j] = floatNAN;
      }
    }
  }
}	/* END COMPUTEHIGHRATEDERIVED */
 
/* END COMPUTE.C */
