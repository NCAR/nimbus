/* dsmAsync.cc
   DSM async program to run on a Motorola Mvme162 board.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

// System include files.
#include <vxWorks.h>
#include <bootLib.h>
#include <intLib.h>
#include <logLib.h>
#include <msgQLib.h>
#include <semLib.h>
#include <stdioLib.h>
#include <sysLib.h>
#include <taskLib.h>
#include <math.h>

// Program include files.
#include <dsmctl.h>
#include <dsmVmeDefs.h>
#include <header.h>
#include <ipicDefs.h>
#include <messageDefs.h>

// Class include files.
#include <AnalogCal.h>
#include <Bc635Vme.h>
#include <Climet.h>
#include <Counter.h>
#include <DigitalIn.h>
#include <DigOut.h>
#include <Dpres.h>
#include <DsmConfig.h>
#include <DsmMessage.h>
#include <Dsp56002.h>
#include <GpsTans2.h>
#include <GreyVme.h>
#include <GreyVmeHouse.h>
#include <HwIrs.h>
#include <HwGps.h>
#include <IP429.h>
#include <IPcounter.h>
#include <Mca.h>
#include <Mcr.h>
#include <MsgQueue.h>
#include <Neph.h>
#include <Ophir3.h>
#include <Palt.h>
#include <Pms1Vme.h>
#include <PmsVme2d.h>
#include <Pms2dHouse.h>
#include <PpsGps.h>
#include <Rdma.h>
#include <SerialTod.h>
#include <SampleTable.h>
#include <Spp.h>
#include <SyncRecord.h>
#include <Synchro.h>
#include <SyncVar.h>
#include <TapeHeader.h>
#include <Tasx.h>
#include <TodClock.h>
#include <UvHyg.h>
#include <VarConfig.h>
#include <Vmio12.h>
#include <Vm3118.h>


// Program functions. 
static void computeDerived (const char *buf);	// compute derived variables
static void checkMessage();		// check for and process messages
static void setPms1Range (char *name, int range);	
static void setTasAlt (float tas, float alt);
static void setTime (int year, int month, int day, 
                     int hour, int minute, int second);
static void setDate (int year, int month, int day);
static void statusMsg (char *msg);      // status message handler
static void analogInit();		// init analog sampling classes
static void digitalInit();		// init digital I/O interfaces
static void arincInit();		// init arinc interfaces
static void greyVmeInit();		// init grey scale interfaces
static void greyhInit();		// init pms VME Grey house interface
static void pms1VmeInit();		// init vme pms 1d interfaces
static void pms2dInit();		// init pms VME 2d interface
static void pms2dhInit();		// init pms VME 2d house interface
static void serialInit();		// init serial interfaces
static void tfpInit();			// init time-freq processor
static void derivedInit();     		// init derived variables
static void initInterrupts();		// initialize interrurpts
static void hertz50_isr(int hertz5_flag); // 50 hertz sampling isr
static void gatherData();		// Function to gather data from cards
static void hwIrsReadIsr (int intf);	// Honeywell irs read isr wrapper
static void hwGpsReadIsr (int intf);	// Honeywell gps read isr wrapper
static void hwIrsWriteIsr (int intf);	// Honeywell irs write isr wrapper
static void isio1Bim1Isr (int intf);	// isio1 chans 3,4 isr, (uv hyg)
static void isio1Bim2Isr (int intf);	// isio1 chans 3,4 isr, (uv hyg)
static void isio1Bim3Isr (int intf);	// isio1 chans 5,6 isr, (gps)
static void isio1Bim4Isr (int intf);	// isio1 chans 7,8 isr, (ophir3 & PPS)
static void dms_isr ();			// DigitalIn Isr for DMS

// Other tasks.
extern int dsmComm (MsgQueue *synQ, MsgQueue *mcQ,  
                    MsgQueue *p2Q, MsgQueue *gryQ, 
                    MsgQueue *txQ, MsgQueue *rxQ);
extern int dsmTape (MsgQueue *synQ, MsgQueue *p2Q, MsgQueue *gryQ, 
                    MsgQueue *txQ, MsgQueue *rxQ);
extern int dsmDisplay (MsgQueue *synQ, MsgQueue *txQ, MsgQueue *rxQ);

extern BOOT_PARAMS sysBootParams;

// Class declarations.

DsmConfig *dsm_config; 		// sram net config
DsmMessage *comm_msg;		// comm message handler class

static AnalogCal *analogCal;		// Analog calibration class
static Bc635Vme *tfp; 			// time-freq processor
static Counter *counter;		// counter sampling class
static DigitalIn *digital_in;		// digital inputs sampling class
static Dpres *dpres[MAX_UVHYG_INTFC];	// digital pressure
static DsmMessage *disp_msg; 		// display message handler class
static DsmMessage *tape_msg;		// tape message handler class
static Dsp56002 *dsp;			// dsp interface class
static GpsTans2 *tans2[MAX_TANS_INTFC];	// Trimble Tans I & II  class
static GpsTans3 *tans3[MAX_TANS_INTFC];	// Trimble Tans III  class
static Cmigits3 *cmigits3[MAX_CMIG_INTFC];	// Cmigits3 class
static Garmin *garmin[MAX_TANS_INTFC];	// Garmin  class
static PpsGps *pps[MAX_PPS_INTFC];	// Collins PPS GPS class
static SerialTod *serialtod[MAX_SERIALTOD_INTFC]; // Serial TOD output class
static GreyVme *greyv[MAX_GREYVME_INTFC]; // Grey scale interface class
static GreyVmeHouse *greyh[MAX_GREYVME_INTFC]; // VME Grey house interface class
static HwIrs *hw_irs[MAX_HWIRS_INTFC];	// Honeywell irs class
static HwGps *hw_gps[MAX_HWGPS_INTFC];	// Honeywell irs class
static IP429 *ip429;			// IP429-1 arinc interface class
static IPcounter *IPcnt;		// IP429-1 arinc interface class
static Mcr *mcr;			// mcr control class
static DigOut *digOut;			// Button control class
static MsgQueue *comm_syncQ;           	// comm sync data intertask queue
static MsgQueue *comm_mcrQ;           	// comm Mcr data intertask queue
static MsgQueue *comm_pms2Q;           	// comm pms 2d data intertask queue
static MsgQueue *comm_greyQ;           	// comm grey scale data intertask queue
static MsgQueue *comm_txQ;		// comm send message data intertask q
static MsgQueue *comm_rxQ;		// comm recv message data intertask q
static MsgQueue *disp_syncQ; 		// display intertask data queue 
static MsgQueue *disp_txQ;		// display intertask send message queue
static MsgQueue *disp_rxQ;		// display intertask recv message queue
static MsgQueue *tape_syncQ;           	// comm sync data intertask queue
static MsgQueue *tape_mcrQ;           	// comm Mcr data intertask queue
static MsgQueue *tape_pms2Q;           	// comm pms 2d data intertask queue
static MsgQueue *tape_greyQ;           	// comm grey scale data intertask queue
static MsgQueue *tape_txQ;		// tape intertask send message queue 
static MsgQueue *tape_rxQ;		// tape intertask recv message queue 
static Ophir3 *ophir3[MAX_OPHIR3_INTFC];  // ophir3 interface classes
static Pms1Vme *pms1v[MAX_PMS1VME_INTFC]; // pms 1d interface classes 
static PmsVme2d *pms2d[MAX_PMSVME2D_INTFC]; // VME 2D interface class
static Pms2dHouse *pms2dh[MAX_PMSVME2D_INTFC]; // VME 2D house interface class
static SampleTable *sample_table;	// sample table class
static Spp *spp[MAX_SPP_INTFC];         // Spp interface classes
static SyncRecord *sync_rec;		// synchronous 1 sec record class
static Synchro *synchro;                // synchro-to-digital class
static TapeHeader *dsm_hdr; 		// sram tapeheader
static TodClock *todClock;		// time of day clock
static UvHyg *uvh[MAX_UVHYG_INTFC];  	// UV hygrometer interface classes
static VarConfig *var_config;    	// derived variable config
static Vmio12 *vmio12[MAX_VMIO12_INTFC]; // vmio12 IO board classes
static Vm3118 *vm3118;			// analog auxiliary inputs class
static JplTdl *jpltdl[MAX_UVHYG_INTFC];	// Laser Hygrometer class
static LhTdl *lhtdl;
static Climet *climet;			// Climet class
static Mca *mca;			// Mca class
static Neph *neph[MAX_UVHYG_INTFC];       // Neph interface classes
static Rdma *rdma;			// Rdma class

// Derived parameter classes.
static SyncVar *qcx;                            // dynamic pressure
static SyncVar *psx;                            // static pressure
static SyncVar *ttx;                            // total temperature
static Palt *palt;                              // pressure altitude
static Tasx *tasx;                              // true air speed

// Global variables.
static SEM_ID isr_sem;              	// isr communication semaphore
static int syncDataReady = FALSE;	// sync data ready flag
static int tape_syncQ_enabled = TRUE;
static char msg_str[DSM_MSG_MAX_LEN];	// message string
static char buffer[128];

static int hz50_cnt = RATE_50;		// 50 hertz interrupt counter
static int which_spp[3];
 

/* -------------------------------------------------------------------- */
void dsmAsync()
{
  int	j;
  int	new_second;
  int	priority;
  int   rec;

  const char *buf;			// local buffer pointer
  int	len;				// record length
  int   num_msg;

  sysClkRateSet(25);	// 25Hz.
  sysAuxClkRateSet(50);	// 50Hz.
  sysAuxClkConnect((FUNCPTR)hertz50_isr, FALSE);

//{ char *p = 0xfff42040; *p = 0x21; }
//{ unsigned long ul, *p = 0xfff42040; ul = *p & 0x00ffffff; *p = ul | 0x21000000; }

// Initialize the sram network configuration class.
//  dsm_config = (DsmConfig*)SRAM_DSMCONFIG_BASE; 
//  dsm_config->DsmConfig();
  dsm_config = new DsmConfig();
  if (!dsm_config->selectByHost (sysBootParams.targetName)) {
    fprintf (stderr, "dsmAsync exiting.\n");
    exit (ERROR);
  }
  /* Create and initialize the tape header class in SRAM.  If
   * not operating in the standalone mode, attempt to read the header file
   * by running the constructor.  Otherwise just use the existing header
   * in SRAM.
   */
//  dsm_hdr = (TapeHeader*)SRAM_TAPEHEADER_BASE;
  if (!dsm_config->standalone()) {
    dsm_hdr =  new TapeHeader();
    fprintf (stderr, "Header file = %s.\n", dsm_config->dsmHeaderName());
    dsm_hdr->readFile (dsm_config->dsmHeaderName());
  }

/* Create and initialize the derived variable configuration class in SRAM.  If
   not operating in the standalone mode, attempt to read the varconfig file
   by running the constructor.  Otherwise just use the existing configuration
   in SRAM.
*/
  var_config = (VarConfig*)SRAM_VARCONFIG_BASE;
  if (!dsm_config->standalone())
    var_config = new VarConfig (sysBootParams.hostName, dsm_config->location());

// Create the sample table class.
  sample_table = new SampleTable (*dsm_hdr);
  sample_table->buildTable ();


// Create the comm intertask message queues, and message handler.
  comm_syncQ = new MsgQueue (dsm_hdr->lrlen(), MSGQ_MAX_MSG);
  comm_txQ = new MsgQueue (DSM_MSG_MAX_LEN * 2, MSGQ_MAX_MSG);
  comm_rxQ = new MsgQueue (DSM_MSG_MAX_LEN * 2, MSGQ_MAX_MSG);
  comm_msg = new DsmMessage (*comm_txQ, *comm_rxQ);

// Initialize the display task data and message queues and message handlers.
  disp_syncQ = new MsgQueue (dsm_hdr->lrlen(), MSGQ_MAX_MSG);
  disp_txQ = new MsgQueue (DSM_MSG_MAX_LEN * 2, MSGQ_MAX_MSG);
  disp_rxQ = new MsgQueue (DSM_MSG_MAX_LEN * 2, MSGQ_MAX_MSG);
  disp_msg = new DsmMessage (*disp_txQ, *disp_rxQ);

// Create the time of day clock class.
  todClock = new TodClock();
  printf("todClock initialized.\n"); fflush(stdout);

// Initialize the time generator class.
  tfpInit();
  printf("tfp initialized.\n"); fflush(stdout);

// Initialize the analog sampling and calibration classes.
  analogInit();
  printf("analog initialized.\n"); fflush(stdout);

// Initialize the digital I/O classes.
  digitalInit();
  printf("digital initialized.\n"); fflush(stdout);

// Initialize the 1d interface classes.
  pms1VmeInit();
  printf("pms1Vme initialized.\n"); fflush(stdout);

// Initialize the vme 2d interface class.
  pms2dInit();
  printf("pms2d initialized.\n"); fflush(stdout);

// Initialize the vme 2d housekeeping interface class.
  pms2dhInit();
  printf("pms2dh initialized.\n"); fflush(stdout);

// Initialize the grey scale interface classes.
  greyVmeInit();
  printf("greyVme initialized.\n"); fflush(stdout);

// Initialize the vme grey housekeeping interface class.
  greyhInit();
  printf("greyh initialized.\n"); fflush(stdout);
 
// Initialize the Honeywell irs classes.
  arincInit();
  printf("arinc initialized.\n"); fflush(stdout);

// Initialize the serial interfaces classes.
  serialInit ();
  printf("serial initialized.\n"); fflush(stdout);

// Initialize the derived variables classes.
  derivedInit();
  printf("derived initialized.\n"); fflush(stdout);

// Create specialty probe msg Qs.
  if ((int)mcr) 
    comm_mcrQ = new MsgQueue (sizeof(Mcr_rec), MSGQ_MCR_MSG);
  else
    comm_mcrQ = new MsgQueue (4, 1);
  
  if ((int)pms2d[0] || (int)pms2d[1])
    comm_pms2Q = new MsgQueue (MX_PHYS, MSGQ_P2D_MSG);
  else
    comm_pms2Q = new MsgQueue (4, 1);

  if ((int)greyv[0] || (int)greyv[1]) 
    comm_greyQ = new MsgQueue (MX_PHYS, MSGQ_MAX_MSG);
  else
    comm_greyQ = new MsgQueue (4, 1);

// Create the 1 second sync record class.
  sync_rec = new SyncRecord(*dsm_hdr, *dsm_config, *sample_table, *tfp, counter,
                        digital_in, dpres, dsp, hw_irs, hw_gps, mcr,
                        ophir3, pms1v, pms2dh, tans2, tans3, garmin, pps, spp, 
                        uvh, vm3118, synchro, greyh, jpltdl, lhtdl, rdma, neph, 
                        cmigits3,climet, mca);
// Set up the isr communication semaphore.
  if (!(isr_sem = semBCreate(SEM_Q_FIFO, SEM_EMPTY))) {
    fprintf (stderr, "Failure creating isr_sem.\n");
    exit (ERROR);
  }

// Initialize interrupts.
  initInterrupts();
printf("interrupts initialized\n");

  if (!dsm_config->timeMaster())
  {
    tfp->syncUp();
    tfp->select1PPSMode();
  }
  else {
    tfp->masterSyncUp();
  }

// Get the priority of this task.
  taskPriorityGet(taskIdSelf(), &priority);

// Spawn the front panel display task.
  if (taskSpawn("dsmDisplay", priority + 1, 0, 0x4000,
      (FUNCPTR)dsmDisplay, (int)disp_syncQ, (int)disp_txQ, (int)disp_rxQ,
      0, 0, 0, 0, 0, 0, 0) == ERROR) {
    fprintf (stderr, "Failure spawning dsmDisplay.\n");
    exit(ERROR);
  }

// Spawn the comm task.
  if (taskSpawn("dsmComm", priority - 1, 0, 0x4000,
      (FUNCPTR)dsmComm, (int)comm_syncQ, (int)comm_mcrQ,
       (int)comm_pms2Q, (int)comm_greyQ, (int)comm_txQ, (int)comm_rxQ, 
       0, 0, 0, 0) == ERROR){
    fprintf (stderr, "Failure spawning dsmComm.\n");
    exit(ERROR);
  }
//      (FUNCPTR)dsmComm, (int)comm_syncQ,

// If local recording is enabled. Create the tape intertask message queues,
// and message handler.
  if (dsm_config->localRecord()) {
    tape_syncQ = new MsgQueue (MX_PHYS, MSGQ_MAX_MSG);
    tape_mcrQ = new MsgQueue (MX_PHYS, MSGQ_MCR_MSG);
    tape_pms2Q = new MsgQueue (MX_PHYS, MSGQ_P2D_MSG);
    tape_greyQ = new MsgQueue (MX_PHYS, MSGQ_MAX_MSG);
    tape_txQ = new MsgQueue (DSM_MSG_MAX_LEN * 2, MSGQ_MAX_MSG);
    tape_rxQ = new MsgQueue (DSM_MSG_MAX_LEN * 2, MSGQ_MAX_MSG);
    tape_msg = new DsmMessage (*tape_txQ, *tape_rxQ);

// Spawn the tape task. 
    if (taskSpawn("dsmTape", priority - 2, 0, 0x4000,
        (FUNCPTR)dsmTape, (int)tape_syncQ, (int)tape_mcrQ,
//        (FUNCPTR)dsmTape, (int)tape_syncQ,
        (int)tape_pms2Q, (int)tape_greyQ, (int)tape_txQ, (int)tape_rxQ, 
        0, 0, 0, 0) == ERROR){
      fprintf (stderr, "Failure spawning dsmTape.\n");
      exit(ERROR);
    }
  }


  comm_msg->sendLogMsg("dsmAsync: at Main Loop.", dsm_config->hostName());
  printf("dsmAsync: at Main Loop.\n");

  // Main loop.
  while (TRUE) {
    // Check for a semaphore release from the 50 hz isr.
    semTake (isr_sem, WAIT_FOREVER); 

//  Call the 2d data collection routine.
    for (j = 0; (j < MAX_PMSVME2D_INTFC); j++) {
      if ((int)pms2d[j]) {
        pms2d[j]->collect (new_second);
 
//      Check for 2d data available, and send it to the comm task.
        if (pms2d[j]->bufFull()) {
          rec += 1;
/*
fprintf(stderr, "%d:%d:%d\n", ((P2d_rec *)(pms2d[j]->buffer()))->hour,
	((P2d_rec *)(pms2d[j]->buffer()))->minute, ((P2d_rec *)(pms2d[j]->buffer()))->second);
*/
          if (comm_pms2Q->msgSend (pms2d[j]->buffer(), pms2d[j]->length()) ==
                ERROR)
            fprintf(stderr, "Failure sending Pms 2D data to comm_pms2Q.\n");

//        If local recording is enabled, send data to the tape task.
          if (dsm_config->localRecord() && tape_syncQ_enabled) {
            if (tape_pms2Q->msgSend (pms2d[j]->buffer(), pms2d[j]->length()) 
                 == ERROR)
              fprintf (stderr, "Failure sending data to tape_pms2Q.\n");
          }
          pms2d[j]->releaseBuf();
        }
      }
    }

    // Gather data from various hardware cards.
    gatherData();

    // Check if it is time for a new sync record.
    if (syncDataReady) {
      syncDataReady = FALSE;
      // Build the 1 second sync record.
      sync_rec->buildRecord ();

      // Send the sync data record to the comm task.
      buf = sync_rec->buf->getBuf();
      len = sync_rec->buf->getLen();
 
      if (((Hdr_blk *)buf)->hour < 24)
        if (comm_syncQ->msgSend (buf, len) == ERROR) {
          comm_msg->sendLogMsg("Failure sending data to syncQ.",
					dsm_config->hostName());
          fprintf(stderr, "Failure sending Sync data to comm_syncQ.\n");
          }

      computeDerived(buf);

      if ((int)mcr) {
        for (j=0; j < NUM_HIGH_CHAN; j++) {
          Mcr_rec *m = (Mcr_rec *)dsp->buf5000(j);

//  printf("%s %2d:%2d:%2d\n", m->id, m->hour, m->minute, m->second); 

          if (comm_mcrQ->msgSend (dsp->buf5000(j), sizeof(Mcr_rec)) ==
             ERROR)
            fprintf (stderr, "Failure sending Mcr data to comm_mcrQ.\n");
 
// If local recording is enabled, send data to the tape task.
          if (dsm_config->localRecord() && tape_syncQ_enabled) {
            if (tape_mcrQ->msgSend (dsp->buf5000(j), sizeof(Mcr_rec)) ==
                ERROR)
              fprintf (stderr, "Failure sending data to tape_mcrQ.\n");
          }
 
        }
      }

//      num_msg = comm_mcrQ->msgQNum();
//      printf("No. messages in Mcr queue = %d\n", num_msg);
      if ((num_msg = comm_mcrQ->msgQNum()) > 7)
        printf("No. messages in Mcr queue = %d\n", num_msg);


// If local recording is enabled, send data to the tape task.
      if (dsm_config->localRecord() && tape_syncQ_enabled) {
        if (tape_syncQ->msgSend (buf, len) == ERROR)
          fprintf (stderr, "Failure sending data to tape_syncQ.\n");
      }

// Send the sync data record to the display task.
      if (disp_syncQ->msgSend (buf, len) == ERROR)
        ;
//      fprintf (stderr, "Failure sending data to disp_syncQ.\n");

// Pass the status word to the mcr class, and toggle the heartbeat bit.
      if ((int)mcr) {
        mcr->setStatus((int)*(short*)(buf +
                       sample_table->mcr_table.startMcrStat()));
        mcr->toggleHeartBeat();
      }

// Check the analog calibration status.
      if ((int)analogCal)
        analogCal->checkCal (buf);
 
// Uncomment these lines to look at particular data locations.
/**
      {
      Hdr_blk *hdr_blk = (Hdr_blk*)buf;

//      printf ("%f %f\n", *(float*)(buf+34), *(float*)(buf + 38));
      printf ("dsmAsync: %02d/%02d/%02d, %02d:%02d:%02d\n", 
          hdr_blk->month, hdr_blk->day, hdr_blk->year, hdr_blk->hour, 
          hdr_blk->minute, hdr_blk->second);
      }
**/
// Release the data buffer.
      sync_rec->buf->releaseBuf();
    }

// Check for the start of a new second.
    if (new_second = tfp->newSecond()) {
//      if (rec > 0) printf("rec = %d\n",rec);
      rec = 0;
      tfp->clearNewSecond();
      if (dsm_config->dsmSerialTod()) {
        for (j = 0; j< MAX_SERIALTOD_INTFC; j++)
        serialtod[j]->sendtod();
      }

      printf ("%02d/%02d/%02d %02d:%02d:%02d\n",
		tfp->month(), tfp->day(), tfp->year(),
		tfp->hour(), tfp->minute(), tfp->second());


// Check the status of the various interfaces once per second.
      for (j = 0; (j < MAX_TANS_INTFC) && (int)tans2[j]; j++)
        tans2[j]->checkStatus();
      for (j = 0; (j < MAX_TANS_INTFC) && (int)tans3[j]; j++)
        tans3[j]->checkStatus();
      for (j = 0; (j < MAX_TANS_INTFC) && (int)garmin[j]; j++)
        garmin[j]->checkStatus();
      for (j = 0; (j < MAX_CMIG_INTFC) && (int)cmigits3[j]; j++)
        cmigits3[j]->checkStatus();
#ifdef NOAA_GIV
      for (j = 0; (j < MAX_PPS_INTFC) && (int)pps[j]; j++)
        pps[j]->checkStatus();
#endif
      for (j = 0; (j < MAX_OPHIR3_INTFC) && (int)ophir3[j]; j++)
        ophir3[j]->checkStatus();
      for (j = 0; (j < MAX_UVHYG_INTFC) && (int)uvh[j]; j++)
        uvh[j]->checkStatus();

      for (j = 0; (j < MAX_SPP_INTFC) && (int)spp[j]; j++){
        spp[j]->checkStatus();
      }

     for (j = 0; (j < MAX_UVHYG_INTFC) && (int)dpres[j]; j++)
       dpres[j]->checkStatus();

      for (j = 0; (j < MAX_UVHYG_INTFC) && (int)jpltdl[j]; j++)
        jpltdl[j]->checkStatus();

      for (j = 0; (j < MAX_UVHYG_INTFC) && (int)neph[j]; j++){
        neph[j]->checkStatus();
      }
      if ((int)lhtdl)
        lhtdl->checkStatus();

      if ((int)climet)
        climet->checkStatus();

      if ((int)mca)
        mca->checkStatus();

      if ((int)rdma)
        rdma->checkStatus();
    }

// Call the Grey scale data collection routine.
    for (j = 0; j < MAX_GREYVME_INTFC; j++) {
      if ((int)greyv[j]) {
        greyv[j]->collect();

// Check for 2d data available, and send it to the comm task.
        if (greyv[j]->bufFull()) {
          if (comm_greyQ->msgSend (greyv[j]->buffer(), greyv[j]->length()) == 
              ERROR)
            fprintf (stderr, 
                     "Failure sending Grey Scale data to comm_greyQ.\n");

// If local recording is enabled, send data to the tape task.
          if (dsm_config->localRecord() && tape_syncQ_enabled) {
            if (tape_greyQ->msgSend (greyv[j]->buffer(), greyv[j]->length()) == 
                ERROR)
              fprintf (stderr, "Failure sending data to tape_greyQ.\n");
          }
          greyv[j]->releaseBuf();
        }
      }
    }

// Check for received messages.
    checkMessage ();
  }

}

