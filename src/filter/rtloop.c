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
#include "psql.h"
#include "NetCDF.h"
#include "brdcast.h"
#include "grnd_feed.h"
#include "timeseg.h"

#include <Xm/TextF.h>

#include <raf/adsIOrt.h>

#include <sys/time.h>
#include <unistd.h>

extern PostgreSQL *psql;
extern NetCDF *ncFile;

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

  snprintf(buffer, 8192, "%s/hosts/%s/rtdata.filename", cfg.ProjectDirectory().c_str(), host);

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
    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    tm.tm_sec = ntohs(hdr->second);
    tm.tm_min = ntohs(hdr->minute);
    tm.tm_hour = ntohs(hdr->hour);
    tm.tm_year = ntohs(hdr->year) + 100;
    tm.tm_mon = ntohs(hdr->month) - 1;
    tm.tm_mday = ntohs(hdr->day);
    long long microsecs = mktime(&tm) * 1e6;
    snprintf(timeStamp, 32, "%04d%02d%02dT%02d%02d%02d",
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

    bcast->BroadcastData(microsecs);

    if (cfg.TransmitToGround())
      grnd_feed->BroadcastData(microsecs);

    if (cfg.OutputNetCDF())
      ncFile->WriteNetCDF();

    UpdateTime(SampledData);

    if (cfg.OutputNetCDF())
      ncFile->Sync();

//gettimeofday(&tv, NULL);
//printf("%s  %d.%d\n", timeStamp, tv.tv_sec, tv.tv_usec); fflush(stdout);

    while (file->NextSyncRecord(ADSrecord) == false)
      usleep(50000);
  }
  while (1);

}	/* END REALTIMELOOP */

/* END RTLOOP.C */
