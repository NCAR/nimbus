/*
-------------------------------------------------------------------------
OBJECT NAME:	minmax.c

FULL NAME:	Track min/max for each variable

ENTRY POINTS:	FindMinMax()

DESCRIPTION:	

REFERENCED BY:	lrloop.c, hrloop.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2010
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "amlib.h"

extern NR_TYPE	*HighRateData, *AveragedData, *SampledData;

/* -------------------------------------------------------------------- */
static void _findMinMax(var_base * vp, const NR_TYPE * value, size_t n)
{
  for (size_t i = 0; i < n; ++i) {
    if (!isnan(value[i])) {
      vp->min = std::min(vp->min, value[i]);
      vp->max = std::max(vp->max, value[i]);
    }
  }
}

/* -------------------------------------------------------------------- */
void FindMinMax()
{
{
  RAWTBL	*rp;
  for (size_t i = 0; i < raw.size(); i++)
  {
    rp = raw[i];
    if (rp->OutputRate > 1) {	// Scan raw data.
      _findMinMax(rp, &SampledData[rp->SRstart], rp->SampleRate * rp->Length);
    }
    else {
      _findMinMax(rp, &AveragedData[rp->LRstart], rp->Length);
    }
  }
}

{
  DERTBL	*dp;
  for (size_t i = 0; i < derived.size(); i++) {
    if ((dp = derived[i])->compute) {
      if (dp->OutputRate > 1) {
        _findMinMax(dp, &HighRateData[dp->HRstart], cfg.HRTRate() * dp->Length);
      }
      else {
        _findMinMax(dp, &AveragedData[dp->LRstart], dp->Length);
      }
    }
  }
}
}
