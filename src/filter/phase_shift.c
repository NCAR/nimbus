/*
-------------------------------------------------------------------------
OBJECT NAME:	phase_shift.c

FULL NAME:	Phase Shift Logical Records

ENTRY POINTS:	PhaseShift()
		AddVariableToSDIlagList()
		AddVariableToRAWlagList()

STATIC FNS:	shift()			Does all except 1hz
		shift_1hz()		Optimized for 1hz variables
		interp_regular()	(no wraparound)
		interp_360_degree()	(0 to 360)
		interp_180_degree()	(-180 to +180)

DESCRIPTION:	For each variable to be time shifted, shift the data in
		the current record to the closest desired ms.  Next,
		interpolate between points if necassary.

INPUT:		Logical Record, CircularBuffers, CircBuff index.

OUTPUT:		phase shifted logical record (in place).

REFERENCES:	GetBuffer(), Interpolate()

REFERENCED BY:	LowRateLoop(), HighRateLoop()

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2005
-------------------------------------------------------------------------
*/

#include <memory.h>

#include "nimbus.h"
#include "decode.h"
#include "bounds.h"
#include "circbuff.h"

#include <gsl/gsl_spline.h>

static std::vector<SDITBL *> sdi_ps;
static std::vector<RAWTBL *> raw_ps;

/* Global to ease parameter pushing in intensive loops.	*/
static NR_TYPE	*prev_prev_rec, *prev_rec,
		*this_rec, *out_rec,
		*next_rec, *next_next_rec;

static void	shift(char name[], int start, int rate, int lag),
		shift_1hz(int start, int lag),
		interp_regular(NR_TYPE, NR_TYPE *, int, int, int, int),
		interp_360_angle(NR_TYPE prev_point, NR_TYPE *next_point,
			int start, int rate, int gap_size, int remainder),
		interp_180_angle(NR_TYPE prev_point, NR_TYPE *next_point,
			int start, int rate, int gap_size, int remainder),
		check_for_wrap(NR_TYPE points[]);

NR_TYPE	PolyInterp(NR_TYPE yaxis[], int ms_gap, int ms_want);


/* -------------------------------------------------------------------- */
void AddVariableToSDIlagList(SDITBL *varp)
{
  sdi_ps.push_back(varp);

  sprintf(buffer, "Time lag for %s enabled, with lag of %d milliseconds.\n",
          varp->name, varp->StaticLag);

  LogMessage(buffer);

}

/* -------------------------------------------------------------------- */
void AddVariableToRAWlagList(RAWTBL *varp)
{
  raw_ps.push_back(varp);

  /* Don't print message for dynamic lagged variables.
   */
  if (varp->StaticLag == 0)
    return;

  sprintf(buffer, "Time lag for %s enabled, with lag of %d milliseconds.\n",
          varp->name, varp->StaticLag);

  LogMessage(buffer);

}

/* -------------------------------------------------------------------- */
void PhaseShift(
	CircularBuffer	*LRCB,
	int		index,		/* Index into CircBuff		*/
	NR_TYPE		*output)	/* Place to put shifted record	*/
{
  size_t i;

  prev_prev_rec	= (NR_TYPE *)GetBuffer(LRCB, index-2);
  prev_rec	= (NR_TYPE *)GetBuffer(LRCB, index-1);
  this_rec	= (NR_TYPE *)GetBuffer(LRCB, index);
  out_rec	= output;
  next_rec	= (NR_TYPE *)GetBuffer(LRCB, index+1);
  next_next_rec	= (NR_TYPE *)GetBuffer(LRCB, index+2);

  /* Copy current rec into output rec.
   */
  memcpy((char *)out_rec, (char *)this_rec, NR_SIZE * nFloats);

  if (!cfg.TimeShifting())
    return;

  {
  SDITBL	*sp;

  for (i = 0; i < sdi_ps.size(); ++i)
    {
    sp = sdi_ps[i];

    if (abs(sp->StaticLag) < JITTER)
      continue;

    if (sp->SampleRate == 1)
      shift_1hz(sp->SRstart, sp->StaticLag);
    else
      shift(sp->name, sp->SRstart, sp->SampleRate, sp->StaticLag);
    }
  }

  {
  RAWTBL	*rp;
  int		lag;

  for (i = 0; i < raw_ps.size(); ++i)
    {
    rp = raw_ps[i];

    if (abs((lag = rp->StaticLag + rp->DynamicLag)) > 1000)
      {
      if (lag < 0)
        lag = -1000;
      else
        lag = 1000;

      sprintf(buffer,
        "Max lag of |1000| ms exceeded, setting to %d ms and continuing.\n",
        lag);
      LogMessage(buffer);
      }

    if (abs(lag) > JITTER)
      if (rp->SampleRate == 1)
        shift_1hz(rp->SRstart, lag);
      else
        shift(rp->name, rp->SRstart, rp->SampleRate, lag);
    }
  }

}	/* END PHASESHIFT */

