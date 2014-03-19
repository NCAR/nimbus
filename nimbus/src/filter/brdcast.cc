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
#include <stdlib.h>  // getenv()

using namespace nidas::util;

/* -------------------------------------------------------------------- */
Broadcast::Broadcast() : 
  UDP_Base(7071),
  RADAR_ALT_INDX(5),
  NOCAL_ALT_INDX(-1),
  NOREC_ALT_INDX(-1),
  InterfacePrefix("192.168")
{
  // Look for a latch-specific version of ascii_parms first, so that
  // existing ascii_parms files remain backwards-compatible with other
  // nimbus versions.
  std::string apfile(BROADCAST);
  apfile += ".latches";
  if (! readFile(apfile.c_str()))
  {
    readFile(BROADCAST);
  }

  // For debugging, allow broadcast to be redirected.
  const char* debug_interface = 0;
  if ((debug_interface = getenv("NIMBUS_DEBUG_BROADCAST_INTERFACE")) != 0)
  {
    InterfacePrefix = debug_interface;
    std::cerr << "WARNING: switching iwg broadcast interface to "
	      << InterfacePrefix << std::endl;
  }

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
void Broadcast::BroadcastData(nidas::core::dsm_time_t tt) 
{
  std::string timeStamp = formatTimestamp(tt);

  if (_varList.size() == 0)
  {
    fprintf(stderr, "Broadcast.cc: No ascii_parms!\n");
    return;
  }

  updateData(tt);

  std::stringstream bcast;
  bcast << "IWG1," << timeStamp;

  for (int i = 0; i < (int)_varList.size(); ++i)
  {
    bcast << "," << formatVariable(i);
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


std::string
Broadcast::
formatVariable(int i)
{
  var_base* vp = _varList[i];
  if (vp && vp->Length > 1)
  {
    // Pass 2D vars to the generic formatter.  It is enough of a special
    // case that it may as well use the same format.
    return UDP_Base::formatVariable(i);
  }

  std::ostringstream bcast;
  if (vp)
  {
    NR_TYPE& current = _lastGoodData[i][0];
    if (i == RADAR_ALT_INDX)
    {
      // Our radar alt is in m, convert to ft as required by IWG1
      if (!isnan(current))
	bcast << current * 3.2808;
    }
    else if (i == NOCAL_ALT_INDX || // Do Not Calibrate flag
	     i == NOREC_ALT_INDX)   // Do Not Record flag
    {
      if (isnan(current))
	bcast << 0;
      else
	bcast << current;
    }
    else
    {
      if (!isnan(current))
	bcast << current;
    }
  }
  return bcast.str();
}
