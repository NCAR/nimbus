/*
-------------------------------------------------------------------------
OBJECT NAME:	despike.c

FULL NAME:	Despiking object routines

ENTRY POINTS:	AddVariableToSDIdespikeList()
		AddVariableToRAWdespikeList()
		DespikeData()
		LogDespikeInfo()

STATIC FNS:	despike()
		check1Hz()
		checkVariable()
		polyinterp()

DESCRIPTION:	This implementation will/can only remove single point
		spikes.  The LaGrange polynomial is used for interpolation.

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	hdr_decode.c, lrloop.c, hrloop.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1995-2001
-------------------------------------------------------------------------
*/

#include <math.h>

#include "nimbus.h"
#include "decode.h"
#include "circbuff.h"

static std::vector<SDITBL *> sdi_spike;
static std::vector<RAWTBL *> raw_spike;
static std::vector<size_t>   nSpikesSDI, nSpikesRAW;

static void checkVariable(var_base *, NR_TYPE, size_t *);
static void check1Hz(var_base *, NR_TYPE, size_t *);
static NR_TYPE despike(NR_TYPE *);
NR_TYPE polyinterp(NR_TYPE xa[], NR_TYPE ya[], int n, int x);


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
  size_t i;

  /* I don't expect to be doing despiking often, so let's check.
  */
  if ((sdi_spike.size() == 0 && raw_spike.size() == 0) || RawData)
    return;

  prev2_rec	= (NR_TYPE *)GetBuffer(LRCB, index-2);
  prev_rec	= (NR_TYPE *)GetBuffer(LRCB, index-1);
  this_rec	= (NR_TYPE *)GetBuffer(LRCB, index);
  next_rec	= (NR_TYPE *)GetBuffer(LRCB, index+1);
  next2_rec	= (NR_TYPE *)GetBuffer(LRCB, index+2);

  for (i = 0; i < sdi_spike.size(); ++i)
    {
    if (sdi_spike[i]->SampleRate == 1)
      {
      check1Hz(sdi_spike[i], sdi_spike[i]->SpikeSlope, &nSpikesSDI[i]);
      }
    else
      {
      checkVariable(sdi_spike[i], sdi_spike[i]->SpikeSlope, &nSpikesSDI[i]);
      }
    }


  for (i = 0; i < raw_spike.size(); ++i)
    {
    if (raw_spike[i]->SampleRate == 1)
      {
      check1Hz(raw_spike[i], raw_spike[i]->SpikeSlope, &nSpikesRAW[i]);
      }
    else
      {
      checkVariable(raw_spike[i], raw_spike[i]->SpikeSlope, &nSpikesRAW[i]);
      }
    }

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
static void checkVariable(var_base *varp, NR_TYPE SpikeSlope, size_t *counter)
{
  size_t	sx, ex, spikeCount = 0,
		spCnt, consecutiveCnt, nPoints;

  NR_TYPE	points[2004], xa[6], ya[6],
		dir1;	/* Direction of data	*/

  /* Copy all points to a seperate place for inspection.
   */
  points[0] = prev_rec[varp->SRstart + varp->SampleRate - 2];
  points[1] = prev_rec[varp->SRstart + varp->SampleRate - 1];
  memcpy((char *)&points[2], (char *)&this_rec[varp->SRstart],
		NR_SIZE * varp->SampleRate);
  memcpy((char *)&points[varp->SampleRate+2], (char *)&next_rec[varp->SRstart],
		NR_SIZE * varp->SampleRate);
  nPoints = 2 * varp->SampleRate + 2;

  for (size_t i = 2; i < varp->SampleRate+2; ++i)
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

//for (j = 0; j < spCnt; ++j)
//  printf("xa[%d] = %f, ya[%d] = %f\n", j, xa[j], j, ya[j]);

      if (ex < nPoints)
        {
        printf("Despike: %s, delta %g - slope=%g, point = %g, nPoints=%d\n",
		varp->name, dir1, SpikeSlope, this_rec[sx+1], ex-sx-1);

        for (size_t j = 1; j < spCnt-2; ++j)
          for (int k = (int)xa[j]+1; k < (int)xa[j+1]; ++k)
            points[k] = polyinterp(xa, ya, spCnt, k);

        i = ex-1;
        ++spikeCount;
        }
//      else
//        printf("Despike, found something, but can't remove it, shouldn't receive this?\n");
      }
    }

  if (spikeCount > 0)
  {
    memcpy(	(char *)&this_rec[varp->SRstart], (char *)&points[2],
		NR_SIZE * varp->SampleRate);

    *counter += spikeCount;
  }

}	/* END CHECKVARIABLE */

/* -------------------------------------------------------------------- */
static NR_TYPE despike(NR_TYPE *points)
{
  NR_TYPE	y = 0.0;

  static NR_TYPE c[] = { -0.166667, 0.666667, 0.0, 0.666667, -0.166667 };

  for (size_t i = 0; i < 5; ++i)
    if (i != 2)
      y += c[i] * points[i];

  return(y);

}	/* END DESPIKE */

/* -------------------------------------------------------------------- */
NR_TYPE polyinterp(NR_TYPE xa[], NR_TYPE ya[], int n, int x)
{
  int		i, m, ns = 0;
  NR_TYPE	den, dif, dift, ho, hp, w;
  NR_TYPE	*c, *d, y, dy;

  dif = fabs(x - xa[0]);
  c = new NR_TYPE[n];
  d = new NR_TYPE[n];

  for (i = 0; i < n; ++i)
    {
    if ((dift = fabs(x - xa[i])) < dif)
      {
      ns = i;
      dif = dift;
      }

    c[i] = d[i] = ya[i];
    }

  y = ya[ns--];

  for (m = 1; m < n; ++m)
    {
    for (i = 0; i < n-m; ++i)
      {
      ho = xa[i] - x;
      hp = xa[i+m] - x;
      w = c[i+1] - d[i];
      if ((den = ho-hp) == 0.0) return(-9.0);
      den = w / den;
      d[i] = hp * den;
      c[i] = ho * den;
      }

    y += (dy = (2 * ns < (n-m) ? c[ns+1] : d[ns--]));
    }

  delete [] c;
  delete [] d;

  return(y);

}

/* END DESPIKE.C */
