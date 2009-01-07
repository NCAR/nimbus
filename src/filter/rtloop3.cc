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
#include <raf/vardb.h>
#include "psql.h"
#include "brdcast.h"
#include "grnd_feed.h"

#include <Xm/TextF.h>

#include <nidas/dynld/raf/SyncRecordReader.h>
#include <nidas/util/Socket.h>
#include <nidas/core/Datagrams.h>

#include <ctime>
#include <unistd.h>

#include <iomanip>

static Broadcast *bcast;
static GroundFeed *grnd_feed;
extern PostgreSQL *psql;

extern NR_TYPE	*SampledData, *AveragedData;


class MultiCastStatus
{
public:
  MultiCastStatus();
  ~MultiCastStatus();

  void sendStatus(const char ts[]);

private:
  nidas::util::MulticastSocket msock;
  nidas::util::Inet4Address maddr;
  nidas::util::Inet4SocketAddress msaddr;

  static const std::string DATA_NETWORK;
};

// Can we use loopback on the gv server?
const std::string MultiCastStatus::DATA_NETWORK = "192.168.184";

MultiCastStatus::MultiCastStatus()
{
  maddr = nidas::util::Inet4Address::getByName(DSM_MULTICAST_ADDR);
  msaddr = nidas::util::Inet4SocketAddress(maddr,DSM_MULTICAST_STATUS_PORT);

  // Set to proper interface if this computer has more than one.
  std::list<nidas::util::Inet4Address> itf = msock.getInterfaceAddresses();
  std::list<nidas::util::Inet4Address>::iterator itfi;
  for (itfi = itf.begin(); itfi != itf.end(); ++itfi)
    if ((*itfi).getHostAddress().compare(0, DATA_NETWORK.size(), DATA_NETWORK) == 0)
      msock.setInterface(*itfi);
}

MultiCastStatus::~MultiCastStatus()
{
  msock.close();
}

void MultiCastStatus::sendStatus(const char timeStamp[])
{
  std::string statstr;
  statstr = "<?xml version=\"1.0\"?><group><name>nimbus</name><clock>";
  statstr += timeStamp;
  statstr += "</clock></group>\n";
  msock.sendto(statstr.c_str(), statstr.length()+1, 0, msaddr);
}

static MultiCastStatus * mcStat = 0;

static const int chk_lag_interval = 20; // seconds
static const int max_lag_delta = 10; // seconds

/* -------------------------------------------------------------------- */
void RTinit_ADS3()
{
  try {
    mcStat = new MultiCastStatus();
  }
  catch(const nidas::util::IOException& e) {
    mcStat = 0;
  }

  if (mcStat)
    mcStat->sendStatus("----- started -----");

printf("RTinit_ADS3, establishing connection....\n");

  cfg.SetADSVersion(Config::ADS_3);
  cfg.SetProcessingMode(Config::RealTime);
  cfg.SetOutputNetCDF(false);
  cfg.SetInteractive(false);
  cfg.SetLoadProductionSetup(false);
  cfg.SetCreateKMLFile(false);

  GetDataDirectory(buffer);
  strcat(buffer, "/real-time.nc");
  XmTextFieldSetString(outputFileText, buffer);
printf("netCDF file = %s\n", buffer);
}	/* END RTINIT_ADS3 */

/* -------------------------------------------------------------------- */
void RealTimeLoop3()
{
  char timeStamp[32];
  size_t cntr = 0;
  nidas::core::dsm_time_t tt;

  ILOG(("RealTimeLoop entered."));

  bcast = new Broadcast();	// ASCII/IWG1 and ground feed.
  grnd_feed = new GroundFeed(cfg.GroundFeedDataRate());	// UDP ground feed.

  if (cfg.OutputSQL())
  {
    std::string BuildPGspecString();
    psql = new PostgreSQL(BuildPGspecString(), cfg.TransmitToGround());
  }

  extern nidas::dynld::raf::SyncRecordReader* syncRecReader;

  for (;;)
  {
    syncRecReader->read(&tt, SampledData, nSRfloats);
    processTimeADS3(SampledData, tt / USECS_PER_SEC);

    time_t ut = tt / USECS_PER_SEC;
    struct tm tm;
    gmtime_r(&ut, &tm);
    strftime(timeStamp, sizeof(timeStamp), "%Y%m%dT%H%M%S", &tm);
//    std::cout << timeStamp << std::endl;

//    int msec = (tt % USECS_PER_SEC) / USECS_PER_MSEC;
//    std::cout << timeStamp << '.' << std::setw(3) << std::setfill('0') << msec
//	<< ' ' << std::endl;

    ApplyCalCoes(SampledData);
    AverageSampledData();
    ComputeLowRateDerived();
 
    if (cfg.OutputSQL())
      psql->WriteSQL(timeStamp);

    bcast->BroadcastData(timeStamp);
    grnd_feed->BroadcastData(timeStamp);

    if (cfg.OutputNetCDF())
      WriteNetCDF_MRF();

    UpdateTime(SampledData);
    if (mcStat)
      mcStat->sendStatus(timeStamp);

    // This typically produces HRT netCDF in real-time.  Not used at this time.
    if (cfg.OutputNetCDF())
      SyncNetCDF();

    /* Check every 20 seconds to see if we are lagging more than 10 seconds
     * behind the system clock.
     */
    if ((++cntr % 20) == 0)
    {
      time_t sys_time = time(0);

      if (abs(sys_time - ut) > max_lag_delta)
      {
        char msg[128];
        sprintf(msg, "WARNING: Time lag > 10 seconds; sys_clock=%ld, dsm_time=%ld !!!", sys_time, ut);
        WLOG((msg));
        fprintf(stderr, "\n%s\n\n", msg);
      }
    }
  }
}	// END REALTIMELOOP3

// END RTLOOP3.CC
