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

const size_t Broadcast::RADAR_ALT_INDX = 5;
const std::string Broadcast::InterfacePrefix = "192.168";

using namespace nidas::util;

/* -------------------------------------------------------------------- */
Broadcast::Broadcast() : UDP_Base(7071)
{
  _varList = readFile(BROADCAST);

  _socket = new DatagramSocket;
  _socket->setBroadcastEnable(true);

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
    }
  }
}

/* -------------------------------------------------------------------- */
std::string Broadcast::extractPQString(PGresult *result, int tuple, int field)
{
  const char* pqstring = PQgetvalue(result, tuple, field);
  if (! pqstring)
    return "";

  int len = strlen(pqstring);
  while (len > 0 &&
          isascii(pqstring[len-1]) &&
          isspace(pqstring[len-1]))
    len--;

  return std::string(pqstring, len);
}

/* -------------------------------------------------------------------- */
std::string Broadcast::getGlobalAttribute(std::string attr)
{
  PGconn *conn = PQconnectdb("host='acserver' dbname='real-time' user ='ads'");

  /* check to see that the backend connection was successfully made
   */
  if (PQstatus(conn) == CONNECTION_BAD)
  {
    PQfinish(conn);
    std::cerr << "PQconnectdb: Connection failed." << std::endl;
    return "";
  }
  std::string query = "SELECT value FROM global_attributes WHERE key='";
  query += attr + "'";
  PGresult * res = PQexec(conn, query.c_str());

  int ntuples = PQntuples(res);

  if (ntuples == 0)
  {
    std::cerr << "No global attribute " << attr << "!\n";
    return "";
  }
  std::string s = extractPQString(res, 0, 0);
  PQclear(res);
  PQfinish(conn);
  return s;
}

/* -------------------------------------------------------------------- */
void Broadcast::BroadcastData(const std::string & timeStamp) 
{
  if (_varList.size() == 0)
  {
    fprintf(stderr, "Broadcast.cc: No ascii_parms!\n");
    return;
  }

  std::stringstream bcast;
  bcast << "IWG1," << timeStamp;

  extern NR_TYPE * AveragedData;

  for (size_t i = 0; i < _varList.size(); ++i)
  {
    bcast << ",";
    if (_varList[i])
    {
      if (i == RADAR_ALT_INDX) // Our radar alt is in m, convert to ft as required by IWG1
        bcast << AveragedData[_varList[i]->LRstart] * 3.2808;
      else
        bcast << AveragedData[_varList[i]->LRstart];
    }
  }

  // append ?CALIB, and ?RESCH flags to message.  DC3/SEAC4RS.
  bcast << "," << (getGlobalAttribute("DoNotCalibrate") == "TRUE" ? "1" : "0");
  bcast << "," << (getGlobalAttribute("DoNotRecord") == "TRUE" ? "1" : "0");

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

  printf(bcast.str().c_str());
}