/*****************************************************************************/
void computeDerived (const char *buf)

// Computes and used derived variables from the passed in logical record.
{
  static float tas = 0.0;		// current values
  static float alt = 0.0;

  if ((int)psx)				// static pressure
    psx->computeFromLR (buf);

  if ((int)qcx)				// dynamic pressure
    qcx->computeFromLR (buf);

  if ((int)ttx)				// total temperature
    ttx->computeFromLR (buf);

  if ((int)tasx) {			// true air speed
    tasx->compute();
    tas = tasx->value();
  }

  if ((int)palt) {			// pressure altitude
    palt->compute();
    alt = palt->value();
  }

//if ((int)tasx)
//  printf ("psx = %f, qcx = %f, ttx = %f, tas = %f, alt = %f\n",
//          psx->value(), qcx->value(), ttx->value(), tas, alt);

// If operating in standalone mode, send the tas and alt to the various
// interfaces that need them.
  if (dsm_config->standalone())
    setTasAlt (tas, alt);
}

/*****************************************************************************/
static void analogInit ()

// Initialize the analog sampling and calibration classes.
{
  dsp = (Dsp56002*)0;
  vm3118 = (Vm3118*)0;
  synchro = (Synchro*)0;
  int i; 
// Create the Dsp and analog calibration classes if standard analog channels
// are to be sampled.
  if (sample_table->ana_table.firstChannel()) {
    dsp = new Dsp56002 ((char*)(A24D16_BASE + M56002_1_BASE), 
                   sample_table->ana_table);
    if (dsp == NULL)
    {
       perror("Creating Dsp56002:");
       exit(ERROR);
    }
    analogCal = new AnalogCal (*dsm_hdr, dsp, *dsm_config, *comm_msg);
    if (analogCal == NULL)
    {
       perror("Creating AnalogCal:");
       exit(ERROR);
    }
  }

#ifdef SAIL
// Create the analog auxiliary class if auxiliary analog channels
// are to be sampled.
  if (sample_table->ana_aux_table.firstChannel()) {
    vm3118 = new Vm3118 ((char*)(A24D16_BASE+VM3118_BASE), 
                    sample_table->ana_aux_table);
    if (vm3118 == NULL)
    {
       perror("Creating Vm3118:");
       exit(ERROR);
    }
  }
#endif

// Create the synchro-to-digital class.
  if (sample_table->synchro_table.firstChannel()) {
    synchro = new Synchro((char*)(IP_b_IO_BASE),sample_table->synchro_table);
  }

// sample_table->ana_table.displayTable();
}

