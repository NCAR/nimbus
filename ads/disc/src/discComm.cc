/* discComm.cc
   DIstributed Sampling Control network communication task.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

// System include files.

#include <semaphore.h>
#include <pthread.h>

#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>

// Application include files.
#include <dsmctl.h>
#include <header.h>

// Class header files.
#include <DiscNatsComm.h>
#include <DsmConfig.h>
#include <DsmHeaders.h>
#include <DsmNet.h>
#include <DsmNets.h>
#include <DsmMessage.h>
#include <DsmTables.h>
#include <DsmCollect.h>
#include <Event.h>
#include <NatsData.h>
#include <Palt.h>
#include <SyncQueue.h>
#include <SyncVar.h>
#include <Tasx.h>
#include <UdpSocket.h>
//#include <UnixTask.h>
#include <VarConfig.h>


// Program functions.
void connectNets();			// make and check net connections
void processSync(DsmNet* cur_net);	// process sync data
void buildRecord(DsmNet* cur_net);	// process sync data, part 2
void processAsync(DsmNet* cur_net, int type);
void processMessage(DsmNet* cur_net);	// process message data
void *serialThread(void *arg);		// watch serial port for AVAPS data
void processNats();			// process NATS data
void checkIRS_GPS(const char *buf);	// Validate IRS & GPS
void computeDerived(const char *buf);	// compute derived variables
void checkMessage ();			// check for messages from gui
void netMessage (int, char*, char*); 	// network message handler
void tasAltMessage (float tas, float alt); // broadcasts tas and palt to dsms
void radarAltMessage (float alt);	// send radar alt to AFT dsm.
void dateMessage (int, int, int);	// broadcasts date to dsms
void dateTimeMessage ();		// broadcasts date to dsms
void catchAlarm (int);			// SIGALRM catcher
void catchPipe (int);			// SIGPIPE catcher
void buildCtlHdr (Hdr_blk*);		// build the ctl station header block
void ualarm (int, int);			// replacement for ucb call
void derivedInit();              	// init derived variables
void natsInit();			// initialize NATS classes
void runNats();				// run in the NATS mode

// Class declarations. The ordering of these declarations is specific.

DsmConfig dsm_config;
DsmNet comm_tape_net (TCP_ACCEPT, MX_PHYS, DSM_COMM_TAPE_PORT, getenv("HOST"),
                 "Comm-Tape", netMessage);	// Comm to Tape net 

DsmNet comm_win_net (TCP_ACCEPT, MX_PHYS,  DSM_COMM_WIN_PORT, getenv("HOST"),
                     "Comm-Win", netMessage);  	// Comm to Win net

DsmMessage win_msg (comm_win_net);	// window task message handler
DsmMessage tape_msg (comm_tape_net);	// tape task message handler
DsmNets dsm_nets (netMessage); 		// dsm network manager
DsmHeaders dsm_headers (FALSE);		// dsm sampling headers
DsmTables dsm_tables (dsm_headers);	// data ordering tables
DsmCollect dsm_collect (dsm_tables, dsm_headers, netMessage); // data collector 
SyncQueue syncQueue(&dsm_nets, netMessage);	// dsm data queue'er class
VarConfig var_config (getenv ("HOST"), CTL_LOCN_STR);	// derived var config
Event events (&dsm_config);		// receive/fetch WINDS events
void sendTapeTask(const char *buf, int len, int type);

#ifdef SOCK_WINDS
void sendWINDS(const char *buf, int len, int type);
#endif

DiscNatsComm *nats_comm;		// nats communications class
NatsData *nats_data;			// nats data handler

// Derived variable classes.
SyncVar *qcx;				// dynamic pressure
SyncVar *psx;				// static pressure
SyncVar *ttx;				// total temperature
SyncVar *hgm;				// Radar Altimeter
Palt *palt;				// pressure altitude
Tasx *tasx;				// true air speed

// Broadcast sockets.
UdpSocket udp_sock(DSM_BROADCAST_PORT, "128.117.184.255");

char ctl_buf[256];			// temp buffer for ctl data
 
static time_t	cur_sec;		// current second count
static char	FlightNumber[16];	// Sent from DiscWin

static sem_t	tapeSema, windsSema;
static pthread_t	tid;

/* -------------------------------------------------------------------- */
int main()   
{
  int	stat;
  int	type;				// data block type

  static time_t	last_sec = 0;		// last second count

  // If NATS is enabled, create the NATS classes, and run in the NATS mode.
  dsm_config.selectByLocn (CTL_LOCN_STR);
  if (dsm_config.nats()) 
    natsInit();

  dsm_config.selectByLocn (CTL_LOCN_STR);

  // Wait for the interprocess network connections.
  while (!comm_tape_net.acceptNet() || !comm_win_net.acceptNet())
    sleep (1);

  comm_tape_net.setSendBufSize(65536);


// Set up the SIGPIPE handler.
#ifdef SVR4
  if ((int)sigset (SIGPIPE, catchPipe) < 0) {
#else
  if ((int)signal (SIGPIPE, catchPipe) < 0) {
#endif
    perror("discComm: sigset SIGPIPE");
    exit (ERROR);
  }

// Initialize the derived variables classes.
  derivedInit();
 
// Open the broadcast message socket.
  udp_sock.openSock(UDP_BROADCAST);

  sem_init(&tapeSema, false, 1);
  sem_init(&windsSema, false, 1);

// Place discComm into the Real-Time scheduling class.
//  UnixTask        chgPrio_task("prio_rt");

// Start AVAPS serial port thread.
/*
  if (dsm_headers.mainHeader()->getAsync(AVAPS_STR))
    stat = pthread_create(&tid, NULL, serialThread, NULL);
*/

// Used when you have no GPS and want to set the DSM time to the host computer.
//  dateTimeMessage();

  // Main loop.  
  while (comm_tape_net.connected() && comm_win_net.connected())
    {
    // Network mode.
    if (!dsm_config.nats())
      {
      cur_sec = time(NULL);

      for (stat = (int)dsm_nets.firstNet(); stat; 
           stat = (int)dsm_nets.nextNet())
        {
        do
          {
          // Read and process data from the nets.
          type = dsm_nets.curNet()->readNet();

          switch (type)
            {
            case DSM_NO_DATA:
              break;

            case DSM_SYNC_DATA:
              processSync(dsm_nets.curNet());
              break;
  
            case DSM_P2D_DATA:
            case DSM_HVPS_DATA:
            case DSM_GREY_DATA:
            case DSM_MCR_DATA:
              processAsync(dsm_nets.curNet(), type);
              break;

            case DSM_MASP_DATA:
              processAsync(dsm_nets.curNet(), type);
              break;

            case DSM_MASP_HISTO:
               processSync(dsm_nets.curNet());
               break;
                
            case DSM_MSG_DATA:
              processMessage(dsm_nets.curNet());
              break;

            default:
              fprintf(stderr, "Unknown data type received from %s net.\n", 
                     dsm_nets.hostName());
            }
          }
        while (type != DSM_NO_DATA);
        }
      }

    // NATS mode.
    else if (nats_comm->recv(nats_data->pickLength())) 
      processNats();

    // Check for messages from the GUI and tape tasks.
    checkMessage();


    // Once each second check the net connections, and check for Events
    // from WINDS.
    if (last_sec != cur_sec)
      {
      last_sec = cur_sec;
      connectNets();	
      events.checkEvents();
      syncQueue.CheckQuLengths();
      }
    }


  /* If we lost connections, then print who lost.
   */
  fprintf(stderr, "discComm: Lost connection with ");

  if (!comm_tape_net.connected())
    fprintf(stderr, "discTape, quiting\n");

  if (!comm_win_net.connected())
    fprintf(stderr, "discWin, quiting\n");

  return(0);

}	/* END MAIN */
 
/* -------------------------------------------------------------------- */
void connectNets()
 
// Make and check the net connections.
{
  int stat;

// If acceptNet returns true, the net is connected.  Send an activity packet 
// to verify the connection.
  for (stat = (int)dsm_nets.firstNet(); stat; stat = (int)dsm_nets.nextNet())
    if (dsm_nets.curNet()->acceptNet())
      dsm_nets.curNet()->sendActivityPacket();

}

/* -------------------------------------------------------------------- */
void processSync(DsmNet *curNet)

// Processes synchronous data from the current dsm.
{
  Hdr_blk *rp;

  if ((rp = (Hdr_blk *)curNet->dsm_buf->getBuf()) == NULL)
    return;
 
// Send time of this data to discWin.
  win_msg.sendDSMTimeMsg(rp);
 
  if (syncQueue.EnterNewData(curNet) == false)
    return;	// data from all connected dsm's must not be in yet.

  while (syncQueue.RetrieveNextRecord(curNet))
    buildRecord(curNet);

}

/* -------------------------------------------------------------------- */
void buildRecord(DsmNet *curNet)
{
  int		j, stat;
  const char	*buf = curNet->dsm_buf->getBuf();

// Enter the dsm data into the tape buffer.
  if ((stat = dsm_collect.buildLogical(buf)) == ERROR) {
    fprintf (stderr, "discComm: buildLogical error.\n");
    curNet->resetNet();		// data error, reset the network
    return;
  }

// If this block is from the time master dsm, or just starting, then enter the
// CTL station data.
  if (stat & DSM_TIME_MASTER) {
    buildCtlHdr ((Hdr_blk*)buf);		// build the ctl block header
    dsm_collect.buildLogical (ctl_buf);		// enter the ctl station data
  }

// Check for a logical record completed.
  if (stat & DSM_LR_COMPLETE)
    {
    buf = dsm_collect.getLogical();

#ifdef SOCK_WINDS
// Send data to Winds.  We do disk files now.  CJW 12/99
    sendWINDS(buf, dsm_collect.lrLen(), DSM_SYNC_DATA);
#endif
    
// Compute derived variables.
    computeDerived(buf);

// Check IRS & GPS
    checkIRS_GPS(buf);

// Broadcast the date to the dsms.
    dateMessage(ntohs(((Hdr_blk *)buf)->year), ntohs(((Hdr_blk *)buf)->month),
		ntohs(((Hdr_blk *)buf)->day));


// Send a time message to the gui task.
    win_msg.sendTimeMsg(	ntohs(((Hdr_blk *)buf)->year),
				ntohs(((Hdr_blk *)buf)->month),
				ntohs(((Hdr_blk *)buf)->day),
				ntohs(((Hdr_blk *)buf)->hour),
				ntohs(((Hdr_blk *)buf)->minute),
				ntohs(((Hdr_blk *)buf)->second));

// Uncomment this line to look at a particular data location.
/*
    for (j = 0; j < 25; j++)
      printf ("0x%X ", *(unsigned short*)(buf+156+(j*144)));
    putchar('\n');
*/
  }

// If a physical record is complete, send it to the tape task.
  if (stat & DSM_PR_COMPLETE) {
    sendTapeTask(dsm_collect.getPhysical(), 
                            dsm_collect.getPhysLen(), DSM_SYNC_DATA);
    dsm_collect.releasePhysical ();	// release the physical buffer
  }

  curNet->dsm_buf->releaseBuf();	// release the buffer
}
 
/* -------------------------------------------------------------------- */
void processAsync(DsmNet *curNet, int type)
 
// Processes async data from the current dsm.
{
  char	*buf;
  static time_t	last_sec = 0;		// last second count

  if (!(int)(buf = curNet->dsm_buf->getBuf()))	// get a pointer to the buffer
    return;

  // Send to the tape task.
  sendTapeTask(buf, curNet->dsm_buf->bufIndex(), type); 

  switch (type)
    {
    case DSM_MCR_DATA:
/*
      {
      Mcr_rec *m = (Mcr_rec *)buf;
      short *p = (short *)m->data;
      printf("%s, %02d:%02d:%02d\n", m->id, m->hour, m->minute, m->second);
      }
*/
      break;

    case DSM_P2D_DATA:
    case DSM_GREY_DATA:
    case DSM_HVPS_DATA:
#ifdef SOCK_WINDS
      if (last_sec != cur_sec)	// only send one logical/second
        {
        if (type != DSM_GREY_DATA) {
          sendWINDS(buf, sizeof(P2d_rec), type);
        }
        else {
          sendWINDS(buf, curNet->dsm_buf->bufIndex(), type);
        }
        last_sec = cur_sec;
        }
#endif
      break;
    }

  curNet->dsm_buf->releaseBuf();	// release the buffer

}	/* END PROCESSASYNC */
 
/* -------------------------------------------------------------------- */
void processMessage(DsmNet *curNet)
 
// Processes message data from the current dsm.
{
// Pass the message to the gui task.
  comm_win_net.writeNet(curNet->dsm_buf->getBuf(),
                        curNet->dsm_buf->bufIndex(), DSM_MSG_DATA);
  curNet->dsm_buf->releaseBuf();	// release the dsm buffer
}

/* -------------------------------------------------------------------- */
void processNats ()

// Processes data blocks received from the NATS port.
{
  int stat;
  const char *buf;

// Expand the pickoff block.
  nats_data->expand (nats_comm->buffer());	// expand the pickoff block
  nats_comm->releaseBuf();

// Enter the dsm data into the tape buffer.
  if ((stat = dsm_collect.buildLogical (nats_data->buffer()))== ERROR) {
    fprintf (stderr, "dsmComm: buildLogical error.\n");
    nats_comm->releaseBuf();
    return;
  }
  nats_comm->releaseBuf();

// Enter the CTL station data.
  buildCtlHdr ((Hdr_blk*)nats_data->buffer()); // build the ctl block header
  dsm_collect.buildLogical (ctl_buf);         // enter the ctl station data
 
// Check for a logical record completed.
  if (stat & DSM_LR_COMPLETE) {
 
// Send data to Winds.
    buf = dsm_collect.getLogical();
#ifdef SOCK_WINDS
    sendWINDS(buf, dsm_collect.lrLen(), DSM_SYNC_DATA);
#endif
 
// Send a time message to the gui task.
    win_msg.sendTimeMsg(
		ntohs(((Hdr_blk *)buf)->year),
		ntohs(((Hdr_blk *)buf)->month),
		ntohs(((Hdr_blk *)buf)->day),
		ntohs(((Hdr_blk *)buf)->hour),
		ntohs(((Hdr_blk *)buf)->minute),
		ntohs(((Hdr_blk *)buf)->second));
  }
 
// If a physical record is complete, send it to the tape task.
  if (stat & DSM_PR_COMPLETE) {
    sendTapeTask(dsm_collect.getPhysical(), 
                            dsm_collect.getPhysLen(), DSM_SYNC_DATA);
    dsm_collect.releasePhysical();     // release the physical buffer
  }
}
 
/* -------------------------------------------------------------------- */
void checkIRS_GPS(const char *buf)
{
// Check to make sure IRS & GPS are not all zeroes (i.e. are they turned on).

  static int firstTime = 0;
  static int IRSstart = 0, GPSstart = 0;
  static char *dsm;
  static char zeroes[sizeof(Irs_blk)];

  if (firstTime++ == 0) {
    int         stat;
    TapeHeader  *hdr = dsm_headers.mainHeader();

    memset(zeroes, 0, sizeof(zeroes));
    dsm_config.selectByLocn("CTL");
    dsm = dsm_config.location();

    for (stat = hdr->firstDesc(); stat; stat = hdr->nextDesc())
      if (hdr->irsType()) {
        IRSstart = hdr->start();
        break;
      }

    for (stat = hdr->firstDesc(); stat; stat = hdr->nextDesc())
      if (hdr->gpsTans3Type()) {
        GPSstart = hdr->start();
        break;
      }
  }

  // Only check every 5 seconds.
  if (!(firstTime % 5) && strcmp(win_msg.flight(), "0")) {
    if (memcmp(&buf[IRSstart], zeroes, sizeof(Irs_blk)) == 0) {
      netMessage(NET_STATUS, dsm, "  *** No IRS data! ***\n");
    }

    if (memcmp(&buf[GPSstart], zeroes, sizeof(Gps_blk)) == 0) {
      netMessage(NET_STATUS, dsm, "  *** No GPS data! ***\n");
    }
  }
}

/* -------------------------------------------------------------------- */
void computeDerived(const char *buf)
 
// Computes and used derived variables from the passed in logical record.
{
  static float tas = 0.0;               // current values
  static float alt = 0.0;
  static float hgm232 = 0.0;
 
  if ((int)psx)                         // static pressure
    psx->computeFromLR(buf);
 
  if ((int)qcx)                         // dynamic pressure
    qcx->computeFromLR(buf);
 
  if ((int)ttx)                         // total temperature
    ttx->computeFromLR(buf);
 
  if ((int)hgm)                         // Radar altimeter
    {
    hgm->computeFromLR(buf);
    radarAltMessage(hgm->value());
    }
 
  if ((int)tasx) {                      // true air speed
    tasx->compute();
    tas = tasx->value();

    if (FlightNumber[0] == '0' && tas > 5.0)
      {
      dsm_config.selectByLocn("CTL");
      netMessage(NET_STATUS, dsm_config.location(),
		"  *** Flight Number is 0! ***\n");
      }
  }

  if ((int)palt) {                      // pressure altitude
    palt->compute();
    alt = tasx->value();
  }
 
//printf ("psx = %f, qcx = %f, ttx = %f, tas = %f, alt = %f\n", 
//          psx->value(), qcx->value(), ttx->value(), tas, alt);

// Broadcast tas and palt to the dsms.
  tasAltMessage(tas, alt);

}
 
/* -------------------------------------------------------------------- */
void checkMessage()
 
// Checks for received messages from the window and tape tasks.
{
  int stat, dummy;

  if (win_msg.readMsg()) {
    switch (win_msg.type()) {
 
// Relay messages to the appropriate dsm.
      case ANALOG_MSG:             	// Analog control message
      case MCR_MSG:             	// Mcr message
      case DIGOUT_MSG:             	// Mcr message
      case PMS1_MSG:             	// Pms 1d message
      case PMS2_MSG:             	// Pms 2d message
        dsm_nets.selectNet (win_msg.location());	// select dest dsm
        dsm_nets.curNet()->writeNet (
                   win_msg.rxMessage(), win_msg.rxLength(), DSM_MSG_DATA);
        break;

      case FLIGHT_MSG:
        for (stat = (int)dsm_nets.firstNet(); stat; 
             stat = (int)dsm_nets.nextNet())
           dsm_nets.curNet()->writeNet (
                   win_msg.rxMessage(), win_msg.rxLength(), DSM_MSG_DATA);

//        sscanf(rx_msg, "%d %s", &dummy, FlightNumber);
        break;

      default:
        fprintf (stderr, 
          "discComm: Unknown message type received from the gui task, %d.\n", 
          win_msg.type());
        exit (ERROR);
    }
  }

// If a message is received from the tape task, turn on or off the data flow
// to the tape task.
  if (tape_msg.readMsg()) {
    if (tape_msg.action() == NET_ENABLE)
      comm_tape_net.enableNet();
    else
      comm_tape_net.suspendNet();
  }
}
 
/* -------------------------------------------------------------------- */
void *serialThread(void *arg)
{
  char	buff[2048];
  FILE	*fp;
  int	len;

  if ((fp = fopen("/dev/tty0", "r")) == NULL)
    {
    fprintf(stderr, "discComm: can't open /dev/tty0.\n");
    sprintf(buff,"WARNING: COULD NOT OPEN SERIAL PORT /dev/tty0 FOR READING AVAPS DATA. WARNING!!!\n");
    win_msg.sendStatusMsg(buff);
    return(NULL);
    }
  else
    fprintf(stderr, "discComm: tty0 open for reading AVAPS.\n");

  {
  struct termios	tty;
  speed_t spd;

  tcgetattr(fileno(fp), &tty);
  tty.c_lflag &= ~ECHO;
//  tty.c_cflag |= CSTOPB;
  cfsetispeed(&tty, B19200);
  cfsetospeed(&tty, B19200);
  tcsetattr(fileno(fp), TCSANOW, &tty);
  }

  while (1)
    {
    fgets(buff, 2048, fp);

    if (buff[0] != 'A')
      continue;

    buff[2047] = '\0';

    len = strlen(buff);

    sendTapeTask(buff, len, AVAPS_DATA);

#ifdef SOCK_WINDS
    if (memcmp(buff, "AVAPS-D", 7) == 0 && len == 132)
	 // Don't send text msgs or corrupt data to WINDS
      sendWINDS(buff, len, AVAPS_DATA);
#endif
    }

  fclose(fp);

}

/* -------------------------------------------------------------------- */
void netMessage(int action, char *name, char *msg_str)
 
// Network message handler.
{
  static int	firstTime = TRUE;
  static FILE	*fp;

  // Don't send interprocess net messages to the gui task.
  if (strcmp(name, "Comm-Win") && strcmp(name, "Comm-Tape")) 
    win_msg.sendNetMsg (action, name, msg_str);

  // Log message to /home/local/proj/hosts/*/discComm.log
  if (firstTime) {
    time_t	ct;
    char	*p, buffer[256];

    p = getenv("PROJ_DIR");
    dsm_config.selectByLocn("CTL");
    sprintf(buffer, "%s/hosts/%s/discComm.log", p, dsm_config.hostName());

    if ((fp = fopen(buffer, "a")) == NULL)
      fp = stderr;

    ct = time(NULL);
    fprintf(fp, "discComm: reset %s", ctime(&ct));
    firstTime = FALSE;
    }

  fprintf(fp, msg_str);

  if (name == NULL && fp != stderr)
    fclose(fp);

}	/* END NETMESSAGE */

#ifdef SOCK_WINDS
/* -------------------------------------------------------------------- */
void sendWINDS(const char *buf, int len, int type)
{
  sem_wait(&windsSema);
  dsm_nets.windsNet()->writeNet(buf, len, type);
  sem_post(&windsSema);

}	/* END SENDWINDS */
#endif

/* -------------------------------------------------------------------- */
void sendTapeTask(const char *buf, int len, int type)
{
  sem_wait(&tapeSema);
  comm_tape_net.writeNet(buf, len, type);
  sem_post(&tapeSema);

}	/* END SENDTAPETASK */

/* -------------------------------------------------------------------- */
void tasAltMessage(float tas, float alt)

// Broadcasts computed tas and palt needed by the dsms.
{
  static char tx_msg[DSM_MSG_MAX_LEN];

  sprintf (tx_msg, "%1d %8.3f %8.3f\n", TASALT_MSG, tas, alt);
  udp_sock.writeSock (tx_msg, strlen (tx_msg)+1);

}

/* -------------------------------------------------------------------- */
void radarAltMessage(float alt)

// Send (as a Generic DIGOUT button) radar altimeter to AFT dsm for SABLE.
{
  static int	action;
  char	tx_msg[DSM_MSG_MAX_LEN];

  static bool	bloSixK = false;
  static char	*locn = "AFT";
  static int	connector = 3;
  static int	channel = 0;
  static float  prev_alt;

/*
  if (alt < 2000.0 && bloSixK ||	// No toggle required.
      alt > 2000.0 && !bloSixK)
    return;

  if (alt < 2000.0)
    action = 1;
  else
    action = 0;
*/

  if (action) {
    if (prev_alt < 2000 && alt > 2000)
      action = 0;
  }
  else {
    if (prev_alt > 1900 && alt < 1900)
      action = 1;
  }
  prev_alt = alt;
netMessage(NET_STATUS, dsm_config.location(), "Crossed 6k feet\n");
return;

  sprintf(tx_msg, "%2d %1d %11s %1d %1d %s", DIGOUT_MSG, action,
                "AFT", connector, channel, "");

  dsm_nets.selectNet("AFT");        // select dest dsm
  dsm_nets.curNet()->writeNet(tx_msg, strlen(tx_msg)+1, DSM_MSG_DATA);

}
 
/* -------------------------------------------------------------------- */
void dateMessage (int year, int month, int day)
 
// Broadcasts the current date to the dsms.
{
  time_t t;
  struct tm *ts;
  static char tx_msg[DSM_MSG_MAX_LEN];
 
  t = time(NULL);
  ts = gmtime(&t);

  sprintf (tx_msg, "%1d %2d %2d %2d\n", DATE_MSG, ts->tm_year,  ts->tm_mon+1, 
           ts->tm_mday);
  udp_sock.writeSock (tx_msg, strlen (tx_msg)+1);
}
 
/* -------------------------------------------------------------------- */
void dateTimeMessage ()
 
// Broadcasts the current date to the dsms.
{
  time_t t;
  struct tm *ts;
  static char tx_msg[DSM_MSG_MAX_LEN];
 
  t = time(NULL);
  ts = gmtime(&t);

  sprintf (tx_msg, "%1d %2d %2d %2d %d %d %d\n", TIME_MSG, ts->tm_year, 
           ts->tm_mon+1, ts->tm_mday, ts->tm_hour, ts->tm_min, ts->tm_sec);
  udp_sock.writeSock (tx_msg, strlen (tx_msg)+1);
}
 
/* -------------------------------------------------------------------- */
void catchAlarm (int stat)
{

}
 
/* -------------------------------------------------------------------- */
void catchPipe (int stat)
{

}

/* -------------------------------------------------------------------- */
void buildCtlHdr (Hdr_blk *hdr_blk)

// Builds the ctl station Hdr_blk struct.
{
  struct EVTPKT	*evt;
  PickTable *table;
  PickEntry *pent;
  char *p = (char *)ctl_buf;

  memcpy (ctl_buf, (char*)hdr_blk, sizeof (Hdr_blk));	// copy time stamp
  strcpy (((Hdr_blk*)ctl_buf)->dsm_locn, CTL_LOCN_STR);	// fill in location

  table = dsm_tables.selectTable(CTL_LOCN_STR);

  for (pent = table->firstEntry(); (pent = table->nextEntry()); )
    *((long *)(&p[pent->source])) = 0;

// Check for events and record in data.
  while ( (evt = events.getEvent()) )
    for (pent = table->firstEntry(); (pent = table->nextEntry()); )
      if (strcmp(pent->name, evt->station) == 0)
        *((long *)(&p[pent->source])) = evt->number;

}
/*****************************************************************************/
/*
void ualarm (int usec, int repeat)
 
// Starts a timer.  Replacement for ucb call.
{
  struct itimerval timer;
 
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = usec;
  timer.it_interval.tv_sec = 0;
 
// If the timer is to be restarted, set the interval struct.
  if (repeat)
    timer.it_interval.tv_usec = usec;
  else
    timer.it_interval.tv_usec = 0;
 
  setitimer (ITIMER_REAL, &timer, (struct itimerval*)0);
}
*/
/*****************************************************************************/
 
void derivedInit()
 
// Creates and initializes the derived variables classes.
{
  int stat;
 
  psx = (SyncVar*)0;
  qcx = (SyncVar*)0;
  ttx = (SyncVar*)0;
  palt = (Palt*)0;
  tasx = (Tasx*)0;
 
//  hgm = new SyncVar(dsm_headers.mainHeader(), "HGM232");

  for (stat = var_config.firstVar(); stat; stat = var_config.nextVar()) {
    if (!strcmp (var_config.varName(), "PSX")) {
      var_config.firstDepend();
      psx = new SyncVar (dsm_headers.mainHeader(), var_config.dependName());
    }
 
    else if (!strcmp (var_config.varName(), "QCX")) {
      var_config.firstDepend();
      qcx = new SyncVar (dsm_headers.mainHeader(), var_config.dependName());
    }
 
    else if (!strcmp (var_config.varName(), "TTX")) {
      var_config.firstDepend();
      ttx = new SyncVar (dsm_headers.mainHeader(), var_config.dependName());
    }
 
    else if (!strcmp (var_config.varName(), "PALT")) {
      if (!(int)psx) {
        fprintf (stderr,
                 "Undefined dependencies for PALT in the varconfig file.\n");
        exit (ERROR);
      }
      palt = new Palt (*psx);
      printf ("palt initialized.\n");
    }
 
    else if (!strcmp (var_config.varName(), "TASX")) {
      if (!(int)psx || !(int)qcx || !(int)ttx) {
        fprintf (stderr,
                 "Undefined dependencies for TASX in the varconfig file.\n");
        exit (ERROR);
      }
      tasx = new Tasx (*qcx, *psx, *ttx);
      printf ("tasx initialized.\n");
    }
 
    else
      fprintf (stderr,
        "Unknown derived variable requested in the varconfig file, %s.\n",
        var_config.varName());
  }

}

/*****************************************************************************/
void natsInit()

// Checks if the NATS is enabled, and creates the classes if so.
{
  int stat;

// Find the DSM for which NATS is enabled and create a NATS data handler, and
// communication class.  For now we are assuming only a single NATS dsm is in
// use.
  for (stat = (int)dsm_config.firstDsm(); stat; 
       stat = (int)dsm_config.nextDsm()) {
    if (dsm_config.isDsmLocation() && dsm_config.nats()) {
      dsm_headers.selectHeader (dsm_config.location());
      nats_data = new NatsData (*dsm_headers.curHeader());
      nats_comm = new DiscNatsComm (nats_data->pickLength());
      printf ("NATS pickLength = %d\n", nats_data->pickLength());
      break;
    }
  }
}
/*****************************************************************************/
