/*
-------------------------------------------------------------------------
OBJECT NAME:	brdcast.h

FULL NAME:	UDP Broadcast output.

DESCRIPTION:	Produce RAF PostgreSQL flight file.

COPYRIGHT:	University Corporation for Atmospheric Research, 2005
-------------------------------------------------------------------------
*/

#ifndef BCAST_H
#define	BCAST_H

#include "nimbus.h"
#include "decode.h"

#include "UdpSocket.h"


/* -------------------------------------------------------------------- */
class Broadcast
{
public:
  Broadcast();

  /** 
   * Write LowRate (LRT) data into the database.
   */
  void	broadcastData(const std::string & timeStamp) const;

protected:
  UdpSocket * _brdcst1, * _brdcst2;

  std::vector<var_base *> _varList;

  static const int RT_UDP_PORT;
  static const std::string RT_UDP_ADDR_1, RT_UDP_ADDR_2;

  /**
   * In order to conform to the IWGADTS IWG1 packet, radar altimeter needs to be
   * in feet, not meters.
   */
  static const size_t RADAR_ALT_INDX;
};

#endif
