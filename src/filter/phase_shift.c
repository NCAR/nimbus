/*
-------------------------------------------------------------------------
OBJECT NAME:	phase_shift.c

FULL NAME:	Phase Shift Logical Records

ENTRY POINTS:	PhaseShift()
		AddVariableToRAWlagList()

STATIC FNS:	resample()
		shift_histogram()

DESCRIPTION:	Interpolate missing data (spikes) and phase shift data.
		Also create HRT interpolated data.  At some point this
		should be renamed "resampler.c".

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2016
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "circbuff.h"
#include "Interpolator.h"

#ifdef DEBUG
#include "decode.h"
#include <nidas/util/UTime.h>
using nidas::util::UTime;
#endif

/* Global to ease parameter pushing in intensive loops.	*/
static NR_TYPE *recPtrs[32], *prev_rec, *this_rec, *next_rec;

void LogThisRecordMsg(NR_TYPE *record, const char msg[]);

static void
  resample(RAWTBL *vp, int lag, NR_TYPE *, NR_TYPE *),
  shift_histogram(RAWTBL *vp, int lag, NR_TYPE *srt_out, NR_TYPE *hrt_out);

/* -------------------------------------------------------------------- */
void AddVariableToRAWlagList(RAWTBL *varp)
{
  if (!cfg.TimeShifting())
    return;

  /* Don't print message for dynamic lagged variables.
   */
  if (varp->StaticLag == 0)
    return;

  snprintf(buffer, 8192, "Time lag for %s enabled, with lag of %d milliseconds.\n",
          varp->name, varp->StaticLag);

  LogMessage(buffer);
}

