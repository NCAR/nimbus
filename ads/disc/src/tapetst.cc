/* tapetst.cc
   Test program for communicating with the dsmTape program.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
#include <signal.h>
#include <sys/time.h>
 
#include <dsmctl.h>
 
// Class include files.
#include <DsmNet.h>
 
// Program functions.
void catchAlarm (int);                     // SIGALRM catcher
 
// Class declarations.
DsmNet tape_net (TCP_CONNECT, 0, 0, sizeof(MsgPacket),
  DSM_SMP_TAPE_PORT, DSM_ASY_TAPE_PORT, DSM_MSG_TAPE_PORT, getenv ("HOST"));
SerialBuf msg_buf (sizeof (MsgPacket)); // message data buffer class
 
// Global variables.
int tick;
char buf[MX_PHYS];
 
main()
{
  int net_connect = FALSE;
  int j;
  MsgPacket msg_pkt;

// Set up the SIGALRM handler.
  if ((int)sigset (SIGALRM, catchAlarm) < 0) {
    perror("dsmctl:sigset SIGALRM");
    exit (ERROR);
  }
 
// Init the test buffer.
  *(short*)buf = SAMPLED_ID;
  for (j = 2; j < MX_PHYS; j++)
    buf[j] = j;

  sprintf (msg_pkt.string, "%s", "Hello from tapetst.\n");

  alarm(1);
 
  while (1) {

    tape_net.queryNet();

    if (tape_net.checkMessage ()) {             // check for message data
      printf ("%s", ((MsgPacket*)tape_net.msg_buf->getBuf())->string);
      tape_net.msg_buf->releaseBuf();
    }

    tape_net.writeSampled (buf, 32768);
    tape_net.writeAsync (buf, 32768);
 
    if (tick) {
      tape_net.connectNet();          		// check net connection
      tape_net.writeMessage((char*)&msg_pkt, sizeof (MsgPacket));
      buf[2]++;
      alarm(1);                                 // restart timer
      tick = FALSE;
    }
  }
}
/*****************************************************************************/
 
void catchAlarm (int status)
 
// SIGALRM catcher.
{
  if (!tick)
    tick = TRUE;
}
/*****************************************************************************/

