/*
-------------------------------------------------------------------------
OBJECT NAME:	filter.c

FULL NAME:	

ENTRY POINTS:	InitMRFilters()
		ClearMRFilters()
		Filter()

STATIC FNS:	See below.

DESCRIPTION:	The mrf filter has it's own circular buffer routines
		which are included in this module and are not associated
		with circbuff.[ch].  filters.c contains the coefficients.

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	interact.c mrf.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2005
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "filters.h"
#include "circbuff.h"

#include <cmath>

extern NR_TYPE	*SampledData, *HighRateData;

static std::vector<mRFilterPtr> sdiFilters;
static std::vector<mRFilterPtr> rawFilters;

static filterData	TwoFiftyTo25, FiftyTo25, OneTo25, FiveTo25, TenTo25,
			ThousandTo25, TwentyFive, vspd;

static size_t	PSCBindex, currentHz, HzDelay;
static size_t	filterRate;

static NR_TYPE	*inputRec;

static circBuffPtr	newCircBuff(int);

static int	disposMultiRateFilter(mRFilterPtr aMRFPtr);

static NR_TYPE	getBuff(int offset, circBuffPtr aCBPtr);
static void	initCircBuff(circBuffPtr aCBPtr),
		disposCircBuff(circBuffPtr aCBPtr),
		putBuff(NR_TYPE datum, circBuffPtr aCBPtr),
		setTimeDelay(size_t, size_t, size_t *, size_t *),
		filterCounter(SDITBL *sp),
		ProcessVariable(CircularBuffer *PSCB, var_base *vp, mRFilterPtr vpFilter),
		SingleStageFilter(CircularBuffer *PSCB, mRFilterPtr thisMRF,
			var_base *vp);

char *GetProcessedBuffer(CircularBuffer *cb, int offset, var_base *vp);


/* -------------------------------------------------------------------- */
void InitMRFilters()
{
  MOD	*mv_p;

  readFilters();

  /* Create filter Data for each variable
   */
  sdiFilters.resize(sdi.size());
  for (size_t i = 0; i < sdi.size(); ++i)
    {
    /* Doesn't require filtering.
     */
    if (sdi[i]->DependedUpon == false && sdi[i]->OutputRate != Config::HighRate)
      {
      sdiFilters[i] = (mRFilterPtr)ERR;
      continue;
      }

    if (sdi[i]->type[0] == 'C')
      sdiFilters[i] = NULL;

    mv_p = sdi[i]->Modulo;

    switch (sdi[i]->SampleRate)
      {
      /* For interpolation cases, L is set to 1 instead of what it really
       * is.  This is because we linear interpolate extra samples
       * instead of zero filling (which is what the filter would normally
       * do).
       */
      case 1:			/* Interpolate     L  M  */
        sdiFilters[i] = createMRFilter(1, 1, &OneTo25, mv_p);
        break;

      case 5:
        sdiFilters[i] = createMRFilter(1, 1, &FiveTo25, mv_p);
        break;

      case 25:		/* No filtering, just copy data */
        sdiFilters[i] = NULL;
        break;

      case 50:		/* Decimate        L  M  */
        sdiFilters[i] = createMRFilter(1, 2, &FiftyTo25, mv_p);
        break;

      case 250:
        sdiFilters[i] = createMRFilter(1, 10, &TwoFiftyTo25, mv_p);
        break;

      case 1000:
        sdiFilters[i] = createMRFilter(1, 40, &ThousandTo25, mv_p);
        break;

      default:
        sdiFilters[i] = (mRFilterPtr)ERR;
        fprintf(stderr, "mrfFilter: non-supported input rate, ");
        fprintf(stderr, "var=%s, rate=%d\n", sdi[i]->name, sdi[i]->SampleRate);
      }
    }


  rawFilters.resize(raw.size());
  for (size_t i = 0; i < raw.size(); ++i)
    {
    /* Doesn't require filtering.
     */
    if (raw[i]->DependedUpon == false && raw[i]->OutputRate != Config::HighRate)
      {
      rawFilters[i] = (mRFilterPtr)ERR;
      continue;
      }

    /* Can't filter Vectors.  And we don't want to filter PMS1D scalars.
     * This data will just be memcpy() into HighRateData[].
     */
    if (raw[i]->Length > 1 || raw[i]->ProbeType & PROBE_PMS1D)
      {
      rawFilters[i] = NULL;
      continue;
      }

    mv_p = raw[i]->Modulo;

    switch (raw[i]->SampleRate)
      {
      case 1:			/* Interpolate	*/
        rawFilters[i] = createMRFilter(1, 1, &OneTo25, mv_p);
        break;

      case 5:
        rawFilters[i] = createMRFilter(1, 1, &FiveTo25, mv_p);
        break;

      case 10:
        rawFilters[i] = createMRFilter(1, 1, &TenTo25, mv_p);
        break;

      case 25:		/* Just filter	*/
        rawFilters[i] = NULL;
        if (strncmp(raw[i]->name, "VSPD", 4) == 0)
          rawFilters[i] = createMRFilter(1, 1, &vspd, mv_p);
        break;

      case 50:		/* Decimate	*/
        rawFilters[i] = createMRFilter(1, 2, &FiftyTo25, mv_p);
        break;

      default:
        rawFilters[i] = (mRFilterPtr)ERR;
        fprintf(stderr, "mrfFilter: non-supported input rate, ");
        fprintf(stderr, "var=%s, rate=%d\n", raw[i]->name, raw[i]->SampleRate);
      }
    }

}	/* END INITMRFILTER */

