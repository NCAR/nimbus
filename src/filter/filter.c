/*
-------------------------------------------------------------------------
OBJECT NAME:	filter.c

FULL NAME:	FIR Filter routines.

ENTRY POINTS:	InitMRFilters()
		ClearMRFilters()
		Filter()

STATIC FNS:	See below.

DESCRIPTION:	The mrf filter has it's own circular buffer routines
		which are included in this module and are not associated
		with circbuff.[ch].  $PROJ_DIR/defaults/filters contains
		the coefficients.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2006
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "filters.h"
#include "circbuff.h"

#include <gsl/gsl_spline.h>

extern NR_TYPE	*SampledData, *HighRateData;

static std::vector<mRFilterPtr> rawFilters;

static int	PSCBindex;
static size_t	currentHz, HzDelay;
static size_t	filterRate;

static NR_TYPE	*inputRec, *interpdData;

static circBuffPtr	newCircBuff(int);

static filterPtr readAfilter(char file[]);
static int	disposMultiRateFilter(mRFilterPtr aMRFPtr);

static NR_TYPE	getBuff(int offset, circBuffPtr aCBPtr);
static void	initCircBuff(circBuffPtr aCBPtr),
		disposCircBuff(circBuffPtr aCBPtr),
		putBuff(NR_TYPE datum, circBuffPtr aCBPtr),
		setTimeDelay(size_t, size_t, int *, size_t *),
		filterCounter(RAWTBL *sp),
		ProcessVariable(CircularBuffer *, CircularBuffer *,
				var_base *vp, mRFilterPtr vpFilter),
		SingleStageFilter(	CircularBuffer *, CircularBuffer *,
					mRFilterPtr thisMRF, var_base *vp);

char *GetProcessedBuffer(CircularBuffer *, CircularBuffer *, int offset, var_base *vp);


/* -------------------------------------------------------------------- */
void InitMRFilters()
{
  MOD	*mv_p;
  filterPtr	OneUp, FiveUp, EightUp, TenUp, ThirteenUp, SixteenUp, TwentyFiveUp,
		FiftyDown, OneHundredDown, TwoFiftyDown, FiveHundredDown, ThousandDown,
		vspd, acins, gsf;

  if (cfg.HRTRate() == Config::TwentyFive)
  {
    OneUp = readAfilter("1to25");
    FiveUp = readAfilter("5to25");
    EightUp = readAfilter("8to25");
    TenUp = readAfilter("10to25");
    ThirteenUp = readAfilter("13to25");	// Really 12.5hz
    SixteenUp = readAfilter("16to25");
    TwentyFiveUp = 0;
    vspd = readAfilter("VSPD");
    gsf = readAfilter("GSF");
    acins = readAfilter("ACINS");
    FiftyDown = readAfilter("50to25");
    OneHundredDown = readAfilter("100to25");
    TwoFiftyDown = readAfilter("250to25");
    FiveHundredDown = readAfilter("500to25");
    ThousandDown = readAfilter("1000to25");
  }
  else
  {
    OneUp = readAfilter("1to50");
    FiveUp = readAfilter("5to50");
    EightUp = readAfilter("8to50");
    TenUp = readAfilter("10to50");
    ThirteenUp = readAfilter("13to50");	// Really 12.5hz
    SixteenUp = readAfilter("16to50");
    TwentyFiveUp = readAfilter("25to50");
    vspd = readAfilter("VSPD");
    gsf = readAfilter("GSF");
    acins = readAfilter("ACINS");
    FiftyDown = 0;
    OneHundredDown = readAfilter("100to50");
    TwoFiftyDown = readAfilter("250to50");
    FiveHundredDown = readAfilter("500to50");
    ThousandDown = readAfilter("1000to50");
  }


  /* Create filter Data for each variable
   */
  rawFilters.resize(raw.size());
  for (size_t i = 0; i < raw.size(); ++i)
  {
    rawFilters[i] = 0;

    // All data being interped is linear interped before coming here, so L=1.
    size_t L = 1; // cfg.HRTRate() / raw[i]->SampleRate;
    size_t M = raw[i]->SampleRate / cfg.HRTRate();

    if (L < 1) L = 1;
    if (M < 1) M = 1;

    /* Doesn't require filtering (as in not output or used).
     */
    if (raw[i]->DependedUpon == false && raw[i]->OutputRate != (size_t)cfg.HRTRate())
      continue;

    /* Don't filter counters.  @see filterCounter()
     */
    if (raw[i]->type[0] == 'C')
      continue;

    /* Can't filter Vectors.  And we don't want to filter PMS1D scalars.
     * This data will just be memcpy() into HighRateData[].
     */
    if (raw[i]->Length > 1 || raw[i]->ProbeType & PROBE_PMS1D)
      continue;

    mv_p = raw[i]->Modulo;

    switch (raw[i]->SampleRate)
    {
      case 1:			/* Interpolate	*/
        rawFilters[i] = createMRFilter(L, M, OneUp, mv_p);
//        rawFilters[i] = 0;
        break;
      case 5:
        rawFilters[i] = createMRFilter(L, M, FiveUp, mv_p);
        break;
      case 8:
        rawFilters[i] = createMRFilter(L, M, EightUp, mv_p);
        break;
      case 10:
        /* At this time no filtering of groundspeeds from the IRS should be
         * performed, due to poor quantization from the instrument.  Filtering
         * introdouces ripple and a scalloping effect when the raw data is
         * "stair-stepping".  Webster/Lenschow May-2005
         */
        if (strncmp(raw[i]->name, "GSF", 3) == 0 ||
            strncmp(raw[i]->name, "VEW", 3) == 0 ||
            strncmp(raw[i]->name, "VNS", 3) == 0)
//          rawFilters[i] = createMRFilter(L, M, gsf, mv_p);
          rawFilters[i] = 0;
        else
          rawFilters[i] = createMRFilter(L, M, TenUp, mv_p);
        break;
      case 13:	// Really 12.5hz
//        rawFilters[i] = createMRFilter(L, M, ThirteenUp, mv_p);
        rawFilters[i] = 0;
        break;
      case 16:
        rawFilters[i] = createMRFilter(L, M, SixteenUp, mv_p);
        break;
      case 25:		/* Just filter	*/
        if (cfg.Aircraft() == Config::C130 && strncmp(raw[i]->name, "VSPD", 4) == 0)
          rawFilters[i] = createMRFilter(L, M, vspd, mv_p);
        else
          rawFilters[i] = createMRFilter(L, M, TwentyFiveUp, mv_p);
        break;
      case 50:		/* Decimate	*/
        if (cfg.Aircraft() == Config::C130 && strncmp(raw[i]->name, "ACINS", 5) == 0)
          rawFilters[i] = createMRFilter(L, M, acins, mv_p);
        else
          rawFilters[i] = createMRFilter(L, M, FiftyDown, mv_p);
        break;
      case 250:
        rawFilters[i] = createMRFilter(L, M, TwoFiftyDown, mv_p);
        break;
      case 500:
        rawFilters[i] = createMRFilter(L, M, FiveHundredDown, mv_p);
        break;
      case 1000:
        rawFilters[i] = createMRFilter(L, M, ThousandDown, mv_p);
        break;
      default:
        fprintf(stderr, "mrfFilter: non-supported input rate, ");
        fprintf(stderr, "var=%s, rate=%d\n", raw[i]->name, raw[i]->SampleRate);
    }

    if (rawFilters[i] == 0 && raw[i]->SampleRate != raw[i]->OutputRate)
    {
      char msg[128];
      sprintf(msg, "%s [sr=%d] has no FIR filter, resampled data will be used.",
              raw[i]->name, raw[i]->SampleRate);
      LogMessage(msg);
    }
  }
}	/* END INITMRFILTER */

