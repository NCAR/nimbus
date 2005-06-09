/*
-------------------------------------------------------------------------
OBJECT NAME:	phase_shift.c

FULL NAME:	Phase Shift Logical Records

ENTRY POINTS:	PhaseShift()
		AddVariableToSDIlagList()
		AddVariableToRAWlagList()

STATIC FNS:	resample()		Does all except 1hz
		resample1hz()		Optimized for 1hz variables

DESCRIPTION:	Interpolate missing data (spikes) and phase shift data.
		Also create HRT interpolated data.

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

/* Global to ease parameter pushing in intensive loops.	*/
static NR_TYPE	*prev_prev_rec, *prev_rec,
		*this_rec,
		*next_rec, *next_next_rec;

static void	resample(var_base *vp, int lag, NR_TYPE *, NR_TYPE *),
  shift_vector(RAWTBL *vp, int lag, NR_TYPE *srt_out, NR_TYPE *hrt_out);

/* -------------------------------------------------------------------- */
void AddVariableToSDIlagList(SDITBL *varp)
{
  if (!cfg.TimeShifting())
    return;

  sprintf(buffer, "Time lag for %s enabled, with lag of %d milliseconds.\n",
          varp->name, varp->StaticLag);

  LogMessage(buffer);
}

/* -------------------------------------------------------------------- */
void AddVariableToRAWlagList(RAWTBL *varp)
{
  if (!cfg.TimeShifting())
    return;

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
  memcpy((char *)output, (char *)this_rec, NR_SIZE * nSRfloats);

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
      resample(sp, sp->StaticLag, srt_out, houtput);
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

    /* Only resample data, if we have a lag or some 'missing values'.
     */
    if (lag == 0 && noMissingData)
      srt_out = 0;
    else
      srt_out = output;

    if (srt_out || houtput)
    {
      if (rp->Length > 1)
        shift_vector(rp, lag, srt_out, houtput);
      else
        resample(rp, lag, srt_out, houtput);
    }
  }
}	/* END PHASESHIFT */

/* -------------------------------------------------------------------- */
static void
resample(var_base *vp, int lag, NR_TYPE *srt_out, NR_TYPE *hrt_out)
{
  size_t	nPoints, goodPoints = 0, T = 0;
  size_t	gap_size = 1000 / vp->SampleRate;

  if (vp->SampleRate == 1)
    nPoints = 5;			// 5 Seconds.
  else
    nPoints = vp->SampleRate * 3;	// 3 Seconds.

  double	x[nPoints], y[nPoints], startTime;

  if (vp->SampleRate == 1)
  {
    T = 0;
    if (prev_prev_rec[vp->SRstart] != MISSING_VALUE) {
      x[goodPoints] = T;
      y[goodPoints] = prev_prev_rec[vp->SRstart];
      ++goodPoints;
    }
    T += gap_size;
    if (prev_rec[vp->SRstart] != MISSING_VALUE) {
      x[goodPoints] = T;
      y[goodPoints] = prev_rec[vp->SRstart];
      ++goodPoints;
    }
    T += gap_size;
    if (this_rec[vp->SRstart] != MISSING_VALUE) {
      x[goodPoints] = T;
      y[goodPoints] = this_rec[vp->SRstart];
      ++goodPoints;
    }
    T += gap_size;
    if (next_rec[vp->SRstart] != MISSING_VALUE) {
      x[goodPoints] = T;
      y[goodPoints] = next_rec[vp->SRstart];
      ++goodPoints;
    }
    T += gap_size;
    if (next_next_rec[vp->SRstart] != MISSING_VALUE) {
      x[goodPoints] = T;
      y[goodPoints] = next_next_rec[vp->SRstart];
      ++goodPoints;
    }

    startTime = 2000.0;
  }
  else  // else SampleRate > 1
  {
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

    startTime = 1000.0;
  }

  startTime -= lag;

  if (goodPoints < 3)
    return;

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
  gsl_interp *linear = 0;
  gsl_spline *spline = 0;

  if (cfg.InterpolationType() == Config::Linear)
  {
    linear = gsl_interp_alloc(gsl_interp_linear, goodPoints);
    gsl_interp_init(linear, x, y, goodPoints);
  }
  else
  {
    if (cfg.InterpolationType() == Config::AkimaSpline && goodPoints >= 5)
      spline = gsl_spline_alloc(gsl_interp_akima, goodPoints);
    else
      spline = gsl_spline_alloc(gsl_interp_cspline, goodPoints);

    gsl_spline_init(spline, x, y, goodPoints);
  }

  if (srt_out)
  {
    double rqst = startTime;
    for (size_t i = 0; i < vp->SampleRate; ++i, rqst += gap_size)
      if (cfg.InterpolationType() == Config::Linear)
        srt_out[vp->SRstart+i] = gsl_interp_eval(linear, x, y, rqst, acc);
      else
        srt_out[vp->SRstart+i] = gsl_spline_eval(spline, rqst, acc);
  }

  if (hrt_out)
  {
    double rqst = startTime;
    for (size_t i = 0; i < (size_t)cfg.ProcessingRate(); ++i, rqst += 40)
      if (cfg.InterpolationType() == Config::Linear)
        hrt_out[vp->HRstart+i] = gsl_interp_eval(linear, x, y, rqst, acc);
      else
        hrt_out[vp->HRstart+i] = gsl_spline_eval(spline, rqst, acc);
  }

  if (cfg.InterpolationType() == Config::Linear)
    gsl_interp_free(linear);
  else
    gsl_spline_free(spline);

  gsl_interp_accel_free(acc);


  if (vp->Modulo)	// Go back through and move to within bounds.
  {
    if (srt_out)
      for (size_t i = 0; i < vp->SampleRate; ++i)
      {
        if (srt_out[vp->SRstart+i] < vp->Modulo->value[0])
          srt_out[vp->SRstart+i] += vp->Modulo->diff;
        if (srt_out[vp->SRstart+i] > vp->Modulo->value[1])
          srt_out[vp->SRstart+i] -= vp->Modulo->diff;
      }

    if (hrt_out)
      for (size_t i = 0; i < (size_t)cfg.ProcessingRate(); ++i)
      {
        if (hrt_out[vp->SRstart+i] < vp->Modulo->value[0])
          hrt_out[vp->SRstart+i] += vp->Modulo->diff;
        if (hrt_out[vp->SRstart+i] > vp->Modulo->value[1])
          hrt_out[vp->SRstart+i] -= vp->Modulo->diff;
      }
  }
}	/* END RESAMPLE */
 