/* -------------------------------------------------------------------- */
void Filter(CircularBuffer *PSCB)
{
  SampledData = (NR_TYPE *)GetBuffer(PSCB, -(PSCB->nbuffers - 1));

  /* Do Analog variables.
   */
  for (size_t i = 0; i < sdi.size(); ++i)
    {
    /* Counters shouldn't need to be filtered.  Applies averaging or
     * linear interpolation.
     */
    if (sdi[i]->type[0] == 'C' && sdi[i]->SampleRate != 25)
      {
      filterCounter(sdi[i]);
      continue;
      }

    ProcessVariable(PSCB, sdi[i], sdiFilters[i]);
    }

  /* Do raw variables
   */
  for (size_t i = 0; i < raw.size(); ++i)
    {
    /* We don't support filtering vector data.
     */
    if (raw[i]->Length > 1)
      continue;

    ProcessVariable(PSCB, raw[i], rawFilters[i]);
    }

}	/* FILTER */

/* -------------------------------------------------------------------- */
static void ProcessVariable(CircularBuffer *PSCB, var_base *vp, mRFilterPtr vpFilter)
{
  if (vpFilter == (mRFilterPtr)ERR)	/* Filtering not needed	*/
    return;

  if (vpFilter == 0)	/* Filtering not needed/wanted	*/
    {
    if (vp->SampleRate == (size_t)cfg.ProcessingRate())
      memcpy(	(char *)&HighRateData[vp->HRstart],
		(char *)&SampledData[vp->SRstart],
		NR_SIZE * vp->SampleRate * vp->Length);
    else
      memset(	(char *)&HighRateData[vp->HRstart], 0,
		NR_SIZE * vp->SampleRate * vp->Length);

    return;
    }

  /* Because data with SampleRate below 25 is linear interpolated first, we
   * can't use SampleRate, but must use 25.  Fake it with filterRate.
   */
  filterRate = vp->SampleRate < (size_t)cfg.ProcessingRate() ? (size_t)cfg.ProcessingRate() : vp->SampleRate;
  PSCBindex = -(PSCB->nbuffers - 1);
  setTimeDelay(filterRate, vpFilter->filter->order, &PSCBindex, &HzDelay);
  inputRec = (NR_TYPE *)GetProcessedBuffer(PSCB, PSCBindex, vp);

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

  SingleStageFilter(PSCB, vpFilter, vp);

}	/* END PROCESSVARIABLE */