/* -------------------------------------------------------------------- */
void Filter(	CircularBuffer *PSCB,	/* SampleRate data. */
		CircularBuffer *HSCB)	/* < 25Hz data interp'ed up to 25 */
{
  SampledData = (NR_TYPE *)GetBuffer(PSCB, -(PSCB->nbuffers - 1));
  interpdData = (NR_TYPE *)GetBuffer(HSCB, -(PSCB->nbuffers - 1));

  /* Do raw variables
   */
  for (size_t i = 0; i < raw.size(); ++i)
  {
    /* Counters shouldn't need to be filtered.  Applies averaging or
     * linear interpolation.
     */
    if (raw[i]->type[0] == 'C' && raw[i]->SampleRate != raw[i]->OutputRate)
    {
      filterCounter(raw[i]);
      continue;
    }

    if ((raw[i]->ProbeType & PROBE_PMS1D) || (raw[i]->ProbeType & PROBE_PMS2D))
      memcpy(	(char *)&HighRateData[raw[i]->HRstart],
		(char *)&SampledData[raw[i]->SRstart],
		sizeof(NR_TYPE) * raw[i]->SampleRate * raw[i]->Length);
    else
      ProcessVariable(PSCB, HSCB, raw[i], rawFilters[i]);
  }
}	/* FILTER */

