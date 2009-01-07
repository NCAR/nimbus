/*
-------------------------------------------------------------------------
OBJECT NAME:	grnd_feed.cc

FULL NAME:	UDP broadcast for Ground Feed.

DESCRIPTION:	Class to produce 1 per second ascii string to broadcast
		to ground, and send it to the dest host.

INPUT:		${PROJ_DIR}/###/groundvars

COPYRIGHT:	University Corporation for Atmospheric Research, 2005-08
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "grnd_feed.h"

#include "UdpSocket.h"

#include <sstream>
#include <bzlib.h>

//const int GroundFeed::GRND_UDP_PORT = 31007;
// eol-rt-data.guest.ucar.edu
const std::string GroundFeed::DEST_HOST_ADDR = "128.117.188.122";


/* -------------------------------------------------------------------- */
GroundFeed::GroundFeed(int rate) : UDP_Base(31007), _dataRate(rate)
{
  std::string fileName(XMIT_VARS);
  fileName += ".rt";
  _varList = readFile(fileName);

  setCoordinatesFrom(_varList);

  _socket = new UdpSocket(UDP_PORT, DEST_HOST_ADDR.c_str());
  _socket->openSock(UDP_UNBOUND);

  // Initialize GroundSummedData
  //size_t counter = 0;
  for (size_t i = 0; i < _varList.size(); ++i)
  {
     for (size_t j = 0; j < _varList[i]->Length; j++)
     {
       //counter++;
       _summedData.push_back(0.0);
     }
  }
}

/* -------------------------------------------------------------------- */
void GroundFeed::setCoordinatesFrom(const std::vector<var_base *> & list) const
{
  std::vector<var_base *>::const_iterator it;
  for (it = list.begin(); it != list.end(); ++it)
  {
    if (strstr((*it)->name, "LON"))
      cfg.SetCoordLON((*it)->name);
    if (strstr((*it)->name, "LAT"))
      cfg.SetCoordLAT((*it)->name);
    // Don't set it to PALTF, we don't want an altitude in feet as the reference.
    if (strstr((*it)->name, "ALT") && strstr((*it)->name, "ALTF") == 0)
      cfg.SetCoordALT((*it)->name);
  }
}

/* -------------------------------------------------------------------- */
void GroundFeed::BroadcastData(const std::string & timeStamp) 
{
  static int rate_cntr = 0;

  if (cfg.GroundFeedType() != Config::UDP)
    return;

  if (rate_cntr++ < 120)  // Don't transmit the first couple minutes.
    return;

  extern NR_TYPE * AveragedData;

  // Accumulate data so we can send an average.
  size_t counter = 0;
  for (size_t i = 0; i < _varList.size(); ++i)
  {
    for (size_t j = 0; j < _varList[i]->Length; j++)
    {
      _summedData[counter++] += AveragedData[(_varList[i]->LRstart)+j];
    }
  }

  // Only send every so often.
  if ((rate_cntr % _dataRate) != 0)
    return;
 
  std::stringstream groundString;
  if (cfg.Aircraft() == Config::HIAPER)
    groundString << "GV";
  if (cfg.Aircraft() == Config::NRL_P3)
    groundString << "NRLP3";
  if (cfg.Aircraft() == Config::C130)
    groundString << "C130";

  groundString << "," << timeStamp;

  counter = 0;
  for (size_t i = 0; i < _varList.size(); ++i)
  {
    if (_varList[i]->Length > 1) 
    {
      groundString << ",'{" << _summedData[counter] / _dataRate;
      _summedData[counter++] = 0.0;
      for (size_t j = 1; j < _varList[i]->Length; j++) 
      {
         groundString << "," << _summedData[counter] / _dataRate;
         _summedData[counter++] = 0.0;
      }
      groundString << "}'";
    } else {
      groundString << "," << _summedData[counter] / _dataRate;
      _summedData[counter++] = 0.0;
    }
  }
  groundString << "\n";

  // compress the stream before sending it to the ground
  char buffer[32000];
  memset(buffer, 0, 32000);
  unsigned int bufLen = sizeof(buffer);
  int ret = BZ2_bzBuffToBuffCompress( buffer, &bufLen, (char *) groundString.str().c_str(),
                                      groundString.str().length(),9,0,0);
  if (ret < 0) {
    typedef struct {     // copied from bzlib.c
      FILE*     handle;
      char      buf[BZ_MAX_UNUSED];
      int       bufN;
      bool      writing;
      bz_stream strm;
      int       lastErr;
      bool      initialisedOk;
    } bzFile;
    bzFile b;
    b.lastErr = ret;
    int errnum;
    char msg[100];
    sprintf(msg, "Failed to compress the ground feed stream: %s\n", BZ2_bzerror(&b, &errnum) );
    LogMessage(msg);
    return;
  }
  _socket->writeSock(buffer, bufLen);

//  printf("\ncompressed %d -> %d\n", groundString.str().length(), bufLen);
  printf("GroundFeed: %s\n", groundString.str().c_str());

}