/*****************************************************************************/
static void digitalInit ()	// Initialize the digital I/O interface classes.
{
  int j;
  int stat;

// Null class pointers.
  counter = (Counter*)0;
  digital_in = (DigitalIn*)0;
  IPcnt = (IPcounter *)0;

  for (j = 0; j < MAX_VMIO12_INTFC; j++)
    vmio12[j] = (Vmio12*)0;

// Create the Vmio12 classes needed for digital inputs.
  for (stat = sample_table->dig_in_table.firstEntry(); stat;
       stat = sample_table->dig_in_table.nextEntry()) {

    // Get the board number
    if ((j = sample_table->dig_in_table.boardNumber()) > MAX_VMIO12_INTFC) {
      fprintf (stderr, 
        "digitalInit: Illegal digital I/O board number requested, %d.\n", j);
      exit (ERROR);
    }

    // If the class has not yet been created, then create it.
    if (!(int)vmio12[j]) {
      if (!j)
        vmio12[j] = new Vmio12 ((char*)(A24D16_BASE + VMIO12_BASE_0));
      else
        vmio12[j] = new Vmio12 ((char*)(A24D16_BASE + VMIO12_BASE_1));
    }
  }


// Create the Vmio12 or IPcounter classes needed for counters.
/*
  if (sample_table->counter_table.firstEntry()) {
    IPcnt->IPcounter((char *)IP_c_IO_BASE);
    IPcnt->readProm((char *)IP_c_ID_BASE);
  }
*/

  for (stat = sample_table->counter_table.firstEntry(); stat;
       stat = sample_table->counter_table.nextEntry()) {
 
    // Get the board number
    if ((j = sample_table->counter_table.boardNumber()) > MAX_VMIO12_INTFC) {
      fprintf (stderr,
        "digitalInit: Illegal digital I/O board number requested, %d.\n", j);
      exit (ERROR);
    }
 
    // If the class has not yet been created, then create it.
    if (!(int)vmio12[j]) {
      if (!j)
        vmio12[j] = new Vmio12 ((char*)(A24D16_BASE + VMIO12_BASE_0));
      else
        vmio12[j] = new Vmio12 ((char*)(A24D16_BASE + VMIO12_BASE_1));
    }
  }

// Create the digital input sampling class.
  if (sample_table->dig_in_table.firstEntry()) {
    digital_in = new DigitalIn(sample_table->dig_in_table, vmio12, *tfp);
  }

// Create the counter sampling class.
  if (sample_table->counter_table.firstEntry()) {
//    counter->Counter (sample_table->counter_table, IPcnt);
    counter = new Counter (sample_table->counter_table, vmio12);
  }

// Create the mcr control class.
  if (sample_table->mcr_table.startMcrOut()) {
    mcr = new Mcr (sample_table->mcr_table, vmio12, *tfp, dsp, *comm_msg, 
              sample_table->ana_table);
  }

// Check for DigOuts file, create vmio12 if necessary.
  digOut = new DigOut(sysBootParams.hostName, dsm_config->location(),
          vmio12, *comm_msg);
}

