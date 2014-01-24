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


using namespace nidas::util;

/* -------------------------------------------------------------------- */
Broadcast::Broadcast() : 
  UDP_Base(7071),
  RADAR_ALT_INDX(5),
  NOCAL_ALT_INDX(-1),
  NOREC_ALT_INDX(-1),
  InterfacePrefix("192.168")
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

  // Setup indices for variables which get special treatment.
  for (size_t i = 0; i < _varList.size(); ++i)
  {
    std::string name;
    if (_varList[i])
    {
      name = _varList[i]->name;
    }
    if (name == "NOREC")
    {
      NOREC_ALT_INDX = i;
    }
    else if (name == "NOCAL")
    {
      NOCAL_ALT_INDX = i;
    }
  }
  std::cerr << "NOCAL_ALT_INDX=" << NOCAL_ALT_INDX << ", " 
	    << "NOREC_ALT_INDX=" << NOREC_ALT_INDX << "." << std::endl;
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

  for (int i = 0; i < (int)_varList.size(); ++i)
  {
    bcast << ",";
    if (_varList[i])
    {
#ifdef DEBUG
      if ((strcmp(_varList[i]->name, "HCN_TOGA") == 0 ||
	   strcmp(_varList[i]->name, "BUTANE_TOGA") == 0 ||
	   strcmp(_varList[i]->name, "CONC_ISAF") == 0) &&
	  !isnan(AveragedData[_varList[i]->LRstart]))
      {
	printf("_varList[%d]->name(%s) = %f\n", (int)i, _varList[i]->name,
	       AveragedData[_varList[i]->LRstart]);
      }
#endif
      if (i == RADAR_ALT_INDX)
      {
	// Our radar alt is in m, convert to ft as required by IWG1
	if (!isnan(AveragedData[_varList[i]->LRstart]))
	  bcast << AveragedData[_varList[i]->LRstart] * 3.2808;
      }
      else if (i == NOCAL_ALT_INDX || // Do Not Calibrate flag
	       i == NOREC_ALT_INDX)   // Do Not Record flag
      {
	if (isnan(AveragedData[_varList[i]->LRstart]))
	  bcast << 0;
	else
	  bcast << AveragedData[_varList[i]->LRstart];
      }
      else
      {
	if (!isnan(AveragedData[_varList[i]->LRstart]))
	  bcast << AveragedData[_varList[i]->LRstart];
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
