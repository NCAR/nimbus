/*
-------------------------------------------------------------------------
OBJECT NAME:	lrloop.c

FULL NAME:	Main loop for low rate processing

ENTRY POINTS:	LowRateLoop()
		LocateFirstRecord()

STATIC FNS:	displayTime()

DESCRIPTION:	

REFERENCES:	circbuff.c, adsIO.c, rec_decode.c, phase_shift.c average.c
		compute.c netcdf.c, timeseg.c

REFERENCED BY:	StartProcessing()

NOTE:		Changes here may also be required in hrloop.c and/or rtloop.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-05
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "timeseg.h"
#include "gui.h"
#include "circbuff.h"
#include "NetCDF.h"
#include "injectsd.h"

#include "trace_variables.h"

#include "amlib.h"

extern SyntheticData sd;

extern char		*ADSrecord;
extern NR_TYPE		*SampledData, *AveragedData;
extern NetCDF	*ncFile;

bool	LocateFirstRecord(time_t starttime, time_t endtime, int nBuffers);
void	DespikeData(CircularBuffer *LRCB, int index), FindMinMax(),
	PhaseShift(CircularBuffer  *LRCB, int index, NR_TYPE *output,
		NR_TYPE *);


/* -------------------------------------------------------------------- */
int LowRateLoop(time_t startTime, time_t endTime)
{
  int32_t		nBytes, thisTime, cntr = 0;
  NR_TYPE		*BuffPtr;
  CircularBuffer	*LRCB = NULL;	// Logical Record Circular Buffers

  TraceVariables tv;

  /* Perform initialization before entering main loop.
   */
  nBytes = nSRfloats * sizeof(NR_TYPE);
  if ((LRCB = CreateCircularBuffer(NLRBUFFERS, nBytes)) == NULL ||
      LocateFirstRecord(startTime, endTime, NLRBUFFERS) == false)
    {
    nBytes = ERR;
    goto exit;
    }

  /* This is the main loop.
   */
  do
    {
    if (CheckForTimeGap(ADSrecord, false) == GAP_FOUND)
      break;

    BuffPtr = (NR_TYPE *)AddToCircularBuffer(LRCB);
    DecodeADSrecord((short *)ADSrecord, BuffPtr);
    tv.trace("after decoding", BuffPtr);
    ApplyCalCoes(BuffPtr);
    tv.trace("after calcoes", BuffPtr);

    /* Despike 1 record ahead of what we will be working with (LRINDEX+1).
     */
    DespikeData(LRCB, LRINDEX+1);
    tv.trace("after despike", BuffPtr);
    PhaseShift(LRCB, LRINDEX, SampledData, 0);
    tv.trace("after phaseshift", BuffPtr);

    AverageSampledData();
    tv.trace("after average", AveragedData, true);

    thisTime = SampledDataTimeToSeconds(SampledData);

    if (SynthData == true)
      sd.InjectSyntheticData(thisTime);

    ComputeLowRateDerived(thisTime);

    // cntr is to get us past buffer load-ups and goobery data at start of file.
    if (cntr++ > 30 &&
        (startTime == BEG_OF_TAPE || thisTime >= startTime))
      {
      FindMinMax();
      ncFile->WriteNetCDF();
      UpdateTime(SampledData);
      }

    while (PauseFlag == true)
      XtAppProcessEvent(context, XtIMAll);

    if (PauseWhatToDo == P_QUIT) {
      nBytes = ERR;
      break;
      }

    nBytes = (*FindNextLogicalRecord)(ADSrecord, END_OF_TAPE);
    }
  while ( nBytes > 0 && (endTime == END_OF_TAPE || thisTime < endTime) );

exit:
  ReleaseCircularBuffer(LRCB);

  return(nBytes);

}	/* END LOWRATELOOP */

/* -------------------------------------------------------------------- */
bool LocateFirstRecord(time_t starttime, time_t endtime, int nBuffers)
{
  int nBytes;

  if (starttime != BEG_OF_TAPE)
    {
    // Start 1 minute before user requested start time.
    if ((nBytes = (*FindFirstLogicalRecord)(ADSrecord, starttime-60)) <= 0)
      return false;
    }
  else
    if ((nBytes = (*FindFirstLogicalRecord)(ADSrecord, starttime)) <= 0)
      return false;

  return true;

}	/* END LOCATEFIRSTRECORD */

/* END LRLOOP.C */
