/* sunudp.cc
   UDP socket test program for execution on the sun workstation.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <memory.h>
#include <signal.h>
#include <sys/time.h>

#include <dsmctl.h>

#include <SerialBuf.h>
#include <UdpSocket.h>

void queryNet ();			// query for data
int readUdp ();				// poll for data
void printBuf ();			// print received message
void catchAlarm (int);			// SIGALRM signal handler

SerialBuf *buf;
UdpSocket *udpsock;

int tick = FALSE;

main (int argc, char *argv[])   
{
  int mode;
  int len;

  if (argc < 3) {
    fprintf (stderr, 
      "Usage: sunudp host mode (0 = receive, 1 = transmit, 2 = broadcast)\n");
    exit (0);
  }

  sscanf (argv[2], "%d", &mode);
  printf ("mode = %d\n", mode);

  buf = new SerialBuf (100);
  udpsock = new UdpSocket (DSM_BROADCAST_PORT, argv[1]);

// Set up the SIGALRM handler.
#ifdef SVR4
  if ((int)sigset (SIGALRM, catchAlarm) < 0) {
#else
  if ((int)signal (SIGALRM, catchAlarm) < 0) {
#endif
    perror("sigset SIGALRM");
    exit (ERROR);
  }

// If transmitting, open an unbound socket.
  if (mode == 1) 
    udpsock->openSock(UDP_UNBOUND);
  else if (mode == 2) 
    udpsock->openSock(UDP_BROADCAST);
  else
    udpsock->openSock(UDP_BOUND);

  alarm(1);

// Main loop.  
  while (TRUE) {
    buf->releaseBuf();

// Check for received data in in the receiving mode.
    if (!mode &&udpsock->querySock() && readUdp())
      printBuf();

// Once per second, send an activity token.
    if (tick) {
      tick = FALSE;
      alarm(1);
      if (mode) {
        len = udpsock->writeSock ("Hello from suntcp.\n", 20);
        printf ("writeSock returned %d\n", len);
      }
    }
  }
}
/*****************************************************************************/

int readUdp ()

// Reads data from the tcp socket.
{
  int len = 80;
 
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

void catchAlarm (int status)
 
// SIGALRM catcher.
{
  tick = TRUE;
}
/*****************************************************************************/