/* -------------------------------------------------------------------- */
static void ProcessVariable(	CircularBuffer *PSCB, CircularBuffer *HSCB,
				var_base *vp, mRFilterPtr vpFilter)
{
  /* Because data with SampleRate below 25 is interpolated first, we
   * can't use SampleRate, but must use 25.  Fake it with filterRate.
   */
  filterRate = vp->SampleRate < (size_t)cfg.HRTRate()
				? (size_t)cfg.HRTRate() : vp->SampleRate;

  if (vpFilter == 0)	/* Filtering not needed/wanted	*/
  {
    if (vp->SampleRate == (size_t)cfg.HRTRate())
      // SampleRate is same as HRT rate, just copy.
      memcpy(	(char *)&HighRateData[vp->HRstart],
		(char *)&SampledData[vp->SRstart],
		sizeof(NR_TYPE) * vp->SampleRate * vp->Length);
    else
    if (vp->SampleRate < (size_t)cfg.HRTRate())
      // Use interpolated data for vars below HRT rate.
      memcpy(	(char *)&HighRateData[vp->HRstart],
		(char *)&interpdData[vp->HRstart],
		sizeof(NR_TYPE) * filterRate * vp->Length);
    else
      // zero out rates above HRT rate.  We could use the resampled data that's there?
      memset(	(char *)&HighRateData[vp->HRstart], 0,
		sizeof(NR_TYPE) * vp->SampleRate * vp->Length);
    return;
  }

  PSCBindex = -(PSCB->nbuffers - 1);
  setTimeDelay(filterRate, vpFilter->filter->order, &PSCBindex, &HzDelay);
  inputRec = (NR_TYPE *)GetProcessedBuffer(PSCB, HSCB, PSCBindex, vp);

  if (vpFilter->task == GET_INPUT)
  {
    currentHz = HzDelay;
    ++PSCBindex;
  }
  else
    if ((currentHz = filterRate - 1 + HzDelay) >= filterRate)
    {
      ++PSCBindex;
      currentHz -= filterRate;
    }

  SingleStageFilter(PSCB, HSCB, vpFilter, vp);

}	/* END PROCESSVARIABLE */

/* -------------------------------------------------------------------- */
static void filterCounter(RAWTBL *sp)
{
  NR_TYPE	*sdp = &SampledData[sp->SRstart];
  NR_TYPE	*hrt = &HighRateData[sp->HRstart];
  size_t	OUTindex;
  size_t	outRate = (size_t)cfg.HRTRate();

  size_t L = outRate / sp->SampleRate;
  size_t M = sp->SampleRate / outRate;
/*
  if (M != 0 && (float)M != (float)outRate / sp->SampleRate)
  {
    fprintf(stderr, "M not even decimation factor for %s, %d -> %d.\n",
	sp->name, sp->SampleRate, outRate);
    return;
  }

  if (L != 0 && (float)L != (float)sp->SampleRate / outRate)
  {
    fprintf(stderr, "L not even interpolation factor for %s, %d -> %d.\n",
	sp->name, sp->SampleRate, outRate);
    return;
  }
*/

  if (L > 0)
    for (OUTindex = 0; OUTindex < outRate; ++OUTindex)
      hrt[OUTindex] = sdp[OUTindex / L] / L;

  if (M > 0)
    for (OUTindex = 0; OUTindex < outRate; ++OUTindex)
    {
      float sum = 0.0;
      for (size_t idx = 0; idx < M; ++idx)
        sum += sdp[(OUTindex*M)+idx];

      hrt[OUTindex] = sum;
    }

}	/* END FILTERCOUNTER */

