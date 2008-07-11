/* vxtcp.cc
   TCP socket test program for execution on a vxWorks station.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <vxWorks.h>
#include <bootLib.h>
#include <logLib.h>
#include <sockLib.h>
#include <stdio.h>
#include <taskLib.h>
#include <wdLib.h>
#include <sysLib.h>

#include <header.h>
#include <dsmctl.h>

#include <TcpSocket.h>

void queryNet ();			// query for data available
void readTcpToken ();
void wdisr ();

extern BOOT_PARAMS sysBootParams;

TcpSocket *tcpsock;
 
char buf[MX_PHYS];
WDOG_ID wid;
int tick = FALSE;
int connected;

void vxtcp ()
{
  int j;
  int len = MX_PHYS;
  int rec_cnt = 0;

// Init the tcp socket class.
  tcpsock =  new TcpSocket (1112, sysBootParams.hostName);

// Init the data buffer.
  *(short*)buf = SAMPLED_ID;

  for (j = 2; j < len; j++)
    buf[j] = j;

// Create a watchdog timer.
  wid = wdCreate();

  while (TRUE) {
    connected = FALSE;

// Wait for tcp connection.
    while (!connected) {
      printf ("Waiting for connection.\n");
      tcpsock->closeSock();
      tcpsock->openSock (TCP_CONNECT);
      connected = tcpsock->connectSock();
      taskDelay(sysClkRateGet());
    }
    printf ("connection made\n");

    wdStart ((WDOG_ID)wid, sysClkRateGet(), (FUNCPTR)wdisr, 0);
    while (connected) {
      if (rec_cnt++ < 6) { 
//      if (tcpsock->writeSock (buf, len) == ERROR)
//        connected = FALSE;
        printf ("%d\n", (int)buf[2]);
        buf[2]++;
      }

      if (tick) {
        rec_cnt = 0;
        tick = FALSE;
      }
    }
    printf ("Resetting the network.\n");
  }
}
/*****************************************************************************/
 
void wdisr ()

// Watchdog timer isr.
{
// Restart the timer.
  wdStart ((WDOG_ID)wid, sysClkRateGet(), (FUNCPTR)wdisr, 0);
//logMsg ("tick\n", 0,0,0,0,0,0);
  tick = TRUE;
}
/*****************************************************************************/

