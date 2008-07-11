/*
-------------------------------------------------------------------------
OBJECT NAME:	Events.cc

FULL NAME:	Handle Events button from WINDS

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	discComm.cc

COPYRIGHT:	University Corporation for Atmospheric Research, 1995
-------------------------------------------------------------------------
*/

#include "Event.h"


/* -------------------------------------------------------------------- */
Event::Event(DsmConfig *dsm_config)
{
  cnt = 0;

  dsm_config->selectByLocn(WINDS_LOCN_STR);
  Evt_sock = new UdpSocket(EVENTS_PORT, dsm_config->hostName());
  Evt_sock->openSock(UDP_BOUND);

}

/* -------------------------------------------------------------------- */
void Event::checkEvents()
{
  while (Evt_sock->querySock())
    {
    Evt_sock->readSock(	(char *)&evtBuff[cnt++], sizeof(struct EVTPKT));

    if (cnt >= EVT_BUFFS)
      {
      fprintf(stderr, "Event: buffer overflow, events lost.\n");
      cnt = 0;
      }
    }
 
}

/* -------------------------------------------------------------------- */
struct EVTPKT *Event::getEvent()
{
  static int indx = 0;

  if (cnt > 0)
    {
    --cnt;
    return(&evtBuff[indx++]);
    }
  else
    {
    indx = cnt = 0;
    return((struct EVTPKT *)NULL);
    }

}

/* END EVENT.C */
