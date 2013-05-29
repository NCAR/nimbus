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
const size_t Broadcast::NOCAL_ALT_INDX = 31;
const size_t Broadcast::NOREC_ALT_INDX = 32;
const std::string Broadcast::InterfacePrefix = "192.168";

using namespace nidas::util;

/* -------------------------------------------------------------------- */
Broadcast::Broadcast() : UDP_Base(7071)
{
  _varList = readFile(BROADCAST);

  _socket = new MulticastSocket;
  _socket->setBroadcastEnable(true);
  _socket->setTimeToLive(2);

  // We want to broadcast on all ethernet interfaces.
  // Get list here.
  std::list<Inet4NetworkInterface> if_list = _socket->getInterfaces();
  std::list<Inet4NetworkInterface>::iterator it;
  for (it = if_list.begin(); it != if_list.end(); ++it)
  {
    if ((*it).getAddress().getHostAddress().compare(0, InterfacePrefix.length(), InterfacePrefix) == 0)
    {
      _toList.push_back(new
	Inet4SocketAddress(Inet4Address((*it).getBroadcastAddress().getInAddrPtr()), UDP_PORT));
      std::cerr << "broadcast to " << _toList.back()->toString() << std::endl;
      // we also want to multicast to this interface
      _mcInterfaces.push_back(*it);
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
//  printf("_varList[%d]->name = %s\n", i, _varList[i]->name);
    bcast << ",";
    if (_varList[i])
    {
      switch (i)
      {
        case RADAR_ALT_INDX: // Our radar alt is in m, convert to ft as required by IWG1
          if (!isnan(AveragedData[_varList[i]->LRstart]))
            bcast << AveragedData[_varList[i]->LRstart] * 3.2808;
          break;
        case NOCAL_ALT_INDX: // Do Not Calibrate flag
        case NOREC_ALT_INDX: // Do Not Record flag
          if (isnan(AveragedData[_varList[i]->LRstart]))
            bcast << 0;
          else
            bcast << AveragedData[_varList[i]->LRstart];
          break;
        default:
          if (!isnan(AveragedData[_varList[i]->LRstart]))
            bcast << AveragedData[_varList[i]->LRstart];
          break;
      }
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
  // Now multicast to interfaces in our multicast interface list
  Inet4Address mcAddr(Inet4Address::getByName("239.0.0.10"));
  Inet4SocketAddress mcSockAddr(mcAddr, UDP_PORT);

  for (size_t i = 0; i < _mcInterfaces.size(); ++i)
  {
    _socket->setInterface(mcAddr, _mcInterfaces[i]);
    try {
      _socket->sendto(bcast.str().c_str(), bcast.str().length(), 0, mcSockAddr);
    }
    catch (const nidas::util::IOException& e) {
      fprintf(stderr, "nimbus::Broadcast multicast: %s\n", e.what());
    }
  }

  printf(bcast.str().c_str());
}