/* -------------------------------------------------------------------- */
size_t gapSize(RAWTBL *vp)
{
  size_t gap_size = 1000 / vp->SampleRate;

  // GV IRS position & ground speed data are at 12.5Hz.
  if (vp->SampleRate == 13)
    gap_size = 80;  // (1000.0 / 12.5);

  return gap_size;	// gap in milli-seconds.
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
  memcpy((char *)output, (char *)this_rec, sizeof(NR_TYPE) * nSRvalues);


  {	// Get pointers into an array.
  int i;
  for (i = 0; i < NLRBUFFERS; ++i)
    recPtrs[i] = (NR_TYPE *)GetBuffer(LRCB, index-3+i);
  recPtrs[i] = 0;	// Terminate array.
  }


  for (size_t i = 0; i < raw.size(); ++i)
  {
    RAWTBL	*rp = raw[i];
    int		lag = 0;

    if (cfg.TimeShifting())
    {
      lag = rp->StaticLag + rp->DynamicLag;

      // Don't time-shift if lag is less than 33% of sample spacing.
      size_t gap_size = gapSize(rp);
      if ((size_t)abs(lag) < gap_size / 3)
        lag = 0;

      if (abs((lag)) > MaxLag)
      {
        if (lag < 0)
          lag = -MaxLag;
        else
          lag = MaxLag;

        snprintf(buffer, 8192,
          "Max lag of |%d| ms exceeded, setting to %d ms and continuing; variable %s.\n",
          MaxLag, lag, rp->name);
        LogMessage(buffer);
      }
    }

    // Only resample data if we have a lag.
    if (lag == 0)
      srt_out = 0;
    else
      srt_out = output;

    if (srt_out || houtput)
    {
      if (rp->Length > 1)
        shift_histogram(rp, lag, srt_out, houtput);
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
  size_t	gap_size = gapSize(vp);

  nPoints = vp->SampleRate * NLRBUFFERS;	// 5 (7?) Seconds.

  double	x[nPoints], y[nPoints], startTime;

#ifdef DEBUG
  static std::string tfmt("%Y/%m/%d;%H:%M:%S.%3f");
  time_t firstRecTime = SampledDataTimeToSeconds(recPtrs[0]);
#endif

  // Loop through all 5 (7?) records (above) and pull out all the valid data
  // points and make one big record.
  for (size_t ri = 0; recPtrs[ri] != 0; ++ri)
  {
    NR_TYPE *curPtr = &recPtrs[ri][vp->SRstart];
    NR_TYPE dynLag = 0;

#ifdef DEBUG
    bool watch = (strcmp(vp->name, "CAVP_DPR") == 0);
    if (watch)
    {
      time_t thisTime = SampledDataTimeToSeconds(recPtrs[ri]);
      std::cerr << "resample() buffer " << ri << " accumulating "
		<< vp->SampleRate << " "
		<< vp->name << " samples for record "
		<< UTime(thisTime).format(tfmt) << std::endl;
    }
#endif

    if (vp->TTindx != -1)
    {
      dynLag = recPtrs[ri][vp->TTindx] / 1000.0;

      if (std::isnan(dynLag))
      {
        if (ri >= 2 && !std::isnan(recPtrs[ri-2][vp->TTindx]))
        {
          recPtrs[ri][vp->TTindx] = recPtrs[ri-2][vp->TTindx];
          dynLag = recPtrs[ri][vp->TTindx] / 1000.0;
        }
        else
          dynLag = 0;
      }
    }

    for (size_t i = 0; i < vp->SampleRate; ++i)
    {
      // The 13hz variables from the IRS are really 12.5 hz.
      // No choice but to interpolate if we want smooth high-rate.
      if (vp->SampleRate == 13 && std::isnan(curPtr[i]))
        continue;

      // Let's interp the few random INS missing values.....
      //   for now...  -- cjw 11/2022
      if ((strncmp(vp->name, "THDG", 4) == 0 ||
           strncmp(vp->name, "PITCH", 5) == 0 ||
           strncmp(vp->name, "ROLL", 4) == 0)
           && std::isnan(curPtr[i]))
        continue;

      // add to x, y if timetag is in this second
      if ((gap_size * i) + dynLag <= 1000)
      {
        x[goodPoints] = (ri * 1000) + (gap_size * i) + dynLag;
        y[goodPoints] = curPtr[i];
        if (goodPoints > 0 && x[goodPoints] <= x[goodPoints-1])
        {
/*
          LogThisRecordMsg(this_rec, "!! Duplicate timestamp from nidas !!");
          printf(" Duplicate timestamp from nidas! %s: ri=%zu i=%zu, sr=%zu:\n", vp->name, ri, i, vp->SampleRate);
          printf("    x=%f y=%f\n", x[goodPoints-1], y[goodPoints-1]);
          printf("    x=%f y=%f\n", x[goodPoints], y[goodPoints]);
          x[goodPoints] += 1;
*/
        }
        else
          ++goodPoints;
      }
    }
  }

//  startTime = 3000.0 - lag;
  startTime = 3000.0 - vp->StaticLag;

  if (goodPoints < 3)
  {
    if (hrt_out)
      for (size_t i = 0; i < (size_t)cfg.HRTRate(); ++i)
        hrt_out[vp->HRstart+i] = floatNAN;

/*  Removed due to flood of messages, drowning out the rest.
    if (goodPoints != 0)
    {
      char msg[256];
      snprintf(msg, 128, "Not enough points for interp, var=%s, sr=%d, goodPoints=%d",
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


  // If x (time) values go backwards, then skip this.
  // Occasional ARINC time data is goofy.
  for (size_t i = 1; i < goodPoints; ++i)
  {
    if (x[i] < x[i-1])
    {
      snprintf(buffer, 8192, "Non-monotonically increasing time values: %s (%zu/%zu).", vp->name, i, goodPoints);
      LogThisRecordMsg(this_rec, buffer);
//    for (size_t j = 0; j < goodPoints; ++j)
//      printf("%d, %f\n", (int)x[j], y[j]);
      return;
    }
  }

  Interpolator interp(static_cast<Interpolator::InterpolationType>(cfg.InterpolationType()),
		      x, y, goodPoints);

  if (srt_out)
  {
    double rqst = startTime;
    for (size_t i = 0; i < vp->SampleRate; ++i, rqst += gap_size)
    {
      srt_out[vp->SRstart+i] = interp.eval(rqst);
#ifdef DEBUG
      if (interp.outOfBounds())
      {
	// X = (ri * 1000) + (gap_size * i) + dynLag, in ms from time of first record.
	std::cerr << vp->name << "@"
		  << UTime((long long)firstRecTime*1e6 + rqst*1000).format(tfmt)
		  << ": " << interp.errorMessage(rqst) << std::endl;
      }
#endif
    }
  }


  // Populate high-rate data buffer.
  if (hrt_out)
  {
    double rqst = startTime;
    gap_size = 1000 / cfg.HRTRate();

    for (size_t i = 0; i < (size_t)cfg.HRTRate(); ++i, rqst += gap_size)
    {
      hrt_out[vp->HRstart+i] = interp.eval(rqst);
#ifdef DEBUG
      if (interp.outOfBounds())
      {
	std::cerr << vp->name << ": " << interp.errorMessage(rqst) << std::endl;
      }
#endif
    }
  }

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
 * Only shift histograms, no interpolation.  Lag will be truncated to mod
 * samplerate.
 */
static void
shift_histogram(RAWTBL *rp, int lag, NR_TYPE *srt_out, NR_TYPE *hrt_out)
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
