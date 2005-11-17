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
  void	broadcastData(std::string timeStamp) const;

protected:
  UdpSocket* _brdcst;

  std::vector<var_base *> _varList;

  static const int RT_UDP_PORT;
  static const std::string RT_UDP_ADDR;
};

#endif
