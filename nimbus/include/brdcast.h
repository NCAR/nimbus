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
  UdpSocket *_brdcst1, *_brdcst2;

  static const std::string RT_UDP_ADDR_1, RT_UDP_ADDR_2;

  /**
   * In order to conform to the IWGADTS IWG1 packet, radar altimeter needs to be
   * in feet, not meters.
   */
  static const size_t RADAR_ALT_INDX;
};

#endif
