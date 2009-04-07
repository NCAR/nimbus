/*
-------------------------------------------------------------------------
OBJECT NAME:	brdcst.cc

FULL NAME:	UDP broadcast.

DESCRIPTION:	Class to produce 1 per second ascii string to broadcast
		around airplane.  At this time it produces the IWGADTS
		string.

COPYRIGHT:	University Corporation for Atmospheric Research, 2005-08
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "brdcast.h"

#include <sstream>

const size_t Broadcast::RADAR_ALT_INDX = 5;

using namespace nidas::util;

/* -------------------------------------------------------------------- */
Broadcast::Broadcast() : UDP_Base(31000)
{
  _varList = readFile(BROADCAST);

  _socket = new DatagramSocket;
  _socket->setBroadcastEnable(true);
  _to = new Inet4SocketAddress(Inet4Address(INADDR_BROADCAST), UDP_PORT);
}

/* -------------------------------------------------------------------- */
void Broadcast::BroadcastData(const std::string & timeStamp) 
{
  if (_varList.size() == 0)
  {
    fprintf(stderr, "Broadcast.cc: No ascii_parms!\n");
    return;
  }

  std::stringstream bcast;
  bcast << "IWG1," << timeStamp;

  extern NR_TYPE * AveragedData;

  for (size_t i = 0; i < _varList.size(); ++i)
  {
    bcast << ",";
    if (_varList[i])
    {
      if (i == RADAR_ALT_INDX)
        bcast << AveragedData[_varList[i]->LRstart] * 3.2808;
      else
        bcast << AveragedData[_varList[i]->LRstart];
    }
  }
  bcast << "\r\n";
  _socket->sendto(bcast.str().c_str(), bcast.str().length(), 0, *_to);
  printf(bcast.str().c_str());

}
