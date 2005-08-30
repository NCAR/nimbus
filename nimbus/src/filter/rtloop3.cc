#define RT_SQL

/**
 * This only affects writing of netCDF file.  File is still created
 * elsewhere.
*/
#define RT_NETCDF

/*
-------------------------------------------------------------------------
OBJECT NAME:    rtloop3.cc

FULL NAME:      RealTime Loop for ADS3

ENTRY POINTS:   RTinit_ADS3()
		RealTimeLoop_ADS3()

STATIC FNS:     none

DESCRIPTION:

COPYRIGHT:      University Corporation for Atmospheric Research, 2005
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "gui.h"
#include "vardb.h"
#include "psql.h"

#include <Xm/TextF.h>

#include <Socket.h>
#include <SyncRecordReader.h>

#include <sys/time.h>
#include <unistd.h>

#include <iomanip>

using namespace atdUtil;
using namespace dsm;

SyncRecordReader* syncRecReader = 0;

static const std::string DSMSERVER = "ac-server";
static const int DSMSERVERPORT = 30001;

static const std::string PGHOST = "ac-server";
static const std::string PGDATABASE = "real-time";
static const std::string PGUSER = "ads";

extern PostgreSQL *psql;

extern NR_TYPE	*SampledData, *AveragedData;

void processTimeADS3(NR_TYPE *output, time_t ut);


/* -------------------------------------------------------------------- */
void RTinit_ADS3()
{
printf("RTinit_ADS3, establishing connection....\n");

  atdUtil::Socket* sock = new atdUtil::Socket(DSMSERVER, DSMSERVERPORT);
  IOChannel* iochan = new dsm::Socket(sock);

  syncRecReader = new SyncRecordReader(iochan);

  cfg.SetADSVersion(Config::ADS_3);
  cfg.SetProcessingMode(Config::RealTime);
  cfg.SetInteractive(false);
  cfg.SetLoadProductionSetup(false);

  GetDataDirectory(buffer);
  strcat(buffer, "/real-time.nc");
  XmTextFieldSetString(outputFileText, buffer);
printf("netCDF file = %s\n", buffer);
}	/* END RTINIT_ADS3 */

/* -------------------------------------------------------------------- */
void RealTimeLoop3()
{
  struct timeval tv;
  double	ts, pts = 0.0;
  char		timeStamp[64];
  dsm_time_t	tt;

#ifdef RT_SQL
{
  std::string specifier;
  char *p;

  specifier = "host=";
  if ((p = getenv("PGHOST")) == 0)
    specifier += PGHOST;
  else
    specifier += p;

  specifier += " dbname=";
  if ((p = getenv("PGDATABASE")) == 0)
    specifier += PGDATABASE;
  else
    specifier += p;

  specifier += " user=";
  if ((p = getenv("PGUSER")) == 0)
    specifier += PGUSER;
  else
    specifier += p;

  psql = new PostgreSQL(specifier);
}
#endif

  for (;;)
  {
    syncRecReader->read(&tt, SampledData, nSRfloats);
    processTimeADS3(SampledData, tt / USECS_PER_SEC);

    time_t ut = tt / USECS_PER_SEC;
    struct tm tm;
    gmtime_r(&ut,&tm);
    int msec = (tt % USECS_PER_SEC) / USECS_PER_MSEC;
    strftime(timeStamp, sizeof(timeStamp), "%Y-%m-%d %H:%M:%S", &tm);
    std::cout << timeStamp << '.' << std::setw(3) << std::setfill('0') << msec <<
	' ' << std::endl;

gettimeofday(&tv, NULL);
ts = tv.tv_sec + ((double)tv.tv_usec/1000000);
if (ts-pts > 2.0)
  fprintf(stderr, "%s %ld.%ld   %lf\n", timeStamp, tv.tv_sec, tv.tv_usec, ts-pts);
pts = ts;

//    CheckForTimeGap((Hdr_blk *)ADSrecord, false);

//    DecodeADSrecord((short *)ADSrecord, SampledData);
    ApplyCalCoes(SampledData);
    AverageSampledData();
    ComputeLowRateDerived();
 
#ifdef RT_SQL
    psql->WriteSQL(timeStamp);
#endif

#ifdef RT_NETCDF
//    WriteNetCDF();
    WriteNetCDF_MRF();
#endif
    UpdateTime(SampledData);
#ifdef RT_NETCDF
    SyncNetCDF();
#endif

//gettimeofday(&tv, NULL);
//printf("%s  %d.%d\n", timeStamp, tv.tv_sec, tv.tv_usec); fflush(stdout);
  }

}	// END REALTIMELOOP3

// END RTLOOP3.CC
