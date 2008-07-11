/* tcprecv.cc
   TCP socket test program for execution on the sun workstation.
   Accepts the socket connection, displays ascii strings received, and
   echoes those strings back.

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

#include <TcpSocket.h>

void queryNet ();			// query dsm nets for data
int readTcp ();				// poll dsms for sampled data
int checkData ();			// poll dsms for sampled data
void catchAlarm (int);			// SIGALRM signal handler
void catchPipe (int);			// SIGPIPE signal handler

char buf[80];
TcpSocket *tcpsock;

int tick = FALSE;
int activity;				// dsm activity counter
int connected;				// socket connected flag

main (int argc, char *argv[])
{
  if (argc < 2) {
    printf ("\nUsage: tcprecv host\n");
    exit (0);
  }
  tcpsock = new TcpSocket (1112, argv[1]);

// Set up the SIGPIPE handler.
#ifdef SVR4
  if ((int)sigset (SIGPIPE, catchPipe) < 0) {
#else
  if ((int)signal (SIGPIPE, catchPipe) < 0) {
#endif
    perror("sigset SIGPIPE");
    exit (ERROR);
  }
 
// Set up the SIGALRM handler.
#ifdef SVR4
  if ((int)sigset (SIGALRM, catchAlarm) < 0) {
#else
  if ((int)signal (SIGALRM, catchAlarm) < 0) {
#endif
    perror("sigset SIGALRM");
    exit (ERROR);
  }

// Main loop.  
  while (TRUE) {
    connected = FALSE;
    tcpsock->closeSock();
    tcpsock->openSock(TCP_ACCEPT);
    tcpsock->listenSock();

    alarm(1);
    while (!connected) {
      if (tick) {
        fprintf (stderr, "Waiting for accept .\n");
        connected = tcpsock->acceptSock();
        tick = FALSE;
        alarm(1);
      }
    }
    printf ("Connection made.\n");

// Check for received data. Echo back any received data
    while (connected) {
      if (tcpsock->querySock() && readTcp()) {
        printf ("Received string: %s", buf);
        if (tcpsock->writeSock (buf, 25) == ERROR)
          connected = FALSE;
      }
    }
  }
}
/*****************************************************************************/
 
int readTcp ()
 
// Reads data from the tcp socket.
{
  int len;
 
// Read the data from the socket.
  if ((len = tcpsock->readSock(buf, 4096)) == 0)
    return FALSE;
  else if (len == ERROR) {
    connected = FALSE;
    return ERROR;
  }
  return TRUE;
}
/*****************************************************************************/

void catchAlarm (int status)
 
// SIGALRM catcher.
{
  tick = TRUE;
}
/*****************************************************************************/
 
void catchPipe (int status)
 
// SIGPIPE catcher.
{
  tcpsock->closeSock();
}
/*****************************************************************************/


