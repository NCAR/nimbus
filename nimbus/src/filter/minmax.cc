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
void FindMinMax()
{
  size_t i, j;
{
  RAWTBL	*rp;
  for (i = 0; i < raw.size(); i++)
  {
    rp = raw[i];
    if (rp->OutputRate > 1) {	// Scan raw data.
      for (j = 0; j < rp->SampleRate * rp->Length; ++j) {
        if (!isnan(SampledData[rp->SRstart + j])) {
          rp->min = std::min(rp->min, SampledData[rp->SRstart + j]);
          rp->max = std::max(rp->max, SampledData[rp->SRstart + j]);
        }
      }
    }
    else {
      for (j = 0; j < rp->Length; ++j) {	// Scan averaged data.
        if (!isnan(AveragedData[rp->LRstart + j])) {
          rp->min = std::min(rp->min, AveragedData[rp->LRstart + j]);
          rp->max = std::max(rp->max, AveragedData[rp->LRstart + j]);
        }
      }
    }
  }
}

{
  DERTBL	*dp;
  for (i = 0; i < derived.size(); i++) {
    if ((dp = ComputeOrder[i])->compute) {
      if (dp->OutputRate > 1) {
        for (j = 0; j < cfg.HRTRate() * dp->Length; ++j) {
          if (!isnan(HighRateData[dp->HRstart + j])) {
            dp->min = std::min(dp->min, HighRateData[dp->HRstart + (SampleOffset * dp->Length)]);
            dp->min = std::min(dp->min, HighRateData[dp->HRstart + (SampleOffset * dp->Length)]);
          }
        }
      }
      else {
        for (j = 0; j < dp->Length; ++j) {
          if (!isnan(AveragedData[dp->LRstart + j])) {
            dp->min = std::min(dp->min, AveragedData[dp->LRstart + j]);
            dp->max = std::max(dp->max, AveragedData[dp->LRstart + j]);
          }
        }
      }
    }
  }
}
}
