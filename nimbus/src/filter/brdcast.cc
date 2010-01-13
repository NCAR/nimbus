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
const size_t Broadcast::MACH_NUMBER_INDX = 9;
const std::string Broadcast::InterfacePrefix = "eth";

using namespace nidas::util;

/* -------------------------------------------------------------------- */
Broadcast::Broadcast() : UDP_Base(31000)
{
  _varList = readFile(BROADCAST);

  _socket = new DatagramSocket;
  _socket->setBroadcastEnable(true);

  // We want to broadcast on all ethernet interfaces.
  // Get list here.
  std::list<Inet4NetworkInterface> if_list = _socket->getInterfaces();
  std::list<Inet4NetworkInterface>::iterator it;
  for (it = if_list.begin(); it != if_list.end(); ++it)
  {
    if ((*it).getName().compare(0, InterfacePrefix.length(), InterfacePrefix) == 0)
    {
      _toList.push_back(new
	Inet4SocketAddress(Inet4Address((*it).getBroadcastAddress().getInAddrPtr()), UDP_PORT));
    }
  }
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
      if (i == RADAR_ALT_INDX) // Our radar alt is in m, convert to ft as required by IWG1
        bcast << AveragedData[_varList[i]->LRstart] * 3.2808;
      else
      if (i == MACH_NUMBER_INDX) // Convert mach squared to mach number.
        bcast << sqrt(AveragedData[_varList[i]->LRstart]);
      else
        bcast << AveragedData[_varList[i]->LRstart];
    }
  }

  bcast << "\r\n";
  for (size_t i = 0; i < _toList.size(); ++i)
  {
    try {
      _socket->sendto(bcast.str().c_str(), bcast.str().length(), 0, *_toList[i]);
    }
    catch (const nidas::util::IOException& e) {
      fprintf(stderr, "nimbus::Broadcast: %s\n", e.what());
    }
  }

  printf(bcast.str().c_str());
}
