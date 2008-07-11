/* dsmComm.cc
   Distributed Sampling Module network communication task 

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
#include <vxWorks.h>
#include <mv162.h>
#include <bootLib.h>
#include <logLib.h>
#include <sigLib.h>
#include <stdioLib.h>
#include <taskLib.h>
#include <sysLib.h>
#include <usrLib.h>
 
#include <header.h>
#include <dsmctl.h>
#include <dsmVmeDefs.h>
#include <messageDefs.h>

// Class include files.
#include <DsmConfig.h>
#include <DsmNatsComm.h>
#include <DsmNet.h>
#include <MsgQueue.h>
#include <NatsData.h>
#include <TapeHeader.h>
#include <UdpSocket.h>

// Program functions.
static void processMessage();		// process received messages
static void checkBroadcast ();		// check for received broadcast msgs
static void netMessage (int, char*, char*);    // network message handler
static void statusMsg (char msg);       // status message handler
static void wdisr();			// watch dog isr
static void catchSigInt (int);		// cleanup on termination
static void natsInit();			// initialize the nats classes

extern BOOT_PARAMS sysBootParams;

// Class declarations. All global classes must be declared as pointers to
// work in a standalone vxWorks system.  Regularly declared global classes
// cause problems with the makeSymTbl program.
static DsmConfig *dsm_config;		// network configuration class
static DsmNatsComm *nats_comm;		// nats communication class
static DsmNet *dsm_net;			// network communications class 

#ifdef FOR_MASP
static MsgQueue *maspDQ;                 // masp data intertask queue
static MsgQueue *maspHQ;                 // masp histo intertask queue
#else
static MsgQueue *syncQ;                 // sync data intertask queue
static MsgQueue *pms2Q;                 // pms 2d data intertask queue
static MsgQueue *mcrQ;                  // mcr data intertask queue
static MsgQueue *greyQ;                 // grey scale data intertask queue
#endif
static MsgQueue *tx_msgQ;               // send message intertask queue
static MsgQueue *rx_msgQ;               // receive message intertask queue

static NatsData *nats_data;		// nats data handler
static TapeHeader *dsm_hdr; 		// tape header class
static UdpSocket *udp_sock;		// broadcast msg socket

/* -------------------------------------------------------------------- */

