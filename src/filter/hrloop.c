/*
-------------------------------------------------------------------------
OBJECT NAME:	hrloop.c

FULL NAME:	Main loop for high rate processing

ENTRY POINTS:	HighRateLoop()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		long beginning and ending times

OUTPUT:		

REFERENCES:	circbuff.c, adsIO.c, rec_decode.c, phase_shift.c, compute.c,
		filter.c, timeseg.c, netcdf.c

REFERENCED BY:	StartProcessing()

NOTE:		Changes here may also be required in lrloop.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "gui.h"
#include "circbuff.h"

#define NLRBUFFERS	5	/* Number of LR Buffers			*/
#define NPSBUFFERS	50

#define LRINDEX		-(NLRBUFFERS-2)

extern char		*ADSrecord;
extern NR_TYPE		*SampledData, *AveragedData;
extern XtAppContext	context;

void	Filter(CircularBuffer *),
        DespikeData(CircularBuffer *LRCB, int index),
        PhaseShift(CircularBuffer  *LRCB, int index, NR_TYPE *output);


/* -------------------------------------------------------------------- */
int HighRateLoop(long starttime, long endtime)
{
  int			i, j = 0;
  long			nbytes;
  NR_TYPE		*ps_data;
  CircularBuffer	*LRCB;	/* Logical Record Circular Buffers	*/
  CircularBuffer	*PSCB;	/* Phase Shifted Circular Buffers	*/


  /* Account for Circular Buffer slop	*/
  if (starttime != BEG_OF_TAPE)
    starttime -= 4 + 10;

  if (endtime != END_OF_TAPE)
    endtime += NPSBUFFERS-1;

  nbytes	= nFloats * NR_SIZE;


  if ((LRCB = CreateCircularBuffer(NLRBUFFERS, nbytes)) == NULL ||
      (PSCB = CreateCircularBuffer(NPSBUFFERS, nbytes)) == NULL)
    {
    nbytes = ERR;
    goto exit;
    }


  /* Perform initialization before entering main loop.
   */
  if ((nbytes = FindFirstLogicalRecord(ADSrecord, starttime)) <= 0)
    goto exit;

  ClearMRFilters();
  SetBaseTime((Hdr_blk *)ADSrecord);		/* See netcdf.c	*/


  /* Fill circular Buffers
   */
  SampledData = (NR_TYPE *)AddToCircularBuffer(LRCB);
  DecodeADSrecord((short *)ADSrecord, SampledData);

  for (i = 0; i < NLRBUFFERS-1; ++i)
    {
    if ((nbytes = FindNextLogicalRecord(ADSrecord, endtime)) <= 0)
      goto exit;

    if (CheckForTimeGap((Hdr_blk *)ADSrecord, False) == GAP_FOUND)
      goto exit;

    SampledData = (NR_TYPE *)AddToCircularBuffer(LRCB);
    DecodeADSrecord((short *)ADSrecord, SampledData);
    ApplyCalCoes(SampledData);
    }

  /* Fill PhaseShifted Buffers for MultiRate
   */
  for (i = 0; i < NPSBUFFERS-1; ++i)
    {
    if ((nbytes = FindNextLogicalRecord(ADSrecord, endtime)) <= 0)
      goto exit;

    if (CheckForTimeGap((Hdr_blk *)ADSrecord, False) == GAP_FOUND)
      goto exit;

    SampledData = (NR_TYPE *)AddToCircularBuffer(LRCB);
    DecodeADSrecord((short *)ADSrecord, SampledData);
    ApplyCalCoes(SampledData);
    DespikeData(LRCB, LRINDEX+1);

    ps_data = (NR_TYPE *)AddToCircularBuffer(PSCB);
    PhaseShift(LRCB, LRINDEX, ps_data);
    }



  /* This is the main control loop.
   */
  while ((nbytes = FindNextLogicalRecord(ADSrecord, endtime)) > 0)
    {
    if (CheckForTimeGap((Hdr_blk *)ADSrecord, False) == GAP_FOUND)
      break;

    SampledData = (NR_TYPE *)AddToCircularBuffer(LRCB);
    DecodeADSrecord((short *)ADSrecord, SampledData);
    ApplyCalCoes(SampledData);
    DespikeData(LRCB, LRINDEX+1);

    ps_data = (NR_TYPE *)AddToCircularBuffer(PSCB);
    PhaseShift(LRCB, LRINDEX, ps_data);

    Filter(PSCB);

    if (j++ < 10)	/* Skip 1st 10 passes, to help load things up. */
      continue;

    AverageSampledData();
    ComputeLowRateDerived();
    ComputeHighRateDerived();

    WriteNetCDF_MRF();
    UpdateTime(SampledData);

    while (PauseFlag == True)
      XtAppProcessEvent(context, XtIMAll);

    if (PauseWhatToDo == P_QUIT)
      {
      nbytes = ERR;
      break;
      }
    }


exit:
  ReleaseCircularBuffer(PSCB);
  ReleaseCircularBuffer(LRCB);

  return(nbytes);

}	/* END HIGHRATELOOP */

/* END HRLOOP.C */