/* -------------------------------------------------------------------- */
static void filterCounter(SDITBL *sp)
{
  NR_TYPE *sdp = &SampledData[sp->SRstart];
  size_t	OUTindex;

  switch (sp->SampleRate)
    {
    case 50:
      for (OUTindex = 0; OUTindex < (size_t)cfg.ProcessingRate(); ++OUTindex)
        HighRateData[sp->HRstart + OUTindex] =
			sdp[OUTindex * 2] + sdp[OUTindex * 2 + 1];

      break;

    case 5:
      for (OUTindex = 0; OUTindex < (size_t)cfg.ProcessingRate(); ++OUTindex)
        HighRateData[sp->HRstart+OUTindex] = sdp[OUTindex / 5] / 5;

      break;

    case 1:
      for (OUTindex = 0; OUTindex < (size_t)cfg.ProcessingRate(); ++OUTindex)
        HighRateData[sp->HRstart+OUTindex] = sdp[0] / 25;

      break;

    case 250:
      for (OUTindex = 0; OUTindex < (size_t)cfg.ProcessingRate(); ++OUTindex)
        HighRateData[sp->HRstart + OUTindex] =
			sdp[OUTindex * 2] + sdp[OUTindex * 2 + 1] +
			sdp[OUTindex * 2 + 2] + sdp[OUTindex * 2 + 3] +
			sdp[OUTindex * 2 + 4];

      break;

    default:
      fprintf(stderr, "Rate [%d] not supported for Counters, exiting.\n", sp->SampleRate);
    exit(1);
    }

}	/* END FILTERCOUNTER */

/* -------------------------------------------------------------------- */
static void SingleStageFilter(CircularBuffer *PSCB, mRFilterPtr thisMRF, var_base *vp)
{
  int		task;
  NR_TYPE	output;

  for (size_t OUTindex = 0; OUTindex < (size_t)cfg.ProcessingRate(); ++OUTindex)
    {
    do
      {
      task = iterateMRFilter(thisMRF, inputRec[currentHz], &output);

      if (task == GET_INPUT && ++currentHz == filterRate)
        {
        currentHz = 0;
        inputRec = (NR_TYPE *)GetProcessedBuffer(PSCB, PSCBindex++, vp);
        }
      }
    while (task == GET_INPUT);

    HighRateData[vp->HRstart + OUTindex] = output;
    }
}

/* -------------------------------------------------------------------- */
static void readAfilter(char *file, filterPtr daFilt)
{
  char	*nimbus = getenv("PROJ_DIR");
  char	*filter[1050];
  double	sum = 0.0;

  sprintf(buffer, "%s/defaults/filters/%s", nimbus, file);
  ReadTextFile(buffer, filter);

  daFilt->order = atoi(filter[0]);
  daFilt->aCoef = new filterType[daFilt->order];

  for (int i = 0; i < daFilt->order; ++i)
  {
    daFilt->aCoef[i] = atof(filter[i+1]);
    sum += daFilt->aCoef[i];
  }

  FreeTextFile(filter);

  printf("filter.c: filter sum of %s\t= %15.8lf\n", file, sum);

}	/* END READAFILTER */

/* -------------------------------------------------------------------- */
static void readFilters()
{
  readAfilter("1to25", &OneTo25);
  readAfilter("5to25", &FiveTo25);
  readAfilter("10to25", &TenTo25);
  readAfilter("25hz", &TwentyFive);
  readAfilter("VSPD", &vspd);
  readAfilter("50to25", &FiftyTo25);
  readAfilter("250to25", &TwoFiftyTo25);
  readAfilter("1000to25", &ThousandTo25);

}	/* END READFILTERS */