/* -------------------------------------------------------------------- */
static void SingleStageFilter(CircularBuffer *PSCB, CircularBuffer *HSCB, mRFilterPtr thisMRF, var_base *vp)
{
  int		task;
  NR_TYPE	output;

  for (size_t OUTindex = 0; OUTindex < (size_t)cfg.HRTRate(); ++OUTindex)
  {
    do
    {
      task = iterateMRFilter(thisMRF, inputRec[currentHz], &output);

      if (task == GET_INPUT && ++currentHz == filterRate)
      {
        currentHz = 0;
        inputRec = (NR_TYPE *)GetProcessedBuffer(PSCB, HSCB, PSCBindex++, vp);
      }
    }
    while (task == GET_INPUT);

    HighRateData[vp->HRstart + OUTindex] = output;
  }
}

/* -------------------------------------------------------------------- */
static filterPtr readAfilter(char file[])
{
  char	*nimbus = getenv("PROJ_DIR");
  char	*filter[2000];
  double	sum = 0.0;

  sprintf(buffer, FILTERS.c_str(), nimbus, file);
  ReadTextFile(buffer, filter);

  if (filter[0] == 0)
    return 0;

  filterPtr daFilt = new filterData;

  daFilt->order = atoi(filter[0]);
  daFilt->aCoef = new filterType[daFilt->order];

  for (int i = 0; i < daFilt->order; ++i)
  {
    daFilt->aCoef[i] = atof(filter[i+1]);
    sum += daFilt->aCoef[i];
  }

  FreeTextFile(filter);

  printf("filter.c: filter sum of %s\t= %15.8lf, nTaps=%d\n", file, sum, daFilt->order);

  return(daFilt);

}	/* END READAFILTER */

/* -------------------------------------------------------------------- */
void ClearMRFilters()
{
  for (size_t i = 0; i < raw.size(); ++i)
    if ((int)rawFilters[i] > 0)
      initMultiRateFilter(rawFilters[i]);

}	/* END CLEARMRFILTER */

/* -------------------------------------------------------------------- */
static mRFilterPtr createMRFilter(int L, int M, filterPtr filter, MOD *modvar)
{
  if (filter == 0)
    return 0;

  mRFilterPtr	aMRFPtr;

  aMRFPtr		= new mRFilterData;
  aMRFPtr->inBuff	= newCircBuff(filter->order / L);
  aMRFPtr->filter	= filter;
  aMRFPtr->L		= L;
  aMRFPtr->M		= M;
  aMRFPtr->modulo	= modvar;
	
  return(aMRFPtr);

}	/* END CREATEMRFILTER */

/* -------------------------------------------------------------------- */
static void setTimeDelay(size_t rate, size_t nTaps, int *sec, size_t *msec)
{
  /* Set up time lags due to filtering.  Theory says delay at input is
   * nTaps / (2 * L)
   * nTaps should be a multiple of 25 and 50.
   */
  *msec = 0;

  int L = 1;	// No actual interp here.
  int samples = nTaps / (2 * L);

  if (rate <= (size_t)cfg.HRTRate())
    *sec += (samples / rate);
  else
    // Why are the decimaters off by 1 sample?
    switch (rate)
      {
      case 50:		// nTaps / (2 * L), almost checks out.
        *sec += 0;
        *msec = (nTaps / 2) - 1;
        break;

      case 250:		// nTaps / (2 * L), almost checks out.
        *sec += 0;
        if (cfg.HRTRate() == Config::TwentyFive)
          *msec = 240;
        else
          *msec = 120;
        break;

      case 1000:		// nTaps / (2 * L), ????
        *sec += 0;
        *msec = 0;
//      *msec = 500;
        break;
      }
}	/* END SETTIMEDELAY */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\

	FILTERS.C                CQRobinson 7Jun91

	Source library for circular buffer and filter functions.

	Data structures are defined in FILTERS.H.

\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

