/* sunudp.cc
   UDP socket test program for communication with winds.

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

#include <UdpSocket.h>

void catchAlarm (int);			// SIGALRM signal handler

UdpSocket *udpsock;

int tick = FALSE;

main (int argc, char *argv[])   
{
  int mode;
  int len;
  char message[45];

  if (argc < 2) {
    fprintf (stderr, 
      "Usage: sunudp host\n");
    exit (0);
  }

  udpsock = new UdpSocket (3370, argv[1]);

// Set up the SIGALRM handler.
#ifdef SVR4
  if ((int)sigset (SIGALRM, catchAlarm) < 0) {
#else
  if ((int)signal (SIGALRM, catchAlarm) < 0) {
#endif
    perror("sigset SIGALRM");
    exit (ERROR);
  }

// Open an unbound socket.
  udpsock->openSock(UDP_UNBOUND);
  (void)sprintf(message,"%d %d %s", 20, FALSE, "T1");


  alarm(1);

// Main loop.  
  while (TRUE) {

// Once per second, send an activity token.
    if (tick) {
      tick = FALSE;
      alarm(1);
      len = udpsock->writeSock (message, strlen(message)+1);
    }
  }
}
/*****************************************************************************/

void catchAlarm (int status)
 
// SIGALRM catcher.
{
  tick = TRUE;
}
/*****************************************************************************/
