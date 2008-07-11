/* discTape.cc
   DIstributed Sampling Control tape control and recording task.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

// System header files.
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Application header files.
#include <dsmctl.h>

// Class include files.
#include <DsmConfig.h>
#include <DsmMessage.h>
#include <DsmNet.h>

#ifdef DISK_WRITE
#include <DsmDisk.h>
#endif
#ifdef TAPE_WRITE
#include <DsmTape.h>
#include <TapeDrv.h>
#endif

#include <TapeHeader.h>

// Program functions.
void checkMessage ();				// check for and process msgs
void netMessage (int, char*, char*);    	// network message handler

// Class declarations.
DsmConfig dsm_config; 				// config file class
DsmNet tape_comm_net (TCP_CONNECT, MX_PHYS, DSM_COMM_TAPE_PORT, getenv ("HOST"),
                      "Tape-Comm", netMessage); // Tape to Comm net
         
DsmNet tape_win_net (TCP_CONNECT, DSM_MSG_MAX_LEN, DSM_TAPE_WIN_PORT,
     getenv ("HOST"), "Tape-Win", netMessage);	// Tape to Win net 

DsmMessage win_msg (tape_win_net);		// window task messages class
DsmMessage comm_msg (tape_comm_net);		// comm task messages class
TapeHeader tape_hdr;				// tape header class

#ifdef TAPE_WRITE
DsmTape *dsm_tape;				// tape drive class
#endif
#ifdef DISK_WRITE
DsmDisk *dsm_disk;                              // removable 9 GB disk class
#endif

/* -------------------------------------------------------------------- */
main()
{
  time_t cur_sec;                       // current second count
  time_t last_sec = 0;                  // last second count
  Hdr_blk *hdr;				// sync record HDR block

  static int	firstSync = TRUE;	// first sync record received flag

// Wait for the interprocess network connections.
  while (!tape_comm_net.connectNet() || !tape_win_net.connectNet())
    sleep(1);

  sleep(2);

// Initialize the tape header class.
  tape_hdr.readFile(dsm_config.mainHeaderName());

// Null the flight number, date and time strings.  Recording is not be enabled 
// in DsmTape until these strings are filled in.
  tape_hdr.setFltnum("");	// null flight number string
  tape_hdr.clearDate();		// null date string
  tape_hdr.clearTime();		// null time string


// Create the tape and disk drive classes.
#ifdef TAPE_WRITE
  dsm_tape = new DsmTape(tape_hdr, win_msg, comm_msg);
#endif
#ifdef DISK_WRITE
  dsm_disk = new DsmDisk(tape_hdr, win_msg, dsm_config);
#endif

  tape_comm_net.setRecvBufSize(65536);

// Loop here as long as connected.
  while (tape_comm_net.connected() && tape_win_net.connected()) {

// Read and process data from the net.
    switch (tape_comm_net.readNet()) {
      case DSM_NO_DATA:
        break;
  
      case DSM_SYNC_DATA:
        hdr = (Hdr_blk *)tape_comm_net.dsm_buf->getBuf();
        tape_hdr.setTime(ntohs(hdr->hour), ntohs(hdr->minute), ntohs(hdr->second));
        tape_hdr.setDate(ntohs(hdr->year), ntohs(hdr->month), ntohs(hdr->day));
 
/*      printf(
          "discTape: date/time received = %02d/%02d/%d %02d:%02d:%02d\n",
		ntohs(hdr->month), ntohs(hdr->day), ntohs(hdr->year),
		ntohs(hdr->hour), ntohs(hdr->minute), ntohs(hdr->sec));
        netMessage(0, "", buffer);
*/

      default:
#ifdef TAPE_WRITE
        dsm_tape->writeDrive(tape_comm_net.dsm_buf->getBuf(), 
                             tape_comm_net.dsm_buf->bufIndex());
#endif
#ifdef DISK_WRITE
        dsm_disk->writeFiles(tape_comm_net.dsm_buf->getBuf(),
                            tape_comm_net.dsm_buf->bufIndex());
#endif
        tape_comm_net.dsm_buf->releaseBuf();
    }

    if (last_sec != time(&cur_sec)) {
      last_sec = cur_sec;
#ifdef TAPE_WRITE
      dsm_tape->checkDrives();		// check the drive status
#endif
      checkMessage();			// check for received messages
    }
  }

  fprintf(stderr, "discTape: Lost connection with ");
 
#ifdef DISK_WRITE
  dsm_disk->closeFiles();
#endif
  
  if (!tape_comm_net.connected())
    fprintf(stderr, "discComm, quiting\n");
 
  if (!tape_win_net.connected())
    fprintf(stderr, "discWin, quiting\n");

  tape_comm_net.closeNet();
  tape_win_net.closeNet();

}  /* END MAIN */

/* -------------------------------------------------------------------- */
void checkMessage ()

// Check for received command messages from the Win program.
{
  if (!win_msg.readMsg ()) 
    return;

// Process the command.
  switch (win_msg.type()) {
    case FLIGHT_MSG:
      tape_hdr.setFltnum(win_msg.flight()); // copy flight num to header
      break;

    case TAPE_MSG:
#ifdef TAPE_WRITE
      if (win_msg.action() == TAPE_UNLOAD)
        dsm_tape->unloadDrive(win_msg.drive());
      else
#endif
        fprintf(stderr,"DiscTape: Unknown tape action rcvd from win_msg.\n");
      break;

    default:
      fprintf(stderr,"DiscTape: Unknown message type received from win_msg.\n");
  }
}
 
/* -------------------------------------------------------------------- */
void netMessage (int action, char *name, char *msg_str)
 
// Network message handler.
{
  static int firstTime = TRUE;
  static FILE *fp;
 
  if (firstTime) {
    time_t      ct;
    char        *p, buffer[256];
 
    p = getenv("PROJ_DIR");
    dsm_config.selectByLocn("CTL");
    sprintf(buffer, "%s/hosts/%s/discTape.log", p, dsm_config.hostName());
 
    if ((fp = fopen(buffer, "a")) == NULL)
      fp = stderr;
 
    ct = time(NULL);
    fprintf(fp, "discTape: reset %s", ctime(&ct));
    firstTime = FALSE;
    }

  fprintf(fp, msg_str);

  if (name == NULL && fp != stderr)
    fclose(fp);

}

#ifdef TAPE_WRITE
/* -------------------------------------------------------------------- */
void *loadolaDrive(void *arg)
{
  dsm_tape->loadDrive((int)arg);
 
}

/* -------------------------------------------------------------------- */
void *ejectolaTape(void *arg)
{
  dsm_tape->ejectTape((int)arg);
 
}
#endif

/* END DISCTAPE.CC */