/*****************************************************************************/
static void arincInit ()
 
// Initialize the Honeywell Irs and Aimr interface classes.
{
  int stat;
  int idx;                             // class index number
 
// Null the class pointers.
  for (idx = 0; idx < MAX_HWIRS_INTFC; idx++)
    hw_irs[idx] = (HwIrs*)0;
  for (idx = 0; idx < MAX_HWGPS_INTFC; idx++)
    hw_gps[idx] = (HwGps*)0;

  ip429 = (IP429*)0;

// If irs class is to be created, then create an arinc interface class.
  if (sample_table->hwirs_table.firstEntry() || 
      sample_table->hwgps_table.firstEntry()) {

    ip429 = new IP429 ((char*)IP_a_IO_BASE, IP429_VCT);
    if (ip429 == NULL)
    {
       perror("Creating IP429:");
       exit(ERROR);
    }
  }

// Create the needed HwIrs classes.
  for (stat = sample_table->hwirs_table.firstEntry(); stat;
       stat = sample_table->hwirs_table.nextEntry()) {

// Get the class index.
    idx = sample_table->hwirs_table.index();      
 
    hw_irs[idx] = new HwIrs (ip429, *tfp, sample_table->hwirs_table.rxChan(), 
                        sample_table->hwirs_table.txChan());
    if (hw_irs[idx] == NULL)
    {
       perror("Creating HwIrs:");
       exit(ERROR);
    }
  }

// Create the needed HwGps classes.
#ifdef NOAA_GIV
  for (stat = sample_table->hwgps_table.firstEntry(); stat;
       stat = sample_table->hwgps_table.nextEntry()) {
 
    idx = sample_table->hwgps_table.index();
 
    hw_gps[idx] = new HwGps (ip429, sample_table->hwgps_table.rxChan());
    if (hw_gps[idx] == NULL)
    {
       perror("Creating HwGps:");
       exit(ERROR);
    }
  }
#endif
}

/*****************************************************************************/
static void pms1VmeInit ()

// Initialize the vme pms 1d interfaces.
{
  int stat;
  int intf;				// interface number
  char *base_adr[MAX_PMS1VME_INTFC] = {(char*)(A24D16_BASE + VME1D_BASE_0),
                                       (char*)(A24D16_BASE + VME1D_BASE_1)};

// Null the class pointers.
  for (intf = 0; intf < MAX_PMS1VME_INTFC; intf++)
    pms1v[intf] = (Pms1Vme*)0;

// Initialize the channels from the sample table.
  for (stat = sample_table->pms1vme_table.firstChan(); stat; 
       stat = sample_table->pms1vme_table.nextChan()) {
    intf = sample_table->pms1vme_table.interface();	// get interface number

// If the class for this interface hasn't been created, create it.
    if (!(int)pms1v[intf]) {
      pms1v[intf] = new Pms1Vme (base_adr[intf]);
      if (pms1v[intf] == NULL) {
          perror ("Creating Pms1Vme:");
          exit (ERROR);
      }
    }

    pms1v[intf]->initChan (sample_table->pms1vme_table.channel(),
                           sample_table->pms1vme_table.rate(),
                           sample_table->pms1vme_table.bins(),
                           sample_table->pms1vme_table.range(),
                           sample_table->pms1vme_table.psLength());
  }
}

/*****************************************************************************/
static void pms2dInit ()
 
// Initialize the vme pms 2d interfaces.
{
  int intf, stat;
  unsigned char *base_adr[MAX_PMSVME2D_INTFC] =
			{(unsigned char *)(A24D16_BASE + PMSVME2D_BASE_0),
			 (unsigned char *)(A24D16_BASE + PMSVME2D_BASE_1)};

// Null the class pointers.
  for (intf = 0; intf < MAX_PMSVME2D_INTFC; intf++)
    pms2d[intf] = (PmsVme2d*)0;

// Initialize the channels from the sample table.
  for (stat = sample_table->pms2d_table.firstEntry(); stat;
       stat = sample_table->pms2d_table.nextEntry())
    {
    intf = sample_table->pms2d_table.interface();     // get interface number
 
// If the class for this interface hasn't been created, create it.
    if (!(int)pms2d[intf])
      {
      pms2d[intf] = new PmsVme2d (base_adr[intf], statusMsg);

      if (pms2d[intf] == NULL) {
        perror ("Creating PmsVme2d:");
        exit (ERROR);
        }
      }

    printf("chan = %d name = %s type = %d res = %d\n",
                            sample_table->pms2d_table.channel(),
                            sample_table->pms2d_table.name(),
                            sample_table->pms2d_table.type(),
                            sample_table->pms2d_table.resolution());

    pms2d[intf]->initProbe (sample_table->pms2d_table.channel(),
                            sample_table->pms2d_table.name(),
                            sample_table->pms2d_table.type(),
                            sample_table->pms2d_table.resolution());
    }


// Start data collection
  for (intf = 0; intf < MAX_PMSVME2D_INTFC; intf++) {
    if ((int)pms2d[intf]) {
      pms2d[intf]->setTime (tfp->hour(), tfp->minute(), tfp->second());
      pms2d[intf]->startSampling();
    }
  }

}

/*****************************************************************************/
static void pms2dhInit ()
 
// Initialize the  pms 2d housekeeping interfaces.
{
  int intf, stat;
  char *base_adr[MAX_PMSVME2D_INTFC] =
                        {(char *)(A24D16_BASE + PMSVME2D_BASE_0),
                         (char *)(A24D16_BASE + PMSVME2D_BASE_1)};

// Null the class pointers.
  for (intf = 0; intf < MAX_PMSVME2D_INTFC; intf++)
    pms2dh[intf] = (Pms2dHouse*)0;

// Initialize the channels from the sample table.
  for (stat = sample_table->pms2dh_table.firstChannel(); stat;
       stat = sample_table->pms2dh_table.nextChannel()) {
    intf = sample_table->pms2dh_table.interface();     // get interface number

// If the class for this interface hasn't been created, create it.
    if (!(int)pms2dh[intf]) {
      pms2dh[intf] = new Pms2dHouse (base_adr[intf]);
      if (pms2dh[intf] == NULL) {
        perror ("Creating Pms2dHouse:");
        exit (ERROR);
      }
    }
  }
}

/*****************************************************************************/
static void greyVmeInit ()
 
// Initialize the grey scale interfaces.
{
  int stat;
  int intf;
  unsigned char *base_adr[MAX_GREYVME_INTFC] =
			{(unsigned char *)(A24D16_BASE + GREYVME_BASE_0),
			 (unsigned char *)(A24D16_BASE + GREYVME_BASE_1)};

// Null the class pointers.
  for (intf = 0; intf < MAX_GREYVME_INTFC; intf++)
    greyv[intf] = (GreyVme*)0;

// Initialize the channels from the sample table.
  for (stat = sample_table->greyvme_table.firstEntry(); stat; 
       stat = sample_table->greyvme_table.nextEntry()) {

    intf = sample_table->greyvme_table.interface();	// get interface number

// If the class for this interface hasn't been created, create it.
    if (!(int)greyv[intf]) {
      greyv[intf] = new GreyVme (base_adr[intf], statusMsg);
      if (greyv[intf] == NULL) {
        perror ("Creating GreyVme:");
        exit (ERROR);
      }
    }
    greyv[intf]->initProbe (sample_table->greyvme_table.channel(),
                            sample_table->greyvme_table.name(),
                            sample_table->greyvme_table.type(),
                            sample_table->greyvme_table.resolution(),
                            sample_table->greyvme_table.chargeSpacing(),
                            sample_table->greyvme_table.chargeLocation());
  }

// Start data collection
  for (intf = 0; intf < MAX_GREYVME_INTFC; intf++) {
    if ((int)greyv[intf]) {
      greyv[intf]->startSampling();		
      greyv[intf]->setTime (tfp->hour(), tfp->minute(), tfp->second());
    }
  }
}
/*****************************************************************************/
static void greyhInit ()
 
// Initialize the  pms grey housekeeping interfaces.
{
  int intf, stat;
  char *base_adr[MAX_GREYVME_INTFC] =
                        {(char *)(A24D16_BASE + GREYVME_BASE_0),
                         (char *)(A24D16_BASE + GREYVME_BASE_1)};
 
 
// Null the class pointers.
  for (intf = 0; intf < MAX_GREYVME_INTFC; intf++)
    greyh[intf] = (GreyVmeHouse*)0;
 
// Initialize the channels from the sample table.
  for (stat = sample_table->greyh_table.firstChannel(); stat;
       stat = sample_table->greyh_table.nextChannel()) {
    intf = sample_table->greyh_table.interface();     // get interface number
 
// If the class for this interface hasn't been created, create it.
    if (!(int)greyh[intf]) {
      greyh[intf] = new GreyVmeHouse (base_adr[intf]);
      if (greyh[intf] == NULL) {
        perror ("Creating GreyVmeHouse:");
        exit (ERROR);
      }
    }
  }
}

/*****************************************************************************/
static void serialInit ()
 
