/*
-------------------------------------------------------------------------
OBJECT NAME:	lrloop.c

FULL NAME:	Main loop for low rate processing

ENTRY POINTS:	LowRateLoop()

STATIC FNS:	none

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

#define NLRBUFFERS	5
#define INDEX		-(NLRBUFFERS-2)

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
    starttime -= NLRBUFFERS+1;

  if (endtime != END_OF_TAPE)
    endtime += -(INDEX+1);

  /* Perform initialization before entering main loop.
   */
  nBytes = nSRfloats * NR_SIZE;
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
    if ((nBytes = FindNextLogicalRecord(ADSrecord, endtime)) <= 0)
      goto exit;

    if (CheckForTimeGap((Hdr_blk *)ADSrecord, false) == GAP_FOUND)
      goto exit;

    BuffPtr = (NR_TYPE *)AddToCircularBuffer(LRCB);
    DecodeADSrecord((short *)ADSrecord, BuffPtr);
    ApplyCalCoes(BuffPtr);
    }
 
  timeindex[0] = raw[SearchTable(raw, "HOUR")]->SRstart;
  timeindex[1] = raw[SearchTable(raw, "MINUTE")]->SRstart;
  timeindex[2] = raw[SearchTable(raw, "SECOND")]->SRstart;

  /* This is the main loop.
   */
  while ((nBytes = FindNextLogicalRecord(ADSrecord, endtime)) > 0)
    {
    if (CheckForTimeGap((Hdr_blk *)ADSrecord, false) == GAP_FOUND)
      break;

    BuffPtr = (NR_TYPE *)AddToCircularBuffer(LRCB);
    DecodeADSrecord((short *)ADSrecord, BuffPtr);
    ApplyCalCoes(BuffPtr);

    /* Despike 1 record ahead of what we will be working with (INDEX+1).
     */
    DespikeData(LRCB, INDEX+1);
    PhaseShift(LRCB, INDEX, SampledData, 0);

    AverageSampledData();
   
    if (SynthData == true)
      {
      int	hr, mins, sec;

      hr = (int)SampledData[timeindex[0]];
      mins = (int)SampledData[timeindex[1]];
      sec = (int)SampledData[timeindex[2]];
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
bool LocateFirstRecord(long starttime, long endtime, int nBuffers)
{
  int nBytes, cntr = 0;
  Hdr_blk *h = (Hdr_blk*)ADSrecord;

  if ((nBytes = FindFirstLogicalRecord(ADSrecord, starttime)) <= 0)
    return false;

  /* Now make sure we have at least 7 contigous records.
   */
  for (int i = 0, cntr = 0; i < nBuffers+3; ++i, ++cntr)
    {
    printf("%d:%d:%d\n", ntohs(h->hour), ntohs(h->minute), ntohs(h->second));
    if (CheckForTimeGap((Hdr_blk *)ADSrecord, true) == GAP_FOUND)
      { i = 0; printf("reset, cntr=%d\n", cntr); }

    nBytes = FindNextLogicalRecord(ADSrecord, endtime);
    }

  cntr -= nBuffers-1;

  nBytes = FindFirstLogicalRecord(ADSrecord, starttime);
  for (int i = 0; i < cntr; ++i)
    nBytes = FindNextLogicalRecord(ADSrecord, endtime);

  ResetTimeGapper();
  printf("%d:%d:%d\n", ntohs(h->hour), ntohs(h->minute), ntohs(h->second));

  SetBaseTime((Hdr_blk *)ADSrecord);		/* See netcdf.c	*/

  return true;

}	/* END LOCATEFIRSTRECORD */

/* END LRLOOP.C */

