/* vxudp.cc
   UDP socket test program for execution on a vxWorks station.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <vxWorks.h>
#include <selectLib.h>
#include <stdio.h>
#include <taskLib.h>
#include <wdLib.h>
#include <sysLib.h>

#include <dsmctl.h>

#include <SerialBuf.h>
#include <UdpSocket.h>

void queryNet ();			// query for data
int readUdp ();				// poll for data
void printBuf ();			// print received message
void wdisr ();				// watch dog isr

SerialBuf *buf;
static UdpSocket *udpsock;

int tick = FALSE;
WDOG_ID wid;

int vxudp (char *host, int mode)
{
  int len;
  printf ("host = %s, mode = %d\n", host, mode);

  buf = new SerialBuf (100);
  udpsock = new UdpSocket (DSM_BROADCAST_PORT, host);

// If transmitting, open an unbound socket.
  if (mode == 1) 
    udpsock->openSock(UDP_UNBOUND);
  else if (mode == 2) 
    udpsock->openSock(UDP_BROADCAST);
  else
    udpsock->openSock(UDP_BOUND);

// Create a watchdog timer.
  wid = wdCreate();
  wdStart ((WDOG_ID)wid, sysClkRateGet(), (FUNCPTR)wdisr, 0);
  printf ("main loop \n");

// Main loop.  
  while (TRUE) {

// Check for received data in in the receiving mode.
    if (!mode && udpsock->querySock() && readUdp())
      printBuf();

// Once per second, send an activity token.
    if (tick) {
      tick = FALSE;
      printf ("tick\n");
      if (mode) {
        len = udpsock->writeSock ("Hello from suntcp.\n", 20);
        printf ("writeSock returned %d\n", len);
      }
    }
    taskDelay (sysClkRateGet() / 10);
  }
}
/*****************************************************************************/

int readUdp ()

// Reads data from the tcp socket.
{
  int len = 20;
 
// Read the data from the socket.
  if ((len = udpsock->readSock(buf->getBufPtr(), len)) == 0)
    return FALSE;
  else if (len == ERROR)
    return ERROR;
 
// Increment the buffer index.
  buf->incrBufIndex (len);
 
  return TRUE;
}
/*****************************************************************************/

void printBuf ()

{
  printf (buf->getBuf());
  buf->releaseBuf();
}
/*****************************************************************************/

void wdisr ()
 
// SIGALRM catcher.
{
  wdStart ((WDOG_ID)wid, sysClkRateGet(), (FUNCPTR)wdisr, 0);
  tick = TRUE;
}
