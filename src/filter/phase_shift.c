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
		*this_rec,
		*next_rec, *next_next_rec;

static void	shift(var_base *vp, int lag, NR_TYPE *, NR_TYPE *),
		shift_1hz(var_base *vp, int lag, NR_TYPE *, NR_TYPE *);

/* -------------------------------------------------------------------- */
void AddVariableToSDIlagList(SDITBL *varp)
{
  if (!cfg.TimeShifting())
    return;

  sdi_ps.push_back(varp);

  sprintf(buffer, "Time lag for %s enabled, with lag of %d milliseconds.\n",
          varp->name, varp->StaticLag);

  LogMessage(buffer);
}

/* -------------------------------------------------------------------- */
void AddVariableToRAWlagList(RAWTBL *varp)
{
  if (!cfg.TimeShifting())
    return;

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
	NR_TYPE		*output,	/* Place to put shifted record	*/
	NR_TYPE		*houtput)	/* Place to put shifted record	*/
{
  NR_TYPE *srt_out;
  prev_prev_rec	= (NR_TYPE *)GetBuffer(LRCB, index-2);
  prev_rec	= (NR_TYPE *)GetBuffer(LRCB, index-1);
  this_rec	= (NR_TYPE *)GetBuffer(LRCB, index);
  next_rec	= (NR_TYPE *)GetBuffer(LRCB, index+1);
  next_next_rec	= (NR_TYPE *)GetBuffer(LRCB, index+2);

  /* Copy current rec into srt output rec.
   */
  memcpy((char *)output, (char *)this_rec, NR_SIZE * nFloats);

  for (size_t i = 0; i < sdi.size(); ++i)
  {
    SDITBL	*sp = sdi[i];
    bool	noMissingData = true;

    for (size_t j = 0; j < sp->SampleRate; ++j)
      if (this_rec[sp->SRstart + j] == MISSING_VALUE)
        noMissingData = false;

    /* Only resample data, if we have a log or some missing values'.
     */
    if (sp->StaticLag == 0 && noMissingData)
      srt_out = 0;
    else
      srt_out = output;

    if (srt_out || houtput)
      if (sp->SampleRate == 1)
        shift_1hz(sp, sp->StaticLag, srt_out, houtput);
      else
        shift(sp, sp->StaticLag, srt_out, houtput);
  }


  for (size_t i = 0; i < raw.size(); ++i)
  {
    RAWTBL	*rp = raw[i];
    int		lag;
    bool	noMissingData = true;

    for (size_t j = 0; j < rp->SampleRate; ++j)
      if (this_rec[rp->SRstart + j] == MISSING_VALUE)
        noMissingData = false;

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

    /* Only resample data, if we have a log or some missing values'.
     */
    if (lag == 0 && noMissingData)
      srt_out = 0;
    else
      srt_out = output;

    if (srt_out || houtput)
      if (rp->SampleRate == 1)
        shift_1hz(rp, lag, srt_out, houtput);
      else
        shift(rp, lag, srt_out, houtput);
  }
}	/* END PHASESHIFT */

