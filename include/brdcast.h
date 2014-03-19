/* -*- C++ -*-
-------------------------------------------------------------------------
OBJECT NAME:	brdcast.h

FULL NAME:	UDP Broadcast output.

DESCRIPTION:	Broadcast a real-time feed data feed around the aircraft
		via UDP.

COPYRIGHT:	University Corporation for Atmospheric Research, 2005-08
-------------------------------------------------------------------------
*/

#ifndef _Broadcast_h_
#define	_Broadcast_h_

#include "UDP_Base.h"


/* -------------------------------------------------------------------- */
class Broadcast : public UDP_Base
{
public:
  Broadcast();

  void
  BroadcastData(nidas::core::dsm_time_t tt);

  virtual
  std::string
  formatVariable(int i);

protected:
  /**
   * addresses to which we send broadcast packets
   */
  std::vector<nidas::util::Inet4SocketAddress *> _toList;

  /**
   * interfaces to which we send multicast packets
   */
  std::vector<nidas::util::Inet4NetworkInterface> _mcInterfaces;

  /**
   * In order to conform to the IWGADTS IWG1 packet, radar altimeter needs to be
   * in feet, not meters.
   */
  int RADAR_ALT_INDX;

  /// Do Not Calibrate and Record flags
  int NOCAL_ALT_INDX;
  int NOREC_ALT_INDX;

  std::string InterfacePrefix;
};

#endif
