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
const std::string Broadcast::RT_UDP_ADDR = "128.117.84.255";
//const std::string Broadcast::RT_UDP_ADDR = "192.168.84.255";

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
void Broadcast::broadcastData(const std::string & timeStamp) const
{
  std::stringstream bcast;
  bcast << "IWG1," << timeStamp;

  extern NR_TYPE * AveragedData;

  for (size_t i = 0; i < _varList.size(); ++i)
  {
    bcast << ",";
    if (_varList[i])
      bcast << AveragedData[_varList[i]->LRstart];
  }
  bcast << "\r\n";
  _brdcst->writeSock(bcast.str().c_str(), bcast.str().length());
printf(bcast.str().c_str());

}	// END BROADCASTDATA

// END BRDCAST.CC