/* -------------------------------------------------------------------- */
static void shift(
	char	name[],	/* This is needed for a couple special cases	*/
	int	start,	/* Index into this_rec & out_rec for this var	*/
	int	rate,
	int	lag)
{
  int		gap_size, ngaps, remainder;
  int		source, dest, nbytes;
  NR_TYPE	prev_point, next_point[2];	/* End points for Interp */

  gap_size	= 1000 / rate;
  ngaps		= abs(lag) / gap_size;
  remainder	= lag - (ngaps * gap_size);
  nbytes	= NR_SIZE * (rate - ngaps);

  /* Start by moving the data to the nearest ms gap, then
   * if the remainder is greater than the allowable JITTER,
   * interpolate between the points.
   */
  if (lag > 0)
    {	/* Shift data backwards	*/
    if ((rate >= 50 && remainder > gap_size / 2) || remainder)
      {
      ++ngaps;
      nbytes -= NR_SIZE;
      }

    source	= start + ngaps;
    dest	= start;

    prev_point = this_rec[source-1];

    memcpy((char *)&out_rec[dest], (char *)&this_rec[source], nbytes);


    /* Get data from Next record
     */
    source	= start;
    dest	= start + rate - ngaps;
    nbytes	= sizeof(NR_TYPE) * ngaps;

    memcpy((char *)&out_rec[dest], (char *)&next_rec[source], nbytes);

    if (ngaps < rate-1)
      memcpy((char *)next_point, (char *)&next_rec[source + ngaps], 2*NR_SIZE);
    else if (ngaps == rate-1)
      {
      next_point[0] = next_rec[source+ngaps];
      next_point[1] = next_next_rec[start];
      }
    else if (rate == ngaps)
      memcpy((char *)next_point, (char *)&next_next_rec[start], 2*NR_SIZE);
    else
      fprintf(stderr, "PhaseShift: impossible.\n");
    }
  else
    {	/* Shift data forward	*/

    source	= start;
    dest	= start + ngaps;

    if (ngaps == 0)
      memcpy((char *)next_point, (char *)&next_rec[start], 2*NR_SIZE);
    if (ngaps == 1)
      {
      next_point[0] = this_rec[start+rate-1];
      next_point[1] = next_rec[start];
      }
    else
      memcpy((char *)next_point, (char *)&this_rec[dest], 2*NR_SIZE);

    memcpy((char *)&out_rec[dest], (char *)&this_rec[source], nbytes);


    /* Get data from Previous record
    */
    source	= start + rate - ngaps;
    dest	= start;
    nbytes	= sizeof(NR_TYPE) * ngaps;

    memcpy((char *)&out_rec[dest], (char *)&prev_rec[source], nbytes);

    if (ngaps == rate)
      prev_point = prev_prev_rec[start+rate-1];
    else if (ngaps == 0)
      prev_point = prev_rec[start+rate-1];
    else
      prev_point = prev_rec[source-1];
    }


  if (remainder > JITTER * 2)		/* Then Interpolate	*/
    if (strncmp(name, "THDG", 4) == 0)
      interp_360_angle(prev_point, next_point, start, rate, gap_size,remainder);
    else
    if (strncmp(name, "LON", 3) == 0)
      interp_180_angle(prev_point, next_point, start, rate, gap_size,remainder);
    else
      interp_regular(prev_point, next_point, start, rate, gap_size, remainder);

}	/* END SHIFT */

/* -------------------------------------------------------------------- */
static const int NPOINTS = 4;

static void
interp_regular(
	NR_TYPE	prevPoint,
	NR_TYPE	*nextPoint,
	int	start,
	int	rate,
	int	gap_size,
	int	remainder)
{
  int		i;
  NR_TYPE	points[NPOINTS];

  for (i = start; i < start + rate - 2; ++i)
    {
    points[0] = prevPoint;
    memcpy((char *)&points[1], (char *)&out_rec[i], 3*NR_SIZE);
    prevPoint = out_rec[i];
    out_rec[i] = PolyInterp(points, gap_size, remainder);
    }

  points[0]	= prevPoint;
  prevPoint	= out_rec[i];
  memcpy((char *)&points[1], (char *)&out_rec[i], 2*NR_SIZE);
  points[3]	= nextPoint[0];
  out_rec[i]	= PolyInterp(points, gap_size, remainder);
  ++i;

  points[0] = prevPoint;
  points[1] = prevPoint = out_rec[i];
  memcpy((char *)&points[2], (char *)nextPoint, 2*NR_SIZE);
  out_rec[i] = PolyInterp(points, gap_size, remainder);

}	/* END INTERP_REGULAR */

