/* dsmTape.cc
   DSM tape control and recording task.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

// System header files.
#include <vxWorks.h>
#include <stdioLib.h>
#include <wdLib.h>

// Application header files.
#include <dsmctl.h>
#include <dsmVmeDefs.h>

// Class include files.
#include <DsmConfig.h>
#include <DsmMessage.h>
#include <DsmTapeCtl.h>
#include <SerialBuf.h>
#include <TapeHeader.h>

// Program functions.
static void checkMessage();		// check for and process msgs
static void wdisr();			// watchdog timer isr

// Class declarations.
static DsmConfig *dsm_config; 		// network config class
static MsgQueue *syncQ;                 // sync data intertask queue
static MsgQueue *pms2Q;                 // pms 2d data intertask queue
static MsgQueue *greyQ;                 // grey scale data intertask queue
static MsgQueue *tape_txQ;   		// send message queue
static MsgQueue *tape_rxQ;   		// recv message queue
static DsmMessage *tape_msg; 		// message handler class
static TapeHeader *dsm_hdr;		// tape header class
static DsmTapeCtl *tape_ctl;		// tape control class
static SerialBuf *smp_buf;		// sampled data buffer
static SerialBuf *grey_buf;		// grey scale data buffer
static SerialBuf *pms2_buf;		// pms 2d data buffer

static WDOG_ID wid;			// watchdog timer id
static int new_sec;			// new second flag

int dsmTape (MsgQueue *synQ, MsgQueue *p2Q, MsgQueue *grQ, MsgQueue *txQ,
             MsgQueue *rxQ)
{
  Hdr_blk *hdr;				// sync record HDR block
  char *buf;				// temp buf pointer
  int len;				// data length
  int first_sync = TRUE;		// first sync record received flag

// Create the message queues.
  syncQ = (MsgQueue*)malloc (sizeof (MsgQueue));
  *syncQ = *synQ;
  pms2Q = (MsgQueue*)malloc (sizeof (MsgQueue));
  *pms2Q = *p2Q;
  greyQ = (MsgQueue*)malloc (sizeof (MsgQueue));
  *greyQ = *grQ;
  tape_txQ = (MsgQueue*)malloc (sizeof (MsgQueue));
  *tape_txQ = *txQ;
  tape_rxQ = (MsgQueue*)malloc (sizeof (MsgQueue));
  *tape_rxQ = *rxQ;

// Create and initialize the network configuration class, using the class 
// already in SRAM.
//  dsm_config = (DsmConfig*)malloc (sizeof (DsmConfig));
//  *dsm_config = *(DsmConfig*)SRAM_DSMCONFIG_BASE;
  dsm_config = new DsmConfig();
  if (!dsm_config->selectByHost (sysBootParams.targetName)) {
    fprintf (stderr, "dsmTape exiting.\n");
    exit (ERROR);
  }

// Create the message handler class.  The txQ is for messages sent to this
// task.  The rxQ is for messages to be received from this task.
  tape_msg = new DsmMessage (*tape_rxQ, *tape_txQ);

// Create and initialize the tape header class, using the class already in SRAM.
//  dsm_hdr = (TapeHeader*)malloc (sizeof (TapeHeader));
//  *dsm_hdr = *(TapeHeader*)SRAM_TAPEHEADER_BASE;
  dsm_hdr = new TapeHeader();

// Null the flight number, date and time strings.  Recording is not be enabled 
// in DsmTape until these strings are filled in.
  dsm_hdr->setFltnum ("");		// null flight number string
  dsm_hdr->clearDate ();			// null date string
  dsm_hdr->clearTime ();			// null time string

// Create the tape control class.
  tape_ctl = new DsmTapeCtl (*dsm_hdr, *tape_msg); 

// Initialize the data buffer classes.
  smp_buf = new SerialBuf (dsm_hdr->lrlen() * dsm_hdr->lrppr());
  pms2_buf = new SerialBuf (MX_PHYS);
  grey_buf = new SerialBuf (MX_PHYS);

// Create and start a watchdog timer.
  wid = wdCreate();
  wdStart ((WDOG_ID)wid, sysClkRateGet(), (FUNCPTR)wdisr, 0);

// Main loop.
  while (TRUE) {
// Check for received sync data to record.
    if ((len = syncQ->msgRecv (smp_buf->getBufPtr(), MX_PHYS)) != ERROR) {
// If this is the first sync record, time stamp the header.
      if (first_sync) {
        hdr = (Hdr_blk*)smp_buf->getBufPtr();
        dsm_hdr->setTime (hdr->hour, hdr->minute, hdr->second);
        dsm_hdr->setDate (hdr->year, hdr->month, hdr->day);
        first_sync = FALSE;
      }

// Increment the buffer index.
      smp_buf->incrBufIndex (len);

// If the receive buffer is full, write the data to tape.
      if (smp_buf->bufFull()) {
        tape_ctl->writeDrive (smp_buf->getBuf(), smp_buf->bufIndex());
        smp_buf->releaseBuf();
      }
    }

// Check for received pms 2d data to record. Record it as it comes in.
    if ((len = pms2Q->msgRecv (buf = pms2_buf->getBufPtr(), MX_PHYS)) != ERROR) 
      tape_ctl->writeDrive (buf, len);

// Check for received grey scale data to record. Record it as it comes in.
    if ((len = greyQ->msgRecv (buf = grey_buf->getBufPtr(), MX_PHYS)) != ERROR) 
      tape_ctl->writeDrive (buf, len);

    if (new_sec) {
      new_sec = FALSE;
      tape_ctl->checkDrives ();		// check the drive status
      checkMessage ();			// check for received messages
    }
    taskDelay(sysClkRateGet() / 10);	// delay 100 ms
  }
}
/*****************************************************************************/

static void checkMessage ()

// Check for received tape command messages. 
{
  if (!tape_msg->readMsg ()) 
    return;
// Process the command.
  switch (tape_msg->type()) {
    case FLIGHT_MSG:
      dsm_hdr->setFltnum (tape_msg->flight ()); // copy flight num to header
      printf ("dsmTape: flight number = %s\n", dsm_hdr->fltnum ());
      break;

    case TAPE_MSG:
      switch (tape_msg->action()) {
        case TAPE_RECORD:
          tape_ctl->requestRecording (tape_msg->drive());
          printf ("dsmTape: record on drive %1d received.\n",tape_msg->drive());
          break;
        case TAPE_STOP:
          tape_ctl->stopRecording ();
          printf ("dsmTape: stop recording.");
          break;
        case TAPE_UNLOAD:
  printf("Go unload drive\n");
          tape_ctl->unloadDrive (tape_msg->drive());
          printf ("dsmTape: unload drive %1d received.\n", tape_msg->drive());
          break;
        default:
          fprintf (stderr, 
            "dsmTape: Unknown tape action received from tape_msg.\n");
      }
      break;

    default:
      fprintf (stderr, 
        "dsmTape: Unknown message type received from tape_msg.\n");
  }
}
/*****************************************************************************/
 
static void wdisr ()
 
// Watchdog timer isr.
{
// Restart the timer.
  wdStart ((WDOG_ID)wid, sysClkRateGet(), (FUNCPTR)wdisr, 0);
  new_sec = TRUE;
}
/*****************************************************************************/
