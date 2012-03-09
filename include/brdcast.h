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

#include <string>
#include <libpq-fe.h>

#include "UDP_Base.h"


/* -------------------------------------------------------------------- */
class Broadcast : public UDP_Base
{
public:
  Broadcast();

  std::string extractPQString(PGresult *result, int tuple, int field);

  std::string getGlobalAttribute(std::string attr);

  void	BroadcastData(const std::string & timeStamp);

protected:
  std::vector<nidas::util::Inet4SocketAddress *> _toList;

  /**
   * In order to conform to the IWGADTS IWG1 packet, radar altimeter needs to be
   * in feet, not meters.
   */
  static const size_t RADAR_ALT_INDX;

  static const std::string InterfacePrefix;
};

#endif
