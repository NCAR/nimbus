/*
-------------------------------------------------------------------------
OBJECT NAME:	rtloop.c

FULL NAME:	RealTime Loop

ENTRY POINTS:	RealTimeLoop()

STATIC FNS:	WaitForNextLogicalRecord()
		OpenSocket()

DESCRIPTION:	

REFERENCES:	none

REFERENCED BY:	rtplot.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1997-9
-------------------------------------------------------------------------
*/

#include "define.h"
#include "decode.h"
#include "raf/Application.h"
#include "raf/adsIOrt.h"

#include <unistd.h>
#include <Xm/Xm.h>

int	Mode	= REALTIME;

extern Application    *application;

static void WaitForNextLogicalRecord(char dest[]);

extern ADS_rtFile	*file;

//extern "C" { int usleep(unsigned int); }

/* -------------------------------------------------------------------- */
void RealTimeLoop()
{
  application->FlushEvents();

  file = new ADS_rtFile();
  file->LastSyncRecord(ADSrecord);

  while (1)
    {
    WaitForNextLogicalRecord(ADSrecord);
    DecodeRecord();
    ApplyCalCoes(SampledData);

    switch (Units)
      {
      case COUNTS:
        AddDataToBuffer(bits);
        break;

      case VOLTS:
        AddDataToBuffer(volts);
        break;

      case ENGINEERING:
        AddDataToBuffer(SampledData);
        break;

      }

    AutoScale(SCALE_YAXIS);

    if (!Frozen)
      if (!FlashMode || Variable[0].buffIndex == 0)
        PlotData(NULL, NULL, NULL);

    application->FlushEvents();
    }

}	/* END REALTIMELOOP */

/* -------------------------------------------------------------------- */
static void WaitForNextLogicalRecord(char dest[])
{
  Hdr_blk *hdr = (Hdr_blk *)dest;

  while (file->NextSyncRecord(dest) == false)
    {
    usleep(100000);

    application->FlushEvents();
    }

  sprintf(buffer, "UTC (%02d:%02d:%02d)", ntohs(hdr->hour),
			ntohs(hdr->minute), ntohs(hdr->second));
  xlabel = buffer;

}	/* END WAITFORNEXTLOGICALRECORD */

/* END RTLOOP.C */
