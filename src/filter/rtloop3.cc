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
#include "psql.h"
#include "brdcast.h"
#include "grnd_feed.h"

#include <Xm/TextF.h>

#include <nidas/dynld/raf/SyncRecordReader.h>
#include <nidas/util/Socket.h>
#include <nidas/core/SocketAddrs.h>

#include <ctime>
#include <unistd.h>

#include <iomanip>

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
const std::string MultiCastStatus::DATA_NETWORK = "192.168.84";

MultiCastStatus::MultiCastStatus()
{
  maddr = nidas::util::Inet4Address::getByName(NIDAS_MULTICAST_ADDR);
  msaddr = nidas::util::Inet4SocketAddress(maddr,NIDAS_STATUS_PORT_UDP);

  // Set to proper interface if this computer has more than one.
  std::list<nidas::util::Inet4NetworkInterface> itf = msock.getInterfaces();
  std::list<nidas::util::Inet4NetworkInterface>::const_iterator itfi;
  for (itfi = itf.begin(); itfi != itf.end(); ++itfi)
    if (itfi->getAddress().getHostAddress().compare(0, DATA_NETWORK.size(), DATA_NETWORK) == 0)
      msock.setInterface(maddr,*itfi);
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
  try {
    msock.sendto(statstr.c_str(), statstr.length()+1, 0, msaddr);
  }
  catch (const nidas::util::IOException& ioe) {
    fprintf(stderr, "rtloop3::sendStatus: Network unreachable.\n");
  }
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

  ILOG(("RTinit_ADS3, establishing connection."));

  cfg.SetADSVersion(Config::ADS_3);
  cfg.SetProcessingMode(Config::RealTime);
  cfg.SetOutputNetCDF(false);
  cfg.SetInteractive(false);
  cfg.SetLoadProductionSetup(false);
  cfg.SetCreateKMLFile(false);

  GetDataDirectory(buffer);
  strcat(buffer, "/real-time.nc");
  XmTextFieldSetString(outputFileText, buffer);

  ILOG(("  netCDF file = %s", buffer));

}	/* END RTINIT_ADS3 */

/* -------------------------------------------------------------------- */
void RealTimeLoop3()
{
  char timeStamp[32];
  size_t cntr = 0;
  Broadcast *bcast;
  GroundFeed *grnd_feed;
  nidas::core::dsm_time_t tt;

  ILOG(("RealTimeLoop3 entered."));

  bcast = new Broadcast();	// ASCII/IWG1 and ground feed.

  if (cfg.TransmitToGround())
    grnd_feed = new GroundFeed(cfg.GroundFeedDataRate());	// UDP ground feed.

  if (cfg.OutputSQL())
  {
    std::string BuildPGspecString();
    psql = new PostgreSQL(BuildPGspecString());
  }

  extern nidas::dynld::raf::SyncRecordReader* syncRecReader;

  int nExpected = syncRecReader->getNumValues();

  for (;;)
  {
    int nRead;
    try {
        nRead = syncRecReader->read(&tt, SampledData, nSRfloats);
    }
    catch(const nidas::util::EOFException& e) {
      ILOG(("syncRecReader EOF"));
      break;
    }
    catch(const nidas::util::IOException& e) {
      PLOG(("syncRecReader error: %s",e.what()));
      break;
    }
    if (nRead != nExpected) {
      WLOG(("short record of ") << nRead << " floats instead of " << nExpected);
      for (int i = nRead; i < nExpected; i++) SampledData[i] = floatNAN;
    }
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
    ComputeLowRateDerived(SampledDataTimeToSeconds(SampledData));
 
    if (cfg.OutputSQL())
      psql->WriteSQL(timeStamp);

    bcast->BroadcastData(tt);

    UpdateTime(SampledData);
    if (mcStat) {
      strftime(timeStamp, sizeof(timeStamp), "%Y-%m-%d %H:%M:%S", &tm);
      mcStat->sendStatus(timeStamp);
    }

    if (cfg.TransmitToGround())
      grnd_feed->BroadcastData(tt);

    if (cfg.OutputNetCDF())
      WriteNetCDF();

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
