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
#include <atdUtil/Socket.h>
#include <Datagrams.h>

#include <ctime>
#include <unistd.h>

#include <iomanip>

static Broadcast * bcast;

extern PostgreSQL *psql;

extern NR_TYPE	*SampledData, *AveragedData;


class MultiCastStatus
{
public:
  MultiCastStatus();
  ~MultiCastStatus();

  void sendStatus(const char ts[]);

private:
  atdUtil::MulticastSocket msock;
  atdUtil::Inet4Address maddr;
  atdUtil::Inet4SocketAddress msaddr;

  static const std::string DATA_NETWORK;
};

// Can we use loopback on the gv server?
const std::string MultiCastStatus::DATA_NETWORK = "192.168.184";

MultiCastStatus::MultiCastStatus()
{
  maddr = atdUtil::Inet4Address::getByName(DSM_MULTICAST_ADDR);
  msaddr = atdUtil::Inet4SocketAddress(maddr,DSM_MULTICAST_STATUS_PORT);

  // Set to proper interface if this computer has more than one.
  std::list<atdUtil::Inet4Address> itf = msock.getInterfaceAddresses();
  std::list<atdUtil::Inet4Address>::iterator itfi;
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
    std::string BuildPGspecString();
    psql = new PostgreSQL(BuildPGspecString(), cfg.TransmitToGround());
  }

  bcast = new Broadcast();	// ASCII feed.
  extern dsm::SyncRecordReader* syncRecReader;

  MultiCastStatus mcStat;

  for (;;)
  {
    syncRecReader->read(&tt, SampledData, nSRfloats);
    processTimeADS3(SampledData, tt / USECS_PER_SEC);

    time_t ut = tt / USECS_PER_SEC;
    struct tm tm;
    gmtime_r(&ut, &tm);
    strftime(timeStamp, sizeof(timeStamp), "%Y-%m-%d %H:%M:%S", &tm);
    std::cout << timeStamp << std::endl;

    mcStat.sendStatus(timeStamp);

//    int msec = (tt % USECS_PER_SEC) / USECS_PER_MSEC;
//    std::cout << timeStamp << '.' << std::setw(3) << std::setfill('0') << msec
//	<< ' ' << std::endl;

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
