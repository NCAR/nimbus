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
int LowRateLoop(long starttime, long endtime)
{
  long			nBytes;
  NR_TYPE		*BuffPtr;
  CircularBuffer	*LRCB;	/* Logical Record Circular Buffers	*/

  /* Account for Circular Buffer slop	*/
  if (starttime != BEG_OF_TAPE)
  {
    starttime -= NLRBUFFERS/2;

    // Hack until we build sync_record into nimbus,  since we can't rewind() a
    // socket.
    if (cfg.isADS3())
      starttime -= 11;
  }

  if (endtime != END_OF_TAPE)
    endtime += -(LRINDEX+1);

  /* Perform initialization before entering main loop.
   */
  nBytes = nSRfloats * sizeof(NR_TYPE);
  if ((LRCB = CreateCircularBuffer(NLRBUFFERS, nBytes)) == NULL ||
      LocateFirstRecord(starttime, endtime, NLRBUFFERS) == false)
    {
    nBytes = ERR;
    goto exit;
    }


  /* Fill circular Buffers
   */
  BuffPtr = (NR_TYPE *)AddToCircularBuffer(LRCB);
  DecodeADSrecord((short *)ADSrecord, BuffPtr);
  ApplyCalCoes(BuffPtr);

  for (int i = 1; i < NLRBUFFERS-1; ++i)
    {
    if ((nBytes = (*FindNextLogicalRecord)(ADSrecord, endtime)) <= 0)
      goto exit;

    if (CheckForTimeGap(ADSrecord, false) == GAP_FOUND)
      goto exit;

    BuffPtr = (NR_TYPE *)AddToCircularBuffer(LRCB);
    DecodeADSrecord((short *)ADSrecord, BuffPtr);
    ApplyCalCoes(BuffPtr);
    }
 

  /* This is the main loop.
   */
  while ((nBytes = (*FindNextLogicalRecord)(ADSrecord, endtime)) > 0)
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
   
    if (SynthData == true)
      {
      int	hr, mins, sec;

      hr = (int)SampledData[timeIndex[0]];
      mins = (int)SampledData[timeIndex[1]];
      sec = (int)SampledData[timeIndex[2]];
      float temptime=(hr*3600)+(mins*60)+sec;

      sd.InjectSyntheticData(temptime); 
      }
    
    ComputeLowRateDerived();

    WriteNetCDF();
    UpdateTime(SampledData);

    while (PauseFlag == true)
      XtAppProcessEvent(context, XtIMAll);

    if (PauseWhatToDo == P_QUIT) {
      nBytes = ERR;
      break;
      }
    }


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

  if ((nBytes = (*FindFirstLogicalRecord)(ADSrecord, starttime)) <= 0)
    return false;

  /* Now make sure we have at least N contiguous records.
   */
  for (i = 0, cntr = 0; i < nConsecutive; ++i, ++cntr)
  {
    displayTime(ADSrecord);
    if (CheckForTimeGap(ADSrecord, true) == GAP_FOUND)
      { i = 0; printf("reset, cntr=%d\n", cntr); }

    nBytes = (*FindNextLogicalRecord)(ADSrecord, endtime);
  }

  cntr -= nConsecutive;

  nBytes = (*FindFirstLogicalRecord)(ADSrecord, starttime);
  for (i = 0; i < cntr; ++i)
    nBytes = (*FindNextLogicalRecord)(ADSrecord, endtime);

  ResetTimeGapper();
  displayTime(ADSrecord);

  SetBaseTime(ADSrecord);		/* See netcdf.c	*/

  return true;

}	/* END LOCATEFIRSTRECORD */

/* END LRLOOP.C */
