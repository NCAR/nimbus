#define SQL
/*
-------------------------------------------------------------------------
OBJECT NAME:    rtloop.c (PostgreS)

FULL NAME:      RealTime Loop

ENTRY POINTS:   RTinit()
		RealTimeLoop()

STATIC FNS:     none

DESCRIPTION:

REFERENCES:     none

REFERENCED BY:  rtplot.c

NOTES:		This can be compiled with #define MOCK_RT for use to fake
		realtime with an existing ADS image.  You must hard code
		ADS filename below for this to work.

COPYRIGHT:      University Corporation for Atmospheric Research, 1997-2003
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "gui.h"
#include "vardb.h"
#include "psql.h"

#include <Xm/TextF.h>

#include "adsIOrt.h"

#include <sys/time.h>
#include <unistd.h>

PostgreSQL	*psql;

extern NR_TYPE	*SampledData, *AveragedData;
extern char	*ADSrecord;

/* -------------------------------------------------------------------- */
void RTinit()
{
#ifdef MOCK_RT
  sprintf(buffer, "%s/mock_rt", "/home/tmp");
//  sprintf(buffer, "%s/mock_rt", getenv("DATA_DIR"));
#else
  FILE  *fp = 0;
  int   j;
  char  *p, host[80];

  gethostname(host, 80);
  if ((p = strchr(host, '.')) ) *p = '\0';

  sprintf(buffer, "%s/hosts/%s/rtdata.filename", ProjectDirectory, host);

  for (j = 0; j < 60 && (fp = fopen(buffer, "r")) == NULL; ++j)
    sleep(1);

  if (fp == NULL) {
    fprintf(stderr, "nimbus -rt: Unable to open %s after 30 seconds, quitting\n", buffer);
    exit(0);
    }
  else
   fprintf(stderr, "nimbus -rt: opened %s for reading.\n", buffer);

  fgets(buffer, 1024, fp);
  fclose(fp);
  if ( (p = strchr(buffer, '\n')) )
    *p = '\0';

#endif

  strcat(buffer, ".ads");
  XmTextFieldSetString(aDSdataText, buffer);

  strcpy(strstr(buffer, ".ads"), ".nc");
  XmTextFieldSetString(outputFileText, buffer);
  fprintf(stderr, "nimbus -rt: realtime netCDF is: %s\n", buffer);

  cfg.SetProcessingMode(Config::RealTime);
  cfg.SetInteractive(false);
  cfg.SetLoadProductionSetup(false);

}	/* END RTINIT */

/* -------------------------------------------------------------------- */
void RealTimeLoop()
{
  struct timeval tv;
  double	ts, pts = 0.0;
  Hdr_blk	*hdr;
  char		timeStamp[32];

  ADS_rtFile	*file = new ADS_rtFile();

//  file->LastSyncRecord(ADSrecord);
  file->FirstSyncRecord(ADSrecord);
printf("rtloop: entered ONBOARD_RT\n");

  /* wait for record and write base_time to netCDF file.
   */
  SetBaseTime((Hdr_blk *)ADSrecord);

#ifdef SQL
  psql = new PostgreSQL("");
#endif
//  GetUserTimeIntervals();
//  NextTimeInterval(&dummy[0], &dummy[1]);

  do
    {
    hdr = (Hdr_blk *)ADSrecord;
    sprintf(timeStamp, "%04d-%02d-%02d %02d:%02d:%02d",
		ntohs(hdr->year)+2000, ntohs(hdr->month), ntohs(hdr->day),
		ntohs(hdr->hour), ntohs(hdr->minute), ntohs(hdr->second));

gettimeofday(&tv, NULL);
ts = tv.tv_sec + ((double)tv.tv_usec/1000000);
if (ts-pts > 2.0)
  fprintf(stderr, "%s %d.%d   %lf\n", timeStamp, tv.tv_sec, tv.tv_usec, ts-pts);
pts = ts;

    CheckForTimeGap((Hdr_blk *)ADSrecord, false);

    DecodeADSrecord((short *)ADSrecord, SampledData);
    ApplyCalCoes(SampledData);
    AverageSampledData();
    ComputeLowRateDerived();
 
#ifdef SQL
    psql->WriteSQL(timeStamp);
#endif

    WriteNetCDF();
    UpdateTime(SampledData);
    SyncNetCDF();

//gettimeofday(&tv, NULL);
//printf("%s  %d.%d\n", timeStamp, tv.tv_sec, tv.tv_usec); fflush(stdout);

    while (file->NextSyncRecord(ADSrecord) == false)
      usleep(50000);
    }
  while (1);

}	/* END REALTIMELOOP */

/* END RTLOOP.C */
