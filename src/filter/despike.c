/*
-------------------------------------------------------------------------
OBJECT NAME:	despike.c

FULL NAME:	Despiking routines

ENTRY POINTS:	AddVariableToSDIdespikeList()
		AddVariableToRAWdespikeList()
		DespikeData()
		LogDespikeInfo()

STATIC FNS:	despike()
		check1Hz()
		checkVariable()

DESCRIPTION:	Locate spikes and replace the data with missing_value.

COPYRIGHT:	University Corporation for Atmospheric Research, 1995-2005
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "circbuff.h"

static std::vector<RAWTBL *> raw_spike;
static std::vector<size_t>   nSpikesRAW;	// Stats.

static void checkVariable(var_base *, NR_TYPE, size_t *);
static void check1Hz(var_base *, NR_TYPE, size_t *);

void LogThisRecordMsg(NR_TYPE *record, const char msg[]);


/* -------------------------------------------------------------------- */
void AddVariableToRAWdespikeList(RAWTBL *varp)
{
  if (!cfg.Despiking())
    return;

  raw_spike.push_back(varp);
  nSpikesRAW.push_back(0);
}

/* -------------------------------------------------------------------- */
void LogDespikeInfo()
{
  size_t i;

  for (i = 0; i < raw_spike.size(); ++i)
  {
    if (nSpikesRAW[i] > 0)
    {
      sprintf(buffer, "%s: %d spikes removed with slope exceeding %f\n",
	raw_spike[i]->name, nSpikesRAW[i], raw_spike[i]->SpikeSlope);

      LogMessage(buffer);
    }
  }
}

/* -------------------------------------------------------------------- */
static NR_TYPE	*prev_rec, *this_rec, *next_rec, *prev2_rec, *next2_rec;

void DespikeData(CircularBuffer *LRCB, int index)
{
  prev2_rec	= (NR_TYPE *)GetBuffer(LRCB, index-2);
  prev_rec	= (NR_TYPE *)GetBuffer(LRCB, index-1);
  this_rec	= (NR_TYPE *)GetBuffer(LRCB, index);
  next_rec	= (NR_TYPE *)GetBuffer(LRCB, index+1);
  next2_rec	= (NR_TYPE *)GetBuffer(LRCB, index+2);

  for (size_t i = 0; i < raw_spike.size(); ++i)
    if (raw_spike[i]->SampleRate == 1)
      check1Hz(raw_spike[i], raw_spike[i]->SpikeSlope, &nSpikesRAW[i]);
    else
      checkVariable(raw_spike[i], raw_spike[i]->SpikeSlope, &nSpikesRAW[i]);

}

/* -------------------------------------------------------------------- */
static void check1Hz(var_base *varp, NR_TYPE SpikeSlope, size_t *counter)
{
  NR_TYPE	points[5], dir1, dir2;	/* Direction of data	*/

  points[0] = prev2_rec[varp->SRstart];
  points[1] = prev_rec[varp->SRstart];
  points[2] = this_rec[varp->SRstart];
  points[3] = next_rec[varp->SRstart];
  points[4] = next2_rec[varp->SRstart];

  dir1 = points[1] - points[2];
  dir2 = points[2] - points[3];

  if (dir1 * dir2 < 0.0 &&
      fabs((double)dir1) > SpikeSlope && fabs((double)dir2) > SpikeSlope)
  {
    this_rec[varp->SRstart] = floatNAN;
    (*counter)++;
    sprintf(buffer, "Despike: %s, deltas %g %g - slope=%g\n", varp->name,dir1,dir2,SpikeSlope);
    LogThisRecordMsg(this_rec, buffer);
  }

}