/* -------------------------------------------------------------------- */
void ClearMRFilters()
{
  for (size_t i = 0; i < sdi.size(); ++i)
    if ((int)sdiFilters[i] > 0)
      initMultiRateFilter(sdiFilters[i]);

  for (size_t i = 0; i < raw.size(); ++i)
    if ((int)rawFilters[i] > 0)
      initMultiRateFilter(rawFilters[i]);

}	/* END CLEARMRFILTER */

/* -------------------------------------------------------------------- */
static mRFilterPtr createMRFilter(int L, int M, filterPtr filter, MOD *modvar)
{
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
static void setTimeDelay(size_t rate, size_t nTaps, size_t *sec, size_t *msec)
{
  /* Set up time lags due to filtering.  nTaps should be a multiple of 25
   * (odd # taps ok i.e. nTaps=101).
   */
  *msec = 0;

  int L = 1;	// No actual interp here.
  int samples = nTaps / (2 * L);

  if (rate <= 25)
    *sec += (samples / rate);
  else
    // Why are the decimaters off by 1 sample?
    switch (rate)
      {
      case 50:		// nTaps / (2 * L), almost checks out.
        *sec += 0;
        *msec = 49;
        break;

      case 250:		// nTaps / (2 * L), almost checks out.
        *sec += 0;
        *msec = 240;
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
  if (aMRFPtr == NULL)
    return(false);

  disposCircBuff(aMRFPtr->inBuff);
  aMRFPtr->inBuff = NULL;
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
NR_TYPE polyinterp(NR_TYPE xa[], NR_TYPE ya[], int n, int x);

char *GetProcessedBuffer(CircularBuffer *cb, int offset, var_base *vp)
{
  NR_TYPE *prevBuff = (NR_TYPE *)GetBuffer(cb, offset-1);
  NR_TYPE *thisBuff = (NR_TYPE *)GetBuffer(cb, offset);
  NR_TYPE *nextBuff = (NR_TYPE *)GetBuffer(cb, offset+1);

  if (vp->SampleRate >= (size_t)cfg.ProcessingRate())
    return (char *)&thisBuff[vp->SRstart];

  /* Load up data. 1 sample from prev second and 1 from the next second also.
   */
  NR_TYPE x[cfg.ProcessingRate()+2], y[cfg.ProcessingRate()+2];
  float L = (float)cfg.ProcessingRate() / vp->SampleRate;

  x[0] = -L;
  y[0] = prevBuff[vp->SRstart + (vp->SampleRate-1)];

  for (size_t i = 0; i < vp->SampleRate; ++i)
  {
    x[i+1] = L * i;
    y[i+1] = thisBuff[vp->SRstart+i];
  }

  x[vp->SampleRate+1] = (float)cfg.ProcessingRate();
  y[vp->SampleRate+1] = nextBuff[vp->SRstart];

  /* We need 4 points, 1hz data only has 3 so for, get another.
   */
  if (vp->SampleRate == 1)
  {
    nextBuff = (NR_TYPE *)GetBuffer(cb, offset+2);
    x[vp->SampleRate+2] = (float)cfg.ProcessingRate()*2;
    y[vp->SampleRate+2] = nextBuff[vp->SRstart];
  }
  else
  {
    x[vp->SampleRate+2] = (float)cfg.ProcessingRate() + L;
    y[vp->SampleRate+2] = nextBuff[vp->SRstart+1];
  }

  /* Inerpolate data to 25hz.
   */
  for (size_t i = 0; i < (size_t)cfg.ProcessingRate(); ++i)
    out[i] = polyinterp(&x[(int)((float)i / L)], &y[(int)((float)i / L)], 4, i);

// Linear interp
//    out[i] =	y[(int)((float)i / L)+1] +
//		((y[(int)((float)i / L)+2] - y[(int)((float)i / L+1)]) *
//		((1.0 / L) * (i % (int)L)));

// poly inter /w whole second, 4 pts seemed better.
//    out[i] = polyinterp(x, y, vp->SampleRate+3, i);

  return (char *)out;
}

/* END FILTER.C */
