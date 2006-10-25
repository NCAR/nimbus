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
#include "gui.h"
#include "circbuff.h"
#include "amlib.h"
#include "injectsd.h"

extern SyntheticData sd;

extern char		*ADSrecord;
extern NR_TYPE		*SampledData, *AveragedData;
extern XtAppContext	context;

bool	LocateFirstRecord(long starttime, long endtime, int nBuffers);
void	DespikeData(CircularBuffer *LRCB, int index),
	PhaseShift(CircularBuffer  *LRCB, int index, NR_TYPE *output,
		NR_TYPE *);


/* -------------------------------------------------------------------- */
int LowRateLoop(long startTime, long endTime)
{
  long			nBytes, thisTime, cntr = 0;
  NR_TYPE		*BuffPtr;
  CircularBuffer	*LRCB;	/* Logical Record Circular Buffers	*/

  /* Perform initialization before entering main loop.
   */
  nBytes = nSRfloats * sizeof(NR_TYPE);
  if ((LRCB = CreateCircularBuffer(NLRBUFFERS, nBytes)) == NULL ||
      LocateFirstRecord(startTime, endTime, NLRBUFFERS) == false)
    {
    nBytes = ERR;
    goto exit;
    }


printf("%ld %ld\n", startTime, endTime);
  /* This is the main loop.
   */
  do
    {
    if (CheckForTimeGap(ADSrecord, false) == GAP_FOUND)
      break;

    BuffPtr = (NR_TYPE *)AddToCircularBuffer(LRCB);
    DecodeADSrecord((short *)ADSrecord, BuffPtr);
    ApplyCalCoes(BuffPtr);

    /* Despike 1 record ahead of what we will be working with (LRINDEX+1).
     */
    DespikeData(LRCB, LRINDEX+1);
    PhaseShift(LRCB, LRINDEX, SampledData, 0);

    AverageSampledData();
   
    thisTime = SampledDataTimeToSeconds();

    if (SynthData == true)
      sd.InjectSyntheticData(thisTime); 
    
    ComputeLowRateDerived();

    // cntr is to get us past buffer load-ups and goobery data at start of file.
    if (cntr++ > 30 &&
        (startTime == BEG_OF_TAPE || thisTime >= startTime))
      {
      WriteNetCDF();
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
static void displayTime(const void *record)
{
  if (cfg.isADS2())
  {
    const Hdr_blk *h = (Hdr_blk *)record;
    printf("%02d:%02d:%02d\n", ntohs(h->hour), ntohs(h->minute), ntohs(h->second));
  }
  else
  {
    const NR_TYPE *r = (NR_TYPE *)record;
    printf("%02d:%02d:%02d\n", (int)r[timeIndex[0]], (int)r[timeIndex[1]], (int)r[timeIndex[2]]);
  }
}

/* -------------------------------------------------------------------- */
static const int nConsecutive = 10;

bool LocateFirstRecord(long starttime, long endtime, int nBuffers)
{
  int i, nBytes, cntr = 0;

  if (starttime != BEG_OF_TAPE)
    {
    // Start 1 minute before user requested start time.
    if ((nBytes = (*FindFirstLogicalRecord)(ADSrecord, starttime-60)) <= 0)
      return false;

    return true;
    }


  if ((nBytes = (*FindFirstLogicalRecord)(ADSrecord, starttime)) <= 0)
    return false;

  return true;

}	/* END LOCATEFIRSTRECORD */

/* END LRLOOP.C */
