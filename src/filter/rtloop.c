/*
-------------------------------------------------------------------------
OBJECT NAME:    rtloop.c (PostgreS)

FULL NAME:      RealTime Loop

ENTRY POINTS:   RTinit_ADS2()
		RealTimeLoop()

STATIC FNS:     none

DESCRIPTION:	Realtime functions for processing and writing data for the
		ADS2 data system.

COPYRIGHT:      University Corporation for Atmospheric Research, 1997-2005
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "gui.h"
#include <raf/vardb.h>
#include "psql.h"
#include "brdcast.h"
#include "grnd_feed.h"

#include <Xm/TextF.h>

#include <raf/adsIOrt.h>

#include <sys/time.h>
#include <unistd.h>

extern PostgreSQL *psql;

static Broadcast *bcast;
static GroundFeed *grnd_feed;

extern NR_TYPE	*SampledData, *AveragedData;
extern char	*ADSrecord;

/* -------------------------------------------------------------------- */
void RTinit_ADS2()
{
  FILE  *fp = 0;
  char  *p, host[80];

  gethostname(host, 80);
  if ((p = strchr(host, '.')) ) *p = '\0';

  sprintf(buffer, "%s/hosts/%s/rtdata.filename", cfg.ProjectDirectory().c_str(), host);

  for (int j = 0; j < 60 && (fp = fopen(buffer, "r")) == NULL; ++j)
    sleep(1);

  if (fp == NULL)
  {
    fprintf(stderr, "nimbus -rt: Unable to open %s after 60 seconds, quitting\n", buffer);
    quit();
  }
  else
    fprintf(stderr, "nimbus -rt: opened %s for reading.\n", buffer);

  fgets(buffer, 1024, fp);
  fclose(fp);
  if ( (p = strchr(buffer, '\n')) )
    *p = '\0';

  strcat(buffer, ".ads");
  XmTextFieldSetString(aDSdataText, buffer);

  strcpy(strstr(buffer, ".ads"), ".nc");
  XmTextFieldSetString(outputFileText, buffer);
  fprintf(stderr, "nimbus -rt: realtime netCDF is: %s\n", buffer);

  cfg.SetProcessingMode(Config::RealTime);
  cfg.SetInteractive(false);
  cfg.SetLoadProductionSetup(false);
  cfg.SetCreateKMLFile(false);

}	/* END RTINIT_ADS2 */

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
  fprintf(stderr, "rtloop: entered ONBOARD_RT, going for 1st record in file, NOT last.\n");

  if (cfg.OutputSQL())
  {
    std::string BuildPGspecString();
    psql = new PostgreSQL(BuildPGspecString());
  }

  bcast = new Broadcast();

  if (cfg.TransmitToGround())
    grnd_feed = new GroundFeed(cfg.GroundFeedDataRate());

  do
    {
    hdr = (Hdr_blk *)ADSrecord;
    sprintf(timeStamp, "%04d%02d%02dT%02d%02d%02d",
		ntohs(hdr->year)+2000, ntohs(hdr->month), ntohs(hdr->day),
		ntohs(hdr->hour), ntohs(hdr->minute), ntohs(hdr->second));

gettimeofday(&tv, NULL);
ts = tv.tv_sec + ((double)tv.tv_usec/1000000);
if (ts-pts > 2.0)
  fprintf(stderr, "%s %ld.%ld   %lf\n", timeStamp, tv.tv_sec, tv.tv_usec, ts-pts);
pts = ts;

    CheckForTimeGap(ADSrecord, false);

    DecodeADSrecord((short *)ADSrecord, SampledData);
    ApplyCalCoes(SampledData);
    AverageSampledData();
    ComputeLowRateDerived(SampledDataTimeToSeconds(SampledData));
 
    if (cfg.OutputSQL())
      psql->WriteSQL(timeStamp);

    bcast->BroadcastData(timeStamp);

    if (cfg.TransmitToGround())
      grnd_feed->BroadcastData(timeStamp);

    if (cfg.OutputNetCDF())
      WriteNetCDF();

    UpdateTime(SampledData);

    if (cfg.OutputNetCDF())
      SyncNetCDF();

//gettimeofday(&tv, NULL);
//printf("%s  %d.%d\n", timeStamp, tv.tv_sec, tv.tv_usec); fflush(stdout);

    while (file->NextSyncRecord(ADSrecord) == false)
      usleep(50000);
    }
  while (1);

}	/* END REALTIMELOOP */

/* END RTLOOP.C */
