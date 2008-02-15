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
		Also create HRT interpolated data.  At some point this
		should be renamed "resampler.c".

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2006
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "circbuff.h"

#include <gsl/gsl_spline.h>

/* Global to ease parameter pushing in intensive loops.	*/
static NR_TYPE *recPtrs[32], *prev_rec, *this_rec, *next_rec;

void LogThisRecordMsg(NR_TYPE *record, const char msg[]);

static void
  resample(RAWTBL *vp, int lag, NR_TYPE *, NR_TYPE *),
  shift_vector(RAWTBL *vp, int lag, NR_TYPE *srt_out, NR_TYPE *hrt_out);

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
	const int	index,		/* Index into CircBuff		*/
	NR_TYPE		*output,	/* Place to put shifted record	*/
	NR_TYPE		*houtput)	/* Place to put shifted record	*/
{
  NR_TYPE *srt_out;

  prev_rec	= (NR_TYPE *)GetBuffer(LRCB, index-1);
  this_rec	= (NR_TYPE *)GetBuffer(LRCB, index);
  next_rec	= (NR_TYPE *)GetBuffer(LRCB, index+1);

  // Copy current rec into srt output rec.
  memcpy((char *)output, (char *)this_rec, sizeof(NR_TYPE) * nSRfloats);


  {	// Get pointers into an array.
  int i;
  for (i = 0; i < NLRBUFFERS; ++i)
    recPtrs[i] = (NR_TYPE *)GetBuffer(LRCB, index-3+i);
  recPtrs[i] = 0;	// Terminate array.
  }


  for (size_t i = 0; i < raw.size(); ++i)
  {
    RAWTBL	*rp = raw[i];
    int		lag = rp->StaticLag + rp->DynamicLag;
    bool	noMissingData = true;

    for (size_t j = 0; j < rp->SampleRate; ++j)
      if (isnan(this_rec[rp->SRstart + j]))
        noMissingData = false;

    if (abs((lag)) > MaxLag)
    {
      if (lag < 0)
        lag = -MaxLag;
      else
        lag = MaxLag;

      sprintf(buffer,
        "Max lag of |%d| ms exceeded, setting to %d ms and continuing.\n",
        MaxLag, lag);
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
resample(RAWTBL *vp, int lag, NR_TYPE *srt_out, NR_TYPE *hrt_out)
{
  size_t	nPoints, goodPoints = 0;
  size_t	gap_size = 1000 / vp->SampleRate;

  // GV IRS position & ground speed data are at 12.5Hz.
  if (vp->SampleRate == 13)
    gap_size = 80;  // (1000.0 / 12.5);

  nPoints = vp->SampleRate * NLRBUFFERS;	// 5 (7?) Seconds.

  double	x[nPoints], y[nPoints], startTime;

  // Loop through all 5 (7?) records (above) and pull out all the valid data
  // points and make one big record.
  for (size_t ri = 0; recPtrs[ri] != 0; ++ri)
  {
    NR_TYPE *curPtr = &recPtrs[ri][vp->SRstart];
    NR_TYPE dynLag = 0;

    if (vp->LAGstart != -1)
    {
      dynLag = recPtrs[ri][vp->LAGstart] / 1000.0;

      if (isnan(dynLag))
      {
        if (ri >= 2 && !isnan(recPtrs[ri-2][vp->LAGstart]))
        {
          recPtrs[ri][vp->LAGstart] = recPtrs[ri-2][vp->LAGstart];
          dynLag = recPtrs[ri][vp->LAGstart] / 1000.0;
        }
        else
          dynLag = 0;
      }
    }

    for (size_t i = 0; i < vp->SampleRate; ++i)
    {
      if (!isnan(curPtr[i]))
      {
        x[goodPoints] = (ri * 1000) + (gap_size * i) + dynLag;
        y[goodPoints] = curPtr[i];
        ++goodPoints;
      }
    }
  }

//  startTime = 3000.0 - lag;
  startTime = 3000.0 - vp->StaticLag;

  if (goodPoints < 3)
  {
/*  Removed due to flood of messages, drowning out the rest.
    if (goodPoints != 0)
    {
      char msg[256];
      sprintf(msg, "Not enough points for interp, var=%s, sr=%d, goodPoints=%d",
		vp->name, vp->SampleRate, goodPoints);
      LogThisRecordMsg(this_rec, msg);
    }
*/
    return;
  }

  // Don't interp past the edge of the earth.
  if (x[goodPoints-1] < startTime || startTime < x[0])
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
    gap_size = 1000 / cfg.HRTRate();

    for (size_t i = 0; i < (size_t)cfg.HRTRate(); ++i, rqst += gap_size)
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
      for (size_t i = 0; i < (size_t)cfg.HRTRate(); ++i)
      {
        if (hrt_out[vp->HRstart+i] < vp->Modulo->value[0])
          hrt_out[vp->HRstart+i] += vp->Modulo->diff;
        if (hrt_out[vp->HRstart+i] > vp->Modulo->value[1])
          hrt_out[vp->HRstart+i] -= vp->Modulo->diff;
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

  /* Truncate lag.
   */
  int nIntervalsToMove = lag / sampleInterval;
  int bytesPerVector = rp->Length * sizeof(NR_TYPE);

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
