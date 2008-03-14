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
static size_t	currentSampleOffset, sampleOffset;
static size_t	filterRate;

static NR_TYPE	*inputRec, *interpdData;

static circBuffPtr	newCircBuff(int);

static filterPtr readAfilter(char file[]);
static const void* BadFilter = (void*)1;

static int	disposMultiRateFilter(mRFilterPtr aMRFPtr);

static NR_TYPE	getBuff(int offset, circBuffPtr aCBPtr);
static void	initCircBuff(circBuffPtr aCBPtr),
		disposCircBuff(circBuffPtr aCBPtr),
		putBuff(NR_TYPE datum, circBuffPtr aCBPtr),
		ProcessVariable(CircularBuffer *, CircularBuffer *,
				RAWTBL *vp, mRFilterPtr vpFilter),
		SingleStageFilter(	CircularBuffer *, CircularBuffer *,
					mRFilterPtr thisMRF, var_base *vp);

char *GetProcessedBuffer(CircularBuffer *, CircularBuffer *, int offset, var_base *vp);


/* -------------------------------------------------------------------- */
void InitMRFilters()
{
  MOD	*mv_p;
  filterPtr fromOne, fromFive, fromEight, fromTen, fromThirteen, fromSixteen, 
    fromTwentyFive, fromFifty, fromOneHundred, fromTwoFifty, fromFiveHundred, 
    fromOneThousand, vspd, acins, gsf;
  char filterSuffix[16];
  char filterFileName[128];

  vspd = readAfilter("VSPD");
  gsf = readAfilter("GSF");
  acins = readAfilter("ACINS");

  int outRate = (int)cfg.HRTRate();

  //
  // Get filters for each in rate to the given out rate.  The filter files
  // should be named <in_rate>to<out_rate>, e.g., 50to250 for the 50 Hz to
  // 250 Hz upsampling filter.
  //
  sprintf(filterFileName, "1to%d", outRate);
  fromOne = (outRate == 1) ? 0 : readAfilter(filterFileName);

  sprintf(filterFileName, "5to%d", outRate);
  fromFive = (outRate == 5) ? 0 : readAfilter(filterFileName);

  sprintf(filterFileName, "8to%d", outRate);
  fromEight = (outRate == 8) ? 0 : readAfilter(filterFileName);

  sprintf(filterFileName, "10to%d", outRate);
  fromTen = (outRate == 10) ? 0 : readAfilter(filterFileName);

  sprintf(filterFileName, "13to%d", outRate);
  fromThirteen = (outRate == 13) ? 0 : readAfilter(filterFileName);

  sprintf(filterFileName, "16to%d", outRate);
  fromSixteen = (outRate == 16) ? 0 : readAfilter(filterFileName);

  sprintf(filterFileName, "25to%d", outRate);
  fromTwentyFive = (outRate == 25) ? 0 : readAfilter(filterFileName);

  sprintf(filterFileName, "50to%d", outRate);
  fromFifty = (outRate == 50) ? 0 : readAfilter(filterFileName);

  sprintf(filterFileName, "100to%d", outRate);
  fromOneHundred = (outRate == 100) ? 0 : readAfilter(filterFileName);

  sprintf(filterFileName, "250to%d", outRate);
  fromTwoFifty = (outRate == 250) ? 0 : readAfilter(filterFileName);

  sprintf(filterFileName, "500to%d", outRate);
  fromFiveHundred = (outRate == 500) ? 0 : readAfilter(filterFileName);

  sprintf(filterFileName, "1000to%d", outRate);
  fromOneThousand = (outRate == 1000) ? 0 : readAfilter(filterFileName);

  /* Create filter Data for each variable
   */
  rawFilters.resize(raw.size());
  for (size_t i = 0; i < raw.size(); ++i)
  {
    rawFilters[i] = 0;

    // All data being interped is linear interped before coming here, so L=1.
    size_t L = 1; // cfg.HRTRate() / raw[i]->SampleRate;
    size_t M = raw[i]->SampleRate / outRate;

    if (L < 1) L = 1;
    if (M < 1) M = 1;

    /* Doesn't require filtering (as in not output or used).
     */
    if (raw[i]->DependedUpon == false && raw[i]->OutputRate != (size_t)outRate)
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
        rawFilters[i] = createMRFilter(L, M, fromOne, mv_p);
        break;
      case 5:
        rawFilters[i] = createMRFilter(L, M, fromFive, mv_p);
        break;
      case 8:
        rawFilters[i] = createMRFilter(L, M, fromEight, mv_p);
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
          rawFilters[i] = createMRFilter(L, M, fromTen, mv_p);
        break;
      case 13:	// Really 12.5hz
//        rawFilters[i] = createMRFilter(L, M, fromThirteen, mv_p);
        rawFilters[i] = 0;
        break;
      case 16:
        rawFilters[i] = createMRFilter(L, M, fromSixteen, mv_p);
        break;
      case 25:		/* Just filter	*/
        if (cfg.Aircraft() == Config::C130 && strncmp(raw[i]->name, "VSPD", 4) == 0)
          rawFilters[i] = createMRFilter(L, M, vspd, mv_p);
        else
          rawFilters[i] = createMRFilter(L, M, fromTwentyFive, mv_p);
        break;
      case 50:		/* Decimate	*/
        if (cfg.Aircraft() == Config::C130 && strncmp(raw[i]->name, "ACINS", 5) == 0)
          rawFilters[i] = createMRFilter(L, M, acins, mv_p);
        else
          rawFilters[i] = createMRFilter(L, M, fromFifty, mv_p);
        break;
      case 100:
        rawFilters[i] = createMRFilter(L, M, fromOneHundred, mv_p);
        break;
      case 250:
        rawFilters[i] = createMRFilter(L, M, fromTwoFifty, mv_p);
        break;
      case 500:
        rawFilters[i] = createMRFilter(L, M, fromFiveHundred, mv_p);
        break;
      case 1000:
        rawFilters[i] = createMRFilter(L, M, fromOneThousand, mv_p);
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
				RAWTBL *vp, mRFilterPtr vpFilter)
{
  /* Because data with SampleRate below 25 is interpolated first, we
   * can't use SampleRate, but must use 25.  Fake it with filterRate.
   */
  bool downsampling = ((size_t)cfg.HRTRate() < vp->SampleRate);
  filterRate = downsampling ? vp->SampleRate : (size_t)cfg.HRTRate();

  if (vpFilter == 0)	/* Filtering not needed/wanted	*/
  {
    // how much space for one second of high-rate data?
    int oneSecondSize = sizeof(NR_TYPE) * cfg.HRTRate() * vp->Length;
    
    if (vp->SampleRate == (size_t)cfg.HRTRate())
      // SampleRate is same as HRT rate, just copy.
      memcpy((char *)&HighRateData[vp->HRstart],
	     (char *)&SampledData[vp->SRstart], oneSecondSize);
    else
    {
      if (downsampling)
	// zero out rates above HRT rate.  We could use the resampled data
	// that's there?
	memset((char *)&HighRateData[vp->HRstart], 0, oneSecondSize);
      else
	// Use interpolated data for vars below HRT rate.
	memcpy((char *)&HighRateData[vp->HRstart],
	       (char *)&interpdData[vp->HRstart], oneSecondSize);
    }
    return;
  }
  else if (vpFilter == (mRFilterPtr)BadFilter)
  {
    fprintf(stderr, "No good filter for %d Hz to %d Hz, variable %s!\n", vp->SampleRate,
	    cfg.HRTRate(), vp->name);
    return;
  }

  //
  // Set the input buffer index and downsampling sample offset
  //
  PSCBindex = 1 - PSCB->nbuffers;

  if (downsampling)
  {
    // When downsampling, grab the output sample (order/2) input samples
    // after the output sample time.
    sampleOffset = vpFilter->filter->order / 2;
  }
  else
  {
    sampleOffset = 0;
    PSCBindex += vpFilter->filter->order / (2 * cfg.HRTRate());
  }

  // XXXX When downsampling, something down the line seems to put us off 
  // by (one output sample period - one input sample period).  
  // Correct here (for now?).
  sampleOffset -= vp->SampleRate / cfg.HRTRate() - 1;


  inputRec = (NR_TYPE *)GetProcessedBuffer(PSCB, HSCB, PSCBindex, vp);

  if (vpFilter->task == GET_INPUT)
  {
    currentSampleOffset = sampleOffset;
    ++PSCBindex;
  }
  else if ((currentSampleOffset = filterRate - 1 + sampleOffset) >= filterRate)
  {
    ++PSCBindex;
    currentSampleOffset -= filterRate;
  }

  SingleStageFilter(PSCB, HSCB, vpFilter, vp);

  if (vp->type[0] == 'C')
  {
    if (vp->SampleRate < (size_t)cfg.HRTRate())
    {
      float L = (float)cfg.HRTRate() / vp->SampleRate;
      for (int i = 0; i < cfg.HRTRate(); ++i)
        HighRateData[vp->HRstart+i] /= L;
    }
    else
    if (vp->SampleRate > (size_t)cfg.HRTRate())
    {
      float M = (float)vp->SampleRate / cfg.HRTRate();
      for (int i = 0; i < cfg.HRTRate(); ++i)
        HighRateData[vp->HRstart+i] *= M;
    }
  }

}	/* END PROCESSVARIABLE */

/* -------------------------------------------------------------------- */
static void SingleStageFilter(CircularBuffer *PSCB, CircularBuffer *HSCB, mRFilterPtr thisMRF, var_base *vp)
{
  int		task;
  NR_TYPE	output;

  for (size_t OUTindex = 0; OUTindex < (size_t)cfg.HRTRate(); ++OUTindex)
  {
    do
    {
      task = iterateMRFilter(thisMRF, inputRec[currentSampleOffset], &output);

      if (task == GET_INPUT && ++currentSampleOffset == filterRate)
      {
        currentSampleOffset = 0;
        inputRec = (NR_TYPE *)GetProcessedBuffer(PSCB, HSCB, PSCBindex++, vp);
      }
    }
    while (task == GET_INPUT);

    HighRateData[vp->HRstart + OUTindex] = output;
  }
}

/* -------------------------------------------------------------------- */
//
// Return a pointer to a filter read from the given file, or 
// (filterPtr)BadFilter if the file cannot be opened or is otherwise
// bad.
//
static filterPtr readAfilter(char file[])
{
  char	*nimbus = getenv("PROJ_DIR");
  char	*filter[2000];
  double	sum = 0.0;

  sprintf(buffer, FILTERS.c_str(), nimbus, file);
  ReadTextFile(buffer, filter);

  if (filter[0] == 0)
    return (filterPtr)BadFilter;

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
    if (rawFilters[i] && rawFilters[i] != (mRFilterPtr)BadFilter)
      initMultiRateFilter(rawFilters[i]);

}	/* END CLEARMRFILTER */

/* -------------------------------------------------------------------- */
static mRFilterPtr createMRFilter(int L, int M, filterPtr filter, MOD *modvar)
{
  if (filter == 0)
    return 0;
  else if (filter == (filterPtr)BadFilter)
    return (mRFilterPtr)BadFilter;

  mRFilterPtr	aMRFPtr;

  aMRFPtr		= new mRFilterData;
  aMRFPtr->inBuff	= newCircBuff(filter->order / L);
  aMRFPtr->filter	= filter;
  aMRFPtr->L		= L;
  aMRFPtr->M		= M;
  aMRFPtr->modulo	= modvar;
	
  return(aMRFPtr);

}	/* END CREATEMRFILTER */

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
