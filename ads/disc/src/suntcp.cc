/* suntcp.cc
   TCP socket test program for execution on the sun workstation.

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
#include <TcpSocket.h>

void queryNet ();			// query dsm nets for data
int readTcp ();				// poll dsms for sampled data
int checkData ();			// poll dsms for sampled data
void catchAlarm (int);			// SIGALRM signal handler
void catchPipe (int);			// SIGPIPE signal handler

SerialBuf *buf;
TcpSocket *tcpsock;

int tick = FALSE;
int activity;				// dsm activity counter
int connected;				// socket connected flag

main()   
{
  buf = new SerialBuf (MX_PHYS);
  tcpsock = new TcpSocket (1112, "dsm1");

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
    buf->releaseBuf();
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
    printf ("connection made.\n");

    alarm(1);
    while (connected) {
      if (tcpsock->querySock() && readTcp())
        checkData();

// Once per second, send an activity token.
      if (tick) {
        tick = FALSE;
        alarm(1);
      }
    }
  }
}
/*****************************************************************************/

int readTcp ()

// Reads data from the tcp socket.
{
  int len;
 
  len = MX_PHYS - buf->bufIndex();
 
// Read the data from the socket.
  if ((len = tcpsock->readSock(buf->getBufPtr(), len)) == 0)
    return FALSE;
  else if (len == ERROR) {
    connected = FALSE;
    return ERROR;
  }
 
// If the alignment is ok, increment the buffer index.
  buf->incrBufIndex (len);
 
// Check for a full buffer.
  if (buf->spaceAvail() > 0)
    return FALSE;
 
// If the buffer is full, the data block is complete.
  return TRUE;
}
/*****************************************************************************/

int checkData ()

// Check the data for validity.
{
  char *tbuf;
  int j;
  int len;
  int stat = OK;
 
  tbuf = buf->getBuf();
  len = buf->bufIndex();
  stat = OK;

// Check the data alignment.
  if (*(unsigned short*)tbuf != (unsigned short)SAMPLED_ID) {
    fprintf (stderr, "readNet: Misaligned data block.\n");
    stat = ERROR;
  }
  else {
    for (j = 3; j < len; j++) {
      if (*(tbuf + j) != (char)j) {
        printf ("misaligned at %d\n", j);
        stat = ERROR;
        break;
      }
    }
  }

// Release the buffer.
  buf->releaseBuf();
  printf ("%d\n", (int)tbuf[2]);
  return stat;
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