/* -------------------------------------------------------------------- */
/*
 * Only shift vectors, no interpolation.  Lag will be truncated to mod
 * samplerate.
 */
static void
shift_vector(RAWTBL *rp, int lag, NR_TYPE *srt_out, NR_TYPE *hrt_out)
{
  if (lag == 0)
    return;

  int sampleInterval = 1000 / rp->SampleRate;
  int nBytesPerVector = rp->Length;

  /* Truncate lag.
   */
  int nIntervalsToMove = lag / sampleInterval;
  int bytesPerVector = rp->Length * NR_SIZE;

  if (lag < 0)
  {
    // SampleRate buffer.
    memcpy(&srt_out[rp->SRstart],
	&this_rec[rp->SRstart + (nIntervalsToMove * rp->Length)],
	(rp->SampleRate - nIntervalsToMove) * bytesPerVector);

    memcpy(&srt_out[rp->SRstart + (rp->Length * (rp->SampleRate - nIntervalsToMove))],
	&next_rec[rp->SRstart],
	nIntervalsToMove * bytesPerVector);

    // HighRate buffer.
    memcpy(&hrt_out[rp->HRstart],
	&this_rec[rp->HRstart + (nIntervalsToMove * rp->Length)],
	(rp->SampleRate - nIntervalsToMove) * bytesPerVector);

    memcpy(&hrt_out[rp->HRstart + (rp->Length * (rp->SampleRate - nIntervalsToMove))],
	&next_rec[rp->HRstart],
	nIntervalsToMove * bytesPerVector);
  }
  else
  {
    // SampleRate buffer.
    memcpy(&srt_out[rp->SRstart + (nIntervalsToMove * rp->Length)],
	&this_rec[rp->SRstart],
	(rp->SampleRate - nIntervalsToMove) * bytesPerVector);

    memcpy(&srt_out[rp->SRstart],
	&prev_rec[rp->SRstart + (rp->Length * (rp->SampleRate - nIntervalsToMove))],
	nIntervalsToMove * bytesPerVector);

    // HighRate buffer.
    memcpy(&srt_out[rp->SRstart + (nIntervalsToMove * rp->Length)],
	&this_rec[rp->SRstart],
	(rp->SampleRate - nIntervalsToMove) * bytesPerVector);

    memcpy(&srt_out[rp->SRstart],
	&prev_rec[rp->SRstart + (rp->Length * (rp->SampleRate - nIntervalsToMove))],
	nIntervalsToMove * bytesPerVector);
  }

}

/* END PHASE_SHIFT.C */
