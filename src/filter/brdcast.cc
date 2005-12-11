/*
-------------------------------------------------------------------------
OBJECT NAME:	brdcst.cc

FULL NAME:	UDP broadcast.

ENTRY POINTS:	ReadBroadcastVariables()

DESCRIPTION:	

INPUT:		${PROJ_DIR}/###/asciilist

OUTPUT:		[sp|rp|dp]->Broadcast updated.

COPYRIGHT:	University Corporation for Atmospheric Research, 2005
-------------------------------------------------------------------------
*/

#include "brdcast.h"

#include <sstream>

const int Broadcast::RT_UDP_PORT = 2102;
//const std::string Broadcast::RT_UDP_ADDR = "128.117.84.255";
const std::string Broadcast::RT_UDP_ADDR = "192.168.84.255";

/* -------------------------------------------------------------------- */
Broadcast::Broadcast()
{
  int	index;
  char	*bcast[512], target[NAMELEN];

  ReadTextFile(BROADCAST, bcast);

  for (int i = 0; bcast[i]; ++i)
  {
    strcpy(target, bcast[i]);

    if ((index = SearchTable(raw, target)) != ERR)
    {
      _varList.push_back(raw[index]);
    }
    else
    if ((index = SearchTable(derived, target)) != ERR)
    {
      _varList.push_back(derived[index]);
    }
    else
    {
      char msg[100];
      sprintf(msg, "brdcast.cc: Variable [%s] not found, substituting ZERO.\n", target);
      LogMessage(msg);
      _varList.push_back(0);
    }
  }

  FreeTextFile(bcast);

  _brdcst = new UdpSocket(RT_UDP_PORT, RT_UDP_ADDR.c_str());
  _brdcst->openSock(UDP_BROADCAST);

}	// END CTOR

/* -------------------------------------------------------------------- */
void Broadcast::broadcastData(std::string timeStamp) const
{
  std::stringstream bcast;
  bcast << timeStamp;

  extern NR_TYPE * AveragedData;

  for (size_t i = 0; i < _varList.size(); ++i)
    if (_varList[i] == 0)
      bcast << " 0.0";
    else
      bcast << " " << AveragedData[_varList[i]->LRstart];

  bcast << '\n';
  _brdcst->writeSock(bcast.str().c_str(), bcast.str().length());

}	// END BROADCASTDATA

// END BRDCAST.CC