/*=============== Circular Buffer Routines ==================================
-   The following routines provide easy interfaces for accesing
-		LIFO circular buffers.  Circular buffers are useful for a
-		variety of DSP applications, particularly FIR filters.
-   The following routines are included:
-       newCircBuff: allocates and inits memory for circular buffer struct.
-       disposCircBuff: deallocates structure memory.
-       putBuff: add an item to a circular buffer.
-       getBuff: retrieve an item from a circular buffer.
*/

/*---------------------------------------------------------------------------
-	circBuffPtr newCircBuff(int size)
-		Allocate new circular buffer of size and type specified.
*/
static circBuffPtr newCircBuff(int size)
{
  circBuffPtr 	aCBPtr;

  /* Check size  */
  if (size < 0)
  {
    puts("newCircBuff: parameter 'size' < 0");
    exit(0);
  }

  /* Allocate memory for structure.		*/
  aCBPtr = new circBuff;

  /* Allocate memory for data buffer.	*/
  aCBPtr->value = new NR_TYPE[size];
  aCBPtr->size  = size;

  return(aCBPtr);
}

/*---------------------------------------------------------------------------
-	circBuffPtr initCircBuff(circBuffPtr aCBPtr, int size)
-		Allocate new circular buffer of size and type specified.
*/
static void initCircBuff(circBuffPtr aCBPtr)
{
  NR_TYPE *buffer = aCBPtr->value;

  for (int i = 0; i < aCBPtr->size; i++)
    buffer[i] = 0.0;

  aCBPtr->index = 0;

}

/*--------------------------------------------------------------
-	void disposCircBuff(circBuffPtr aCBPtr)
-		Free memory of circular buffer specified.
*/
static void disposCircBuff(circBuffPtr aCBPtr)
{
  delete [] aCBPtr->value;
  delete aCBPtr;
}

/*------------------------------------------------------------
-	void putBuff(datum, aCBPtr)
-		Put datum into aCBPtr buffer.
*/
static void putBuff(NR_TYPE datum, circBuffPtr aCBPtr)
{
  /* Update index, wrap if necessary.	*/
  aCBPtr->index++;
  if ((aCBPtr->index >= aCBPtr->size) || (aCBPtr->index < 0))
    aCBPtr->index = 0;

  /*  Store value.                        */
  aCBPtr->value[aCBPtr->index] = datum;

}

/*------------------------------------------------------------------*/
static NR_TYPE getBuff(int offset, circBuffPtr aCBPtr)
/*
-	Return datum at offset 'offset' from aCBPtr buffer.
-	Note: getBuff does not remove items from buffer.
*/
{
  /* Calculate index from offset and index.	  */
  offset = (aCBPtr->index - offset) % aCBPtr->size;

  while (offset < 0)
    offset += aCBPtr->size;

  return(aCBPtr->value[offset]);
}

/*======================= Multi Rate Filter Routines =====================
-   The following routines implement a multi rate filter for
-       changing the sample rate of an input sample stream.
-   The process accepts an input stream, interpolates by L, filters,
-       then decimates by M.
-   The routines are implemented such that the interpolation and
-       decimation are incorperated into the filter so that multiplies
-       by zero (from interpolation) are avoided, and samples to be
-       thrown out (during decimation) are not computed.
-   The following routines are included:
-       initMultiRateFilter - To be called before any samples are computed.
-       iterateMultiRateFilter - To be called as samples are available for
-           input or output.
-       disposMultiRateFilter - Used to free up allocated memory.
-
-   For a copmlete discussion of multirate filtering techniques see
-       Crochiere, Ronald E. and Lawrence R. Rabiner. "Multirate
-       Digital Signal Processing", Prentice-Hall 1983.
*/

/*--------------------------------------------------------------------------
-   initMultiRateFilter
-       initialize MultiFilt Struct with filter data and instance vars.
*/
static void initMultiRateFilter(mRFilterPtr aMRFPtr)
{
  initCircBuff(aMRFPtr->inBuff);
  aMRFPtr->inTime	= 0;	/* Do not return for input on first iteration */
  aMRFPtr->outTime	= aMRFPtr->M; /* Return with output on first iteration*/
  aMRFPtr->coefPhase	= 0;		/* Start with zero phase offset.*/
  aMRFPtr->task		= GET_INPUT;	/* Start by collecting input.	*/

}

