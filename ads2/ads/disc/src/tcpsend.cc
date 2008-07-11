/* tcpsend.cc
   TCP socket test program for execution on the sun workstation.
   Connects to the socket, sends an ascii strings, listens for echoed data,
   and displays the echoed data.

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
int connected;				// socket connected flag

main (int argc, char *argv[])   
{
  if (argc < 2) {
    printf ("\nUsage: tcpsend host\n");
    exit (0);
  }

// Create the socket class.
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

  alarm(1);

// Main loop.  
  while (TRUE) {

// Wait for the socket connection.
    while (!connected) {
      if (tick) {
        fprintf (stderr, "Waiting for accept .\n");
        connected = tcpsock->acceptSock();
        tick = FALSE;
        alarm(1);
      }

      tcpsock->closeSock();
      tcpsock->openSock(TCP_CONNECT);
      connected = tcpsock->connectSock();
    }
    printf ("Connection made.\n");

    alarm(1);
    while (connected) {

// Send a test string once per second.
      if (tick) {
        tick = FALSE;
        alarm(1);
        if (tcpsock->writeSock ("TCP socket test string.\n", 25) == ERROR)
          connected = FALSE;
      }
      if (tcpsock->querySock() && readTcp())
        printf ("Received string: %s", buf);
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