// Initialize the serial interfaces.
{
  int stat;
  int idx;
 
// Null the class pointers.
  for (idx = 0; idx < MAX_TANS_INTFC; idx++) {
    tans2[idx]	= (GpsTans2*)0;
    tans3[idx]	= (GpsTans3*)0;
    garmin[idx]	= (Garmin*)0;
  }
  for (idx = 0; idx < MAX_PPS_INTFC; idx++)
    pps[idx] = (PpsGps*)0;
  for (idx = 0; idx < MAX_OPHIR3_INTFC; idx++) 
    ophir3[idx] = (Ophir3*)0;
  for (idx = 0; idx < MAX_UVHYG_INTFC; idx++) 
    uvh[idx] = (UvHyg*)0;
  for (idx = 0; idx < MAX_SERIALTOD_INTFC; idx++)
    serialtod[idx] = (SerialTod*)0;
  for (idx = 0; idx < MAX_UVHYG_INTFC; idx++)
    dpres[idx] = (Dpres*)0;
  for (idx = 0; idx < MAX_UVHYG_INTFC; idx++)
    jpltdl[idx] = (JplTdl*)0;
  for (idx = 0; idx < MAX_UVHYG_INTFC; idx++)
    neph[idx] = (Neph*)0;
  for (idx = 0; idx < MAX_CMIG_INTFC; idx++)
    cmigits3[idx] = (Cmigits3*)0;
  lhtdl = (LhTdl*)0;
  climet = (Climet*)0;
  mca = (Mca*)0;
  rdma = (Rdma*)0;

// Create the Trimble Tans 2 class objects.
  for (stat = sample_table->tans2_table.firstEntry(), idx = 0;
       stat; stat = sample_table->tans2_table.nextEntry(), idx++) {

    tans2[idx] = new GpsTans2 ((const char*)(A24D16_BASE + ISIO1_BASE), 
                          sample_table->tans2_table.port(), statusMsg, setTime);
    if (tans2[idx] == NULL) {
      perror ("Creating GpsTans2:");
      exit (ERROR);
    }
  }

// Create the Trimble Tans 3 class objects.
  for (stat = sample_table->tans3_table.firstEntry(), idx = 0;
       stat; stat = sample_table->tans3_table.nextEntry(), idx++) {
 
    tans3[idx] = new GpsTans3 ((char*)(A24D16_BASE + ISIO1_BASE),
                          sample_table->tans3_table.port(), statusMsg, setTime);
    if (tans3[idx] == NULL) {
      perror ("Creating GpsTans3:");
      exit (ERROR);
    }
  }
// Create the Garmin class objects.
  for (stat = sample_table->garmin_table.firstEntry(), idx = 0;
       stat; stat = sample_table->garmin_table.nextEntry(), idx++) {
    garmin[idx] = new Garmin ((char*)(A24D16_BASE + ISIO1_BASE),
                      sample_table->garmin_table.port(), statusMsg,setTime);
    if (garmin[idx] == NULL) {
      perror ("Creating Garmin:");
      exit (ERROR);
    }
  }
// Create the Cmigits3 class objects.
  for (stat = sample_table->cmigits3_table.firstEntry(), idx = 0;
       stat; stat = sample_table->cmigits3_table.nextEntry(), idx++) {
    cmigits3[idx] = new Cmigits3((char*)(A24D16_BASE + ISIO1_BASE),
                      sample_table->cmigits3_table.port(), statusMsg);
    if (cmigits3[idx] == NULL) {
      perror ("Creating Cmigits3:");
      exit (ERROR);
    }
  }

// Create the Collins PPS GPS class objects.
#ifdef NOAA_GIV
  for (stat = sample_table->pps_table.firstEntry(), idx = 0;
       stat; stat = sample_table->pps_table.nextEntry(), idx++) {
 
    pps[idx] = new PpsGps ((char*)(A24D16_BASE + ISIO1_BASE),
                          sample_table->pps_table.port(), statusMsg, setTime);
    if (pps[idx] == NULL) {
      perror ("Creating PpsGps:");
      exit (ERROR);
    }
  }
#endif
// Create the Ophir3 class objects.
  for (stat = sample_table->ophir3_table.firstEntry(), idx = 0;
       stat; stat = sample_table->ophir3_table.nextEntry(), idx++) {

    ophir3[idx] = new Ophir3 ((char*)(A24D16_BASE + ISIO1_BASE),
                          sample_table->ophir3_table.port(), statusMsg);
    if (ophir3[idx] == NULL) {
      perror ("Creating Ophir3:");
      exit (ERROR);
    }
  }

// Create the SPP probe class objects.
  dsm_hdr->firstDesc(PMS1V3_STR);
  for (stat = sample_table->spp_table.firstEntry(), idx = 0;
       stat; stat = sample_table->spp_table.nextEntry(), idx++) {
    if(!strcmp (sample_table->spp_table.real_name(), SPP100_STR)) {
      which_spp[idx] = 0;
    } 
    if(!strcmp (sample_table->spp_table.real_name(), SPP200_STR)) {
      which_spp[idx] = 1;
    } 
    if(!strcmp (sample_table->spp_table.real_name(), SPP300_STR)) {
      which_spp[idx] = 2;
    } 

    spp[idx] = new Spp((char*)(A24D16_BASE + ISIO1_BASE), *dsm_hdr, (int)which_spp[idx], statusMsg);

    if (spp[idx] == NULL) {
      perror ("Creating SPP:");
      exit (ERROR);
    }

    dsm_hdr->nextDesc(PMS1V3_STR);
  }

// Create the UV hygrometer class objects.
  for (stat = sample_table->uvh_table.firstEntry(), idx = 0;
       stat; stat = sample_table->uvh_table.nextEntry(), idx++) {
 
    uvh[idx] = new UvHyg ((char*)(A24D16_BASE + ISIO1_BASE),
                          sample_table->uvh_table.port(), statusMsg);
    if (uvh[idx] == NULL) {
      perror ("Creating UvHyg:");
      exit (ERROR);
    }
  }

// Create the digital pressure class object.

  for (stat = sample_table->dpres_table.firstEntry(), idx = 0;
       stat; stat = sample_table->dpres_table.nextEntry(), idx++) {
    dpres[idx] = new Dpres ((char*)(A24D16_BASE + ISIO1_BASE), DPRES_PORT_1+idx,
                       statusMsg);
    if (dpres[idx] == NULL) {
      perror ("Creating Dpres:");
      exit (ERROR);
    }
  }

// Create the JplTdl class object.

  for (stat = sample_table->jpltdl_table.firstEntry(), idx = 0;
       stat; stat = sample_table->jpltdl_table.nextEntry(), idx++) {
    jpltdl[idx] = new JplTdl ((char*)(A24D16_BASE + ISIO1_BASE),
		JPLTDL_PORT_1+idx, statusMsg);
    if (jpltdl[idx] == NULL) {
      perror ("Creating JplTdl:");
      exit (ERROR);
    }
  }

// Create the LhTdl class object.

  if (sample_table->lhtdl_table.firstEntry()) {
    lhtdl = new LhTdl ((char*)(A24D16_BASE + ISIO1_BASE), LHTDL_PORT_1,
                  statusMsg);
    
    if (lhtdl == NULL) {
      perror ("Creating LhTdl:");
      exit (ERROR);
    }
  }

// Create the Climet class object.

  if (sample_table->climet_table.firstEntry()) {
    climet = new Climet ((char*)(A24D16_BASE + ISIO1_BASE), CLIMET_PORT_1,
                  statusMsg);

    if (climet == NULL) {
      perror ("Creating Climet:");
      exit (ERROR);
    }
  }

// Create the Mca class object.

  if (sample_table->mca_table.firstEntry()) {
    mca = new Mca ((char*)(A24D16_BASE + ISIO1_BASE), MCA_PORT_1,
                  statusMsg);

    if (mca == NULL) {
      perror ("Creating Mca:");
      exit (ERROR);
    }
  }

// Create the Neph class object.

  for (stat = sample_table->neph_table.firstEntry(), idx = 0;
       stat; stat = sample_table->neph_table.nextEntry(), idx++) {
    neph[idx] = new Neph ((char*)(A24D16_BASE + ISIO1_BASE),
                NEPH_PORT_1+idx, statusMsg);
    if (neph[idx] == NULL) {
      perror ("Creating Neph:");
      exit (ERROR);
    }
  }

// Create the Rdma class object.

  if (sample_table->rdma_table.firstEntry()) {
    rdma = new Rdma ((char*)(A24D16_BASE + ISIO1_BASE), RDMA_PORT_1,
                  statusMsg);
    
    if (rdma == NULL) {
      perror ("Creating Rdma:");
      exit (ERROR);
    }
  }

// Create the Serial TOD out class object.
 
  if (dsm_config->dsmSerialTod()) {
    for (idx = 0; idx < MAX_SERIALTOD_INTFC; idx++) {
      serialtod[idx] = new SerialTod ((char*)(A24D16_BASE + ISIO1_BASE),
                               SERTOD_PORT + idx, statusMsg, *tfp);
      if (serialtod[idx] == NULL) {
        perror ("Creating SerialTod:");
        exit (ERROR);
      }
    }
  }
}

/*****************************************************************************/
static void checkMessage ()

