/*
-------------------------------------------------------------------------
OBJECT NAME:	brdcst.cc

FULL NAME:	UDP broadcast.

ENTRY POINTS:	

DESCRIPTION:	Class to produce 1 per second ascii string to broadcast
		around airplane.  At this time it produces the IWGADTS
		string.

INPUT:		${PROJ_DIR}/###/asciilist, groundvars

OUTPUT:		[rp|dp]->Broadcast updated.

COPYRIGHT:	University Corporation for Atmospheric Research, 2005-08
-------------------------------------------------------------------------
*/

#include "brdcast.h"

#include <sstream>

const int Broadcast::RT_UDP_PORT = 31000;
//const std::string Broadcast::RT_UDP_ADDR = "128.117.84.255";
const std::string Broadcast::RT_UDP_ADDR_1 = "192.168.84.255";
const std::string Broadcast::RT_UDP_ADDR_2 = "192.168.184.255";

// eol-rt-data.guest.ucar.edu
const int Broadcast::GRND_UDP_PORT = 31007;
const std::string Broadcast::GRND_UDP_ADDR = "128.117.188.122";

const size_t Broadcast::RADAR_ALT_INDX = 5;

const size_t Broadcast::GroundFeedDataRate = 5;	// in seconds.

/* -------------------------------------------------------------------- */
Broadcast::Broadcast()
{
  _varList = readFile(BROADCAST);
  std::string fileName(XMIT_VARS);
  fileName += ".rt";
  _groundVarList = readFile(fileName);

  setCoordinatesFrom(_groundVarList);

  _brdcst1 = new UdpSocket(RT_UDP_PORT, RT_UDP_ADDR_1.c_str());
  _brdcst1->openSock(UDP_BROADCAST);

  _brdcst2 = new UdpSocket(RT_UDP_PORT, RT_UDP_ADDR_2.c_str());
  _brdcst2->openSock(UDP_BROADCAST);

  _groundBrdcst = new UdpSocket(GRND_UDP_PORT, GRND_UDP_ADDR.c_str());
  _groundBrdcst->openSock(UDP_UNBOUND);
}

/* -------------------------------------------------------------------- */
void Broadcast::setCoordinatesFrom(const std::vector<var_base *> & list) const
{
  std::vector<var_base *>::const_iterator it;
  for (it = list.begin(); it != list.end(); ++it)
  {
    if (strstr((*it)->name, "LON"))
      cfg.SetCoordLON((*it)->name);
    if (strstr((*it)->name, "LAT"))
      cfg.SetCoordLAT((*it)->name);
    //  Don't set it to PALTF, we don't want an altitude in feet as the ref.
    if (strstr((*it)->name, "ALT") && strstr((*it)->name, "ALTF") == 0)
      cfg.SetCoordALT((*it)->name);
  }
}

/* -------------------------------------------------------------------- */
std::vector<var_base *> Broadcast::readFile(const std::string & fileName) const
{
  char	*varList[512];
  std::vector<var_base *> list;

  ReadTextFile(fileName, varList);

  for (int i = 0; varList[i]; ++i)
  {
    char target[BUFSIZ], *p;
    strcpy(target, varList[i]);

    p = strtok(target, ", \t\n");
    while (p)
    {
      int index;

      if ((index = SearchTable(raw, p)) != ERR)
      {
        list.push_back(raw[index]);
      }
      else
      if ((index = SearchTable(derived, p)) != ERR)
      {
        list.push_back(derived[index]);
      }
      else
      {
        char msg[100];
        sprintf(msg, "brdcast.cc: Variable [%s] not found, substituting ZERO.\n", p);
        LogMessage(msg);
        list.push_back(0);
      }
      p = strtok(NULL, ", \t\n");
    }
  }

  FreeTextFile(varList);

  return list;
}

/* -------------------------------------------------------------------- */
void Broadcast::broadcastData(const std::string & timeStamp) const
{
  static int rate_cntr = 0;

  std::stringstream bcast;
  bcast << "IWG1," << timeStamp;

  extern NR_TYPE * AveragedData;

  for (size_t i = 0; i < _varList.size(); ++i)
  {
    bcast << ",";
    if (_varList[i])
    {
      if (i == RADAR_ALT_INDX)
        bcast << AveragedData[_varList[i]->LRstart] * 3.2808;
      else
        bcast << AveragedData[_varList[i]->LRstart];
    }
  }
  bcast << "\r\n";
  _brdcst1->writeSock(bcast.str().c_str(), bcast.str().length());
  _brdcst2->writeSock(bcast.str().c_str(), bcast.str().length());
printf(bcast.str().c_str());

  // Send UDP Ground Feed.
  if (_groundVarList.size() == 0 || cfg.GroundFeedType() != Config::UDP ||
      rate_cntr++ < 180 ||			// Don't send for first couple minutes.
      (rate_cntr % GroundFeedDataRate) != 0)
    return;

  std::stringstream groundString;
  groundString << "C130," << timeStamp;
  for (size_t i = 0; i < _groundVarList.size(); ++i)
  {
    groundString << "," << AveragedData[_groundVarList[i]->LRstart];
  }
  groundString << "\n";
  _groundBrdcst->writeSock(groundString.str().c_str(), groundString.str().length());
printf(groundString.str().c_str());

}	// END BROADCASTDATA

// END BRDCAST.CC
