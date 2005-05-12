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

#define NLRBUFFERS	5	/* Number of LR Buffers			*/
#define NPSBUFFERS	50

#define LRINDEX		-(NLRBUFFERS-2)

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
int HighRateLoop(long starttime, long endtime)
{
  int			j = 0;
  long			nBytes;
  NR_TYPE		*ps_data, *hrt_data;
  CircularBuffer	*LRCB;	/* Logical Record Circular Buffers	*/
  CircularBuffer	*PSCB;	/* Phase Shifted Circular Buffers	*/
  CircularBuffer	*HSCB;	/* 25Hz resampled data (interped only).*/

  /* Account for Circular Buffer slop	*/
  if (starttime != BEG_OF_TAPE)
    starttime -= 4 + 10;

  if (endtime != END_OF_TAPE)
    endtime += NPSBUFFERS-1;

  nBytes = nFloats * NR_SIZE;
  if ((LRCB = CreateCircularBuffer(NLRBUFFERS, nBytes)) == NULL ||
      (PSCB = CreateCircularBuffer(NPSBUFFERS, nBytes)) == NULL)
    {
    nBytes = ERR;
    goto exit;
    }

  nBytes = cfg.ProcessingRate() * NR_SIZE * (sdi.size() + raw.size());
  if ((HSCB = CreateCircularBuffer(NPSBUFFERS, nBytes)) == NULL)
    {
    nBytes = ERR;
    goto exit;
    }

  /* Perform initialization before entering main loop.
   */
  if (LocateFirstRecord(starttime, endtime, NLRBUFFERS) == false)
    {
    nBytes = ERR;
    goto exit;
    }

  ClearMRFilters();


  /* Fill circular Buffers
   */
  SampledData = (NR_TYPE *)AddToCircularBuffer(LRCB);
  DecodeADSrecord((short *)ADSrecord, SampledData);
  ApplyCalCoes(SampledData);

  for (int i = 0; i < NLRBUFFERS-1; ++i)
    {
    if ((nBytes = FindNextLogicalRecord(ADSrecord, endtime)) <= 0)
      goto exit;

    if (CheckForTimeGap((Hdr_blk *)ADSrecord, false) == GAP_FOUND)
      goto exit;

    SampledData = (NR_TYPE *)AddToCircularBuffer(LRCB);
    DecodeADSrecord((short *)ADSrecord, SampledData);
    ApplyCalCoes(SampledData);
    }

  /* Fill PhaseShifted Buffers for MultiRate
   */
  for (int i = 0; i < NPSBUFFERS-1; ++i)
    {
    if ((nBytes = FindNextLogicalRecord(ADSrecord, endtime)) <= 0)
      goto exit;

    if (CheckForTimeGap((Hdr_blk *)ADSrecord, false) == GAP_FOUND)
      goto exit;

    SampledData = (NR_TYPE *)AddToCircularBuffer(LRCB);
    DecodeADSrecord((short *)ADSrecord, SampledData);
    ApplyCalCoes(SampledData);
    DespikeData(LRCB, LRINDEX+1);

    ps_data = (NR_TYPE *)AddToCircularBuffer(PSCB);
    hrt_data = (NR_TYPE *)AddToCircularBuffer(HSCB);
    PhaseShift(LRCB, LRINDEX, ps_data, hrt_data);
    }

 timeindex[0] = raw[SearchTable(raw, "HOUR")]->SRstart;
 timeindex[1] = raw[SearchTable(raw, "MINUTE")]->SRstart;
 timeindex[2] = raw[SearchTable(raw, "SECOND")]->SRstart;

  /* This is the main control loop.
   */
  while ((nBytes = FindNextLogicalRecord(ADSrecord, endtime)) > 0)
    {
    if (CheckForTimeGap((Hdr_blk *)ADSrecord, false) == GAP_FOUND)
      break;

    SampledData = (NR_TYPE *)AddToCircularBuffer(LRCB);
    DecodeADSrecord((short *)ADSrecord, SampledData);
    ApplyCalCoes(SampledData);
    DespikeData(LRCB, LRINDEX+1);

    ps_data = (NR_TYPE *)AddToCircularBuffer(PSCB);
    hrt_data = (NR_TYPE *)AddToCircularBuffer(HSCB);
    PhaseShift(LRCB, LRINDEX, ps_data, hrt_data);

    Filter(PSCB, HSCB);

    if (j++ < 10)	/* Skip 1st 10 passes, to help load things up. */
      continue;

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
    ComputeHighRateDerived();

    WriteNetCDF_MRF();
    UpdateTime(SampledData);

    while (PauseFlag == true)
      XtAppProcessEvent(context, XtIMAll);

    if (PauseWhatToDo == P_QUIT)
      {
      nBytes = ERR;
      break;
      }
    }


exit:
  ReleaseCircularBuffer(PSCB);
  ReleaseCircularBuffer(HSCB);
  ReleaseCircularBuffer(LRCB);

  return(nBytes);

}	/* END HIGHRATELOOP */

/* END HRLOOP.C */