int dsmComm (
#ifdef FOR_MASP
	MsgQueue *masDQ,
	MsgQueue *masHQ,
#else
	MsgQueue *synQ, MsgQueue *mcQ, MsgQueue *p2Q, 
	MsgQueue *grQ,
#endif
	MsgQueue *txQ, MsgQueue *rxQ)
{
  static char buf[MX_PHYS];
  int	i, len;

  FILE	*fp;

// Create and initialize the message queues.
#ifdef FOR_MASP
  maspDQ = (MsgQueue*)malloc (sizeof (MsgQueue));
  *maspDQ = *masDQ;
  maspHQ = (MsgQueue*)malloc (sizeof (MsgQueue));
  *maspHQ = *masHQ;
#else
  syncQ = (MsgQueue*)malloc (sizeof (MsgQueue));
  *syncQ = *synQ;

  mcrQ = (MsgQueue*)malloc (sizeof (MsgQueue));
  *mcrQ = *mcQ;

  pms2Q = (MsgQueue*)malloc (sizeof (MsgQueue));
  *pms2Q = *p2Q;

  greyQ = (MsgQueue*)malloc (sizeof (MsgQueue));
  *greyQ = *grQ;
#endif
  tx_msgQ = (MsgQueue*)malloc (sizeof (MsgQueue));
  *tx_msgQ = *txQ;

  rx_msgQ = (MsgQueue*)malloc (sizeof (MsgQueue));
  *rx_msgQ = *rxQ;

// Create and initialize the network configuration class from the class 
// already in sram.
//  dsm_config = (DsmConfig*)malloc (sizeof (DsmConfig));
  dsm_config = new DsmConfig ();
//  *dsm_config = *(DsmConfig*)SRAM_DSMCONFIG_BASE;
  if (!dsm_config->selectByHost (sysBootParams.targetName)) {
    fprintf(stderr, "dsmComm exiting.\n");
    exit(ERROR);
  }

// Create and initialize the tape header class, using the class already in SRAM.
//  dsm_hdr = (TapeHeader*)malloc (sizeof (TapeHeader));
//  *dsm_hdr = *(TapeHeader*)SRAM_TAPEHEADER_BASE;
  dsm_hdr = new TapeHeader();
  dsm_hdr->readFile (dsm_config->dsmHeaderName());

// If the network is enabled, create the network communication classes.
  if (!dsm_config->standalone()) {
    dsm_net = new DsmNet(TCP_CONNECT, DSM_MSG_MAX_LEN, dsm_config->port(), 
                   sysBootParams.hostName, dsm_config->location(), netMessage);

// Create and open the broadcast message socket.
    udp_sock = new UdpSocket(DSM_BROADCAST_PORT, "");
    udp_sock->openSock(UDP_BROADCAST);
  }

// Initialize the NATS classes if telemetry is enabled.
  if (dsm_config->nats()) 
    natsInit(); 

// Set up the SIGINT handler.
  if ((int)signal(SIGINT, catchSigInt) < 0) {
    perror("signal SIGINT");
    exit(ERROR);
  }


#ifdef FOR_MASP
  taskDelay(sysClkRateGet());
#endif

  printf("dsmComm Main Loop.\n");
  // Main loop.  
  while (TRUE) {
    // Check the network connection.
    if (!dsm_config->standalone() && !dsm_net->connected()) {
      dsm_net->connectNet();
      dsm_net->setSendBufSize(32768);	// Max for vxWorks.
    }
#ifdef FOR_MASP

// Check for Masp data to send.
    if ((len = maspDQ->msgRecv (buf, MX_PHYS)) != ERROR) {
      if (!dsm_config->standalone()){
        dsm_net->writeNet (buf, len, DSM_MASP_DATA);
      }
    }

    if ((len = maspHQ->msgRecv (buf, MX_PHYS)) != ERROR) {
      if (!dsm_config->standalone()){
        dsm_net->writeNet (buf, len, DSM_MASP_HISTO);
      }
    }

#else

     // Check for sync data to send.
    if ((len = syncQ->msgRecv (buf, MX_PHYS)) != ERROR) {
//printf("%s: %02d:%02d:%02d\n", ((Hdr_blk *)buf)->dsm_locn,
//  ((Hdr_blk *)buf)->hour, ((Hdr_blk *)buf)->minute, ((Hdr_blk *)buf)->second);
      if (!dsm_config->standalone()) {
        dsm_net->writeNet (buf, len, DSM_SYNC_DATA);
      }

// If NATS is enabled, and the sampled data interval has elapsed,
// create the pickoff block, and send it. 
      if (dsm_config->nats() && nats_data->checkSmpIntv()) {
        nats_data->pickoff (buf);
        nats_comm->send (nats_data->buffer(), nats_data->pickLength());
        nats_data->releaseBuf();
//      printf ("nats block sent, len = %d\n", nats_data->pickLength());
      }

    }

// Check for 2d data to send.
    if ((len = pms2Q->msgRecv (buf, MX_PHYS)) != ERROR) {
      if (!dsm_config->standalone()){
{ P2d_rec *p = (P2d_rec*)buf;
        dsm_net->writeNet (buf, len, DSM_P2D_DATA);
// printf("%x %02d:%02d:%02d\n", p->id, p->hour, p->minute, p->second);
}

      }
    }

// Check for Mcr data to send.
    if ((len = mcrQ->msgRecv (buf, MX_PHYS)) != ERROR) {
      if (!dsm_config->standalone()){
        dsm_net->writeNet (buf, len, DSM_MCR_DATA);
      }
    }

// Check for grey scale data to send.
    if ((len = greyQ->msgRecv (buf, MX_PHYS)) != ERROR) {
      if (!dsm_config->standalone())
        dsm_net->writeNet (buf, len, DSM_GREY_DATA);
    }


// Check for message data to send.
    if ((len = tx_msgQ->msgRecv (buf, DSM_MSG_MAX_LEN)) != ERROR) {
      if (!dsm_config->standalone()) 
        dsm_net->writeNet (buf, len, DSM_MSG_DATA);
    }

#endif

// Check for net data to read.
    if (!dsm_config->standalone()) {
      switch (dsm_net->readNet()) {
        case DSM_NO_DATA:
        break;

        case DSM_MSG_DATA:
          processMessage ();
          break;

        default:
          fprintf (stderr, "dsmComm: Unknown data type received.\n");
      }

// Check for broadcast messages received.
      checkBroadcast();
    }

//    taskDelay (sysClkRateGet() / 10);		// delay 100 ms
    taskDelay(2);
  }

}
 
/* -------------------------------------------------------------------- */
static void processMessage ()
 
// Passes received messages from the net to the message Q.
{
#ifndef FOR_MASP
  if (rx_msgQ->msgSend (dsm_net->dsm_buf->getBuf(), 
      dsm_net->dsm_buf->bufIndex()) == ERROR) {
    fprintf (stderr, "dsmComm: processMessage: Error writing to rx_msgQ, errno = %d.\n", errno);

  }
#endif

  fprintf(stderr, "dsmComm: processMessage: %s %d.\n", dsm_net->dsm_buf->getBuf(), dsm_net->dsm_buf->bufIndex());

  dsm_net->dsm_buf->releaseBuf();      		// release the dsm buffer
}

/* -------------------------------------------------------------------- */
static void checkBroadcast ()

// Checks for broadcast messages received, and passes them to the message Q.
{
  char buf[DSM_MSG_MAX_LEN];
  int len;

  if (udp_sock->querySock()) {
    if ((len = udp_sock->readSock (buf, DSM_MSG_MAX_LEN)) > 0) {
#ifndef FOR_MASP
      if (rx_msgQ->msgSend (buf, len) == ERROR)
        fprintf (stderr, "dsmComm: checkBroadcast: Error writing to rx_msgQ, errno = %d.\n", len);
#endif
    }
  }
}
 
/* -------------------------------------------------------------------- */
static void netMessage (int action, char *name, char *msg_str)
 
// Network message handler.
{
  fprintf(stderr, msg_str);
}
 
/* -------------------------------------------------------------------- */
static void statusMsg (char *msg)
 
// Status message handler.
{
  fprintf(stderr, msg);
}
 
/* -------------------------------------------------------------------- */
static void catchSigInt (int t)

// Cleanup on termination.
{
  dsm_net->closeNet();
}

/* -------------------------------------------------------------------- */
static void natsInit()

// Initialize the NATS classes.
{
// Create the nats communication class.
  nats_comm = new DsmNatsComm (statusMsg);

// Create the nats data handler.
  nats_data = new NatsData (*dsm_hdr);
}
/* END DSMCOMM.CC */
