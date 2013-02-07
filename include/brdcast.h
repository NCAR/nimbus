/*
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

  void	BroadcastData(const std::string & timeStamp);

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
  static const size_t RADAR_ALT_INDX;

  /// Do Not Calibrate and Record flags
  static const size_t NOCAL_ALT_INDX;
  static const size_t NOREC_ALT_INDX;

  static const std::string InterfacePrefix;
};

#endif