// Checks for and processes received messages.
{
  int	stat;

  if (comm_msg->readMsg()) {

    switch (comm_msg->type()) {

      case ANALOG_MSG:
        if ((int)analogCal)
          analogCal->control(comm_msg->action(), comm_msg->address(), 
                        comm_msg->volt(),comm_msg->gain(), comm_msg->offset());
        break;

      case MCR_MSG:
// printf("MCR message received.\n");
        if ((int)mcr)
          mcr->control (comm_msg->action(), comm_msg->value());
        break;
 
      case DIGOUT_MSG:
        if ((int)digOut) {
          digOut->control(comm_msg->action(), comm_msg->connector(),
			comm_msg->channel());
	}
        break;
 
      case PMS1_MSG:             	// Pms 1d message
// printf("PMS1 range change\n");
        setPms1Range (comm_msg->name(), comm_msg->action());
        break;

      case PMS2_MSG:             	// Pms 2d message
// printf("PMS2 change\n");
        switch (comm_msg->action()) {

          case PMS2_TAS_SELECT:
            for (stat = sample_table->pms2d_table.firstEntry(); stat;
                 stat = sample_table->pms2d_table.nextEntry())
              if ((int)pms2d[sample_table->pms2d_table.interface()])
                pms2d[sample_table->pms2d_table.interface()]->
					setTasMode(comm_msg->value());

            for (stat = sample_table->greyvme_table.firstEntry(); stat;
                 stat = sample_table->greyvme_table.nextEntry())
              if ((int)greyv[sample_table->greyvme_table.interface()])
                greyv[sample_table->greyvme_table.interface()]->
					setTasMode(comm_msg->value());
            break;

          case PMS2_RATE_SELECT:
            for (stat = sample_table->pms2d_table.firstEntry(); stat;
                 stat = sample_table->pms2d_table.nextEntry())
              if ((int)pms2d[sample_table->pms2d_table.interface()])
                pms2d[sample_table->pms2d_table.interface()]->
					setRate(comm_msg->value());

/*  Uncomment as soon as Mike ads setRate() for Grey. 6/98
            for (stat = sample_table->greyvme_table.firstEntry(); stat;
                 stat = sample_table->greyvme_table.nextEntry())
              if ((int)greyv[sample_table->greyvme_table.interface()])
                greyv[sample_table->greyvme_table.interface()]->
					setRate(comm_msg->value());
*/
            break;

          case PMS2_PROBE_SELECT:
            if (sample_table->greyvme_table.selectByName(comm_msg->name())) {
              if (comm_msg->value())
                greyv[sample_table->greyvme_table.interface()]->
					enableProbe (comm_msg->name());
              else
                greyv[sample_table->greyvme_table.interface()]->
					disableProbe(comm_msg->name());
            }
            else {
              if (comm_msg->value())
                {
                if ((int)pms2d[sample_table->pms2d_table.interface()])
                  pms2d[sample_table->pms2d_table.interface()]->
					enableProbe(comm_msg->name());
                }
              else
                pms2d[sample_table->pms2d_table.interface()]->
					disableProbe(comm_msg->name());
            }
            break;

          default:
            fprintf(stderr, "Unknown PMS2_MSG action requested, %d.\n",
                     comm_msg->action());
          break;
        }
        break;
 
      case TASALT_MSG:             	// tas-altitude message
        setTasAlt(comm_msg->tas(), comm_msg->altitude());
        break;

      case TIME_MSG:
      case DT_MSG:
        printf("Garmin date = %d/&d/%d\n",garmin[0]->month(),garmin[0]->day(),
                garmin[0]->year()); 
        setTime(comm_msg->year(), comm_msg->month(), comm_msg->day(),
		comm_msg->hour(), comm_msg->minute(), comm_msg->second());
        break;

      case DATE_MSG:
        if (!dsm_config->timeMaster())
          setDate(comm_msg->year(), comm_msg->month(), comm_msg->day());
        break;

      case FLIGHT_MSG:
// printf("Flight number received = [%s]\n", comm_msg->flight());
        break;

      default:
        fprintf(stderr, "Unknown message type received, %d.\n",
                 comm_msg->type());
    }
  }

// Check for messages from the display task.
  if (disp_msg->readMsg()) {
    switch (disp_msg->type()) {

      case FLIGHT_MSG:
      case TAPE_MSG:
        if ((int)tape_msg)
          tape_msg->relayMessage (disp_msg->rxMessage());  // relay to display
        break;

      default:;
    }
  }


// Check for messages from the tape task.
  if ((int)tape_msg && tape_msg->readMsg()) {
    switch (tape_msg->type()) {

      case NET_MSG:
        if (tape_msg->action() == NET_ENABLE)
          tape_syncQ_enabled = TRUE;
        else
          tape_syncQ_enabled = FALSE;
        break;

      case TAPE_MSG:
        disp_msg->relayMessage (tape_msg->rxMessage());	// relay to display
        break;
      default:;
    }
  }
}

/*****************************************************************************/
static void setPms1Range (char *name, int range)

// Set the range on the specified probe.
{
  int pmsstat,sppstat;

// Select the probe.
  for (pmsstat = sample_table->pms1vme_table.firstChan(); 
       pmsstat && strcmp (sample_table->pms1vme_table.name(), name);
       pmsstat = sample_table->pms1vme_table.nextChan());

  if (!pmsstat)
    for (sppstat = sample_table->spp_table.firstEntry(); 
         sppstat && strcmp (sample_table->spp_table.name(), name);
         sppstat = sample_table->spp_table.nextEntry());

  if (!pmsstat && !sppstat) {
    sprintf (msg_str, "Unknown Pms 1D probe requested, %s.\n", name);
    comm_msg->sendPms1Msg (range, name, "", msg_str);
    return;
  } 

// Set the range.
  if (pmsstat) {
    if (!pms1v[sample_table->pms1vme_table.interface()]->
         setRange (sample_table->pms1vme_table.channel(), range)) {
      sprintf (msg_str,"Error setting range on Pms 1d probe %s.\n",name);
      comm_msg->sendPms1Msg (range, name, "", msg_str);
      return;
    } 
  }

  else {
    if (!spp[sample_table->spp_table.port()]->initSpp (range)) {
      sprintf (msg_str,"Error setting range on Spp probe %s.\n",name);
      comm_msg->sendPms1Msg (range, name, "", msg_str);
      return;
    } 
  }

// Send an acknowledgement message.
  if (!strncmp (ASAS_STR, name, 4) ||
      !strncmp (PCAS_STR, name, 4)) {
    if (range == ASAS_ON) 
      sprintf (msg_str, "Pms 1d probe %s pump turned on.\n", name);
    else
      sprintf (msg_str, "Pms 1d probe %s pump turned off.\n", name);
  }
  else
    sprintf (msg_str, "Pms 1d probe %s range set to %1d.\n", name, range);

  comm_msg->sendPms1Msg (range, name, "", msg_str);
  fprintf (stderr, msg_str);
}

/*****************************************************************************/
static void setTasAlt (float tas, float alt)

// Sends true air speed and altitude to the necessary interfaces.
{
  int j;

  for (j = 0; (j < MAX_TANS_INTFC) && (int)tans2[j]; j++)
    tans2[j]->setAltitude (alt);

  for (j = 0; (j < MAX_TANS_INTFC) && (int)tans3[j]; j++)
    tans3[j]->setAltitude (alt);
/*
  for (j = 0; (j < MAX_TANS_INTFC) && (int)garmin[j]; j++)
    garmin[j]->setAltitude (alt);
*/
  if (sample_table->hwirs_table.firstEntry())
    hw_irs[sample_table->hwirs_table.index()]->setTasAlt (tas, alt);
 
  for (j = 0; j < MAX_PMSVME2D_INTFC; j++) { 
    if ((int)pms2d[j])
//
// Floor function is used here to change tas to an integer.
// Casting to an integer caused the program to crash.
//
      pms2d[j]->setTas (floor(tas));
  }

  for (j = 0; j < MAX_GREYVME_INTFC; j++) {
    if ((int)greyv[j])
      greyv[j]->setTas (floor(tas));
  }
}

/*****************************************************************************/
static void setTime (int year, int month, int day, 
                     int hour, int minute, int second)
 
// Sets the system time.
{
  char dtBuff[64];

  if (year >= 100) year -= 100;
  sprintf(dtBuff, "dsmAsync::setTime: %02d/%02d/%02d %02d:%02d:%02d\n",
    year, month, day, hour, minute, second);

// logMsg(dtBuff, 0,0,0,0,0,0);
comm_msg->sendLogMsg(dtBuff, dsm_config->hostName());

  tfp->setMajorTime(year, month, day, hour, minute, second);
  todClock->setTime(year, month, day, hour, minute, second);
}

/*****************************************************************************/
static void setDate(int year, int month, int day)
 
// Sets the system date.
{
  static int date_set = false;

  if (!date_set) {
    setTime(year, month, day, tfp->hour(), tfp->minute(), tfp->second());
    date_set = true;
  }
}

/*****************************************************************************/
static void statusMsg (char *msg)
 
// Class wrapper for sending status messages.
{
  comm_msg->sendStatusMsg (msg);
  fprintf (stderr, msg);
}

/*****************************************************************************/
static void tfpInit ()

// Initialize the time-frequency processor.
{
  tfp = new Bc635Vme ((char*)(A24D16_BASE + TFP_BASE));

  if (!dsm_config->timeMaster())
    tfp->disableJamSync();

  tfp->setPath();			// set default path packet
  tfp->selectModulatedInput();          // select modulated irig input
  tfp->selectModulatedOutput();         // select modulated irig output

// Sync to the 1PPS if operating as the time master.
  if (dsm_config->timeMaster())
    tfp->select1PPSMode();
 
// Sync to irig time code if operating as a time slave.
  else
    tfp->selectTimeCodeMode();
 
// Set the periodic output at 10 Khz, and a 50 usec pulse width.
  tfp->setPeriodicOutput(10000, 50);
 
// Set the major time from the time of day clock.
  todClock->readTime();

  if (dsm_config->timeMaster()) {
    tfp->setMajorTime(todClock->year(), todClock->month(), todClock->date(),
                  todClock->hour(), todClock->minute(), todClock->second());
  }

/*  printf ("tfpInit: TodClock = %02d/%02d/%02d %02d:%02d:%02d\n", 
          todClock->year(), todClock->month(), todClock->date(), 
          todClock->hour(), todClock->minute(), todClock->second());
*/
}

/*****************************************************************************/
static void derivedInit()
 
// Creates and initializes the derived variables classes.
{
  int stat;
 
  psx = (SyncVar*)0;
  qcx = (SyncVar*)0;
  ttx = (SyncVar*)0;
  palt = (Palt*)0;
  tasx = (Tasx*)0;
 
  for (stat = var_config->firstVar(); stat; stat = var_config->nextVar()) {
    if (!strcmp(var_config->varName(), "PSX")) {
      var_config->firstDepend();
      psx = new SyncVar(dsm_hdr, var_config->dependName());
    }

    else if (!strcmp(var_config->varName(), "QCX")) {
      var_config->firstDepend();
      qcx = new SyncVar(dsm_hdr, var_config->dependName());
    }

    else if (!strcmp(var_config->varName(), "TTX")) {
      var_config->firstDepend();
      ttx = new SyncVar(dsm_hdr, var_config->dependName());
    }
 
    else if (!strcmp(var_config->varName(), "PALT")) {
      if (!(int)psx) {
        fprintf (stderr,
                 "Undefined dependencies for PALT in the varconfig file.\n");
        exit (ERROR);
      }
      palt = new Palt (*psx);
//      printf ("palt initialized.\n");
    }
    else if (!strcmp (var_config->varName(), "TASX")) {
      if (!(int)psx || !(int)qcx || !(int)ttx) {
        fprintf (stderr, 
                 "Undefined dependencies for TASX in the varconfig file.\n");
        exit (ERROR);
      }
      tasx = new Tasx (*qcx, *psx, *ttx);
//      printf ("tasx initialized.\n");
    }
    else
      fprintf (stderr, 
        "Unknown derived variable requested in the varconfig file, %s.\n", 
        var_config->varName());
  }
}

/*****************************************************************************/
static void initInterrupts ()
 
