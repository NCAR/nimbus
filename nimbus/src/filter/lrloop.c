/*
-------------------------------------------------------------------------
OBJECT NAME:	lrloop.c

FULL NAME:	Main loop for low rate processing

ENTRY POINTS:	LowRateLoop()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		long beginning and ending times

OUTPUT:		

REFERENCES:	circbuff.c, adsIO.c, rec_decode.c, phase_shift.c average.c
		compute.c netcdf.c, timeseg.c

REFERENCED BY:	StartProcessing()

NOTE:		Changes here may also be required in hrloop.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/


#include "nimbus.h"
#include "decode.h"
#include "gui.h"
#include "circbuff.h"
#include "amlib.h"
#include <iostream>
#include <string.h>
#include "injectsd.h"



#define NBUFFERS	5
#define INDEX		-(NBUFFERS-2)

extern SyntheticData sd;

extern char		*ADSrecord;
extern NR_TYPE		*SampledData, *AveragedData;
extern XtAppContext	context;


void	DespikeData(CircularBuffer *LRCB, int index),
	PhaseShift(CircularBuffer  *LRCB, int index, NR_TYPE *output);  


/* -------------------------------------------------------------------- */
int LowRateLoop(long starttime, long endtime)
{
  int			i, cntr;
  long			nBytes;
  NR_TYPE		*BuffPtr;
  CircularBuffer	*LRCB;	/* Logical Record Circular Buffers	*/
  Hdr_blk *h = (Hdr_blk*)ADSrecord;


  /* Account for Circular Buffer slop	*/
  if (starttime != BEG_OF_TAPE)
    starttime -= 2;

  if (endtime != END_OF_TAPE)
    endtime += -(INDEX+1);

  nBytes = nFloats * NR_SIZE;
  if ((LRCB = CreateCircularBuffer(NBUFFERS, nBytes)) == NULL)
    {
    nBytes = ERR;
    goto exit;
    }


  /* Perform initialization before entering main loop.
   */
  if ((nBytes = FindFirstLogicalRecord(ADSrecord, starttime)) <= 0)
    goto exit;

  /* Now make sure we have at least 7 contigous records.
   */
  for (i = 0, cntr = 0; i < NBUFFERS+3; ++i, ++cntr)
    {
printf("%d:%d:%d\n", ntohs(h->hour), ntohs(h->minute), ntohs(h->second));
    if (CheckForTimeGap((Hdr_blk *)ADSrecord, true) == GAP_FOUND)
      { i = 0; printf("reset, cntr=%d\n", cntr); }

    nBytes = FindNextLogicalRecord(ADSrecord, endtime);
    }

printf("cntr=%d\n", cntr);
  cntr -= NBUFFERS-1;

  nBytes = FindFirstLogicalRecord(ADSrecord, starttime);
  for (i = 0; i < cntr; ++i)
    nBytes = FindNextLogicalRecord(ADSrecord, endtime);

  ResetTimeGapper();
  printf("%d:%d:%d\n", ntohs(h->hour), ntohs(h->minute), ntohs(h->second));

  SetBaseTime((Hdr_blk *)ADSrecord);		/* See netcdf.c	*/


  /* Fill circular Buffers
   */
  BuffPtr = (NR_TYPE *)AddToCircularBuffer(LRCB);
  DecodeADSrecord((short *)ADSrecord, BuffPtr);
  ApplyCalCoes(BuffPtr);

  for (i = 1; i < NBUFFERS-1; ++i)
    {
    if ((nBytes = FindNextLogicalRecord(ADSrecord, endtime)) <= 0)
      goto exit;

    if (CheckForTimeGap((Hdr_blk *)ADSrecord, false) == GAP_FOUND)
      goto exit;

    BuffPtr = (NR_TYPE *)AddToCircularBuffer(LRCB);
    DecodeADSrecord((short *)ADSrecord, BuffPtr);
    ApplyCalCoes(BuffPtr);
    }
 
/* should this be in inject constructor? */
 timeindex[0] = raw[SearchTable((char **)raw, nraw, "HOUR")]->SRstart;
 timeindex[1] = raw[SearchTable((char **)raw, nraw, "MINUTE")]->SRstart;
 timeindex[2] = raw[SearchTable((char **)raw, nraw, "SECOND")]->SRstart;

  /* This is the main loop.
   */
  while ((nBytes = FindNextLogicalRecord(ADSrecord, endtime)) > 0)
    {
    if (CheckForTimeGap((Hdr_blk *)ADSrecord, false) == GAP_FOUND)
      break;

    BuffPtr = (NR_TYPE *)AddToCircularBuffer(LRCB);
    DecodeADSrecord((short *)ADSrecord, BuffPtr);
    ApplyCalCoes(BuffPtr);

    DespikeData(LRCB, INDEX+1);
    PhaseShift(LRCB, INDEX, SampledData);

    AverageSampledData();
   
    if (SynthData==true)   //checks to see if the user has chosen to use synthetic data 
      {
/* and this in the Inject fn? */
	hr = (int)SampledData[timeindex[0]];
	mins = (int)SampledData[timeindex[1]];
	sec = (int)SampledData[timeindex[2]];
	temptime=(hr*3600)+(mins*60)+sec;

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

/* END LRLOOP.C */

