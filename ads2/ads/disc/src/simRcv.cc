/* simRcv.cc
   discComm simulator

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

// System include files.
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <time.h>

// Application include files.
#include <dsmctl.h>
#include <header.h>

// Class header files.
#include <DsmConfig.h>
#include <DsmNet.h>
#include <DsmNets.h>
#include <DsmMessage.h>


// Program functions.
void processSync(DsmNet *);
void connectNets ();			// make and check net connections
void catchAlarm (int);			// SIGALRM catcher
void catchPipe (int);			// SIGPIPE catcher
void netMessage (int, char*, char*);    // network message handler


// Class declarations. The ordering of these declarations is specific.

DsmConfig dsm_config;
DsmNets dsm_nets (netMessage); 		// dsm network manager

char ctl_buf[256];			// temp buffer for ctl data
 

/* -------------------------------------------------------------------- */
main()   
{
  int	avaps = FALSE;
  int	stat;
  int	type;				// data block type
  time_t	cur_sec;		// current second count
  time_t	last_sec = 0;		// last second count

  dsm_config.selectByLocn (CTL_LOCN_STR);

// Set up the SIGPIPE handler.
#ifdef SVR4
  if ((int)sigset (SIGPIPE, catchPipe) < 0) {
#else
  if ((int)signal (SIGPIPE, catchPipe) < 0) {
#endif
    perror("simRcv: sigset SIGPIPE");
    exit (ERROR);
  }


//Main loop.  
  while (1) {

// Network mode.
      for (stat = (int)dsm_nets.firstNet(); stat; 
           stat = (int)dsm_nets.nextNet()) {

// Read and process data from the nets.
        switch (type = dsm_nets.curNet()->readNet()) {
          case DSM_NO_DATA:
            break;

          case DSM_SYNC_DATA:
            processSync (dsm_nets.curNet());
            break;
  
          default:
            fprintf (stderr, "Unknown data type received from %s net.\n", 
                     dsm_nets.hostName ());
        }
    }


// Once each second check the net connections, and check for Events from WINDS
    if (last_sec != time(&cur_sec)) {
      last_sec = cur_sec;
      connectNets();	
    }
  }

}	/* END MAIN */
 
/* -------------------------------------------------------------------- */
void processSync(DsmNet *curNet)
{
  const char *buf;
  Hdr_blk *hblk;

  if (!(int)(buf = curNet->dsm_buf->getBuf()))  // get a pointer to the buffer
    return;

  hblk = (Hdr_blk *)buf;

  printf("%s: %02d:%02d:%02d\n", hblk->dsm_locn, hblk->hour, hblk->minute,
	hblk->second);
  fflush(stdout);

  curNet->dsm_buf->releaseBuf();
}

/* -------------------------------------------------------------------- */
void connectNets ()
 
// Make and check the net connections.
{
  int stat;

// If acceptNet returns true, the net is connected.  Send an activity packet 
// to verify the connection.

  for (stat = (int)dsm_nets.firstNet(); stat; stat = (int)dsm_nets.nextNet()) {
    if (dsm_nets.curNet()->acceptNet())
      dsm_nets.curNet()->sendActivityPacket();
  }
}

/* -------------------------------------------------------------------- */
void netMessage (int action, char *name, char *msg_str)
 
// Network message handler.
{
  static int firstTime = TRUE;
  static FILE *fp;

  if (firstTime) {
    time_t	ct;
    char	*p, buffer[256];

    p = getenv("DISC");
    dsm_config.selectByLocn("CTL");
    sprintf(buffer, "%s/config/%s/discComm.log", p, dsm_config.hostName());

    if ((fp = fopen(buffer, "a")) == NULL)
      fp = stderr;

    ct = time(NULL);
    fprintf(fp, "discComm: reset %s", ctime(&ct));
    firstTime = FALSE;
    }

  fprintf(fp, msg_str); fflush(fp);
}

/* -------------------------------------------------------------------- */
void catchAlarm (int stat)
 
// SIGALRM catcher.
{
}
 
/* -------------------------------------------------------------------- */
void catchPipe (int stat)
 
// SIGPIPE catcher.
{
}
