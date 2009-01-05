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
#include <bzlib.h>

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

  // Initialize GroundSummedData
  //size_t counter = 0;
  for (size_t i = 0; i < _groundVarList.size(); ++i)
  {
     for (size_t j = 0; j < _groundVarList[i]->Length; j++)
     {
       //counter++;
       GroundSummedData.push_back(0.0);
     }
  }
  //GroundSummedData = new NR_TYPE[counter];

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
void Broadcast::broadcastData(const std::string & timeStamp) 
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
  if (cfg.GroundFeedType() != Config::UDP || rate_cntr++ < 180)  // Don't transmit the first couple minutes.
    return;


  // Sum the AveragedData
  size_t counter = 0;
  for (size_t i = 0; i < _groundVarList.size(); ++i)
  {
    for (size_t j = 0; j < _groundVarList[i]->Length; j++)
    {
      GroundSummedData[counter++] += AveragedData[(_groundVarList[i]->LRstart)+j];
    }
  }

  // Only send every so often.
  if ((rate_cntr % GroundFeedDataRate) != 0)
    return;
 
  std::stringstream groundString;
  if (cfg.Aircraft() == Config::HIAPER)
    groundString << "GV";
  if (cfg.Aircraft() == Config::NRL_P3)
    groundString << "NRLP3";
  if (cfg.Aircraft() == Config::C130)
    groundString << "C130";

  groundString << "," << timeStamp;

  counter = 0;
  for (size_t i = 0; i < _groundVarList.size(); ++i)
  {
    if (_groundVarList[i]->Length > 1) 
    {
      groundString << ",'{" << GroundSummedData[counter]/GroundFeedDataRate;
      GroundSummedData[counter++] = 0.0;
      for (size_t j = 1; j < _groundVarList[i]->Length; j++) 
      {
         groundString << "," << GroundSummedData[counter]/GroundFeedDataRate;
         GroundSummedData[counter++] = 0.0;
      }
      groundString << "}'";
    } else {
      groundString << "," << GroundSummedData[counter]/GroundFeedDataRate;
      GroundSummedData[counter++] = 0.0;
    }
  }
  groundString << "\n";

  // compress the stream before sending it to the ground
  char buffer[32000];
  memset(buffer, 0, 32000);
  unsigned int bufLen = sizeof(buffer);
  int ret = BZ2_bzBuffToBuffCompress( buffer, &bufLen, (char *) groundString.str().c_str(),
                                      groundString.str().length(),9,0,0);
  if (ret < 0) {
    typedef struct {     // copied from bzlib.c
      FILE*     handle;
      char      buf[BZ_MAX_UNUSED];
      int       bufN;
      bool      writing;
      bz_stream strm;
      int       lastErr;
      bool      initialisedOk;
    } bzFile;
    bzFile b;
    b.lastErr = ret;
    int errnum;
    char msg[100];
    sprintf(msg, "Failed to compress the ground feed stream: %s\n", BZ2_bzerror(&b, &errnum) );
    LogMessage(msg);
    return;
  }
  _groundBrdcst->writeSock(buffer, bufLen);

  printf("\ncompressed %d -> %d\n", groundString.str().length(), bufLen);
  printf("to ground: %s\n", groundString.str().c_str());
}	// END BROADCASTDATA

// END BRDCAST.CC
