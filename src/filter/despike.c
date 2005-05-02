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

#include <cmath>

static std::vector<SDITBL *> sdi_spike;
static std::vector<RAWTBL *> raw_spike;
static std::vector<size_t>   nSpikesSDI, nSpikesRAW;

static void checkVariable(var_base *, NR_TYPE, size_t *);
static void check1Hz(var_base *, NR_TYPE, size_t *);

void LogThisRecordMsg(NR_TYPE *record, char msg[]);


/* -------------------------------------------------------------------- */
void AddVariableToSDIdespikeList(SDITBL *varp)
{
  if (!cfg.Despiking())
    return;

  sdi_spike.push_back(varp);
  nSpikesSDI.push_back(0);
}

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

  for (i = 0; i < sdi_spike.size(); ++i)
    {
    if (nSpikesSDI[i] > 0)
      {
      sprintf(buffer, "%s: %d spikes removed with slope exceeding %f\n",
	sdi_spike[i]->name, nSpikesSDI[i], sdi_spike[i]->SpikeSlope);

      LogMessage(buffer);
      }
    }

  for (i = 0; i < raw_spike.size(); ++i)
    {
    if (nSpikesRAW[i] > 0)
      {
      sprintf(buffer, "%s: %d spikes removed with slope exceeding %f\n",
	raw_spike[i]->name, nSpikesRAW[i], raw_spike[i]->SpikeSlope);

      LogMessage(buffer);
      }
    }
}	/* END LOGDESPIKEINFO */

/* -------------------------------------------------------------------- */
static NR_TYPE	*prev_rec, *this_rec, *next_rec, *prev2_rec, *next2_rec;

void DespikeData(CircularBuffer *LRCB, int index)
{
  prev2_rec	= (NR_TYPE *)GetBuffer(LRCB, index-2);
  prev_rec	= (NR_TYPE *)GetBuffer(LRCB, index-1);
  this_rec	= (NR_TYPE *)GetBuffer(LRCB, index);
  next_rec	= (NR_TYPE *)GetBuffer(LRCB, index+1);
  next2_rec	= (NR_TYPE *)GetBuffer(LRCB, index+2);

  for (size_t i = 0; i < sdi_spike.size(); ++i)
    if (sdi_spike[i]->SampleRate == 1)
      check1Hz(sdi_spike[i], sdi_spike[i]->SpikeSlope, &nSpikesSDI[i]);
    else
      checkVariable(sdi_spike[i], sdi_spike[i]->SpikeSlope, &nSpikesSDI[i]);


  for (size_t i = 0; i < raw_spike.size(); ++i)
    if (raw_spike[i]->SampleRate == 1)
      check1Hz(raw_spike[i], raw_spike[i]->SpikeSlope, &nSpikesRAW[i]);
    else
      checkVariable(raw_spike[i], raw_spike[i]->SpikeSlope, &nSpikesRAW[i]);

}	/* END DESPIKEDATA */

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
    this_rec[varp->SRstart] = MISSING_VALUE;
    (*counter)++;
    sprintf(buffer, "Despike: %s, deltas %g %g - slope=%g\n", varp->name,dir1,dir2,SpikeSlope);
    LogThisRecordMsg(this_rec, buffer);
    }

}	/* END CHECK1HZ */

/* -------------------------------------------------------------------- */
static void checkVariable(var_base *vp, NR_TYPE SpikeSlope, size_t *counter)
{
  static const int nPrevPts = 3;

  size_t	sx, ex, spikeCount = 0, nPoints;
  double	xa[vp->SampleRate+nPrevPts], ya[vp->SampleRate+nPrevPts];
  NR_TYPE	points[vp->SampleRate*nPrevPts], dir1; /* Direction of data	*/

  /* Copy all points to a seperate place for inspection.
   */
  points[0] = prev_rec[vp->SRstart + vp->SampleRate - 3];
  points[1] = prev_rec[vp->SRstart + vp->SampleRate - 2];
  points[2] = prev_rec[vp->SRstart + vp->SampleRate - 1];
  memcpy((char *)&points[nPrevPts], (char *)&this_rec[vp->SRstart],
		NR_SIZE * vp->SampleRate);
  memcpy((char *)&points[vp->SampleRate+nPrevPts], (char *)&next_rec[vp->SRstart],
		NR_SIZE * vp->SampleRate);
  nPoints = 2 * vp->SampleRate + nPrevPts;

  for (size_t i = nPrevPts; i < vp->SampleRate + nPrevPts; ++i)
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
        sprintf(buffer, "Despike: %s, delta %g - slope=%g, point = %g, nPoints=%d\n",
		vp->name, dir1, SpikeSlope, this_rec[sx+1], ex-sx-1);
//        LogThisRecordMsg(this_rec, buffer);

        for (int k = (int)xa[nPrevPts-1]+1; k < (int)xa[nPrevPts]; ++k)
          points[k] = MISSING_VALUE;

        i = ex-1;
        ++spikeCount;
        }
      }
    }

  if (spikeCount > 0)
  {
    memcpy(	(char *)&this_rec[vp->SRstart], (char *)&points[nPrevPts],
		NR_SIZE * vp->SampleRate);

    *counter += spikeCount;
  }

}	/* END CHECKVARIABLE */

/* END DESPIKE.C */