// Initialize interrupts and handlers.
{
  int stat;
  int j;

// Connect the time-freq processor isr.
  if (intConnect ((VOIDFUNCPTR*)TFP_ADR,(VOIDFUNCPTR)hertz50_isr, TRUE)){
    perror ("intConnect tfp_isr");
    exit(ERROR);
  }
// Connect the DMS isr.
  if (intConnect ((VOIDFUNCPTR*)DMS_ADR,(VOIDFUNCPTR)dms_isr, TRUE)){
    perror ("intConnect dms_isr");
    exit(ERROR);
  }

// Connect the isio1 bim 2 isr.  UV hygrometer, JPL TDL, and NOAA ozone.
  if ((int)garmin[0] || (int)uvh[0] || (int)jpltdl || (int)lhtdl || (int)rdma || (int)neph || (int)cmigits3){
    if (intConnect((VOIDFUNCPTR*)ISIO1_BIM2_ADR, (VOIDFUNCPTR)isio1Bim2Isr, 0)){
      perror ("intConnect isio1Bim2Isr");
      exit(ERROR);
    }
  }

// Connect the isio1 bim 3 isr. 
  if ((int)dpres[0] || (int)spp[0] || (int)climet) {
    if (intConnect((VOIDFUNCPTR*)ISIO1_BIM3_ADR, (VOIDFUNCPTR)isio1Bim3Isr, 0)){
      perror ("intConnect isio1Bim3Isr");
      exit(ERROR);
    }
  }
 
// Connect the isio1 bim 4 isr.  tans2 and tans3, ophir3, mca.
  if ((int)tans2[0] || (int)tans3[0] || (int)ophir3[0] || (int)pps[0] || 
      (int)mca || (int)cmigits3){
    if (intConnect((VOIDFUNCPTR*)ISIO1_BIM4_ADR, (VOIDFUNCPTR)isio1Bim4Isr, 0)){
      perror ("intConnect isio1Bim4Isr");
      exit(ERROR);
    }
  }
 
// Connect the HwIrs receive isr.  The IP429 uses a base vector with the lower
// three bits of the vector specifying the interrupting channel.
  for (stat = sample_table->hwirs_table.firstEntry(); stat;
       stat = sample_table->hwirs_table.nextEntry()) {
 
    if (intConnect (
          (VOIDFUNCPTR*)(IP429_ADR + (sample_table->hwirs_table.rxChan() * 4)),
          (VOIDFUNCPTR)hwIrsReadIsr, sample_table->hwirs_table.index())) {
      perror ("intConnect hwIrsReadIsr");
      exit(ERROR);
    }
// Enable the receive interrupts in the IPIC chip at level 2.
    *IPIC_IP_a_INT0_CTL = IPIC_INT_CTL_IEN | IPIC_INT_CTL_LEVEL_2;
 
// Enable the IP429-1 interrupts.
    ip429->enableInterrupt (sample_table->hwirs_table.rxChan());
  }

// Connect the HwGps receive isr.  The IP429 uses a base vector with the lower
// three bits of the vector specifying the interrupting channel.
  for (stat = sample_table->hwgps_table.firstEntry(); stat;
       stat = sample_table->hwgps_table.nextEntry()) {
 
    if (intConnect (
          (VOIDFUNCPTR*)(IP429_ADR + (sample_table->hwgps_table.rxChan() * 4)),
          (VOIDFUNCPTR)hwGpsReadIsr, sample_table->hwgps_table.index())) {
      perror ("intConnect hwGpsReadIsr");
      exit(ERROR);
    }
// Enable the receive interrupts in the IPIC chip at level 2.
    *IPIC_IP_a_INT0_CTL = IPIC_INT_CTL_IEN | IPIC_INT_CTL_LEVEL_2;

// Enable the IP429-1 interrupts.
    ip429->enableInterrupt (sample_table->hwgps_table.rxChan());
  }
 
// Connect the HwIrs transmit isr.
  if (sample_table->hwirs_table.firstEntry()) {
    if (intConnect (
          (VOIDFUNCPTR*)(IP429_ADR + (sample_table->hwirs_table.txChan() * 4)),
          (VOIDFUNCPTR)hwIrsWriteIsr, sample_table->hwirs_table.index())) {
      perror ("intConnect hwIrsWriteIsr");
      exit(ERROR);
    }
 
// Enable the transmit interrupts in the IPIC chip at level 1.
    *IPIC_IP_a_INT1_CTL = IPIC_INT_CTL_IEN | IPIC_INT_CTL_LEVEL_1;
 
// Enable the IP429-1 interrupts.
    ip429->enableInterrupt (sample_table->hwirs_table.txChan());
  }

// Enable the vmebus interrupt handler on the mv162 board for levels 3 and 1.
  *VMECHIP2_LBIER = 0x07;
 
// Enable strobe interrupts from the tfp at 5 hertz and level 3.
  tfp->enableMinorStrobeInt (TFP_VCT, 3, 200);

// Enable strobe interrupts from the dms at 55 hertz.
  if((int)digital_in && strcmp(sysBootParams.targetName, "dsm2") == 0)
    digital_in->enableInt (2,DMS_VCT);
 
// Enable interrupts from the tans at level 1.
  for (j = 0; (j < MAX_TANS_INTFC) && (int)tans2[j]; j++)
    tans2[j]->enableInterrupt (ISIO1_BIM4_VCT, 1);

  for (j = 0; (j < MAX_TANS_INTFC) && (int)tans3[j]; j++) {
    tans3[j]->enableInterrupt (ISIO1_BIM4_VCT, 1);
    taskDelay(sysClkRateGet());
    tans3[j]->checkStatus();
  }

  for (j = 0; (j < MAX_TANS_INTFC) && (int)garmin[j]; j++) {
    garmin[j]->enableInterrupt (ISIO1_BIM2_VCT, 1);
    taskDelay(sysClkRateGet());
//    garmin[j]->checkStatus();
    garmin[j]->initGarmin();
  }

  for (j = 0; (j < MAX_CMIG_INTFC) && (int)cmigits3[j]; j++) {
    cmigits3[j]->enableInterrupt (ISIO1_BIM4_VCT, 1);
    taskDelay(sysClkRateGet());
//    cmigits3[j]->checkStatus();
    cmigits3[j]->initCmigits3();
  }

// Enable interrupts from the ophir3 at level 1.
  for (j = 0; (j < MAX_OPHIR3_INTFC) && (int)ophir3[j]; j++)
{
    ophir3[j]->enableInterrupt (ISIO1_BIM4_VCT, 1);
    ophir3[j]->initOphir();
}
  
// Enable interrupts from the Collins PPS at level 1.
  for (j = 0; (j < MAX_PPS_INTFC) && (int)pps[j]; j++)
    pps[j]->enableInterrupt (ISIO1_BIM4_VCT, 1);

// Enable interrupts from the UV hygrometer at level 1.
  for (j = 0; (j < MAX_UVHYG_INTFC) && (int)uvh[j]; j++)
    uvh[j]->enableInterrupt (ISIO1_BIM2_VCT, 1);

// Enable interrupts from the SPP probes at level 1.
  for (j = 0; (j < MAX_SPP_INTFC) && (int)spp[j]; j++) {
    spp[j]->initSpp(0);
    spp[j]->enableInterrupt (ISIO1_BIM3_VCT, 1);
  }

// Enable interrupts from the digital pressure at level 1.
  for (j = 0; (j < MAX_UVHYG_INTFC) && (int)dpres[j]; j++) {
    dpres[j]->enableInterrupt (ISIO1_BIM3_VCT, 1);
//    dpres[j]->initDpres();
  }

// Enable interrupts from the JPL TDL at level 1.
  for (j = 0; (j < MAX_UVHYG_INTFC) && (int)jpltdl[j]; j++) {
    jpltdl[j]->enableInterrupt (ISIO1_BIM2_VCT, 1);
    jpltdl[j]->initTdl();
  }

// Enable interrupts from the LHTDL at level 1.
  if ((int)lhtdl) {
    lhtdl->enableInterrupt (ISIO1_BIM2_VCT, 1);
    lhtdl->initTdl();
  }

// Enable interrupts from the Climet at level 1.
  if ((int)climet) {
    climet->enableInterrupt (ISIO1_BIM3_VCT, 1);
    climet->initClimet();
  }

// Enable interrupts from the Mca at level 1.
  if ((int)mca) {
    mca->enableInterrupt (ISIO1_BIM4_VCT, 1);
    mca->initMca();
  }

// Enable interrupts from the NEPH at level 1.
  for (j = 0; (j < MAX_UVHYG_INTFC) && (int)neph[j]; j++) {
    neph[j]->enableInterrupt (ISIO1_BIM2_VCT, 1);
    neph[j]->initNeph();
  }

// Enable interrupts from the RDMA at level 1.
  if ((int)rdma) {
    rdma->enableInterrupt (ISIO1_BIM2_VCT, 1);
    rdma->initRdma();
  }

}

