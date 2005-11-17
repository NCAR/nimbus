/*
-------------------------------------------------------------------------
OBJECT NAME:    rtloop3.cc

FULL NAME:      RealTime Loop for ADS3

ENTRY POINTS:   RTinit_ADS3()
		RealTimeLoop_ADS3()

DESCRIPTION:

COPYRIGHT:      University Corporation for Atmospheric Research, 2005
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "gui.h"
#include "vardb.h"
#include "psql.h"
#include "brdcast.h"

#include <Xm/TextF.h>

#include <SyncRecordReader.h>

#include <ctime>
#include <unistd.h>

#include <iomanip>

static const std::string PGHOST = "ac-server";
static const std::string PGDATABASE = "real-time";
static const std::string PGUSER = "ads";

static Broadcast * bcast;

extern PostgreSQL *psql;

extern NR_TYPE	*SampledData, *AveragedData;


/* -------------------------------------------------------------------- */
void RTinit_ADS3()
{
printf("RTinit_ADS3, establishing connection....\n");

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
  char timeStamp[64];
  dsm::dsm_time_t tt;

  if (cfg.OutputSQL())
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

    psql = new PostgreSQL(specifier, cfg.TransmitToGround());
  }

  bcast = new Broadcast();
  extern dsm::SyncRecordReader* syncRecReader;

  for (;;)
  {
    syncRecReader->read(&tt, SampledData, nSRfloats);
    processTimeADS3(SampledData, tt / USECS_PER_SEC);

    time_t ut = tt / USECS_PER_SEC;
    struct tm tm;
    gmtime_r(&ut, &tm);
    int msec = (tt % USECS_PER_SEC) / USECS_PER_MSEC;
    strftime(timeStamp, sizeof(timeStamp), "%Y-%m-%d %H:%M:%S", &tm);
    std::cout << timeStamp << '.' << std::setw(3) << std::setfill('0') << msec
	<< ' ' << std::endl;

    ApplyCalCoes(SampledData);
    AverageSampledData();
    ComputeLowRateDerived();
 
    if (cfg.OutputSQL())
      psql->WriteSQL(timeStamp);
    if (bcast)
      bcast->broadcastData(timeStamp);
    if (cfg.OutputNetCDF())
      WriteNetCDF_MRF();

    UpdateTime(SampledData);

    if (cfg.OutputNetCDF())
      SyncNetCDF();
  }

}	// END REALTIMELOOP3

// END RTLOOP3.CC
