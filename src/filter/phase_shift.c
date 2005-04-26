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

DESCRIPTION:	Use Akima non-periodic spline for time shifting.

INPUT:		Logical Record, CircularBuffers, CircBuff index.

OUTPUT:		phase shifted logical record (in place).

REFERENCES:	GetBuffer(), Interpolate()

REFERENCED BY:	LowRateLoop(), HighRateLoop()

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2005
-------------------------------------------------------------------------
*/

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

static void	shift(var_base *vp, int lag), shift_1hz(int start, int lag);


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
  prev_prev_rec	= (NR_TYPE *)GetBuffer(LRCB, index-2);
  prev_rec	= (NR_TYPE *)GetBuffer(LRCB, index-1);
  this_rec	= (NR_TYPE *)GetBuffer(LRCB, index);
  next_rec	= (NR_TYPE *)GetBuffer(LRCB, index+1);
  next_next_rec	= (NR_TYPE *)GetBuffer(LRCB, index+2);
  out_rec	= output;

  /* Copy current rec into output rec.
   */
  memcpy((char *)out_rec, (char *)this_rec, NR_SIZE * nFloats);

  if (!cfg.TimeShifting())
    return;

  for (size_t i = 0; i < sdi_ps.size(); ++i)
    {
    SDITBL *sp = sdi_ps[i];

    if (abs(sp->StaticLag) < JITTER)
      continue;

    if (sp->SampleRate == 1)
      shift_1hz(sp->SRstart, sp->StaticLag);
    else
      shift(sp, sp->StaticLag);
    }

  for (size_t i = 0; i < raw_ps.size(); ++i)
    {
    RAWTBL *rp = raw_ps[i];
    int		lag;

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
        shift(rp, lag);
    }
}	/* END PHASESHIFT */

/* -------------------------------------------------------------------- */
static void shift(var_base *vp, int lag)
{
  size_t	nPoints = vp->SampleRate * 3;
  double	gap_size = 1000.0 / vp->SampleRate;
  double	x[nPoints], y[nPoints];

  for (size_t i = 0; i < nPoints; ++i)
    x[i] = gap_size * i;

  for (size_t i = 0; i < vp->SampleRate; ++i)
  {
    y[i] = prev_rec[vp->SRstart + i];
    y[vp->SampleRate + i] = this_rec[vp->SRstart + i];
    y[(vp->SampleRate*2) + i] = next_rec[vp->SRstart + i];
  }

  if (vp->Modulo)	// 0 - 360 stuff.
  {
    bool high_value = false, low_value = false;

    for (size_t i = 0; i < nPoints; ++i)
    {
      if (y[i] < vp->Modulo->bound[0])
        low_value = true;
      if (y[i] > vp->Modulo->bound[1])
        high_value = true;
    }

    if (low_value && high_value)
      for (size_t i = 0; i < nPoints; ++i)
        if (y[i] < vp->Modulo->bound[0])
          y[i] += vp->Modulo->diff;
  }


  gsl_interp_accel *acc = gsl_interp_accel_alloc();
  gsl_spline *spline;

  if (nPoints >= 5)
    spline = gsl_spline_alloc(gsl_interp_akima, nPoints);
  else
    spline = gsl_spline_alloc(gsl_interp_cspline, nPoints);

  gsl_spline_init(spline, x, y, nPoints);

  double rqst = 1000.0 - lag;
  for (size_t i = 0; i < vp->SampleRate; ++i, rqst += gap_size)
    out_rec[vp->SRstart+i] = gsl_spline_eval(spline, rqst, acc);

  gsl_spline_free(spline);
  gsl_interp_accel_free(acc);


  if (vp->Modulo)	// Go back through and move to within bounds.
  {
    for (size_t i = 0; i < nPoints; ++i)
    {
      if (y[i] < vp->Modulo->value[0])
        y[i] += vp->Modulo->diff;
      if (y[i] > vp->Modulo->value[1])
        y[i] -= vp->Modulo->diff;
    }
  }
}	/* END SHIFT */

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

  gsl_spline *spline = gsl_spline_alloc(gsl_interp_akima, spCnt);
  gsl_spline_init(spline, x, y, spCnt);

  out_rec[start] = gsl_spline_eval(spline, (double)2000 - lag, 0);

  gsl_spline_free(spline);

}	/* END SHIFT_1HZ */

/* END PHASE_SHIFT.C */
