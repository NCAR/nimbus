/*
-------------------------------------------------------------------------
OBJECT NAME:	hrloop.c

FULL NAME:	Main loop for high rate processing

ENTRY POINTS:	HighRateLoop()

STATIC FNS:	none

DESCRIPTION:	

REFERENCES:	circbuff.c, adsIO.c, rec_decode.c, phase_shift.c, compute.c,
		filter.c, timeseg.c, netcdf.c

REFERENCED BY:	StartProcessing()

NOTE:		Changes here may also be required in lrloop.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-05
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "gui.h"
#include "circbuff.h"
#include "amlib.h"
#include "injectsd.h"

#define NPSBUFFERS	20

extern SyntheticData sd;

extern char		*ADSrecord;
extern NR_TYPE		*SampledData, *AveragedData;
extern XtAppContext	context;

bool	LocateFirstRecord(long starttime, long endtime, int nBuffers);
void	Filter(CircularBuffer *, CircularBuffer *),
        DespikeData(CircularBuffer *LRCB, int index),
        PhaseShift(	CircularBuffer  *LRCB, int index,
			NR_TYPE *output, NR_TYPE *hout);


/* -------------------------------------------------------------------- */
int HighRateLoop(long startTime, long endTime)
{
  int		j = 0, cntr = 0;
  long		nBytes, thisTime;
  NR_TYPE	*ps_data, *hrt_data;

  /* Basic circ-buff of records going into despiking and resampler.
   */
  CircularBuffer	*LRCB;	/* Logical Record Circular Buffers	*/

  /* Records coming out of resampler.  2 circ buffs, one for SampleRate
   * Rate data and the other for HRT data.  PSCB will be FIR filtered
   * into HSCB.
   */
  CircularBuffer	*PSCB;	/* Phase Shifted Circular Buffers	*/
  CircularBuffer	*HSCB;	/* 25Hz resampled data (interped only).*/

  nBytes = nSRfloats * sizeof(NR_TYPE);
  if ((LRCB = CreateCircularBuffer(NLRBUFFERS, nBytes)) == NULL ||
      (PSCB = CreateCircularBuffer(NPSBUFFERS, nBytes)) == NULL)
    {
    nBytes = ERR;
    goto exit;
    }

  nBytes = nHRfloats * sizeof(NR_TYPE);
  if ((HSCB = CreateCircularBuffer(NPSBUFFERS, nBytes)) == NULL)
    {
    nBytes = ERR;
    goto exit;
    }

  /* Perform initialization before entering main loop.
   */
  if (LocateFirstRecord(startTime, endTime, NLRBUFFERS) == false)
    {
    nBytes = ERR;
    goto exit;
    }

  ClearMRFilters();


  /* This is the main control loop.
   */
  do
    {
    if (CheckForTimeGap(ADSrecord, false) == GAP_FOUND)
      break;

    SampledData = (NR_TYPE *)AddToCircularBuffer(LRCB);
    DecodeADSrecord((short *)ADSrecord, SampledData);
    ApplyCalCoes(SampledData);
    DespikeData(LRCB, LRINDEX+1);

    ps_data = (NR_TYPE *)AddToCircularBuffer(PSCB);
    hrt_data = (NR_TYPE *)AddToCircularBuffer(HSCB);
    PhaseShift(LRCB, LRINDEX, ps_data, hrt_data);

    Filter(PSCB, HSCB);

    AverageSampledData();

    thisTime = SampledDataTimeToSeconds();

    if (SynthData == true)
      sd.InjectSyntheticData(thisTime); 

    ComputeLowRateDerived();
    ComputeHighRateDerived();

    if ((startTime == BEG_OF_TAPE && cntr++ > 30) ||
        (startTime != BEG_OF_TAPE && thisTime >= startTime))
      {
      WriteNetCDF_MRF();
      UpdateTime(SampledData);
      }

    while (PauseFlag == true)
      XtAppProcessEvent(context, XtIMAll);

    if (PauseWhatToDo == P_QUIT)
      {
      nBytes = ERR;
      break;
      }

    nBytes = (*FindNextLogicalRecord)(ADSrecord, END_OF_TAPE);
    }
  while ( nBytes > 0 && (endTime == END_OF_TAPE || thisTime < endTime) );


exit:
  ReleaseCircularBuffer(PSCB);
  ReleaseCircularBuffer(HSCB);
  ReleaseCircularBuffer(LRCB);

  return(nBytes);

}	/* END HIGHRATELOOP */

/* END HRLOOP.C */