/* -------------------------------------------------------------------- */
static void interp_360_angle(NR_TYPE prev_point, NR_TYPE *next_point, int start, int rate, int gap_size, int remainder)
{
  int		i;
  NR_TYPE	points[NPOINTS];

  for (i = start; i < start + rate - 2; ++i)
    {
    points[0] = prev_point;
    memcpy((char *)&points[1], (char *)&out_rec[i], 3*NR_SIZE);
    prev_point = out_rec[i];

    check_for_wrap(points);
    out_rec[i] = PolyInterp(points, gap_size, remainder);

    /* mod 360	*/
    if (out_rec[i] >= 360.0)
      out_rec[i] -= 360.0;
    }

  points[0] = prev_point;
  prev_point = out_rec[i];
  memcpy((char *)&points[1], (char *)&out_rec[i], 2*NR_SIZE);
  points[3] = next_point[0];
  check_for_wrap(points);

  if ((out_rec[i] = PolyInterp(points, gap_size, remainder)) >= 360.0)
    out_rec[i] -= 360.0;
  ++i;

  points[0] = prev_point;
  points[1] = prev_point = out_rec[i];
  memcpy((char *)&points[2], (char *)next_point, 2*NR_SIZE);

  check_for_wrap(points);
  if ((out_rec[i] = PolyInterp(points, gap_size, remainder)) >= 360.0)
    out_rec[i] -= 360.0;

}	/* END INTERP_360_ANGLE */

/* -------------------------------------------------------------------- */
static void interp_180_angle(NR_TYPE prev_point, NR_TYPE *next_point, int start, int rate, int gap_size, int remainder)
{
  int		i;
  NR_TYPE	points[NPOINTS];

  for (i = start; i < start + rate - 2; ++i)
    {
    points[0] = prev_point;
    memcpy((char *)&points[1], (char *)&out_rec[i], 3*NR_SIZE);
    prev_point = out_rec[i];

    check_for_wrap(points);
    out_rec[i] = PolyInterp(points, gap_size, remainder);

    /* mod 360	*/
    if (out_rec[i] >= 360.0)
      out_rec[i] -= 360.0;
    }

  points[0] = prev_point;
  prev_point = out_rec[i];
  memcpy((char *)&points[1], (char *)&out_rec[i], 2*NR_SIZE);
  points[3] = next_point[0];

  check_for_wrap(points);
  if ((out_rec[i] = PolyInterp(points, gap_size, remainder)) >= 360.0)
    out_rec[i] -= 360.0;
  ++i;

  points[0] = prev_point;
  points[1] = prev_point = out_rec[i];
  memcpy((char *)&points[2], (char *)next_point, 2*NR_SIZE);

  check_for_wrap(points);
  if ((out_rec[i] = PolyInterp(points, gap_size, remainder)) >= 360.0)
    out_rec[i] -= 360.0;

}	/* END INTERP_180_ANGLE */

/* -------------------------------------------------------------------- */
/* Make sure all the points to be used by interpolation aren't wrapping
 * around.  (i.e. Degrees are 0-360 and then wrap around).
 */
static void check_for_wrap(NR_TYPE points[])
{
  int	i;
  bool	needs_fixing = false;

  /* Check for wrap around
   */
  for (i = 0; i < NPOINTS-1; ++i)
    if (abs((int)(points[i] - points[i+1])) > 300)
      {
      needs_fixing = true;
      break;
      }

  if (needs_fixing)
    for (i = 0; i < NPOINTS; ++i)
      if (points[i] < 75)
        points[i] += 360.0;

}	/* END CHECK_FOR_WRAP */

/* -------------------------------------------------------------------- */
static void shift_1hz(int start, int lag)
{
  static const size_t	spCnt = 5;

  double	x[spCnt], y[spCnt];

  for (size_t i = 0; i < spCnt; ++i)
    x[i] = i * 1000;

  y[0] = prev_prev_rec[start];
  y[1] = prev_rec[start];
  y[2] = this_rec[start];
  y[3] = next_rec[start];
  y[4] = next_next_rec[start];

  gsl_spline *spline = gsl_spline_alloc(gsl_interp_cspline, spCnt);
  gsl_spline_init(spline, x, y, spCnt);

  out_rec[start] = gsl_spline_eval(spline, (double)2000 - lag, 0);

  gsl_spline_free(spline);

}	/* END SHIFT_1HZ */

/* END PHASE_SHIFT.C */
