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

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "filters.h"
#include "circbuff.h"

extern NR_TYPE	*SampledData, *HighRateData;

static mRFilterPtr	sdiFilters[MAX_SDI], rawFilters[MAX_RAW];
static filterData	TwoFiftyTo25, FiftyTo25, OneTo25, FiveTo25, TenTo25,
			TwentyFive, ThousandTo25;

static int	PSCBindex, currentHz, HzDelay;
static NR_TYPE	*inputRec;

static circBuffPtr	newCircBuff(int);

static int	mrf_debug = false;

static NR_TYPE	getBuff();
static void	initCircBuff(), disposCircBuff(), putBuff(),
		setTimeDelay(int, int *, int *), filterCounter(SDITBL *sp),
		LinearInterpAndSingleStageFilter(CircularBuffer *PSCB,
		 mRFilterPtr thisMRF, int inputRate, int SRstart, int HRstart),
		SingleStageFilter(CircularBuffer *PSCB, mRFilterPtr thisMRF,
		 int inputRate, int SRstart, int HRstart);

/* -------------------------------------------------------------------- */
void InitMRFilters()
{
  int	i;
  MOD	*mv_p;

  readFilters();

  /* Create filter Data for each variable
   */
  for (i = 0; i < nsdi; ++i)
    {
    /* Doesn't require filtering.
     */
    if (sdi[i]->DependedUpon == false && sdi[i]->OutputRate != HIGH_RATE)
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


  for (i = 0; i < nraw; ++i)
    {
    /* Doesn't require filtering.
     */
    if (raw[i]->DependedUpon == false && raw[i]->OutputRate != HIGH_RATE)
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
        rawFilters[i] = createMRFilter(1, 2, &TenTo25, mv_p);
        break;

      case 25:		/* Just filter	*/
        rawFilters[i] = NULL;
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
  int			i, OUTindex, task;
  SDITBL		*sp;
  RAWTBL		*rp;
  NR_TYPE		output;

  SampledData = (NR_TYPE *)GetBuffer(PSCB, -(PSCB->nbuffers - 1));

  for (i = 0; (sp = sdi[i]); ++i)
    {
    if (sdiFilters[i] == (mRFilterPtr)ERR)	/* Filtering not needed	*/
      continue;

    if (sp->SampleRate == ProcessingRate)
      {
      memcpy(	(char *)&HighRateData[sp->HRstart],
		(char *)&SampledData[sp->SRstart],
		NR_SIZE * HIGH_RATE);
      continue;
      }

    /* Counters shouldn't need to be filtered.  Applies averaging or
     * linear interpolation.
     */
    if (sp->type[0] == 'C')
      {
      filterCounter(sp);
      continue;
      }

    PSCBindex = -(PSCB->nbuffers - 1);
    setTimeDelay(sp->SampleRate, &PSCBindex, &HzDelay);
    inputRec = (NR_TYPE *)GetBuffer(PSCB, PSCBindex);

    if (sdiFilters[i]->task == GET_INPUT)
      {
      currentHz = HzDelay;
      ++PSCBindex;
      }
    else
      if ((currentHz = sp->SampleRate - 1 + HzDelay) >= sp->SampleRate)
        {
        ++PSCBindex;
        currentHz -= sp->SampleRate;
        }

    switch (sp->SampleRate)
      {
      case 1:
        LinearInterpAndSingleStageFilter(PSCB, sdiFilters[i],
				sp->SampleRate, sp->SRstart, sp->HRstart);
        break;

      case 5:
        LinearInterpAndSingleStageFilter(PSCB, sdiFilters[i],
				sp->SampleRate, sp->SRstart, sp->HRstart);
        break;

      case 50:
        SingleStageFilter(PSCB, sdiFilters[i],
				sp->SampleRate, sp->SRstart, sp->HRstart);
        break;

      case 250:
        SingleStageFilter(PSCB, sdiFilters[i],
				sp->SampleRate, sp->SRstart, sp->HRstart);
        break;

      case 1000:
        SingleStageFilter(PSCB, sdiFilters[i],
				sp->SampleRate, sp->SRstart, sp->HRstart);
        break;
      }
    }



  /* Do raw variables
   */
  for (i = 0; (rp = raw[i]); ++i)
    {
    if (rawFilters[i] == (mRFilterPtr)ERR)	/* Filtering not needed	*/
      continue;

    if (rawFilters[i] == NULL)
      {
      memcpy(	(char *)&HighRateData[rp->HRstart],
		(char *)&SampledData[rp->SRstart],
		NR_SIZE * rp->SampleRate * rp->Length);
      continue;
      }

    /* We don't support filtering vector data.
     */
    if (rp->Length > 1)
      continue;

    PSCBindex = -(PSCB->nbuffers - 1);
    setTimeDelay(rp->SampleRate, &PSCBindex, &HzDelay);
    inputRec = (NR_TYPE *)GetBuffer(PSCB, PSCBindex);

    if (rawFilters[i]->task == GET_INPUT)
      {
      currentHz = HzDelay;
      ++PSCBindex;
      }
    else
      if ((currentHz = rp->SampleRate - 1 + HzDelay) >= rp->SampleRate)
        {
        ++PSCBindex;
        currentHz -= rp->SampleRate;
        }

    switch (rp->SampleRate)
      {
      case 1:
        LinearInterpAndSingleStageFilter(PSCB, rawFilters[i],
				rp->SampleRate, rp->SRstart, rp->HRstart);
        break;

      case 5:
        LinearInterpAndSingleStageFilter(PSCB, rawFilters[i],
				rp->SampleRate, rp->SRstart, rp->HRstart);
        break;

      case 10:
        LinearInterpAndSingleStageFilter(PSCB, rawFilters[i],
				rp->SampleRate, rp->SRstart, rp->HRstart);
        break;

      case 50:
        SingleStageFilter(PSCB, rawFilters[i],
				rp->SampleRate, rp->SRstart, rp->HRstart);
        break;
      }

    }

}	/* END FILTER */

/* -------------------------------------------------------------------- */
static void filterCounter(SDITBL *sp)
{
  NR_TYPE *sdp = &SampledData[sp->SRstart];
  int		OUTindex;

  switch (sp->SampleRate)
    {
    case 50:
      for (OUTindex = 0; OUTindex < ProcessingRate; ++OUTindex)
        HighRateData[sp->HRstart + OUTindex] =
			sdp[OUTindex * 2] + sdp[OUTindex * 2 + 1];

      break;

    case 5:
      for (OUTindex = 0; OUTindex < ProcessingRate; ++OUTindex)
        HighRateData[sp->HRstart+OUTindex] = sdp[OUTindex / 5] / 5;

      break;

    case 1:
      for (OUTindex = 0; OUTindex < ProcessingRate; ++OUTindex)
        HighRateData[sp->HRstart+OUTindex] = sdp[0] / 25;

      break;

    case 250:
      for (OUTindex = 0; OUTindex < ProcessingRate; ++OUTindex)
        HighRateData[sp->HRstart + OUTindex] =
			sdp[OUTindex * 2] + sdp[OUTindex * 2 + 1];
			sdp[OUTindex * 2 + 2] + sdp[OUTindex * 2 + 3];
			sdp[OUTindex * 2 + 4];

      break;

    default:
      fprintf(stderr, "Rate [%d] not supported for Counters, exiting.\n", sp->SampleRate);
    exit(1);
    }

}	/* END FILTERCOUNTER */

/* -------------------------------------------------------------------- */
static void LinearInterpAndSingleStageFilter(
	CircularBuffer	*PSCB,
	mRFilterPtr	thisMRF,
	int		inputRate,
	int		SRstart,
	int		HRstart)
{
  int		task, OUTindex;
  NR_TYPE	output;
  NR_TYPE	input, diff;
  int		L = (ProcessingRate * thisMRF->M) / inputRate;
  int		intermedHzcnt = L - 1;

  input = inputRec[SRstart];

  for (OUTindex = 0; OUTindex < ProcessingRate; ++OUTindex)
    {
    do
      {
      task = iterateMRFilter(thisMRF, input, &output);

      if (task == GET_INPUT)
        {
        if (++intermedHzcnt == L)
          {
          if (++currentHz == inputRate)
            {
            currentHz = 0;
            inputRec=(NR_TYPE *)GetBuffer(PSCB,PSCBindex++);
            }

          intermedHzcnt = 0;

          if (currentHz == inputRate - 1)
            {
            NR_TYPE *tmpRec = (NR_TYPE *)GetBuffer(PSCB,PSCBindex);
            diff = (tmpRec[SRstart] - inputRec[SRstart+currentHz]) / L;
            }
          else
            diff = (inputRec[SRstart+currentHz+1] - inputRec[SRstart+currentHz]) / L;
          }

        input = inputRec[SRstart+currentHz] + (diff*intermedHzcnt);
        }
      }
    while (task == GET_INPUT);

    HighRateData[HRstart + OUTindex] = output;
    }

}

/* -------------------------------------------------------------------- */
static void SingleStageFilter(
	CircularBuffer	*PSCB,
	mRFilterPtr	thisMRF,
	int		inputRate,
	int		SRstart,
	int		HRstart)
{
  int		task, OUTindex;
  NR_TYPE	output;

  for (OUTindex = 0; OUTindex < ProcessingRate; ++OUTindex)
    {
    do
      {
      task = iterateMRFilter(thisMRF,inputRec[SRstart+currentHz],&output);

      if (task == GET_INPUT && ++currentHz == inputRate)
        {
        currentHz = 0;
        inputRec = (NR_TYPE *)GetBuffer(PSCB, PSCBindex++);
        }
      }
    while (task == GET_INPUT);

    HighRateData[HRstart + OUTindex] = output;
    }
}

/* -------------------------------------------------------------------- */
static void readAfilter(char *file, filterPtr daFilt)
{
	int		i;
	char	*nimbus = getenv("PROJ_DIR");
	char	*filter[1050];
	double	sum = 0.0;

	sprintf(buffer, "%s/defaults/filters/%s", nimbus, file);
	ReadTextFile(buffer, filter);

	daFilt->order = atoi(filter[0]);
	daFilt->aCoef = (filterType *)GetMemory(sizeof(filterType) * daFilt->order);

	for (i = 0; i < daFilt->order; ++i)
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
  readAfilter("50to25", &FiftyTo25);
  readAfilter("250to25", &TwoFiftyTo25);
  readAfilter("1000to25", &ThousandTo25);

}	/* END READFILTERS */

/* -------------------------------------------------------------------- */
void ClearMRFilters()
{
	int		i, j;

	for (i = 0; i < nsdi; ++i)
		if ((int)sdiFilters[i] > 0)
			initMultiRateFilter(sdiFilters[i]);

	for (i = 0; i < nraw; ++i)
		if ((int)rawFilters[i] > 0)
			initMultiRateFilter(rawFilters[i]);

}	/* END CLEARMRFILTER */

/* -------------------------------------------------------------------- */
static mRFilterPtr createMRFilter(int L, int M, filterPtr filter, MOD *modvar)
{
  mRFilterPtr	aMRFPtr;

  aMRFPtr		= (mRFilterPtr)GetMemory(sizeof(mRFilterData));
  aMRFPtr->inBuff	= newCircBuff(filter->order / L);
  aMRFPtr->filter	= filter;
  aMRFPtr->L		= L;
  aMRFPtr->M		= M;
  aMRFPtr->modulo	= modvar;
	
  return(aMRFPtr);

}	/* END CREATEMRFILTER */

/* -------------------------------------------------------------------- */
static void setTimeDelay(int rate, int *sec, int *msec)
{
  *msec = 0;

  /* Set up time lags due to filtering.  Don't ask me why (nTaps / 2) isn't
   * the exact delay.
   */
  switch (rate)
    {
    case 1:		// nTaps / (2 * L), checks out.
			//  Sort of, 3.5 would be optimal 7/14/04.
			// Adjusted # of taps to make it align
      *sec += 3;
      break;

    case 5:		// nTaps / (2 * L), checks out.
      *sec += 2;
      break;

    case 10:		// nTaps / (2 * L), doesn't check out (should be 2 sec).
      *sec += 1;
      break;

    case 25:		// nTaps / (2 * L), checks out.
      *sec += 2;
      break;

    case 50:		// nTaps / (2 * L), checks out.
      *sec += 1;
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
  aCBPtr = (circBuffPtr)GetMemory(sizeof(circBuff));

  /* Allocate memory for data buffer.	*/
  aCBPtr->value = (NR_TYPE *)GetMemory(sizeof(NR_TYPE) * size);
  aCBPtr->size  = size;

  return(aCBPtr);
}

/*---------------------------------------------------------------------------
-	circBuffPtr initCircBuff(circBuffPtr aCBPtr, int size)
-		Allocate new circular buffer of size and type specified.
*/
static void initCircBuff(circBuffPtr aCBPtr)
{
  NR_TYPE	*buffer;
  int		i;

  buffer = aCBPtr->value;

  for (i = 0; i < aCBPtr->size; i++)
    buffer[i] = 0.0;

  aCBPtr->index = 0;

}

/*--------------------------------------------------------------
-	void disposCircBuff(circBuffPtr aCBPtr)
-		Free memory of circular buffer specified.
*/
static void disposCircBuff(circBuffPtr aCBPtr)
{
  free((char *)aCBPtr->value);
  free((char *)aCBPtr);
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
  free((char *)aMRFPtr);

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

/* END FILTER.C */