/*--------------------------------------------------------------------------
-   disposMultiRateFilter
-       delete MultiFilt Struct with filter data and instance vars.
*/
static int disposMultiRateFilter(mRFilterPtr aMRFPtr)
{
  if (aMRFPtr == 0)
    return(false);

  disposCircBuff(aMRFPtr->inBuff);
  aMRFPtr->inBuff = 0;
  delete aMRFPtr;

  return(true);
}

/*--------------------------------------------------------------------------
-   iterateMultiRateFilter
-       The multi rate filter collects input and generates output
-       at different rates.  Each call exacutes the filter algorithm
-       untill an input is required or an output is available.  The
-       return value specifies which condition is in effect.
*/
static int iterateMRFilter(mRFilterPtr thisMRF, NR_TYPE input, NR_TYPE *output)
{
  int		tap, i;	/* Indecies for coef and input data arrays.	*/
  filterType	result;	/* Temporary accumulator for output calc.	*/

  /* Input was just requested; here it is.	*/
  if (thisMRF->task == GET_INPUT)
    {
    putBuff(input, thisMRF->inBuff);
    thisMRF->coefPhase = 0;
    }


  /* Execute this loop until an input sample is needed,
   * or an output sample is available.
   */
  while (true)
    {
    /* Time to get new input sample?   */
    if (thisMRF->inTime >= thisMRF->L)
      {
      thisMRF->inTime = 0;
      return(thisMRF->task = GET_INPUT);
      }

    /* Time to calc output sample? 	*/
    if (thisMRF->outTime >= thisMRF->M)
      {
      thisMRF->outTime = 0;
      tap = thisMRF->coefPhase;

      /* Filter. */
      result = 0.0;

      if (thisMRF->modulo)
        {
        NR_TYPE	value;
        bool	low_value = false,
		high_value = false;

        for(i = 0; tap < thisMRF->filter->order; tap += thisMRF->L, i++)
          {
          value = getBuff(i, thisMRF->inBuff);

          if (value > thisMRF->modulo->bound[1])
            high_value = true;

          if (value < thisMRF->modulo->bound[0])
            low_value = true;
          }

        tap = thisMRF->coefPhase;

        if (low_value && high_value)
          {
          for (i = 0; tap < thisMRF->filter->order; tap += thisMRF->L, i++)
            {
            if ((value=getBuff(i, thisMRF->inBuff)) < thisMRF->modulo->bound[0])
              value += thisMRF->modulo->diff;

            result += thisMRF->filter->aCoef[tap] * (double)value;
            }
          }
        else
          {
          for(i = 0; tap < thisMRF->filter->order; tap += thisMRF->L, i++)
            result += thisMRF->filter->aCoef[tap] *
                    (double)getBuff(i, thisMRF->inBuff);
          }
        }
      else
        {
        for(i = 0; tap < thisMRF->filter->order; tap += thisMRF->L, i++)
          result += thisMRF->filter->aCoef[tap] *
                    (double)getBuff(i, thisMRF->inBuff);
        }

      *output = result;

      /*  Advance time.   */
      thisMRF->inTime++;
      thisMRF->outTime++;
      thisMRF->coefPhase++;

      return(thisMRF->task = OUT_AVAIL);
      }

    /*  Advance time.   */
    thisMRF->inTime++;
    thisMRF->outTime++;
    thisMRF->coefPhase++;
    }
}

/* -------------------------------------------------------------------- */
static NR_TYPE out[250];

char *GetProcessedBuffer(
	CircularBuffer *srt_cb,
	CircularBuffer *hrt_cb,
	int offset, var_base *vp)
{

  if (vp->SampleRate >= (size_t)cfg.HRTRate())
  {
    NR_TYPE *thisBuff = (NR_TYPE *)GetBuffer(srt_cb, offset);
    return (char *)&thisBuff[vp->SRstart];
  }
  else
  {
    NR_TYPE *thisBuff = (NR_TYPE *)GetBuffer(hrt_cb, offset);
    memcpy(out, &thisBuff[vp->HRstart], (size_t)cfg.HRTRate()*sizeof(NR_TYPE));
    return((char *)out);
  }
}

/* END FILTER.C */