/* -------------------------------------------------------------------- */
static void shift(var_base *vp, int lag, NR_TYPE *srt_out, NR_TYPE *hrt_out)
{
  size_t	nPoints = vp->SampleRate * 3, goodPoints = 0, T = 0;
  size_t	gap_size = 1000 / vp->SampleRate;
  double	x[nPoints], y[nPoints];

  for (size_t i = 0; i < vp->SampleRate; ++i, T += gap_size)
  {
    if (prev_rec[vp->SRstart + i] != MISSING_VALUE)
    {
      x[goodPoints] = T;
      y[goodPoints] = prev_rec[vp->SRstart + i];
      ++goodPoints;
    }
  }

  for (size_t i = 0; i < vp->SampleRate; ++i, T += gap_size)
  {
    if (this_rec[vp->SRstart + i] != MISSING_VALUE)
    {
      x[goodPoints] = T;
      y[goodPoints] = this_rec[vp->SRstart + i];
      ++goodPoints;
    }
  }

  for (size_t i = 0; i < vp->SampleRate; ++i, T += gap_size)
  {
    if (next_rec[vp->SRstart + i] != MISSING_VALUE)
    {
      x[goodPoints] = T;
      y[goodPoints] = next_rec[vp->SRstart + i];
      ++goodPoints;
    }
  }


  if (vp->Modulo)	// 0 - 360 stuff.
  {
    bool high_value = false, low_value = false;

    for (size_t i = 0; i < goodPoints; ++i)
    {
      if (y[i] < vp->Modulo->bound[0])
        low_value = true;
      if (y[i] > vp->Modulo->bound[1])
        high_value = true;
    }

    if (low_value && high_value)
      for (size_t i = 0; i < goodPoints; ++i)
        if (y[i] < vp->Modulo->bound[0])
          y[i] += vp->Modulo->diff;
  }


  gsl_interp_accel *acc = gsl_interp_accel_alloc();
  gsl_spline *spline;

  if (nPoints >= 5)
    spline = gsl_spline_alloc(gsl_interp_akima, goodPoints);
  else
    spline = gsl_spline_alloc(gsl_interp_cspline, goodPoints);

  gsl_spline_init(spline, x, y, goodPoints);

  if (srt_out)
  {
    double rqst = 1000.0 - lag;
    for (size_t i = 0; i < vp->SampleRate; ++i, rqst += gap_size)
      srt_out[vp->SRstart+i] = gsl_spline_eval(spline, rqst, acc);
  }

  if (hrt_out)
  {
    double rqst = 1000.0 - lag;
    for (size_t i = 0; i < (size_t)cfg.ProcessingRate(); ++i, rqst += 40)
      hrt_out[vp->HRstart+i] = gsl_spline_eval(spline, rqst, acc);
  }

  gsl_spline_free(spline);
  gsl_interp_accel_free(acc);


  if (vp->Modulo)	// Go back through and move to within bounds.
  {
    for (size_t i = 0; i < vp->SampleRate; ++i)
    {
      if (srt_out)
      {
        if (srt_out[vp->SRstart+i] < vp->Modulo->value[0])
          srt_out[vp->SRstart+i] += vp->Modulo->diff;
        if (srt_out[vp->SRstart+i] > vp->Modulo->value[1])
          srt_out[vp->SRstart+i] -= vp->Modulo->diff;
      }

      if (hrt_out)
      {
        if (hrt_out[vp->SRstart+i] < vp->Modulo->value[0])
          hrt_out[vp->SRstart+i] += vp->Modulo->diff;
        if (hrt_out[vp->SRstart+i] > vp->Modulo->value[1])
          hrt_out[vp->SRstart+i] -= vp->Modulo->diff;
      }
    }
  }
}	/* END SHIFT */

/* -------------------------------------------------------------------- */
static void shift_1hz(var_base *vp, int lag, NR_TYPE *srt_out, NR_TYPE *hrt_out)
{
  static const size_t	spCnt = 5;

  double	x[spCnt], y[spCnt];

  for (size_t i = 0; i < spCnt; ++i)
    x[i] = i * 1000;

  y[0] = prev_prev_rec[vp->SRstart];
  y[1] = prev_rec[vp->SRstart];
  y[2] = this_rec[vp->SRstart];
  y[3] = next_rec[vp->SRstart];
  y[4] = next_next_rec[vp->SRstart];

  gsl_spline *spline = gsl_spline_alloc(gsl_interp_akima, spCnt);
  gsl_spline_init(spline, x, y, spCnt);

  if (srt_out)
    srt_out[vp->SRstart] = gsl_spline_eval(spline, (double)2000 - lag, 0);

  if (hrt_out)
  {
    double rqst = 2000.0 - lag;
    for (size_t i = 0; i < (size_t)cfg.ProcessingRate(); ++i, rqst += 40)
      hrt_out[vp->HRstart+i] = gsl_spline_eval(spline, rqst, 0);
  }

  gsl_spline_free(spline);

}	/* END SHIFT_1HZ */

/* END PHASE_SHIFT.C */
