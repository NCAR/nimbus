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

DESCRIPTION:	This implementation uses the gnu gsl library cubic spline.

COPYRIGHT:	University Corporation for Atmospheric Research, 1995-2005
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "circbuff.h"

#include <cmath>
#include <gsl/gsl_spline.h>

static std::vector<SDITBL *> sdi_spike;
static std::vector<RAWTBL *> raw_spike;
static std::vector<size_t>   nSpikesSDI, nSpikesRAW;

static void checkVariable(var_base *, NR_TYPE, size_t *);
static void check1Hz(var_base *, NR_TYPE, size_t *);
static NR_TYPE despike(NR_TYPE *);


/* -------------------------------------------------------------------- */
void AddVariableToSDIdespikeList(SDITBL *varp)
{
  sdi_spike.push_back(varp);
  nSpikesSDI.push_back(0);
}

/* -------------------------------------------------------------------- */
void AddVariableToRAWdespikeList(RAWTBL *varp)
{
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
  if (cfg.Despiking() == false)
    return;

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
    this_rec[varp->SRstart] = despike(points);
    (*counter)++;
    printf("Despike: %s, deltas %g %g - slope=%g\n", varp->name,dir1,dir2,SpikeSlope);
    }

}	/* END CHECK1HZ */

/* -------------------------------------------------------------------- */
static void checkVariable(var_base *vp, NR_TYPE SpikeSlope, size_t *counter)
{
  size_t	sx, ex, spikeCount = 0,
		spCnt, consecutiveCnt, nPoints;

  double	xa[vp->SampleRate+2], ya[vp->SampleRate+2];
  NR_TYPE	points[vp->SampleRate*2+4], dir1; /* Direction of data	*/

  /* Copy all points to a seperate place for inspection.
   */
  points[0] = prev_rec[vp->SRstart + vp->SampleRate - 2];
  points[1] = prev_rec[vp->SRstart + vp->SampleRate - 1];
  memcpy((char *)&points[2], (char *)&this_rec[vp->SRstart],
		NR_SIZE * vp->SampleRate);
  memcpy((char *)&points[vp->SampleRate+2], (char *)&next_rec[vp->SRstart],
		NR_SIZE * vp->SampleRate);
  nPoints = 2 * vp->SampleRate + 2;

  for (size_t i = 2; i < vp->SampleRate+2; ++i)
    {
    sx = ex = i;

    dir1 = points[sx-1] - points[sx];

    if (fabs((double)dir1) > SpikeSlope)
      {
      ya[0] = points[sx-2]; xa[0] = sx-2;
      ya[1] = points[sx-1]; xa[1] = sx-1;
      spCnt = 2;
      consecutiveCnt = 0;

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
      while (consecutiveCnt < 2 && ex < nPoints);

      if (ex < nPoints)
        {
        printf("Despike: %s, delta %g - slope=%g, point = %g, nPoints=%d\n",
		vp->name, dir1, SpikeSlope, this_rec[sx+1], ex-sx-1);

        gsl_interp_accel *acc = gsl_interp_accel_alloc();
        gsl_spline *spline = gsl_spline_alloc(gsl_interp_cspline, spCnt);

        gsl_spline_init(spline, xa, ya, spCnt);

        for (size_t j = 1; j < spCnt-2; ++j)
          for (int k = (int)xa[j]+1; k < (int)xa[j+1]; ++k)
            points[k] = gsl_spline_eval(spline, k, acc);

        gsl_spline_free(spline);
        gsl_interp_accel_free(acc);

        i = ex-1;
        ++spikeCount;
        }
//      else
//        printf("Despike, found something, but can't remove it, shouldn't receive this?\n");
      }
    }

  if (spikeCount > 0)
  {
    memcpy(	(char *)&this_rec[vp->SRstart], (char *)&points[2],
		NR_SIZE * vp->SampleRate);

    *counter += spikeCount;
  }

}	/* END CHECKVARIABLE */

/* -------------------------------------------------------------------- */
static NR_TYPE despike(NR_TYPE *points)
{
  NR_TYPE	y = 0.0;

  static const NR_TYPE c[] = { -0.166667, 0.666667, 0.0, 0.666667, -0.166667 };

  for (size_t i = 0; i < 5; ++i)
    if (i != 2)
      y += c[i] * points[i];

  return(y);

}	/* END DESPIKE */

/* END DESPIKE.C */