/*****************************************************************************/
static void hertz50_isr(int hertz5_flag)
{
  int	j, hz25_cnt;
  int	lockKey = intLock();

  // Start 50hz tic timer.
  if (hertz5_flag)
    sysAuxClkEnable();

  /* There are 2 places to unlock interupts.  Use this one when counters
   * aren't critical.  Use the one ~12 lines below for when counting is
   * critical.  This is to help aleviate loss of IRS data in the FWD DSM.
   */
//  intUnlock(lockKey);

  if ((int)counter && (hz50_cnt % RATE_2) == 0)	// counters
    {
    if (tfp->newSecond() || (hz50_cnt >= RATE_50))
      hz25_cnt = 0;
    else
      hz25_cnt = hz50_cnt / 2;

    counter->getData(hz25_cnt);
    }

  intUnlock(lockKey);

  // Check for correct 10 hertz interrupt count.
  if (hz50_cnt % RATE_5 == 0)
  // Call the spp 1d sampling routine.
  for (j = 0; j < MAX_SPP_INTFC; j++) {
    if ((int)spp[j])
      spp[j]->getSppData();
    }

  // Check if this is a 5 hertz interrupt from the tfp.
  if (hertz5_flag || (hz50_cnt >= RATE_50)) {
    tfp->strobeIsr();				// run the tfp isr

    // Check for correct 5 hertz interrupt count.
    if (hz50_cnt % RATE_10)     
      logMsg("Incorrect interrupt counter at 5 hz interval, hz50_cnt = %d.\n",
             hz50_cnt, 0,0,0,0,0);

    // Check for the start of a new second from the tfp, or for a hertz50_cnt
    // overflow. This happens if the timing interface gets a spike on its
    // 1 hertz line.  This upsets the sync in the interface.

    if (tfp->newSecond() || (hz50_cnt >= RATE_50)) {
      hz50_cnt = 0;
/*
  // Call the spp 1d sampling routine.
  for (j = 0; j < MAX_SPP_INTFC; j++) {
    if ((int)spp[j])
      spp[j]->getSppData();
    }
*/
      if ((int)dsp)
        dsp->hertz1_isr();		// align the dsp class with the 1 hertz

      if ((int)mcr)
        mcr->secondAlign();

#ifdef SAIL
      if ((int)vm3118)
        vm3118->hertz1_isr();        	// align the vm3118 class with the 1 hz
#endif

      for (j = 0; (int)hw_irs[j] && j < MAX_HWIRS_INTFC; j++) 
        hw_irs[j]->secondAlign();  	// align the ARINC IRS data

      for (j = 0; (int)spp[j] && j < MAX_SPP_INTFC; j++) 
        spp[j]->secondAlign();  	// align the SPP data

      for (j = 0; (int)garmin[j] && j < MAX_GPS_INTFC; j++) 
        garmin[j]->secondAlign();  	// align the Garmin data

      for (j = 0; (int)cmigits3[j] && j < MAX_CMIG_INTFC; j++) 
        cmigits3[j]->secondAlign();  	// align the Cmigits3 data

      for (j = 0; (int)ophir3[j] && j < MAX_OPHIR3_INTFC; j++) 
        ophir3[j]->secondAlign();
#ifdef NOAA_GIV
      for (j = 0; (int)hw_gps[j] && j < MAX_HWGPS_INTFC; j++) 
        hw_gps[j]->secAlign();  	// align the ARINC GPS data
#endif
      for (j = 0; (int)uvh[j] && j < MAX_UVHYG_INTFC; j++) 
        uvh[j]->secondAlign();  	// align the uv hygrometer data

      for (j = 0; (int)dpres[j] && j < MAX_UVHYG_INTFC; j++) 
        dpres[j]->secondAlign();

      for (j = 0; (int)jpltdl[j] && j < MAX_UVHYG_INTFC; j++) 
        jpltdl[j]->secondAlign();

      if ((int)lhtdl)
        lhtdl->secondAlign();

      if ((int)climet)
        climet->secondAlign();

      if ((int)mca)
        mca->secondAlign();

      for (j = 0; (int)neph[j] && j < MAX_UVHYG_INTFC; j++)
        neph[j]->secondAlign();

      if ((int)rdma)
        rdma->secondAlign();

      // Send the time to the 2d interface on the start of each new second.
      for (j = 0; (int)pms2dh[j] && j < MAX_PMSVME2D_INTFC; j++) {
        pms2dh[j]->secondAlign();
        pms2d[j]->setTime(tfp->hour(), tfp->minute(), tfp->second()); 
      }

      // Set the time on the grey scale interfaces once per hour.
      if (!tfp->minute() && !tfp->second()) {
        for (j = 0; j < MAX_GREYVME_INTFC; j++) {
          if ((int)greyv[j])
            greyv[j]->setTime(tfp->hour(), tfp->minute(), tfp->second());
        }
      }

      // Make sure the dsp still has a valid sample table.

      if ((int)dsp && !dsp->checkTable())
        logMsg("New Analog setup table assigned to the DSP.\n", 0,0,0,0,0,0);
    }

  } // End if (5hz)
  else
    if (((hz50_cnt + 1) % RATE_10) == 0)
      sysAuxClkDisable();


#ifdef SAIL
  if ((int)vm3118)
    if (!(hz50_cnt % RATE_2))
      vm3118->startConversion(hz50_cnt);	// start conv on even ints
    else if (!vm3118->getAdcData())		// read data on odd ints
      logMsg("Failure acquiring data from the Vm3118.\n", 0,0,0,0,0,0);
#endif

  ++hz50_cnt;

  // Release the semaphore to let the main loop run.
  semGive(isr_sem);

}	/* END HERTZ50_ISR */

/*****************************************************************************/
static void gatherData()
{
  int	i, k, stat, hertz50_cnt, hz25_cnt, hz10_cnt, hz5_cnt;

  hertz50_cnt = hz50_cnt - 1;
  hz25_cnt = hertz50_cnt / 2;
  hz10_cnt = hertz50_cnt / 5;
  hz5_cnt = hertz50_cnt / 10;

  // Things to gather at 50 hz.

  if ((int)dsp)
    dsp->getDspData();
  if ((int)digital_in)                        // digital inputs
    digital_in->getData(hertz50_cnt);

  // Things to gather at 25 hz.
  if (!(hertz50_cnt % RATE_2))
    {
//    if ((int)digital_in)                        // digital inputs
//      digital_in->getData(hertz25_cnt);

    if ((int)synchro)
      synchro->getData(hz25_cnt);
    }

  // Things to gather at 10 hz.
  if (!(hertz50_cnt % RATE_5))
    {
    // Call the pms 1d sampling routine.
    for (i = 0; i < MAX_PMS1VME_INTFC; i++) {
      if ((int)pms1v[i])
        pms1v[i]->pms1VmeIsr(hz10_cnt);
      }
    }

  // Things to gather at 5 hz.
  if (!(hertz50_cnt % RATE_10)) {

    for (i = 0; i < MAX_UVHYG_INTFC; i++) {
      if ((int)dpres[i]) {
//        dpres[i]->clrbuf();
//        dpres[i]->checkStatus();
        dpres[i]->reqstData();
      }
    } 

  }

  // Increment the 50 hertz counter.  Set the sync data ready flag on the
  // first 50 hz interrupt following the start of a new second.  This allows
  // time for the irs block to complete at the end of the second before using 
  // the data.


// Things to gather at 1 hz.

  if (hertz50_cnt == 1) {
    syncDataReady = TRUE; 

    if ((int)climet) {
      climet->reqstData();
    }
    if ((int)mca) {
      mca->reqstData();
    }

  // PMS 2D house data.

    for (i = 0; i < MAX_PMSVME2D_INTFC; i++) {
      if ((int)pms2dh[i]) {
        for (k = 0, stat = sample_table->pms2dh_table.firstChannel(); stat;
             k++, stat = sample_table->pms2dh_table.nextChannel()) {
          pms2dh[i]->getData(sample_table->pms2dh_table.channel());
          }
        }
      }

    for (i = 0; i < MAX_GREYVME_INTFC; i++) {
      if ((int)greyh[i]) {
        for (k = 0, stat = sample_table->greyh_table.firstChannel(); stat;
             k++, stat = sample_table->greyh_table.nextChannel()) {
          greyh[i]->getData(k);
          }
        }
      }
    }

}	/* END GATHERDATA */
 
/*****************************************************************************/
/*	Start ISR wrappers						*/
/*****************************************************************************/
static void hwIrsReadIsr(int intf)
 
// Honeywell irs read isr wrapper.
{
  hw_irs[intf]->readIsr();
}

/*****************************************************************************/
static void hwIrsWriteIsr(int intf)
 
// Honeywell irs write isr wrapper.
{
  hw_irs[intf]->sendTasAlt();
}

/*****************************************************************************/
static void hwGpsReadIsr(int intf)
 
// Honeywell gps read isr wrapper.
{
  hw_gps[intf]->readgpsIsr();
}

/*****************************************************************************/
static void isio1Bim1Isr (int intf)

// Interrupt service routine for the isio #1 ports 1 and 2.  Climet.
{
/*
  if ((int)climet)
    climet->isr();
*/
}

/*****************************************************************************/
static void isio1Bim2Isr (int intf)
 
// Interrupt service routine for the isio #1 ports 3 and 4.  Ophir3.
{
  int j;
 
  for (j = 0; (j < MAX_UVHYG_INTFC) && (int)uvh[j]; j++)
    uvh[j]->isr();

  for (j = 0; (j < MAX_UVHYG_INTFC) && (int)jpltdl[j]; j++)
    jpltdl[j]->isr();

  if ((int)lhtdl)
    lhtdl->isr();

  for (j = 0; (j < MAX_UVHYG_INTFC) && (int)neph[j]; j++)
    neph[j]->isr();

  if ((int)rdma)
    rdma->isr();

  for (j = 0; (j < MAX_TANS_INTFC) && (int)garmin[j]; j++)
    garmin[j]->isr();

}

/*****************************************************************************/
static void isio1Bim3Isr (int intf)
 
// Interrupt service routine for the isio #1 ports 5 and 6.  
{
  int j;

  for (j = 0; (j < MAX_UVHYG_INTFC) && (int)dpres[j]; j++)
    dpres[j]->isr();

  for (j = 0; (j < MAX_SPP_INTFC) && (int)spp[j]; j++) {
    spp[j]->isr();
  }

  if ((int)climet)
    climet->isr();

}

/*****************************************************************************/
static void isio1Bim4Isr (int intf)

// Interrupt service routine for the isio #1 ports 7 and 8.  Ophir3.
{
  int	j;

  for (j = 0; (j < MAX_TANS_INTFC) && (int)tans3[j]; j++)
    tans3[j]->isr();
/*
  for (j = 0; (j < MAX_TANS_INTFC) && (int)garmin[j]; j++)
    garmin[j]->isr();
*/
  for (j = 0; (j < MAX_OPHIR3_INTFC) && (int)ophir3[j]; j++)
    ophir3[j]->isr();
/*
  for (j = 0; (j < MAX_SPP_INTFC) && (int)spp[j]; j++)
    spp[j]->isr();
*/
  for (j = 0; (j < MAX_TANS_INTFC) && (int)tans2[j]; j++)
    tans2[j]->isr();

  for (j = 0; (j < MAX_CMIG_INTFC) && (int)cmigits3[j]; j++)
    cmigits3[j]->readIsr();

#ifdef NOAA_GIV
  for (j = 0; (j < MAX_PPS_INTFC) && (int)pps[j]; j++)
    pps[j]->isr();
#endif

  if ((int)mca)
    mca->isr();

}
/*****************************************************************************/
static void dms_isr()

// DigitalIn read isr wrapper.
{
  digital_in->readIsr();
}
/*****************************************************************************/

/* END DSMASYNC.CC */