/* -------------------------------------------------------------------- */
static void checkVariable(var_base *vp, NR_TYPE SpikeSlope, size_t *counter)
{
  static const int nPrevPts = 3;

  size_t	nFirstHalfPoints = vp->SampleRate + nPrevPts;
  size_t	nPoints = 2 * vp->SampleRate + nPrevPts;

  size_t	sx, ex, spikeCount = 0;
  double	xa[nFirstHalfPoints], ya[nFirstHalfPoints];
  NR_TYPE	points[nFirstHalfPoints], dir1; /* Direction of data	*/

  /* Copy all points to a seperate place for inspection.
   */
  points[0] = prev_rec[vp->SRstart + vp->SampleRate - 3];
  points[1] = prev_rec[vp->SRstart + vp->SampleRate - 2];
  points[2] = prev_rec[vp->SRstart + vp->SampleRate - 1];
  memcpy((char *)&points[nPrevPts], (char *)&this_rec[vp->SRstart],
		sizeof(NR_TYPE) * vp->SampleRate);
  memcpy((char *)&points[nFirstHalfPoints], (char *)&next_rec[vp->SRstart],
		sizeof(NR_TYPE) * vp->SampleRate);


  /* During PASE & ICE-L the HGM232 radar altimeter had numerous spikes, the
   * despiker was only able to detect about 3/4 of them, do a pre-screen here
   * to remove any point that is "way out".
   * All spikes go to between 0 and about 300 feet.  So above 300 feet all spikes
   * go down, this is easy to test by determining the max, and tossing anything
   * more than say 300 drop from max for the second.  When flying low off the deck
   * say below 500 feet, that test does not work so well.  Change threshold to 30
   * feet and test that against the median for the second.
   */
  if (cfg.isADS3() && strcmp(vp->name, "HGM232") == 0)
  {
    NR_TYPE max = 0.0;
    std::vector<NR_TYPE> v;

    for (size_t i = nPrevPts; i < nFirstHalfPoints; ++i)
    {
      max = std::max(max, points[i]);	// Find max.
      v.push_back(points[i]);		// In case we want median instead of max.
    }

    NR_TYPE threshold = 300.0;	// 300 feet is arbitrary.
    NR_TYPE test = max;

    // The "max" test does not work below about 300-500 feet.  So change to median test.
    if (max < 500.0)
    {
      std::sort(v.begin(), v.end());
      NR_TYPE median = v[(nFirstHalfPoints+nPrevPts)/2];
      threshold = 30.0;
      test = median;
    }

    // Remove any point that is more than 300 feet from the max.
    for (size_t i = nPrevPts; i < nFirstHalfPoints; ++i)
    {
      if (!isnan(points[i]) && fabs(test - points[i]) > threshold)
      {
        points[i] = floatNAN;
        ++spikeCount;
      }
    }
  }


  for (size_t i = nPrevPts; i < nFirstHalfPoints; ++i)
  {
    sx = ex = i;

    dir1 = points[sx-1] - points[sx];

    if (fabs((double)dir1) > SpikeSlope)
      {
      ya[0] = points[sx-3]; xa[0] = sx-3;
      ya[1] = points[sx-2]; xa[1] = sx-2;
      ya[2] = points[sx-1]; xa[2] = sx-1;
      int spCnt = nPrevPts;
      int consecutiveCnt = 0;

      do
      {
        if (fabs(points[sx-1] - points[++ex]) < SpikeSlope)
        {
          xa[spCnt] = ex;
          ya[spCnt] = points[ex];
          ++consecutiveCnt;
          ++spCnt;
        }
        else
          consecutiveCnt = 0;
      }
      while (consecutiveCnt < 3 && ex < nPoints);

      if (ex < nPoints)
      {
        sprintf(buffer, "Despike: %s, delta %g - slope=%g, point = %g, nPoints=%d, pos=%d\n",
		vp->name, dir1, SpikeSlope, this_rec[sx+1], ex-sx-1, i);
        LogThisRecordMsg(this_rec, buffer);

        for (int k = (int)xa[nPrevPts-1]+1; k < (int)xa[nPrevPts]; ++k)
          points[k] = floatNAN;

        i = ex-1;
        ++spikeCount;
      }
    }
  }

  if (spikeCount > 0)
  {
    memcpy(	(char *)&this_rec[vp->SRstart], (char *)&points[nPrevPts],
		sizeof(NR_TYPE) * vp->SampleRate);

    *counter += spikeCount;
  }
}
