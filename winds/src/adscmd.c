/*
-------------------------------------------------------------------------
OBJECT NAME:	evt.c

FULL NAME:	Events

ENTRY POINTS:	set_adsevt_proc()

STATIC FNS:	none

DESCRIPTION:	Send "Events" from the events buttons to discComm.

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	Callback

COPYRIGHT:	University Corporation for Atmospheric Research, 1995
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/panel.h>

#include "constants.h"
#include "Events.h"
#include "network.h"

#include "proto.h"

static struct EVTPKT evt_pkt;		/* command struct sent to ads	*/
static int send_message = FALSE;	/* TRUE when the "Event Sent" message
					 * appears in the events panel item */

 
/* -------------------------------------------------------------------- */
void set_adsevt_proc(Panel_item item, int value, Event *event)
 
/* Event handler for ADS event entries from the adsevt_item. */
{
  static int	evt_sock;
  static int	firstTime = TRUE;
  static struct hostent *hp;
  static struct sockaddr_in	client_addr,	/* this machine */
				srvr_addr;	/* DiscWin	*/
  static char	display_locn[64];
  static char	ads_hostname[64];

  /* If send_message is still TRUE, the user may have double clicked on
   * the mouse button too quickly for chk_ads_stuff to have cleared the
   * panel label string, so clear it here.
   */
/* printf("notify_proc event: %d XGetAttr value: %d value %d \n", 
        event_action(event),(int)XGetAttr(adsevt_item(),PANEL_VALUE),value); */

  if (send_message && XGetAttr(adsevt_item(), PANEL_VALUE) == 0) {
    XSetAttr(adsevt_item(), PANEL_LABEL_STRING, "Pick Event", NULL);
    send_message = FALSE;
    return;
  }

  /* Initializations on 1st call to this routine.
   */
  if (firstTime)
    {
    if (get_entry_by_locn(CONTROL_CENTRAL) == ERR) {
      /* find ads in netconfig file*/
      time_to_log();
      fprintf(stderr, "adscmd - get_entry_by_locn('ads') returned ERR.\n");

      RequestAlertNow("Events: adscmd.c: problem on ADS location in netconfig file.");
      return;
      }

    (void)strcpy(ads_hostname, get_disp_host());

    /* Find this machine's entry in the netconfig file. */
    if ((get_netconfig_entry( (char *)GetDisplayHost(), "",
			  display_pnum,display_file)) == ERR)
      {
      time_to_log();
      (void)fprintf(stderr, "adscmd - get_netconfig_entry() returned ERR.\n");

      RequestAlertNow("Events: adscmd.c: problem on getting entry in netconfig file.");
      return;
      }

    (void)strcpy(display_locn, "EVT");
    (void)strcat(display_locn, get_disp_locn());
    (void)strupr(display_locn);

    /* Set up server (discWin) addr.
     */
    memset((char*)&srvr_addr, 0, sizeof(srvr_addr));
    srvr_addr.sin_family = AF_INET;
    srvr_addr.sin_port = htons(EVENTS_PORT);

    if ((hp = gethostbyname(ads_hostname)) == 0)
      {
      time_to_log();
      (void)fprintf(stderr, "FATAL: adscmd - %s host unknown\n", ads_hostname);

      RequestAlertNow("Events: adscmd.c: unknown host in adscmd.");
      return;
      }

    memcpy((char*)&srvr_addr.sin_addr.s_addr,
		(char*)hp->h_addr_list[0], hp->h_length);


printf("ctl=[%s], disp=[%s], %d\n", ads_hostname, display_locn, EVENTS_PORT);
    /* Open the datagram socket to send events to the ads. */ 
    if ((evt_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
      {
      perror ("FATAL: adscmd - Opening evt_sock");
      RequestAlertNow("Events: adscmd.c: problem opening evt_sock.");
      return;
      }

    memset((void *)&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    client_addr.sin_port = htons(0);

    if (bind(evt_sock, (struct sockaddr *)&client_addr, sizeof(client_addr))< 0)
      {
      perror ("Events: UdpSocket: bind");
      RequestAlertNow("Events: adscmd.c: bind failure..");
      return;
      }

    firstTime = FALSE;
    }


  /* Send the Event.
  */
  evt_pkt.number = htonl((int)XGetAttr(adsevt_item(), PANEL_VALUE));
  (void)strcpy(evt_pkt.station, display_locn);

  if (sendto(evt_sock, (char *)&evt_pkt, sizeof(evt_pkt), 0,
		(struct sockaddr*)&srvr_addr, sizeof(srvr_addr)) < 0) {
    perror ("adscmd - sendto event datagram packet");
    RequestAlertNow("Events: adscmd.c: problem sending event packet.");
    return;
    }

  XSetAttr(adsevt_item(), PANEL_LABEL_STRING, "Event Sent", NULL);

  send_message=TRUE;

  if (send_message) {
    XSetAttr(adsevt_item(),
               PANEL_VALUE, 0 ,  /* clear the panel item */
               PANEL_LABEL_STRING, "Pick Event",
               NULL);
    send_message=FALSE;
    }

}	/* END SET_ADSEVT_ */

/* END EVT.C */
