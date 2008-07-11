// maspDas.cc - This vxWorks based C++ code forms the entire data
//                acquisition system for the Multi Anlgle Spectrometer
//                Probe (MASP).  This code draws heavily from er2_das.cc,
//                which was written in support of ASHOE/MAESA.
//
// Written by Keith Barr in support of DLR/RAF #144911
//
// Copyright 1995, National Center for Atmospheric Research
//
// Modification History
// --------------------
// 980208, ksb, modifications for new MASP electronics
// 950926, ksb, base er2_das code heavily modified for MASP only operation
//              4772 lines of code at beginning.
//

/* include files */
#include <dasIncl.h>
#include <Bc635Vme.h>
#include <TodClock.h>
#include <MsgQueue.h>
#include <DsmConfig.h>
#include <dsmctl.h>
#include <header.h>
#include <dsmVmeDefs.h>

#define DSM_MSG_MAX_LEN	160

static Bc635Vme *tfp;                   // time-freq processor
static TodClock *todClock;              // time of day clock
static void tfpInit();                  // init time-freq processor
DsmConfig *dsm_config;          // sram net config

extern int dsmComm(	MsgQueue *masDQ, MsgQueue *masHQ,
			MsgQueue *txQ, MsgQueue *rxQ);

static MsgQueue *comm_maspHQ;            // comm Masp histo queue
static MsgQueue *comm_maspDQ;            // comm Masp data queue
static MsgQueue *comm_txQ;              // comm send message data intertask q
static MsgQueue *comm_rxQ;              // comm recv message data intertask q


// Stuff for WINDS.
struct
{
  struct Hdr_blk hdr;
  struct P32v2_blk pb;
} outBlk;

short histo[32];


/* -------------------------------------------------------------------- */
int maspDas()
{	
  int	iflag, stat, i, j, rec;
  int	new_second;
  int	pcnt=0;		/* number of packets received since start */
  short	rfindx=0;	/* buffer number of next packet */
  int	rcSum = 0;	/* number of checksums errors read */
  int	acSum = 0;	/* number of checksums errors calculated */
  int	bpSum = 0;	/* number of bad packets received */

  int das_tid;	/* the task id of this routine */

  /* create the buffers for the data */
  unsigned short *mhb[2];
  unsigned short *mdb[2];

  /* create the pointers to the data */
  unsigned short *mhNext;
  unsigned short *mdNext;

  /* create a pointer to the MASP housekeeping data */
  MASP_HK_PKT *pMHKP;
  MASP_PART_PKT *pMPDP;

  /* create the holder for the number of packets since the last write */
  int mh_pkts = 0;
  int md_pkts = 0;

  /* create the buffer number place holder */
  int mh_buf = 0;
  int md_buf = 0;

  unsigned short p_size, p_type, f_time, p_cksm, p_mode;
  unsigned long m_time;
  unsigned short *data_pk;
  unsigned short p_buf[1024];

  int cycle_fd;
  char chcycle[5];

  unsigned short* msb;
  unsigned short* lsb;



  // Create the time of day clock class.
  todClock = (TodClock*)malloc (sizeof (TodClock));
  todClock->TodClock();
  printf("todClock initialized.\n"); fflush(stdout);

  // Initialize the time generator class.
  tfpInit();
  printf("tfp initialized.\n"); fflush(stdout);

  // Initialize the sram network configuration class.
  dsm_config = (DsmConfig*)SRAM_DSMCONFIG_BASE;
  dsm_config->DsmConfig ();
  if (!dsm_config->selectByHost (sysBootParams.targetName)) {
    fprintf (stderr, "dsmAsync exiting.\n");
    exit (ERROR);
    }

  comm_maspDQ = (MsgQueue*)malloc (sizeof (MsgQueue));
  comm_maspDQ->MsgQueue (MX_PHYS, MSGQ_MASP_MSG);

  comm_maspHQ = (MsgQueue*)malloc (sizeof (MsgQueue));
  comm_maspHQ->MsgQueue (sizeof(outBlk), MSGQ_MASP_MSG);

  comm_txQ = (MsgQueue*)malloc (sizeof (MsgQueue));
  comm_txQ->MsgQueue (DSM_MSG_MAX_LEN * 2, MSGQ_MAX_MSG);
 
  comm_rxQ = (MsgQueue*)malloc (sizeof (MsgQueue));
  comm_rxQ->MsgQueue (DSM_MSG_MAX_LEN * 2, MSGQ_MAX_MSG);


  // Spawn the comm task.
  if (taskSpawn("dsmComm", das_pri + 10, 0, 0x4000,
                (FUNCPTR)dsmComm, (int)comm_maspDQ, (int)comm_maspHQ,
                (int)comm_txQ, (int)comm_rxQ, 0,0,0,0,0,0) == ERROR){
    fprintf (stderr, "Failure spawning dsmComm.\n");
    exit(ERROR);
  }

  /* tell user how we are configured */
  if (MASP) printf ("MASP software installed\n");
  if (DC8)  printf ("NASA DC-8 Software Installed\n");
  if (ER2)  printf ("NASA ER2 Software Installed\n");
  if (C130)  printf ("NCAR C-130 Software Installed\n");
  if (FALCON) printf ("FALCON Software Installed\n");
  if (WB57F) printf ("WB57F Software Installed\n");

  taskDelay(300);
	
  /* Preload Fail file message */
  sprintf(fail_flags, "Bad Connection with VMEMA Board           \n");

  /* load the time buffer */
  get_time();

  /* get last masp set, to give masp time to start */
  last_masp = time_now.sec_o_yr;


  /* first open the serial port for reading & writing */
  // moved to here for polaris, ksb, 970409
  if ( (navPort = open( "/tyCo/1", O_RDWR, 0666)) < 0)
    if (WARNING)
      perror("error opening serial port");

  /* now set the baud rate */
  if ( (ioctl(navPort, FIOBAUDRATE, 9600)) < 0)
    if (WARNING)
      perror("ioctl FIOBAUDRATE");

  /* now set up navPort as 8,n,1 */
  if ( (ioctl(navPort, FIOSETOPTIONS, OPT_RAW)) < 0)
    if (WARNING)
      perror("ioctl FIOSETOPTIONS");

  /* clear ports of any old data */
//  if ( (ioctl(navPort, FIOFLUSH, 0)) < 0)
//    if (WARNING)
//      perror("ioctl FIOFLUSH");

  /* if we have an IP-Digital 24, initialize the digital I/O bitmap */
//  if (IP_PIO_IP_A) pDigital = (unsigned short *) IP_A_BASE;
//  if (IP_PIO_IP_B) pDigital = (unsigned short *) IP_B_BASE;
//  if (IP_PIO_IP_C) pDigital = (unsigned short *) IP_C_BASE;
//  if (IP_PIO_IP_D) pDigital = (unsigned short *) IP_D_BASE;
//
//  *pDigital = 0x0;	// turn all lines off

  bad_save = 0;
  p_buf[0]=0x0;
  pack_ex = 0x0;

  /* initialize dh and nd flags */
  dh_pkts = 0;
  dh_buf = 0;
  nd_pkts = 0;
  nd_buf = 0;

  /* initialize the memory for buffering all data */
  for (i = 0; i < 2; i++) {
    if ( (mhb[i] = (unsigned short *)malloc(73*2*HK_BLOCK)) == NULL)
      if (WARNING)
        printf("mhb[%i] malloc\n", i);

    if ((mdb[i]=(unsigned short *)malloc(1020*2*BLOCK_COUNT)) == NULL)
      if (WARNING)
        printf("mdb[%i] malloc\n", i);

    if ( (dhb[i] = (unsigned short *)malloc(40*2*HK_BLOCK)) == NULL)
      if (WARNING)
        printf("dhb[%i] malloc\n", i);

    if (ER2)	/* 511 char + 2 (cr/lf)  + 9 = 522 */
      if ( (ndb[i] = (unsigned short *)malloc(522*HK_BLOCK)) == NULL)
        if (WARNING)
          printf("ndb[%i] malloc\n", i);

    if (DC8)	/* 56*7 = 392 char + 4 = 396 bytes */
      if ( (ndb[i] = (unsigned short *)malloc(396*HK_BLOCK)) == NULL)
        if (WARNING)
          printf("ndb[%i] malloc\n", i);

    if (C130)	/* 249 characters - 2 (cr/lf) +1 + 4 = 252 bytes */
      if ( (ndb[i] = (unsigned short *)malloc(252*HK_BLOCK)) == NULL)
        if (WARNING)
          printf("ndb[%i] malloc\n", i);

    if (FALCON)	/* 186 characters - 2 (cr/lf) + 4 = 188 bytes */
      if ( (ndb[i] = (unsigned short *)malloc(188*HK_BLOCK)) == NULL)
        if (WARNING) printf("ndb[%i] malloc\n", i);
  }


  /* set the data buffer pointers to point to the first position */
  mhNext = mhb[0];
  mdNext = mdb[0];
  dhNext = dhb[0];
  ndNext = ndb[0];

  /* create the disk semaphores */
  mh_disk = semBCreate(SEM_Q_FIFO, SEM_FULL);
  md_disk = semBCreate(SEM_Q_FIFO, SEM_FULL);
  dh_disk = semBCreate(SEM_Q_FIFO, SEM_FULL);
  nd_disk = semBCreate(SEM_Q_FIFO, SEM_FULL);

  /* set this tasks priority so we know what it is */
  das_pri = 50;			/* 0-255, 255 = lowest priority */
  das_tid = taskIdSelf();
  taskPrioritySet( das_tid, das_pri);

  /* initialize the housekeeping packet */
  das_hk.sync_flag = 0x4450;
  das_hk.length = 0x28;
  das_hk.type = 0x4448;
  das_hk.packets = 0;
  das_hk.ch_read = 0;
  das_hk.ch_calc = 0;
  das_hk.corrupt = 0;
  das_hk.b_calb = 0;
  das_hk.b_time = 0;
  das_hk.write_time = 0;
  das_hk.xons_sent = 0;
  das_hk.tas = 0;
  das_hk.masp_trig = thresh;
  das_hk.masp_xsit = minTtime;
  das_hk.masp_maGain = maGain;
  das_hk.masp_tsGain = tsGain;
  das_hk.spare1 = 0;
  das_hk.spare2 = 0;
  das_hk.spare3 = 0;
  das_hk.spare4 = 0;

  /* create the histogram semaphore */
  hist_sid = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
  if ( taskSpawn("tHisto", das_pri+40, 0, 10000, (FUNCPTR)drawHistos,
                 0,0,0,0,0,0,0,0,0,0) < 0)
    if (WARNING)
      perror("spawning tHisto");

  /* create the synchronization semaphores */
  mode_sid = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
//  adc_sid = semBCreate(SEM_Q_FIFO, SEM_EMPTY);

  /* spawn the adc reading routine */
  /* not needed for polaris
  if ( taskSpawn("tAdc", das_pri-10, 0, 10000, (FUNCPTR)readADC,
					0,0,0,0,0,0,0,0,0,0) < 0)
    if (WARNING)
      perror("spawning tAdc");
*/

  /*
  if ( taskSpawn("tCal", das_pri-10, 0, 10000, (FUNCPTR)calibrate,
					0,0,0,0,0,0,0,0,0,0) < 0)
    if (WARNING)
      perror("spawning tCal");
*/

  taskDelay(50);

  /* create the histogram drawing ID */
  if ( (hist_wid = wdCreate()) == NULL)
    if (WARNING)
      perror("creating histo timer");

  /* connect the watchdog ISR to go off histo_int from now */
  wdStart(hist_wid,(sysClkRateGet()*histo_int), (FUNCPTR) hist_isr, 0);

  /* create the adc watchdog */
//  adc_wid = wdCreate();

  /* create the mode-watchdog ID */
  if ( (mode_wid = wdCreate()) == NULL)
    if (WARNING)
      perror("creating mode timer");

  /* create the death watchdog ID */
  if ( (death_wid = wdCreate()) == NULL)
    if (WARNING)
      perror("creating death timer");


  /* connect the death watchdog to go off in 15 seconds */
//  wdStart(death_wid, (sysClkRateGet()*15), (FUNCPTR) dead_das, 0);

  /* open the message queue */
  if ( (iflag = create_msgQ()) < 0 ) {
    if (WARNING)
      printf("create_msgQ failure, error=%3d",iflag);
    exit(2);
    }

  /* initialize the FILE_DATA structures */
  /* MASP Housekeeping */
  MH_info.pcnt = 0;
  strcpy(MH_info.leader, "MH");
  strcpy(MH_info.drive, "DOSC:MH/");
  MH_info.max_count = 3600/HK_BLOCK;
  MH_info.filenum = 0;
  MH_info.max_files = 15;

  /* MASP Data */
  MD_info.pcnt = 0;
  strcpy(MD_info.leader, "MD");
  strcpy(MD_info.drive, "DOSD:MD/");
  MD_info.max_count = 1024/BLOCK_COUNT;
  MD_info.filenum = 0;
  MD_info.max_files = 200;

  /* DAS Housekeeping */
  DH_info.pcnt = 0;
  strcpy(DH_info.leader, "DH");
  strcpy(DH_info.drive, "DOSC:DH/");
  DH_info.max_count = 3600/HK_BLOCK;
  DH_info.filenum = 0;
  DH_info.max_files = 15;

  /* Navigational Data */
  ND_info.pcnt = 0;
  strcpy(ND_info.leader, "ND");
  strcpy(ND_info.drive, "DOSC:ND/");
  ND_info.max_count = 3600/HK_BLOCK;
  ND_info.filenum = 0;
  ND_info.max_files = 15;

  /* start the user interface */
  if ( taskSpawn("tUsrI", das_pri+20, 0, 10000, (FUNCPTR)user_Iface,
					0,0,0,0,0,0,0,0,0,0) < 0)
    if (WARNING)
      perror("spawning tUsrI");
	

  /* read in the cal data for histograms */
  maspLim[ 0] =      0;
  maspLim[ 1] =     57;
  maspLim[ 2] =     81;
  maspLim[ 3] =    112;
  maspLim[ 4] =    152;
  maspLim[ 5] =    202;
  maspLim[ 6] =    300;
  maspLim[ 7] =    384;
  maspLim[ 8] =    702;
  maspLim[ 9] =    866;
  maspLim[10] =   1011;
  maspLim[11] =   1139;
  maspLim[12] =   1293;
  maspLim[13] =   1442;
  maspLim[14] =   1604;
  maspLim[15] =   1759;
  maspLim[16] =   1785;
  maspLim[17] =   1811;
  maspLim[18] =   1835;
  maspLim[19] =   1859;
  maspLim[20] =   1882;
  maspLim[21] =   4114;
  maspLim[22] =   6505;
  maspLim[23] =   8571;
  maspLim[24] =  11560;
  maspLim[25] =  15077;
  maspLim[26] =  24023;
  maspLim[27] =  38432;
  maspLim[28] = 136016;
  maspLim[29] = 291079;
  maspLim[30] =	504822;


  /* initialize bins in case histos are printed */
  sizeBins();

  // build the xon packet
  xon_pkt.sync_flag = 0x4450;
  xon_pkt.length = 0x0004;
  xon_pkt.xon = 0x4f4e;
  xon_pkt.checksum = 0x93a2;

  /* figure number of data cycles */
  if ( (cycle_fd = open("DOSC:CYCLES", O_RDONLY, 0666)) < 0)
    {        /* cycle file doesn't exist...i.e. first time */
    data_cycle = 1;
    }
  else
    { /* file does exist */
    read(cycle_fd, chcycle, 5);
    data_cycle = atoi(chcycle) + 1;
    printf("data cycle #%i\n", data_cycle);
//    close(cycle_fd);
//    remove("DOSC:CYCLES");
    }

  /* set file so we know how many data cycles we have had */
  if ( (cycle_fd = creat("DOSC:CYCLES", O_WRONLY)) < 0) {
    if (WARNING)
      perror("creating cycle file");
    }
  else
    {
    sprintf(chcycle, "%4d", data_cycle);
    write(cycle_fd, chcycle, 5);
    close(cycle_fd);
    }
	
  /* start gathering data from the NAV serial stream
  if ( taskSpawn("tSerial", das_pri+10, 0, 10000, (FUNCPTR)nav_stream,
				0,0,0,0,0,0,0,0,0,0) < 0)
    if (WARNING)
      perror("spawning tSerial");
*/

  /* initialize the MASP and connect the ISR */
  taskSpawn("tMinit", das_pri+1, 0, 10000, (FUNCPTR)probe_init,
					0,0,0,0,0,0,0,0,0,0);

  /* connect the mode watchdog ISR to go off in 1.25 seconds */
//  wdStart(mode_wid, 125, (FUNCPTR) mode_isr, 0);

  /* connect the adc watchdog to go off in 1.75 seconds */
//  wdStart(adc_wid, 2, (FUNCPTR)adc_isr, 0);

  tfp->readTime();
  outBlk.hdr.id = SAMPLED_ID;
  strcpy(outBlk.hdr.dsm_locn, "MASP"); 

  tfp->selectFreeRunMode();

  // Main loop
  while (1)
    {
    // Check for the start of a new second.
    if ((new_second = tfp->newSecond()))
      {
      tfp->readTime();
      outBlk.hdr.year = tfp->year();
      outBlk.hdr.month = tfp->month();
      outBlk.hdr.day = tfp->day();
      outBlk.hdr.hour = tfp->hour();
      outBlk.hdr.minute = tfp->minute();
      outBlk.hdr.second = tfp->second();
      outBlk.pb.bins[0] = 0;

      for (j = 1; j < 32; j++) {
        outBlk.pb.bins[j] = histo[j-1];
        }

      memset((char *)histo, 0, sizeof(histo));

      if (comm_maspHQ->msgSend ((char *)&outBlk, sizeof(outBlk)) == ERROR)
        printf("Failure sending Masp histo header to comm_maspHQ.\n");

      tfp->clearNewSecond();
      }
/**
   printf ("%02d/%02d/%02d %02d:%02d:%02d\n", tfp->month(), tfp->day(),
   tfp->year(), tfp->hour(), tfp->minute(), tfp->second());
**/


    /* read the message queue */
    if ((iflag = msgQReceive(masp_qid, (char*)&rfindx,2,sysClkRateGet()*5)) > 0)
      {
      /* should we exit? */
      if (param_map & 0x8000) {
        if (WARNING) printf("MAIN is exiting\n");
        return(0);
        }

      /* Check for read checksum errors -- data type dependent code */
      if (rfindx < 7) {
        /* take a look at the flag in the register */
        if ( ( (stat=(int)dpr->rx_flags[rfindx]) > 1) && (pcnt > 0) ) {
          if (WARNING) printf("MASP rx_flag shows checksum error!\n");
          rcSum += 1;
          das_hk.ch_read++;	/* increment housekeeping data */
        }
        /* set up a pointer to the data buffer */
        data_pk = (unsigned short *)&dpr->rxb[rfindx];

        }
      else
        if (WARNING)
          printf("Warning...rfindx = %i\n",rfindx);

      /* get the data block size, type, frac time, and real time */
      p_size = *(data_pk + 1);
      p_type = *(data_pk + 2);
      f_time = *(data_pk + 3);
      lsb = data_pk + 4;
      msb = data_pk + 5;
      m_time = (unsigned long)( (*msb << 16) + *lsb);
      p_mode = *(data_pk + 6);

      p_cksm = *(data_pk + p_size - 1);

      /* now copy the data from the VMEMA to the appropriate buffer */
      if ((p_type == 1) && (p_size == 73))
        {		/* masp hkng */
        /* copy data from vmema to buffer */
        bcopyWords( (char*)data_pk, (char*)mhNext, (int)p_size);

        // fix the swapped time
        pMHKP = (MASP_HK_PKT*)mhNext;
        pMHKP->time = m_time;

        /* get the command counter*/
        mhCommandCount = ( p_mode >> 8 ) & 0xff;
        // get the time...unswap the time
        mhTime = pMHKP->time;

        /* reset the receive buffer flag */
        dpr->rx_flags[rfindx] = 0;
        dpr->dsp_irq = 1;

        /* increment the packets buffered counter */
        mh_pkts++;

        /* calculate the checksum */
        if (p_cksm != (checksum(mhNext, p_size))) {
          if (WARNING) printf("MH Checksum Error Detected\n");
          acSum += 1;
          das_hk.ch_calc++;	/* increment housekeeping data */
          }

        /* print to serial port if requested */
        if (print_map&0x02) {
          if(p_buf[0] == 0x0) {
            bcopyWords((char*)mhNext,(char*)p_buf, p_size);
            taskSpawn("tDmh", das_pri+40, 0, 10000, (FUNCPTR)data_to_screen,
			(int)&p_buf[0],p_size, p_type, 0,0,0,0,0,0,0);
            }
          }

        /* do the one second jobs */
        one_sec();

        /* send to disk if buffer is full */
        if (mh_pkts == HK_BLOCK) {
          /* first wait for semaphore==other buffer free */
          semTake(mh_disk, WAIT_FOREVER);

				/* now spawn the saving task */
          taskSpawn("tSmh", das_pri-1, 0, 10000,
                    (FUNCPTR)data_to_disk, (int)mhb[mh_buf],
                    (int)p_size*2*HK_BLOCK,(int)p_type,(int)&MH_info,
                    0,0,0,0,0,0);

          printf("Sending Masp mh data, size = %d.\n",(int)p_size*2*HK_BLOCK);
          if (comm_maspDQ->msgSend((char *)mhb[mh_buf],(int)p_size*2*HK_BLOCK)
								== ERROR)
            fprintf(stderr, "Failure sending Masp mh data to comm_maspDQ.\n");

          /* go to next buffer for writing */
          mh_buf++;
          mh_buf %= 2;
          mh_pkts = 0;
          mhNext = mhb[mh_buf];
          }
        else
          mhNext+=p_size;

        /* increment last packet time */
        last_masp = time_now.sec_o_yr;
        pcnt++;
        das_hk.packets++;		/* increment housekeeping */

        }
      else
      if ((p_type == 2) && (p_size == 1020))	/* masp data */
        {
        /* copy data from vmema to buffer */
        bcopyWords( (char*)data_pk, (char*)mdNext, (int)p_size);

        /* reset the receive buffer flag */
        dpr->rx_flags[rfindx] = 0;
        dpr->dsp_irq = 1;

        /* increment the counter */
        md_pkts++;

        // fix the swapped time
        pMPDP = (MASP_PART_PKT*)mdNext;
        pMPDP->time = m_time;

        /* calculate the checksum */
        if (p_cksm != (checksum(mdNext, p_size)))
          {
          if (WARNING)
            printf("MD Checksum Error Detected\n");

          acSum += 1;
          das_hk.ch_calc++;	/* increment housekeeping data */
          }
/*
        else if ( (pretime == m_time) && (f_time < prefract))
          {
          printf("Reversed Fractional Time:\n");
          printf("     Last Packet: %u  %u\n",pretime,prefract);
          printf("     This Packet: %u  %u\n",m_time,f_time);
          }

        pretime = m_time;
        prefract = f_time;
*/
        /* print to serial port if requested */
        if (print_map & 0x01)
          {
          if (p_buf[0] == 0x0)
            {
            if (cal_flag == 1)
              {
              cal_pkt_cnt++;
              if ((cal_pkt_cnt % 10) == (unsigned)0)
                {
                bcopyWords((char*)mdNext,(char*)p_buf, p_size);
                taskSpawn("tDmd", das_pri+40, 0, 10000, (FUNCPTR)data_to_screen,
			(int)&p_buf[0],p_size, p_type, 0,0,0,0,0,0,0);
                } /* we should print this one */
              }
            else
              {		/* we aren't calibrating */
              bcopyWords((char*)mdNext,(char*)p_buf, p_size);
              taskSpawn("tDmd", das_pri+40, 0, 10000, (FUNCPTR)data_to_screen,
			(int)&p_buf[0],p_size, p_type, 0,0,0,0,0,0,0);
              }
            }
          }

        /* do we want histograms? */
//        if (print_map & 0x200)
          buildHisto((struct MASP_PART_PKT *)mdNext);

        /* send to disk if buffer is full */
        if (md_pkts == BLOCK_COUNT) {
          /* first wait for semaphore==other buffer free */
          semTake(md_disk, WAIT_FOREVER);

          /* now spawn the saving task */
          taskSpawn("tSmd", das_pri-1, 0, 10000,
                    (FUNCPTR)data_to_disk, (int)mdb[md_buf],
                    p_size*2*BLOCK_COUNT, p_type, (int)&MD_info,
                    0,0,0,0,0,0);

printf("Sending Masp md data, size = %d.\n",(int)p_size*2*BLOCK_COUNT);
          if (comm_maspDQ->msgSend ((char *)mdb[md_buf], 
                                    (int)p_size*2*BLOCK_COUNT) == ERROR)
            fprintf(stderr, "Failure sending Masp md data to comm_maspDQ.\n");
	
          /* go to next buffer for writing */
          md_buf++;
          md_buf %= 2;
          md_pkts = 0;
          mdNext = mdb[md_buf];
          }
        else
          mdNext += p_size;

        /* increment last packet time */
        last_masp = time_now.sec_o_yr;
        pcnt++;
        das_hk.packets++;		/* increment housekeeping */

        }
      else
        {				/* unknown */
        /* tell anyone watching */
        if (WARNING) 
          printf("Unknown packet: Type %i, Size %i, Index %i\n",
						p_type, p_size, rfindx);
        das_hk.corrupt++;
        bpSum++;

        /* clear the buffer */
        if (rfindx < 7) {		/* MASP data */
          dpr->rx_flags[rfindx] = 0;
          dpr->dsp_irq = 1;		/* force buffer check */
          }
        }


      /* print packet information if requested */
      if ((print_map & 0x20) && (pack_ex == 0x0)) {
        pack_ex = 0x1;
        taskSpawn("tDpack", das_pri+40,0,10000, (FUNCPTR)print_pack,p_size,
                  rfindx,p_type,m_time,f_time,pcnt,rcSum,acSum,bpSum,0);
        }
      }
    else
    if ( errno == S_objLib_OBJ_TIMEOUT )
      {
      printf("Message Queue Timeout, resetting VME board\n");

      // clear the buffers
      for (i = 0; i < 7; i++)
        dpr->rx_flags[ i ] = 0;

      // clear the interrupts
      *(unsigned short *) (VMEMA_BASE+A24D16_BASE+BIC_CTL_REG2) = 0x43;
      *(unsigned short *) (VMEMA_BASE+A24D16_BASE+BIC_CTL_REG2) = 0x63;

      // force a buffer check
      dpr->dsp_irq = 1;

      // send an XON
      bcopyWords((char*)&xon_pkt, (char*)&dpr->txb, xon_pkt.length);
      dpr->dsp_irq = 3;
      } 
    else 
      if (WARNING)
        printf("MESSAGE QUEUE ERROR:  IFLAG=%i\n",iflag);

    }	/* END MAIN LOOP */

} /* end of er2_das */


/* ################################################################ */
int create_msgQ()
/* ################################################################ */
/*     Revision History:                                            */
/*                         10/06/93 KSB  created                    */
/*                                                                  */
/*     This routine will create the message queue.                  */
/*                                                                  */
/*     Returns:                                                     */
/*              0  successful completion                            */
/*             -1  failure upon creation of message queue           */
/*                                                                  */
/* ################################################################ */
{
  /* create the message queue (30 messages, 2 bytes each, FIFO) */
  masp_qid = msgQCreate(30,2,MSG_Q_FIFO);
	
  return(0);

} /* end of create_msgQ */


/* ################################################################ */
int probe_init()
/* ################################################################ */
/*     Revision History:                                            */
/*                         10/12/93 KSB  subroutine-ified           */
/*                         10/11/93 KAW  added XON packet           */
/*                         09/29/93 KAW  created                    */
/*                                                                  */
/*     This routine will initialize the MASP probe, and connect     */
/*               the ISR.                                           */
/*                                                                  */
/*     Returns:                                                     */
/*              0  successful completion                            */
/*             -1  failure upon connection of ISR                   */
/*                                                                  */
/* ################################################################ */
{
  int i, iflag = 0;
  int sp2_vct = SP2_VCT_NUM*4;

  taskDelay(50);

  *VMECHIP2_LBIER = 0x04;		/* enable interrupts (KAW) */

  /* initialize the ISR - VN 0x76, level 3 (written by KAW) */
  dpr = (struct DPR *) (DPR_A);

  *(unsigned short *) (VMEMA_BASE+A24D16_BASE+SW_RESET) = 0x00;
  *(unsigned short *) (VMEMA_BASE+A24D16_BASE+BIC_CTL_REG1) =SP1_VCT_NUM;
	
  /* connect the interrupt */
  if (iflag = intConnect((VOIDFUNCPTR*)sp2_vct,(VOIDFUNCPTR)masp_isr,0)) {
    if (WARNING) printf("masp_isr connect error");
    exit(-1);
    }

  /* allow connection to complete */
  taskDelay(33);

  /* clear the buffers */
  for (i = 0; i < 7; i++)
    dpr->rx_flags[i] = 0;

  dpr->dsp_irq = 1;	/* force a buffer check */

  return (0);
		
} /* END PROBE_INIT */


/* ################################################################ */
void masp_isr()
/* ################################################################ */
/*     Revision History:                                            */
/*                         11/01/93 KSB  fixed static hold          */
/*                         10/26/93 KSB  added static hold          */
/*                         10/06/93 KSB  added support for MsgQ     */
/*                         09/29/93 KAW  created                    */
/*                                                                  */
/*                                                                  */
/*     Returns:                                                     */
/*               Nothing                                            */
/*                                                                  */
/* ################################################################ */
{
  static short n_buf = 0;
  int i;

  /* clear the innterrupt on the VMEMA card */
  *(unsigned short *) (VMEMA_BASE+A24D16_BASE+BIC_CTL_REG2) = 0x43;
  *(unsigned short *) (VMEMA_BASE+A24D16_BASE+BIC_CTL_REG2) = 0x63;

		
  for (i = 0; i < 7; i++)
    {
    if (dpr->rx_flags[n_buf])
      {
      msgQSend(masp_qid, (char*)&n_buf, 2, NO_WAIT, MSG_PRI_NORMAL);
      i = 6;
      } 

    n_buf++;
    n_buf %= 7;
    }

}	/* END MASP_ISR */


/* ####################################################################### */
void one_sec()
/* ####################################################################### */
/* Revision History:                                                       */
/*                    10/29/93 KSB created                                 */
/*                    11/11/93 KSB  moved synch jobs to semaphores         */
/*                                                                         */
/* This routine will start the "every second" items.  Which include        */
/*      filling the time_now structure, and informing the MASP what        */
/*      time it is.                                                        */
/*                                                                         */
/* Returns:                                                                */
/*           Nothing                                                       */
/*                                                                         */
/* ####################################################################### */
{
  /* first check the time to see if there is an error */
  /* if time received is off by more than 5 seconds, resend the time */
  /* update the clock structure */
  unsigned int seconds = get_time();

  if (( mhTime > (seconds + 5) ) || ( mhTime < (seconds - 5) ))
    {
    sendTime();

    if (WARNING) {
      printf("one_sec: Time sent to probe.\n");
      printf("         MHTime = %u, MV162Time=%u\n", mhTime, seconds);
      }
    }

  /* do we need to send the mode? */
  if (modeSentCount == 0)
    {
    // count this one
    modeSentCount = 1;

    sendMode();
    if (WARNING) printf("one_sec: Mode sent to probe.\n");
    }
  else if ( modeSentCount != mhCommandCount )
    {
    if (WARNING)
      printf("one_sec: %d=sent count, %d=mh count\n",
				modeSentCount, mhCommandCount);
    sendMode();
    if (WARNING)
      printf("one_sec: Mode sent to probe, counts not equal.\n" );
    }


  /* close pilot warning watch dog, and restart */
  wdCancel (death_wid);
//  wdStart(death_wid, (sysClkRateGet()*4), (FUNCPTR) dead_das, 0);

  /* rewrite register just in case */
  *(unsigned short *) (VMEMA_BASE+A24D16_BASE+BIC_CTL_REG2) = 0x63;


  /* call the das housekeeping routine */
  taskSpawn("tDHE", das_pri-5, 0,10000, (FUNCPTR)das_hkng, 0,0,0,0,0,0,0,0,0,0);

  /* how long since we have seen data from the masp */
  int t_diff = time_now.sec_o_yr - last_masp;

  /* check to see if masp is hung */
  if ((t_diff%10 == 0) && (t_diff > 9))
    if (WARNING)
      printf("No data from MASP in %d seconds!\n", time_now.sec_o_yr-last_masp);

  if ((t_diff > 1) && (t_diff%3 == 0) && (t_diff < 86400))
    {
    if (WARNING) printf("one_sec: No data from MASP, clearing.\n" );

    // show the user the problem
    for (int i = 0; i < 7; i++)
      {
      printf(" %d", dpr->rx_flags[i]);
      dpr->rx_flags[i] = 0;
      }

    /* clear the innterrupt on the VMEMA card */
    *(unsigned short *) (VMEMA_BASE+A24D16_BASE+BIC_CTL_REG2) = 0x43;
    *(unsigned short *) (VMEMA_BASE+A24D16_BASE+BIC_CTL_REG2) = 0x63;
    }


  /* do we want to light the fail light? */
  if ( (t_diff > 5) || (bad_save > 200) ) 
    {
    /* tell the user */
    if (WARNING) printf("%u: t_diff=%i bad_save=%i data_cycle=%i\n", 
                        time_now.sec_o_yr,t_diff,bad_save,data_cycle);

    /* build the line for the fail file */
    sprintf(fail_flags, "-> %2i:%2i:%2i: t_dif=%3i bd_save=%3i\n",
            time_now.hour,time_now.minute, time_now.second,t_diff,bad_save);

    /* save the data if we can */
    if ((fail_id = open("DOSC:FAIL1", O_RDWR, 0777)) <= 0)
      fail_id = creat("DOSC:FAIL1", O_WRONLY);
    else
      lseek(fail_id, 0, SEEK_END);

    printf("Opened Fail File\n");
    write(fail_id, fail_flags, 42);
    close(fail_id);


    /* do we want to tell the pilot, or reboot? */
    if ( (data_cycle%3) == 0 && (data_cycle < 9) )
      if (WARNING)
        printf("FAIL LIGHT ON");
    else if (data_cycle < 20)
      ;
//      dead_das();
    else
      {
//      outdated for polaris, ksb
//      *pDigital |= 0x1;	/* pin 5 high */
//      taskDelay(5);
//      *pDigital ^= 0x1;	/* pin 5 low */

      // simply write to serial port
      write(navPort, "B", 1);
      }
    }
  else
    {
//    outdated for polaris, ksb
//    *pDigital |= 0x1;	/* pin 5 high */
//    taskDelay(5);
//    *pDigital ^= 0x1;	/* pin 5 low */
			
    // simply write to serial port
    write(navPort, "G", 1);
    }

}


/* ####################################################################### */
void sendTime()
/* ####################################################################### */
{
  int		i;
  unsigned int	seconds = get_time();

  /* initialize the MASP time packet */
  masp_time.sync_flag = 0x4450;
  masp_time.length = 0x7;
  masp_time.type = 0x544d;
  masp_time.spare = 0xabcd;

  /* fill in the data to be sent to the MASP */
  /* we need to swap the words */
  unsigned short *msb;
  unsigned short *lsb;

  msb = (unsigned short*)&seconds;
  lsb = msb + 1;
  masp_time.seconds_yr = (unsigned long)((*lsb << 16) + *msb);

  /* calculate the checksum */
  masp_time.cksum = checksum((unsigned short *)&masp_time, masp_time.length);

  /* check to see if we can send data to MASP...if so, send it */
  for (i = 0; dpr->cmd_stat > 0 && i < 5; ++i)
    {
    das_hk.b_time++;
    taskDelay(2);
    }

  if (dpr->cmd_stat == 0)
    {
    bcopyWords((char*)&masp_time, (char*)&dpr->txb, masp_time.length);
    dpr->dsp_irq = 3;
    }
}

/*****************************************************************************/
static void tfpInit ()
 
// Initialize the time-frequency processor.
{
  tfp = (Bc635Vme*)malloc (sizeof (Bc635Vme));
  tfp->Bc635Vme ((char*)(A24D16_BASE + TFP_BASE));
 
  tfp->disableJamSync();
 
  tfp->setPath();                       // set default path packet
  tfp->selectModulatedInput();          // select modulated irig input
  tfp->selectModulatedOutput();         // select modulated irig output
 
// Sync to irig time code if operating as a time slave.
  tfp->selectTimeCodeMode();
 
// Set the periodic output at 10 Khz, and a 50 usec pulse width.
//  tfp->setPeriodicOutput (10000, 50);

// Set the major time from the time of day clock.
  todClock->readTime();
 
  tfp->setMajorTime (todClock->year(), todClock->month(), todClock->date(),
                     todClock->hour(), todClock->minute(), todClock->second());
/* 
  printf("tfpInit: TodClock = %02d/%02d/%02d %02d:%02d:%02d\n",
	todClock->year(), todClock->month(), todClock->date(),
	todClock->hour(), todClock->minute(), todClock->second());
*/

}	/* END TFPINIT */
 
/* ####################################################################### */
void adc_isr()
{
  /* call this function again in 1/10 of a second */
  wdStart(adc_wid, 10, (FUNCPTR) adc_isr, 0);

  /* give the semaphore to  */
  semGive(adc_sid);

}

/* ####################################################################### */
void readADC()
{
  unsigned short *pControl;
  unsigned short *pReading;
  int i;

  /* SLOT A  (channels 9-12) */
  /* set up the control register address */
  pControl = (unsigned short *) (IP_A_BASE + 0x00);
	
  /* set up the data address */
  pReading = (unsigned short *) (IP_A_BASE + 0x10);
	
  while (1)
    {
    /* first wait for semaphore from time_isr */
    semTake(adc_sid, WAIT_FOREVER);
	
    for (i = 0; i < 4; i++) {
      // set up the control register -- Differential input with resistor
      *pControl = 0x99f0 + i;
	
      // wait for reading to arrive 1/60 = 0.0167, need .000014
      taskDelay(2);
	
      // now get the value -- only first 12 bits matter
      adc[i+8] += (*pReading - 0xf000);
      adcCount[i+8]++;
		
      // are we ready to average?
      if (adcCount[i+8] == 10)
        {
        das_hk.adc[i+8] = adc[i+8] / adcCount[i+8];
        adc[i+8] = 0;
        adcCount[i+8] = 0;
        }
      }
    }
}

/* ####################################################################### */

/* ####################################################################### */
void das_hkng()
/* ####################################################################### */
/* Revision History:                                                       */
/*                    11/15/93 KSB created                                 */
/*                    11/30/93 KSB added disk write timer                  */
/*                    01/13/93 KSB fixed data overwrite bug                */
/*                                                                         */
/* This routine will gather and record the das housekeeping data.          */
/*                                                                         */
/* Returns:                                                                */
/*           Nothing                                                       */
/*                                                                         */
/* ####################################################################### */
{
  int i=0;
  int j=0;

//  unsigned short *pControl;
//  unsigned short *pReading;
//  unsigned short volts;
  unsigned short dec;
  long ones;
  long gain;
  long offset;
  long	volts;

  /* fill housekeeping packet */
  das_hk.time = time_now.sec_o_yr;
  das_hk.masp_trig = thresh;
  das_hk.masp_xsit = minTtime;
  das_hk.masp_maGain = maGain;
  das_hk.masp_tsGain = tsGain;
	
  /*  not needed for polaris
  // SLOT A -- first 4 read at 10 Hz...others useless
  for (i = 12; i < 16; i++) {
    // zero them
    das_hk.adc[i] = 0x806;
    }
*/

/*
  // modifications made for Polaris, ksb, 4/9/97
  // SLOT B  (channels 1-16)
  // set up the control register address
  pControl = (unsigned short *) (IP_B_BASE + 0x00);

  // set up the data address
  pReading = (unsigned short *) (IP_B_BASE + 0x10);

  // loop through, reading all 16 single differential channels
  // first 8

  for (i = 0; i < 8; i++)
    {
    // set up the control register
    // single ended, gain 1
    *pControl = 0xf0f0 + i;

    // wait for reading to arrive 1/60 = 0.0167, need .000014
    taskDelay(2);

    // now get the value -- only first 12 bits matter
    das_hk.adc[i] = *pReading & 0xfff;
//    printf("%d: 0x%04x\n", i,  das_hk.adc[i] );
    }

  // second 8
  for (i = 0; i < 8; i++)
    {
    // set up the control register
    // single ended, gain 1
    *pControl = 0x0ff0 + i;

    // wait for reading to arrive 1/60 = 0.0167, need .000014
    taskDelay(2);

    // now get the value -- only first 12 bits matter
    das_hk.adc[ i + 8 ] = *pReading & 0xfff;
//    printf("%d: 0x%04x\n", i+8,  das_hk.adc[i + 8]);
    }
*/

  /* now calculate checksum */
  das_hk.cksum = checksum((unsigned short *)&das_hk, das_hk.length);

  /* copy das_hk to ping pong buffer */
  bcopyWords((char*)&das_hk, (char*)dhNext, (int)das_hk.length);
  dh_pkts++;

  /* send data to disk */
  if (dh_pkts == HK_BLOCK)
    {
    /* first wait for semaphore==other buffer free */
    semTake(dh_disk, WAIT_FOREVER);

    /* now spawn the saving task */
    taskSpawn("tSdh", das_pri-1, 0, 10000, (FUNCPTR)data_to_disk,
              (int)dhb[dh_buf], (int)das_hk.length*2*HK_BLOCK,
              6, (int)&DH_info,0,0,0,0,0,0);

printf("Sending Masp DH data, size = %d\n",(int)das_hk.length*2*HK_BLOCK);

    if (comm_maspDQ->msgSend ((char *)dhb[dh_buf], 
                              (int)das_hk.length*2*HK_BLOCK) == ERROR)
      fprintf (stderr, "Failure sending Masp dh data to comm_maspDQ.\n");

    /* go to next buffer for writing */
    dh_buf++;
    dh_buf %= 2;
    dh_pkts = 0;
    dhNext = dhb[dh_buf];
    }
  else
    /* increment pointer */
    dhNext += das_hk.length;

  /* send a mode packet? */
//  if ((das_hk.time == 0x15555) || (das_hk.time == 0)) {
//    sendMode();
//    ++modeSentCount %= 256;
//    printf("Mode sent from das_hkng\n");
//    }

  /* if requested, print data */
  if ((print_map & 0x40) == 0x40) {
    cls();
    printf("DAS HOUSEKEEPING DATA   Time: %8u           %02u:%02u:%02u    XONs sent: %4u\n",
	das_hk.time, time_now.hour, time_now.minute, time_now.second,
	das_hk.xons_sent);
    printf(" Checksum Errors: Read (masp): %4u                  Calculated (all): %4u \n",
	das_hk.ch_read, das_hk.ch_calc);
    printf("           Waits: calibration: %4u Time (masp): %4u                      \n",
	das_hk.b_calb, das_hk.b_time); 
    printf("           Misc.:     Packets: %4u   Save time: %4u  Corrupt Packets: %4u \n",
	das_hk.packets, das_hk.write_time, das_hk.corrupt);
    printf("   ADCB:");

    for (i = 0; i < 8; i++)
      {
      if ((i > 1) && (i < 5)) {
        gain = 42;
        offset = 30;
        }

      if ((i < 2) || (i == 5)) {
        gain = 62;
        offset = 10;
        }

      if (i == 6) {
        gain = 2;
        offset = 0;
        }

      if (i == 7) {
        gain = -8;
        offset = 253;
        }

      // from 970724 calibration
      volts = ((((das_hk.adc[i] - 2048) * 10) / gain));
//      if (volts > 60000) volts = 0;
      ones = volts / 10;
      dec = volts - ones * 10;
      ones += offset;
      printf(" %d)%d.%01u", i, ones, dec);
      }

    printf("\nRAW ADCB:");
    for (i = 0; i < 8; i++)
      printf(" %d)%01u", i, das_hk.adc[i]);

    printf("\n    ADCB:");
    for (i = 8; i < 16; i++)
      {
      if (i == 8) {
        gain = -46;
        offset = 114;
        }
      if ((i > 8) && (i < 12)) {
        gain = 410;
        offset = 0;
        }
      if (i == 12) {
        gain = 68;
        offset = 0;
        }
      if (i == 13) {
        gain = 205;
        offset = 0;
        }
      if (i > 13) {
        gain = 137;
        offset = 0;
        }

      volts = -((das_hk.adc[i] - 2048)*10)/gain;
      if (volts > 60000) volts = 0;
      ones = volts / 1000;
      dec = volts - ones * 1000;

      ones = volts / 10;
      dec = volts - ones * 10;
      ones += offset;
      printf(" %d)%d.%01u", i, ones, dec);
      }

    printf("\nRAW ADCB:");
    for (i = 8; i < 16; i++)
      printf(" %d)%u", i, das_hk.adc[i]);

    printf(
      "\n        TAS:%3d  MASP_TG:%4d  MASP_XT:%4d                          \n",
		das_hk.tas, das_hk.masp_trig, das_hk.masp_xsit);
    printf("        Max_Pkts %3u  M/T_Ratio %4u  Histo_Lock %5u\n",
		max_masp_pkts, mt_ar, histo_lock); 
    printf("        MAGain   %4u TSGain    %4u  MD: %3u  MH: %3u\n", 
		das_hk.masp_maGain, das_hk.masp_tsGain,
		MD_info.filenum, MH_info.filenum);
    printf("\n");
    }


  /* now reset counts */
  das_hk.packets = 0;
  das_hk.ch_read = 0;
  das_hk.ch_calc = 0;
  das_hk.corrupt = 0;
  das_hk.b_calb = 0;
  das_hk.b_time = 0;
  das_hk.xons_sent = 0;
  das_hk.tas = 0;

} /* end of das_hk */


/* ####################################################################### */
void mode_isr()
/* ####################################################################### */
/* Revision History:                                                       */
/*                    10/29/93 KSB created                                 */
/*                    11/11/93 KSB  moved synch jobs to semaphores         */
/*                                                                         */
/* This routine will start the task to put the MASP in calibration mode    */
/*      and bring it back to data mode                                     */
/*                                                                         */
/* Returns:                                                                */
/*           Nothing                                                       */
/*                                                                         */
/* ####################################################################### */
{
  /* first we need to start the timer again so it will call again */
  wdStart(mode_wid, (sysClkRateGet()*60*INTERVAL), (FUNCPTR) mode_isr, 0);

  /* set the cal_flag 
  cal_flag = 1;
*/

  /* give the semaphore to the calibration task */
  semGive(mode_sid);
}

/* ####################################################################### */
void calibrate()
/* ####################################################################### */
/* Revision History:                                                       */
/*                    10/29/93 KSB created                                 */
/*                    11/11/93 KSB  moved synch jobs to semaphores         */
/*                                                                         */
/* This routine will put the MASP in calibration mode, and bring it back   */
/*      to data mode                                                       */
/*                                                                         */
/* Returns:                                                                */
/*           Nothing                                                       */
/*                                                                         */
/* ####################################################################### */
{
  int i,ignoreFlag;
  ignoreFlag = 1;

  /* initialize the MASP mode packet */
  masp_mode.sync_flag = 0x4450;	
  masp_mode.length = 0x13;
  masp_mode.type = 0x4d44;
  masp_mode.level[0] = MODE_LEVEL_1;
  masp_mode.level[1] = MODE_LEVEL_2;
  masp_mode.level[2] = MODE_LEVEL_3;	/* laser Amplitudes */
  masp_mode.width[0] = MODE_WIDTH_1;
  masp_mode.width[1] = MODE_WIDTH_2;
  masp_mode.width[2] = MODE_WIDTH_3;	/* Pulse Durations */
  masp_mode.ki = KI;
  masp_mode.kp = KP;
  masp_mode.tas = 200;

  while (1)
    {
    /* wait for semaphore */
    semTake(mode_sid, WAIT_FOREVER);

    if (ignoreFlag != 1)
      {
      /* change the threshold */
      masp_mode.trig_thresh = thresh;
      masp_mode.min_ttime = minTtime;
      masp_mode.maGain = maGain;
      masp_mode.tsGain = tsGain;
      masp_mode.mode = 0x19;		
      masp_mode.max_pkts = 50;	
      masp_mode.cksum = checksum((unsigned short *)&masp_mode,masp_mode.length);
      }

    /* should we exit? */
    if (param_map & 0x8000) {
      if (WARNING) printf("CALIBRATE is exiting\n");
      return;
      }

    if (ignoreFlag != 1)
      {
      if ((param_map & 0x1) || ( cal_flag == 1) )
        {
        taskDelay(20);

        /* set calibration flag high */
        cal_flag = 1;
        cal_pkt_cnt = 0;

        /* we enter the routine with the mode packet set for calibrate */
        /* so all we have to do is send it */
        for (i = 0; dpr->cmd_stat > 0 && i < 10; ++i)
          {
          if (WARNING) printf("blocked while sending cal packet\n");
          das_hk.b_calb++;
          taskDelay(2);
          }

        if (dpr->cmd_stat == 0) {
          bcopyWords((char *)&masp_mode,(char *)&dpr->txb, masp_mode.length);
          dpr->dsp_irq = 3;
          }

        /* count it */
        ++modeSentCount %= 256;

        /* wait a bit and turn the cal_flag off */
        taskDelay(60);
        cal_flag = 0;

	/* lets reset the throttle rate */
	masp_mode.mode = 0x18;		/* 1 = Calibrate, 0 = data */
	masp_mode.max_pkts = max_masp_pkts;
	masp_mode.cksum=checksum((unsigned short *)&masp_mode,masp_mode.length);

	for (i = 0; dpr->cmd_stat > 0 && i < 10; ++i) {
          if (WARNING) printf("blocked while sending cal packet\n");
          das_hk.b_calb++;
          taskDelay(2);
          }

	if (dpr->cmd_stat == 0) {
          bcopyWords ( (char *)&masp_mode,(char *)&dpr->txb, masp_mode.length);
          dpr->dsp_irq = 3;
          }

        /* count it */
        ++modeSentCount %= 256;

        /* run pulse now in case the cal shut the mode off */
        pulse_now();
        }
      }

    if (ignoreFlag == 1)
      {
      sendMode();
      ++modeSentCount %= 256;
      }
    }

}


/* ####################################################################### */
void sendMode()
/* ####################################################################### */
/* Revision History:                                                       */
/*                    03/19/94 KSB created                                 */
/*                                                                         */
/* This routine will update the mode parameters on both probes             */
/*                                                                         */
/* Returns:  Nothing                                                       */
/*                                                                         */
/* ####################################################################### */
{
  struct MODE_PACKET      masp_set;
  int i;

  /* build packets */
  masp_set.sync_flag   = 0x4450;
  masp_set.length      = 0x13;
  masp_set.type        = 0x4d44;
  masp_set.mode        = 0x18;		/* 1 = Calibrate, 0 = data */
  masp_set.level[0]    = MODE_LEVEL_1;
  masp_set.level[1]    = MODE_LEVEL_2;
  masp_set.level[2]    = MODE_LEVEL_3;	/* laser Amplitudes */
  masp_set.width[0]    = MODE_WIDTH_1;
  masp_set.width[1]    = MODE_WIDTH_2;
  masp_set.width[2]    = MODE_WIDTH_3;	/* Pulse Durations */
  masp_set.ki          = KI;
  masp_set.kp          = KP;
  masp_set.trig_thresh = thresh;
  masp_set.max_pkts    = max_masp_pkts;
  masp_set.min_ttime   = minTtime;
  masp_set.maGain      = maGain;
  masp_set.tsGain      = tsGain;

  /* fill in the true airspeed to be sent to the MASP */
  if (param_map & 0x2)		/* user defined */
    masp_set.tas = tas;
  else
    masp_set.tas = 200;

  /* fill in the TAS for the das housekeeping */
  das_hk.tas = masp_set.tas;

  unsigned short commandNum = modeSentCount << 8;
  masp_set.mode &= 0x00ff;
  masp_set.mode |= commandNum;

  /* calculate the checksum */
  masp_set.cksum = checksum((unsigned short *)&masp_set,masp_set.length);

  /* send packet to probe */
  for (i = 0; dpr->cmd_stat > 0 && i < 10; ++i)
    taskDelay(3);

  if (dpr->cmd_stat == 0) {
    bcopyWords ( (char *)&masp_set,(char *)&dpr->txb, masp_set.length);
    dpr->dsp_irq = 3;
    }

}

/* ####################################################################### */
unsigned int get_time()
/* ####################################################################### */
/*   Revision History:                                                     */
/*                      10/25/93 KSB created                               */
/*                      10/27/93 KSB added support for Julian date         */
/*                                                                         */
/*   This routine will fill the time_now structure with the current time   */
/*                                                                         */
/*   Returns:                                                              */
/*            -1  time retreival failure                                   */
/*           > 0  time in seconds since January 1, 00:00:00                */
/*                                                                         */
/* ####################################################################### */
{

  /* first create the pointers to the time data on the MVME162...*/
  /*    The time is held on an MK48T08B-15 Timekeeper RAM Chip */
  unsigned char *pControl;
  unsigned char *pSecond;
  unsigned char *pMinute;
  unsigned char *pHour;
  unsigned char *pDay;
  unsigned char *pDate;
  unsigned char *pMonth;
  unsigned char *pYear;

  /* create variables to hold the non BCD times */
  unsigned int control;
  unsigned int second;
  unsigned int minute;
  unsigned int hour;
  unsigned int date;
  unsigned int day;
  unsigned int julian;
  unsigned int month;
  unsigned int year;
  unsigned int seconds_year;

  julian = 0;
  seconds_year = 0;

  /* load the pointers with the location of the respective data */
  pControl = (unsigned char *) 0xfffc1ff8;
  pSecond = (unsigned char *) 0xfffc1ff9;
  pMinute = (unsigned char *) 0xfffc1ffa;
  pHour = (unsigned char *) 0xfffc1ffb;
  pDay = (unsigned char *) 0xfffc1ffc;
  pDate = (unsigned char *) 0xfffc1ffd;
  pMonth = (unsigned char *) 0xfffc1ffe;
  pYear = (unsigned char *) 0xfffc1fff;

  /* convert BCD to normal numbers */
  control = (unsigned int)*pControl;
/*
  second = (((unsigned int)*pSecond >> 4)*10) + 
		((((unsigned int)*pSecond << 28) >> 28));
  minute = (((unsigned int)*pMinute >> 4)*10) + 
		((((unsigned int)*pMinute << 28) >> 28));
  hour = (((unsigned int)*pHour >> 4)*10) + 
		((((unsigned int)*pHour << 28) >> 28));
*/
  second = tfp->second();
  minute = tfp->minute();
  hour = tfp->hour();
  day = (((unsigned int)*pDay >> 4)*10) + ((((unsigned int)*pDay << 28) >> 28));
  date = (((unsigned int)*pDate >> 4)*10) + 
	((((unsigned int)*pDate << 28) >> 28));
  month = (((unsigned int)*pMonth >> 4)*10) + 
	((((unsigned int)*pMonth << 28) >> 28));
  year = (((unsigned int)*pYear >> 4)*10) + 
	((((unsigned int)*pYear << 28) >> 28));

  /* now calculate the seconds of the year */
	
  /* first calculate how many seconds to the end of last month */
  /* use a switch statement without breaks, so the result will
     'fall-through' the other months adding as it goes. */
  switch (month) {
    case 12: julian += (30);	/* add Nov seconds */
    case 11: julian += (31);	/* add Oct seconds */
    case 10: julian += (30);	/* add Sep seconds */
    case  9: julian += (31);	/* add Aug seconds */
    case  8: julian += (31);	/* add Jul seconds */
    case  7: julian += (30);	/* add Jun seconds */
    case  6: julian += (31);	/* add May seconds */
    case  5: julian += (30);	/* add Apr seconds */
    case  4: julian += (31);	/* add Mar seconds */
    case  3: julian += (28);	/* add Feb seconds */
    case  2: julian += (31);	/* add Jan seconds */
    case  1: julian += ( 0);	/* no prior seconds*/
    }

  /* now add days to today */
  julian += date;

  /* now calculate seconds through last midnight */
  seconds_year = 3600 * 24 * (julian - 1);

  seconds_year += (3600 * hour);
  seconds_year += (60 * minute);
  seconds_year += (second);

  /* correct year to years AD (instead of years after 1900) */
  year += 1900;

  /* now load the structure */
  time_now.sec_o_yr = seconds_year;
  time_now.year     = year;
  time_now.month    = month;
  time_now.date     = date;
  time_now.day      = day;
  time_now.julian   = julian;
  time_now.hour     = hour;
  time_now.minute   = minute;
  time_now.second   = second;
  time_now.control  = control;
	
  /* now return the seconds of the year */
  return(seconds_year);
}

/* ####################################################################### */
void nav_stream()
/* ####################################################################### */
/*   Revision History:                                                     */
/*                      11/15/93 KSB created                               */
/*                      02/27/96 KSB added baud rate line                  */
/*                                                                         */
/*   This routine will watch the serial port for data, and record what it  */
/*          reads on disk.                                                 */
/*                                                                         */
/*   Returns:                                                              */
/*            Nothing                                                      */
/*                                                                         */
/* ####################################################################### */
{
  unsigned short dtas;
  unsigned char rx[512];
  int i,j;
  int k=0;
  int eflag;


  taskDelay(500);

  while (1)
    {
    /* should we exit? */
    if (param_map & 0x8000) {
      if (WARNING) printf("NAV_STREAM is exiting\n");
      return;
      }

    // zero the buffers
    for (i = 0; i < 512; i++)
      rx[i] = 0x0;

    // reset data flag
    k=0;
	
    if (FALCON) {
      // get a character to start off with (necessary to be compatible
      // with other aircraft software...DLR doesn't send sync char,
      read(navPort,(char*)&rx[k], 1);

      // first read in the data
      do
        {
// for debugging
//        if (WARNING) printf("%c", rx[k]);

        // increment the counter
        ++k;

	// read next byte
        read(navPort,(char*)&rx[k], 1);
	
        // check for escape from do loop
        if (k == 511)
          break;
        }
      while ( (rx[k-1] != 0x0d) && (rx[k] != 0x0a) );

      // should have read 186 characters
      // since there is no sync character, we need to check the length
      if (k == 185)
        {
	// copy data to saving structure (forget the cr/lf)
        bcopyBytes((char*)&rx[0], (char*)&fnav_dat, 184);
	
	// put our time in packet
        fnav_dat.our_time = time_now.sec_o_yr;
			
	// if user is not setting TAS, try to set it here
        if ((param_map & 0x2) == 0)
          {
          dtas = 0;
          eflag = 9;
          // 1 2 3 4 5 6 7 8 9 0 1 2 3 4
          // 0 1 2 3 4 5 6 7 8 9 0 1 2 3
          // + 9 . 9 9 9 9 9 9 e + 9 9 _

          if( (fnav_dat.tas[1] >= 0x30) && (fnav_dat.tas[1] <= 0x39) )
            dtas = 1000*(unsigned short)(fnav_dat.tas[1]-0x30);
			
          if( (fnav_dat.tas[3] >= 0x30) && (fnav_dat.tas[3] <= 0x39) )
            dtas += 100*(unsigned short)(fnav_dat.tas[3]-0x30);
			
          if( (fnav_dat.tas[4] >= 0x30) && (fnav_dat.tas[4] <= 0x39) )
            dtas += 10*(unsigned short)(fnav_dat.tas[4]-0x30);
			
          if( (fnav_dat.tas[5] >= 0x30) && (fnav_dat.tas[5] <= 0x39) )
            dtas += (unsigned short)(fnav_dat.tas[5]-0x30);

          if( (fnav_dat.tas[12] >= 0x30) && (fnav_dat.tas[12] <= 0x39) )
            eflag = (unsigned short)(fnav_dat.tas[12]-0x30);

          // convert the eformat data to tas in m/s
          if (eflag == 0) {
            dtas += 500;
            tas = dtas / 1000;
          } else if (eflag == 1) {
            dtas += 50;
            tas = dtas / 100;
          } else if (eflag == 2) {
            dtas += 5;
            tas = dtas / 10;
          } else {	// bad eflag?
            tas = 999;
          }

          // set last_tas
          last_tas = time_now.sec_o_yr;
          }
	
	// copy data to ping pong buffer
        bcopyWords((char*)&fnav_dat, (char*)ndNext, 94);
        nd_pkts++;
			
	// write data to disk
        if (nd_pkts == HK_BLOCK)
          {
          /* wait for semaphore */
          semTake(nd_disk, WAIT_FOREVER);
			
          taskSpawn("tSnd", das_pri+1, 0, 10000, (FUNCPTR)data_to_disk,
		(int)ndb[nd_buf], 188*HK_BLOCK,7,(int)&ND_info,0,0,0,0,0,0);
			
          /* go to next buffer for writing */
          nd_buf++;
          nd_buf %= 2;
          nd_pkts = 0;
          ndNext = ndb[nd_buf];
          }
        else
          ndNext += 94;
	
	// should we print some stuff out?
        if ((print_map & 0x80) == 0x80)
          {
          cls();

          printf("Flt. Time\n");
          printf("---- -----------------\n");
//		        ####_DD.MM.YY_HH:MM:SS.SEC_
//		        0123456789 123456789 123456
//		       (0-26)
          for (j = 0 ; j < 27 ; j++) printf("%c", rx[j]);
          printf("\x0d\x0a\x0a");

          printf("Latitude      Longitude     Pressure      Altitude\n");
          printf("------------- ------------- ------------- -------------\n");
//		        +2.456789E+23_+2.456789E+23_+2.456789E+23_+2.456789E+23_
//		        789 123456789 123456789 123456789 123456789 123456789 12
//		        (27-82)
          for (j = 27 ; j < 83 ; j++) printf("%c", rx[j]);
          printf("\x0d\x0a\x0a");

          printf("Temperature   TAS           Heading\n");
          printf("------------- ------------- -------------\n");
//		        +2.456789E+23_+2.456789E+23_+2.456789E+23_
//		        3456789 123456789 123456789 123456789 1234
//		        (83-124)
          for (j = 83 ; j < 125 ; j++) printf("%c", rx[j]);
          printf("\x0d\x0a\x0a");

          printf("Wind_Dir      Wind_Vel      Pitch         Roll          SUM\n");
          printf("------------- ------------- ------------- ------------- ---\n");
//	        +2.456789E+23_+2.456789E+23_+2.456789E+23_+2.456789E+23_123CL
//	        56789 123456789 123456789 123456789 123456789 123456789 12345
//	        (125-183)
          for (j = 125 ; j < 184 ; j++) printf("%c", rx[j]);
          printf("\x0d\x0a\x0a");

          } // printing
        }
      else
        { // bad length
	// flush the data port
        ioctl(navPort, FIOFLUSH, 0);

        if (WARNING) {
          printf("Bad NAV Data Length Received, k=%d\n",k);
//          for (j = 0 ; j < k ; j++) printf("%c", rx[j]);
          }
        }
      } // end if FALCON


	
    if (C130)
      {
      // get a character to start off with (necessary to be compatible
      // with other aircraft software...NCAR doesn't send sync char,
      read(navPort,(char*)&rx[k], 1);

      // first read in the data
      do
        {
// for debugging
// if (WARNING) printf("%c", rx[k]);
				
	// increment the counter
        ++k;

	// read next byte
        read(navPort,(char*)&rx[k], 1);
	
	// check for escape from do loop
        if (k == 511)
          break;
        }
      while ( (rx[k-1] != 0x0d) && (rx[k] != 0x0a) );

      // should have read 249 characters
      // since there is no sync character, we need to check the length
      if (k == 248)
        {
	// copy data to saving structure (forget the cr/lf)
        bcopyBytes((char*)&rx[0], (char*)&cnav_dat, 247);
	
	// put our time in packet
        cnav_dat.spare[0] = 0x23;
        cnav_dat.our_time = time_now.sec_o_yr;
			
	// if user is not setting TAS, try to set it here
        if ((param_map & 0x2) == 0)
          {
          dtas = 0;
          eflag = 9;
          // 1 2 3 4 5 6 7 8 9 0 1 2 3 4
          // 0 1 2 3 4 5 6 7 8 9 0 1 2 3
          // + 9 . 9 9 9 9 9 9 e + 9 9 _

          if( (cnav_dat.tas[1] >= 0x30) && (cnav_dat.tas[1] <= 0x39) )
            dtas = 1000*(unsigned short)(cnav_dat.tas[1]-0x30);
			
          if( (cnav_dat.tas[3] >= 0x30) && (cnav_dat.tas[3] <= 0x39) )
            dtas +=  100*(unsigned short)(cnav_dat.tas[3]-0x30);
			
          if( (cnav_dat.tas[4] >= 0x30) && (cnav_dat.tas[4] <= 0x39) )
            dtas +=  10*(unsigned short)(cnav_dat.tas[4]-0x30);
			
          if( (cnav_dat.tas[5] >= 0x30) && (cnav_dat.tas[5] <= 0x39) )
            dtas +=     (unsigned short)(cnav_dat.tas[5]-0x30);

          if( (cnav_dat.tas[12] >= 0x30) && (cnav_dat.tas[12] <= 0x39) )
            eflag =   (unsigned short)(cnav_dat.tas[12]-0x30);

          // convert the eformat data to tas in m/s
          if (eflag == 0) {
            dtas += 500;
            tas = dtas / 1000;
          } else if (eflag == 1) {
            dtas += 50;
            tas = dtas / 100;
          } else if (eflag == 2) {
            dtas += 5;
            tas = dtas / 10;
          } else {	// bad eflag?
            tas = 999;
            }

          // set last_tas
          last_tas = time_now.sec_o_yr;
          }

	// copy data to ping pong buffer
        bcopyWords((char*)&cnav_dat, (char*)ndNext, 126);
        nd_pkts++;
			
	// write data to disk
        if (nd_pkts == HK_BLOCK)
          {
          /* wait for semaphore */
          semTake(nd_disk, WAIT_FOREVER);
			
          taskSpawn("tSnd", das_pri+1, 0, 10000,
                    (FUNCPTR)data_to_disk, (int)ndb[nd_buf],
                    252*HK_BLOCK,7,(int)&ND_info,0,0,0,0,0,0);
			
          /* go to next buffer for writing */
          nd_buf++;
          nd_buf%=2;
          nd_pkts = 0;
          ndNext = ndb[nd_buf];
          }
        else
          ndNext += 126;
	
	// should we print some stuff out?
        if ((print_map & 0x80) == 0x80)
          {
          cls();

          printf("    Time     Latitude      Longitude     Pressure\n");
          printf("------------ ------------- ------------- -------------\n");
//                  ____hh mm ss +1.345678e+12 +1.345678e+12 +1.345678e+12
//                      0123456789 123456789 123456789 123456789 123456789 1

          printf("    ");
          for (j = 0 ; j < 51 ; j++) printf("%c", rx[j]);
          printf("\x0d\x0a\x0a");
		
          printf("Cabin_Press   Altitude      Temperature   Dew_Point\n");
          printf("------------- ------------- ------------- -------------\n");
//                  +1.345678e+12 +1.345678e+12 +1.345678e+12 +1.345678e+12
//                  123456789 123456789 123456789 123456789 123456789 1234567

          for (j = 51 ; j < 107 ; j++) printf("%c", rx[j]);
          printf("\x0d\x0a\x0a");
		
          printf("Wind_Vel      Wind_Dir      Vert_Wind     Mixing_Ratio\n");
          printf("------------- ------------- ------------- -------------\n");
//                  +1.345678e+12 +1.345678e+12 +1.345678e+12 +1.345678e+12
//                  789 123456789 123456789 123456789 123456789 123456789 123

          for (j = 107 ; j < 163 ; j++) printf("%c", rx[j]);
          printf("\x0d\x0a\x0a");
		
          printf("Heading       TAS           Surface_Temp  Pitch\n");
          printf("------------- ------------- ------------- -------------\n");
//                  +1.345678e+12 +1.345678e+12 +1.345678e+12 +1.345678e+12
//                  3456789 123456789 123456789 123456789 123456789 123456789

          for (j = 163 ; j < 219 ; j++) printf("%c", rx[j]);
          printf("\x0d\x0a\x0a");
		
          printf("Roll          L.W.C.\n");
          printf("------------- -------------\n");
//                  +1.345678e+12 +1.345678e+12 cl
//                  9 123456789 123456789 12345678

          for (j = 219 ; j < 249 ; j++) printf("%c", rx[j]);
          } // printing
        }
      else
        { // bad length
        ioctl(navPort, FIOFLUSH, 0);

        if (WARNING) {
          printf("Bad NAV Data Length Received, k=%d\n",k);
          //for (j = 0 ; j < k ; j++) printf("%c", rx[j]);
          }
        }
      } // end if C130


    if (DC8)
      {
      // sync on the letter "C"
      do
        {
        read(navPort,(char*)&rx[0], 1);
        // if (WARNING) printf("%c 0x%x\n", rx[0],rx[0]); 
        }
      while (rx[0] != 0x43);

      // read 7 CR/LFs (end of 7th line of data)
      for (i = 0; i < 7; i++)
        {
        do
          {
          // increment the counter
          ++k;
	
          // read next bytes
          read(navPort,(char*)&rx[k], 1);
	
          // check for escape from do loop
          if (k == 511)
            break;
          }
        while ( (rx[k-1] != 0x0d) && (rx[k] != 0x0a) );
        }
	
      // print the data
      if ((print_map & 0x80) == 0x80)
        {
	cls();
	printf("  Day Time         Latitude Longitude Pitch Roll   WS\n");
	printf("  ---v------------v--------v---------v-----v------v---\n");
	//      C aaa bb:bb:bb.bbb +cc cc.c +ddd dd.d eee.e ffff.f ggg
	for (j = 0 ; j < 56 ; j++) printf("%c", rx[j]);

	printf("  WD  TAS GS   HDG   DANG  PAlt  RAlt  DFPTG   DFPTE\n");
	printf("  ---v---v----v-----v-----v-----v-----v-------v------- \n");
	//      D hhh iii jjjj kkk.k lll.l mmmmm nnnnn ooooo.o ppppp.p
	for (j = 56; j < 112; j++) printf("%c", rx[j]);

	printf("  TStat TTTL  TIR   TCALC CAS VertSp DME_D   DME_T  ST\n");
	printf("  -----v-----v-----v-----v---v------v-------v------v--\n");
	//      E qqq.q rrr.r sss.s ttt.t uuu vvvvvv wwwww.w xxxx.x yy
	for (j = 112; j < 168; j++) printf("%c", rx[j]);

	printf("  Cabin Press  Mach  CT_D    OBS    TAErr  TA    SpHum\n");
	printf("  -----v------v-----v-------v------v------v-----v-----\n");
	//      F zzzzz JJJJ.J K.KKK LLLLL.L MMMM.M NNNN.N OOO.O P.PPP
	for (j = 168; j < 224; j++) printf("%c", rx[j]);
		
	printf("  PH20 RH_I RH_W SVP_W SVP_I SE_G  SE_A  SA_G  SA_A\n");
	printf("  ----v----v----v-----v-----v-----v-----v-----v------\n");
	//      G QQ.Q RR.R SS.S TT.TT UU.UU VVV.V WWW.W XXX.X YYYY.Y
	for (j = 224; j < 280; j++) printf("%c", rx[j]);
		
	printf("  GPS Time     Latitude Longitude Alt   Theta\n");
	printf("  ------------v--------v---------v-----v-----\n");
	//      H aa:aa:aa.aaa +bb bb.b +ccc cc.c ddddd eee.e
	for (j = 280; j < 336; j++) printf("%c", rx[j]);
		
	printf("  ME_G  ME_A  MA_G   MA_A  W Way_Lat  Way_Lon\n");
	printf("  -----v-----v-----v------v-v--------v---------\n");
	//      I eee.e fff.f ggg.g hhhh.h i +jj jj.j +kkk kk.k
	for (j = 336; j < 392; j++) printf("%c", rx[j]);
        }
	
      // copy data to saving structure
      bcopyBytes((char*)&rx[0], (char*)&dnav_dat, 392);
	
      // put our time in packet
      dnav_dat.our_time = time_now.sec_o_yr;
	
      // if user is not setting TAS, try to set it here
      if ((param_map & 0x2) == 0)
        {
        // using characters 63, 65, and 65 for TAS
        dtas = 0;
        if ( (rx[62] >= 0x30) && (rx[62] <= 0x39) )
          dtas = 100*(unsigned short)(rx[62]-0x30);
	
        if ( (rx[63] >= 0x30) && (rx[63] <= 0x39) )
          dtas += 10*(unsigned short)(rx[63]-0x30);
	
        if ( (rx[64] >= 0x30) && (rx[64] <= 0x39) )
          dtas +=    (unsigned short)(rx[64]-0x30);
	
        // convert knots to m/s
        tas = ((dtas*5144)+5000)/10000;
	
        // set last_tas
        last_tas = time_now.sec_o_yr;
        }

      // copy data to ping pong buffer
      bcopyWords((char*)&dnav_dat, (char*)ndNext, 198);
      nd_pkts++;
	
      // write data to disk
      if (nd_pkts == HK_BLOCK) {
        /* wait for semaphore */
        semTake(nd_disk, WAIT_FOREVER);
	
        taskSpawn("tSnd", das_pri+1, 0, 10000,
                  (FUNCPTR)data_to_disk, (int)ndb[nd_buf],
                  198*2*HK_BLOCK,7,(int)&ND_info,0,0,0,0,0,0);
	
        /* go to next buffer for writing */
        nd_buf++;
        nd_buf%=2;
        nd_pkts = 0;
        ndNext = ndb[nd_buf];
      } else {
        ndNext += 198;
        }
	
      } // end of if DC8

    if (ER2) {
      // get a character to start off with (necessary to be compatible
      // with other aircraft software...ER-2 doesn't send sync char,
      k=0;

      // fill the navDat buffer with spaces
      for(j = 0; j < 511; j++ )
        enav_dat.navDat[ j ] = 0x20;

      // wait for a SOH character
      do
        {
        read(navPort,(char*)&enav_dat.navDat[ k ], 1);
        }
      while (enav_dat.navDat[k] != 0x01);

      // first read in the data
      do 
        {
// for debugging
// printf( "%c", enav_dat.navDat[k] );

        // increment the counter
        ++k;

	// read next byte
        read(navPort,(char*)&enav_dat.navDat[k], 1);
	
	// check for escape from do loop
        if (k == 510) break;

        if ( (enav_dat.navDat[k] == 0x01) && (WARNING) )
          printf("*** Mid record SOH %d ***\n",k);
        }
      while ((enav_dat.navDat[k] != 0x0d) && (enav_dat.navDat[k] != 0x0a));

      // put our time in packet
      sprintf( (char*)enav_dat.ourSeconds, "%08u", time_now.sec_o_yr );

      // replace null from above sprintf with a space
      enav_dat.ourSeconds[ 8 ] = 0x20;
	
      // put a tail on the packet
      enav_dat.tail[ 0 ] = 0x0d;
      enav_dat.tail[ 1 ] = 0x0a;

      // copy data to ping pong buffer
      bcopyWords((char*)&enav_dat, (char*)ndNext, 261);
      nd_pkts++;
			
      // write data to disk
      if (nd_pkts == HK_BLOCK) { /* wait for semaphore */
        semTake(nd_disk, WAIT_FOREVER);
			
        taskSpawn("tSnd", das_pri+1, 0, 10000,
                  (FUNCPTR)data_to_disk, (int)ndb[nd_buf],
                  522*HK_BLOCK,7,(int)&ND_info,0,0,0,0,0,0);
			
	/* go to next buffer for writing */
        nd_buf++;
        nd_buf%=2;
        nd_pkts = 0;
        ndNext = ndb[nd_buf];
      } else
        ndNext += 261;

      // should we print some stuff out?
      if ((print_map & 0x80) == 0x80) {
        cls();
        printf( "DAY:HH:MM:SS Latitude  Longitude  Heading\n" );
	//       XXX:XX:XX:XX NXX.XXXXX WXXX.XXXXX XXX.XX
        for(j = 3; j < 44; j++) printf( "%c", enav_dat.navDat[j]);
        }

      } /* end of ER2 loop */
    } /* end of forever loop */

} /* end of nav_stream */


/* ################################################################ */
int data_to_disk(char * data,
                 long int num_bytes, 
                 unsigned short file_type, 
                 struct FILE_DATA * file_data)
/* ################################################################ */
/*     Revision History:                                            */
/*                         10/12/93 KSB  created                    */
/*                         11/16/93 KSB  major rev.--generalized    */
/*                                                                  */
/*     This routine will send any type of data to disk.             */
/*                                                                  */
/*     Returns:                                                     */
/*                0  successful completion                          */
/*               -1  error opening data file                        */
/*               -2  error opening next file                        */
/*               -3  unknown data type                              */
/*                                                                  */
/* ################################################################ */
{
  int next_fd;
  unsigned short next_num;
  int bw;

  char next_file[12], chnum[4];

  if (C130)
    {
    if (file_type == 1) semGive(mh_disk);
    if (file_type == 2) semGive(md_disk);
    if (file_type == 6) semGive(dh_disk);
    if (file_type == 7) semGive(nd_disk);

    return(0);	// Turn off local disk recording for C130.
    }

  /* update time in DOS Structures */
  dosFsDateSet(time_now.year, time_now.month, time_now.date);
  dosFsTimeSet(time_now.hour, time_now.minute, time_now.second);

  /* see if data file is already open...if not, open it */
  if (file_data->pcnt == 0 ) {	/* if zero, file isn't open */

    /* get next_file name setup: DOSC:leaderNEXT  (error files on C) */
    sprintf(next_file, "DOSC:%2sNEXT", file_data->leader);

    if ((next_fd = open(next_file, O_RDONLY, 0666)) < 0) {
      /* next_file doesn't exist */
      next_num = 1; 
      }
    else
      {
      /* next_file does exist */
      read(next_fd, chnum, 4);
      next_num = (unsigned short)strtol(chnum,(char**)NULL,10);

      if (next_num > 999)
        if (WARNING)
          printf("next_num = %u\n",next_num);
      }

    /* close next_file */
    close(next_fd);

    /* create name of data file: drive,leader,next_num */
    sprintf(file_data->filename,"%8s%2s%u",
            file_data->drive, file_data->leader, next_num);
    next_num += 1;	/* increment next_num */

    /* now load the character string for next file */
    sprintf(chnum,"%3u", next_num);

    /* now save the file number in the file structure */
    file_data->filenum = next_num-1;

    if (file_data->max_files >= file_data->filenum) {

      /* create and open the new data_file */
      if ( (file_data->file_id = 
            creat(file_data->filename, O_WRONLY)) < 0) {
        if (WARNING) perror("opening data file");
        if (WARNING) printf("opening %s\n",file_data->filename);
        return(-1);
      } else {	/* we have a good file, close it for now */
        close(file_data->file_id);
        }

      /* now remove the old next_file */
      remove(next_file);

      /* create and open next_file for writing */
      if ( (next_fd = creat(next_file, O_WRONLY)) < 0) {
        if (WARNING) perror("opening next file");
        return(-2);
        }

      /* now write new next file number to next_file */
      write(next_fd, chnum, 4);
		
      /* now close next_file */
      close(next_fd);
      }
    }


  /* now write data to file */

  if (file_data->max_files >= file_data->filenum)
    {
    file_data->file_id = open(file_data->filename, O_RDWR, 0777);
    lseek(file_data->file_id, 0, SEEK_END);
    if ( (bw=write(file_data->file_id, data, num_bytes)) != num_bytes)
      {
      if (WARNING)
        printf("WRITE ERROR: num_bytes=%d, written=%d\n", num_bytes,bw);

      bad_save++;
      }
    else
      bad_save = 0;

    close(file_data->file_id);

    file_data->pcnt++;	/* increment packet count */

    /* if we have a full file close it */
    if ( file_data->pcnt >= file_data->max_count )
      file_data->pcnt = 0;	/* reset packet count */
    }

  if (file_type == 1) semGive(mh_disk);
  if (file_type == 2) semGive(md_disk);
  if (file_type == 6) semGive(dh_disk);
  if (file_type == 7) semGive(nd_disk);

  return(0);

} /* END DATA_TO_DISK */


/* ################################################################ */
void dead_das()
/* ################################################################ */
/*     Revision History:                                            */
/*                         12/07/93 KSB  created                    */
/*                                                                  */
/*     This routine will only run when the watchdog timer expires   */
/*                                                                  */
/*     Returns:                                                     */
/*              Nothing                                             */
/*                                                                  */
/* ################################################################ */
{
  wdCancel(death_wid);
  /* try to get stuff stopped */
  param_map = 0x8000;

  /* lets close the files if we can */
  cls_files();

  /* tell user if we can */
  printf("**** DAS SEEMS DEAD!!!! ****\n");
  if ((data_cycle%5 !=0)&&(data_cycle < 50))
    {
    taskDelay(500);
//    reboot(BOOT_CLEAR);
    }
  else
    exit(0);

}

/* ################################################################ */
void data_to_screen( char *pData,
                     int size,
                     int type)
/* ################################################################ */
/*     Revision History:                                            */
/*                         12/10/93 KSB  created                    */
/*                         12/13/93 KSB  generalized for all types  */
/*                                                                  */
/*     This routine prints data to an ascii terminal                */
/*                                                                  */
/*     Returns:                                                     */
/*               Nothing                                            */
/*                                                                  */
/* ################################################################ */
{
  unsigned short *pStart;
  unsigned short *pUsi;
  int i,j;
  int lines;

  /* home the cursor */
  if (type < 4)
    cls();

  /* save the starting address */
  pStart = (unsigned short*)pData;
  pUsi = pStart;

  /* now choose the type of output based on data type */
  switch (type)
    {
    /* MASP Housekeeping */
    case 1:
      PrintMaspHk((struct MASP_HK_PKT *)pData);
      break;

    /* MASP Particle Data */
    case 2:
      PrintMaspPd((struct MASP_PART_PKT *)pData);
      break;

    /* Hex-Dump */
    case 5:
      lines = size / 8;			/* how many lines? */
      if (size % 8 > 0)
        lines++;

      for (j = 0; j < lines; j++) {
        for(i = 0; i < 8; i++)
          printf("%4x ", *pUsi++);

        printf("\n");
        }

      break;
  }

  /* done with data, lets clear the buffer */
  *pStart = 0x0;	
	
}

/* ################################################################ */
void cls_files()
/* ################################################################ */
/*     Revision History:                                            */
/*                         12/13/93 KSB  created                    */
/*                                                                  */
/*     This routine closes any open data files                      */
/*                                                                  */
/*     Returns:                                                     */
/*               Nothing                                            */
/*                                                                  */
/* ################################################################ */
{
  if (WARNING) printf("closing files...");

  /* close the data files */
  ioctl(MD_info.file_id,FIOFLUSH,0);
  ioctl(MH_info.file_id,FIOFLUSH,0);
  ioctl(ND_info.file_id,FIOFLUSH,0);
  ioctl(DH_info.file_id,FIOFLUSH,0);
	
  if (WARNING) printf("closed\n");

  return;
}


/* ################################################################ */
void print_pack( int p_size,
                 int rfindx,
                 unsigned short p_type,
                 unsigned long m_time,
                 unsigned short ftime,
                 int pcnt,
                 int read,
                 int calc,
                 int bad)
/* ################################################################ */
/*     Revision History:                                            */
/*                         12/17/93 KSB  created                    */
/*                                                                  */
/*     This routine prints packet data                              */
/*                                                                  */
/*     Returns:                                                     */
/*               Nothing                                            */
/*                                                                  */
/* ################################################################ */
{
  printf("rd=%d,cl=%d,bd=%d,ty=%u,sz=%i,mtime=%u,ftime=%u,pkt=%d,secs=%u\n",
         read,calc,bad,p_type,p_size,m_time, ftime, pcnt, time_now.sec_o_yr);

  taskDelay(2);
  pack_ex = 0x0;
  return;
}


/* ################################################################ */
unsigned short checksum(unsigned short *data, unsigned short size)
/* ################################################################ */
/*     Revision History:                                            */
/*                         01/19/94 KSB created                     */
/*                                                                  */
/*     This routine calculates the checksum on a packet             */
/*                                                                  */
/*     Returns:                                                     */
/*               The calculated checksum                            */
/*                                                                  */
/* ################################################################ */
{
  unsigned short cksum = 0;
  int i;

  for (i = 0; i < (size - 1); i++)
    cksum += *data++;

  return(cksum);

}


/* ################################################################ */
void pulse_now()
/* ################################################################ */
/*     Revision History:                                            */
/*                         02/05/94 KSB created                     */
/*                                                                  */
/*     This routine will toggle the 16Hz pulse on the MASP          */
/*                                                                  */
/*     Returns:                                                     */
/*               Nothing                                            */
/*                                                                  */
/* ################################################################ */
{
  struct MODE_PACKET hz_mode;
  int i;

  /* initialize the packet */
  hz_mode.sync_flag = 0x4450;	
  hz_mode.length = 0x13;
  hz_mode.type = 0x4d44;

  /* set the bitmapped mode word 0x2 is pulses on */
  if (param_map & 0x10) {
    hz_mode.mode = 0x1a;	
  } else {
    hz_mode.mode = 0x18;
  }

  hz_mode.level[0] = MODE_LEVEL_1;
  hz_mode.level[1] = MODE_LEVEL_2;
  hz_mode.level[2] = MODE_LEVEL_3;	/* laser Amplitudes */
  hz_mode.width[0] = MODE_WIDTH_1;
  hz_mode.width[1] = MODE_WIDTH_2;
  hz_mode.width[2] = MODE_WIDTH_3;	/* Pulse Durations */

  hz_mode.ki = KI;
  hz_mode.kp = KP;

  hz_mode.trig_thresh = thresh;

  hz_mode.max_pkts = max_masp_pkts;
  hz_mode.min_ttime = minTtime;

  hz_mode.maGain = maGain;
  hz_mode.tsGain = tsGain;

  hz_mode.tas = 200;

  hz_mode.cksum = checksum((unsigned short *)&hz_mode, hz_mode.length);

  /* we enter the routine with the mode packet set for calibrate */
  /* so all we have to do is send it */
  for (i = 0; dpr->cmd_stat > 0 && i < 10; ++i) {
    das_hk.b_calb++;
    taskDelay(2);
    }

  if (dpr->cmd_stat == 0) {
    bcopyWords((char *)&hz_mode,(char *)&dpr->txb, hz_mode.length);
    dpr->dsp_irq = 3;
    }

  /* count it */
// ++modeSentCount %= 256;

}


/* ################################################################ */
void cls()
/* ################################################################ */
/*     Revision History:                                            */
/*                         03/03/94 KSB created                     */
/*                                                                  */
/*     This routine puts the cursur at the home position            */
/*                                                                  */
/*     Returns:                                                     */
/*               Nothing                                            */
/*                                                                  */
/* ################################################################ */
{
  printf("\E[H");
}


/* ################################################################ */
void PrintMaspHk (struct MASP_HK_PKT *hkpkt)
/* ################################################################ */
/*      Revision History:                                           */
/*                          03/04/94 KSB created                    */
/*                                                                  */
/*      This routine prints a packet of MASP housekeeping to the    */
/*          serial port.                                            */
/*                                                                  */
/*      Returns:                                                    */
/*                Nothing                                           */
/*                                                                  */
/* ################################################################ */
{
  struct CNV_TIME convtime;

  /* get the time converted */
  ConvertTime(hkpkt->time, &convtime);

  /* print data */
  printf("MASP HOUSEKEEPING    Mode Count:%3i           %3i    %02i/%02i/%4i  %02i:%02i:%02i\n",
         (hkpkt->probeMode >> 8) & 0xff ,
         convtime.julian,
         convtime.month,
         convtime.day,
         convtime.year,
         convtime.hh,
         convtime.mm,
         convtime.ss);
  printf("Miscellaneous ------------------------------------------------------------\n");
  printf(" (Lost Part): %5u  (Diff Pr): %5i  (Abs Pr): %5i  (M laser I): %5i\n",
         hkpkt->missed,
         hkpkt->pDiff,
         hkpkt->pAbs,
         hkpkt->mlPower);
  printf("Voltages -----------------------------------------------------------------\n");
  printf(" (+28): %3i.%02u    (+12): %3i.%02u    (+05): %3i.%02u   (-12): %3i.%02u\n",
         hkpkt->psP28*30/2048, (abs(hkpkt->psP28*30*100/2048))%100,
         hkpkt->psP12*15/2048, (abs(hkpkt->psP12*15*100/2048))%100,
         hkpkt->psP5*6/2048, (abs(hkpkt->psP5*6*100/2048))%100,
         hkpkt->psM12*15/2048, (hkpkt->psM12*15*100/2048)%100);
  printf(" (+5 Anlg): %3i.%02u    (-5 Anlg): %3i.%02u\n",
         hkpkt->apsP5*6/2048, (abs(hkpkt->apsP5*6*100/2048))%100,
         hkpkt->apsM5*6/2048, (abs(hkpkt->apsM5*6*100/2048))%100);
  printf("Quad Detectors -----------------------------------------------------------\n");
  printf(" (Q1): %5i  (Q2): %5i  (Q3): %5i  (Q4): %5i  (Total): %5i\n",
         hkpkt->q1uAmp,
         hkpkt->q2uAmp,
         hkpkt->q3uAmp,
         hkpkt->q4uAmp,
         hkpkt->q1uAmp+hkpkt->q2uAmp+hkpkt->q3uAmp+hkpkt->q4uAmp);
  printf("Misc ---------------------------------------------------------------------\n");
  printf(" (Mode): %5i  (Htr): %5i  (Trig): %5i  (VCA): %5i  (CalP): %5i\n",
         hkpkt->probeMode,
         hkpkt->dspHeat,
         hkpkt->dac1Trigger,
         hkpkt->dac2VCAGain,
         hkpkt->dac3CalAmp);
  printf(" (TS TEC): %5i  (MA TEC): %5i  (FS TEC): %5i  (BS TEC): %5i\n",
         hkpkt->dac4TSTec,
         hkpkt->dac5MATec,
         hkpkt->dac6FSTec,
         hkpkt->dac7BSTec);
  printf("Baseline Voltages --------------------------------------------------------\n");
  printf(" (TS A): %1i.%03u  (MA A): %1i.%03u  (FS A): %1i.%03u  (BS A): %1i.%03u\n",
         hkpkt->tsDCVA*3/2048, (abs(hkpkt->tsDCVA*3*1000/2048))%1000,
         hkpkt->maDCVA*3/2048, (abs(hkpkt->maDCVA*3*1000/2048))%1000,
         hkpkt->fsDCVA*3/2048, (abs(hkpkt->fsDCVA*3*1000/2048))%1000,
         hkpkt->bsDCVA*3/2048, (abs(hkpkt->bsDCVA*3*1000/2048))%1000);
  printf(" (TS B): %1i.%03u  (MA B): %1i.%03u  (FS B): %1i.%03u  (BS B): %1i.%03u\n",
         hkpkt->tsDCVB*3/2048, (abs(hkpkt->tsDCVB*3*1000/2048))%1000,
         hkpkt->maDCVB*3/2048, (abs(hkpkt->maDCVB*3*1000/2048))%1000,
         hkpkt->fsDCVB*3/2048, (abs(hkpkt->fsDCVB*3*1000/2048))%1000,
         hkpkt->bsDCVB*3/2048, (abs(hkpkt->bsDCVB*3*1000/2048))%1000);
  printf("Temperatures -------------------------------------------------------------\n");
  printf(" (QUAD ):%5i\n",
         hkpkt->qTemp);
  printf(" (M Las):%5i  (C Las):%5i  (AbsP):%5i  (DiffP):%5i  (PwrS):%5i\n",
         hkpkt->mlTemp,
         hkpkt->clTemp,
         hkpkt->absTemp,
         hkpkt->diffTemp,
         hkpkt->psTemp);
  printf(" (FOBen):%5i  (AOBen):%5i  (A Can):%5i  (F Can):%5i\n",
         hkpkt->fobTemp,
         hkpkt->aobTemp,
         hkpkt->acanTemp,
         hkpkt->fcanTemp);
  printf(" (DSP):%5i  (PCC):%5i  (SIC):%5i  (SC):%5i  (HKC):%5i\n",
         hkpkt->dspcTemp,
         hkpkt->pccTemp,
         hkpkt->sicTemp,
         hkpkt->scTemp,
         hkpkt->hkcTemp);
  printf(" (TS A):%5i  (MA A):%5i  (FS A):%5i  (BS A):%5i\n",
         hkpkt->tsTempA,
         hkpkt->maTempA,
         hkpkt->fsTempA,
         hkpkt->bsTempA);
  printf(" (TS B):%5i  (MA B):%5i  (FS B):%5i  (BS B):%5i\n",
         hkpkt->tsTempB,
         hkpkt->maTempB,
         hkpkt->fsTempB,
         hkpkt->bsTempB);
  printf(" (TS C):%5i  (MA C):%5i  (FS C):%5i  (BS C):%5i\n",
         hkpkt->tsTempC,
         hkpkt->maTempC,
         hkpkt->fsTempC,
         hkpkt->bsTempC);
  printf(" (TS D):%5i  (MA D):%5i  (FS D):%5i  (BS D):%5i\n",
         hkpkt->tsTempD,
         hkpkt->maTempD,
         hkpkt->fsTempD,
         hkpkt->bsTempD);
}


/* ################################################################ */
void ConvertTime (long seconds, struct CNV_TIME *time)
/* ################################################################ */
/*      Revision History:                                           */
/*                          03/04/94 KSB created                    */
/*                                                                  */
/*      This routine converts seconds of the year to normal time    */
/*                                                                  */
/*      Returns:                                                    */
/*                Nothing                                           */
/*                                                                  */
/* ################################################################ */
{
  unsigned short d_sum = 0;
  unsigned short int mn[12] = { 31,28,31,30,31,30,31,31,30,31,30,31};
  unsigned int i;

  /* unpack the time */
  time->julian = (seconds/86400) + 1;
  time->gmts = seconds % 86400;
  time->hh = time->gmts / 3600;
  time->mm = (time->gmts % 3600) / 60;
  time->ss = (time->gmts % 3600) % 60;

  /* hardwired...sorry */
  time->year = 1995;
	
  /* adjust for leap years */
  if ((time->year % 4) == 0)
    mn[1] = 29;

  /* convert julian to something useful */
  for (i = 0; i < 12; i++)
    {
    d_sum += mn[i];
    if (d_sum >= time->julian)
      {
      time->month = i + 1;
      time->day = time->julian - (d_sum - mn[i]);
      i=11;
      }
    }
	
}

/* ################################################################ */
void PrintMaspPd (struct MASP_PART_PKT *data)
/* ################################################################ */
/*      Revision History:                                           */
/*                          03/05/94 KSB created                    */
/*                                                                  */
/*      This routine prints the first 15 channels of a MASP PD      */
/*           packet, and displays the average values for all 144    */
/*                                                                  */
/*      Returns:                                                    */
/*                Nothing                                           */
/*                                                                  */
/* ################################################################ */
{
  struct CNV_TIME time;
  struct UNPACKED_MASP unpkpart[144];
  struct UNPACKED_MASP avgpart;
  struct L_UNPACKED_MASP sumpart;

  unsigned short LowGain, MidGain, HighGain;
  unsigned short TMLow, TMMid, TMHigh;
  unsigned short TMLowDec, TMMidDec, TMHighDec;
  int i;

  /* get the time into something easier to use */
  ConvertTime( data->time, &time);

  /* zero the average structure */
  sumpart.sptt = 0;
  sumpart.spat = 0;
  sumpart.tsLg = 0;
  sumpart.tsMg = 0;
  sumpart.tsHg = 0;
  sumpart.maLg = 0;
  sumpart.maMg = 0;
  sumpart.maHg = 0;
  sumpart.fsLg = 0;
  sumpart.fsHg = 0;
  sumpart.bsLg = 0;
  sumpart.bsHg = 0;
  sumpart.ratioLg = 0;
  sumpart.ratioHg = 0;

  /* unpack the masp data */
  for (i = 0; i < 144; i++)
    {
    /* mask bit 15 */
    unpkpart[i].sptt = data->part[i].ttimer & 0x7fff;

    unpkpart[i].spat = data->part[i].itimer;

    /* three to one compression */
    unpak31(data->part[i].total, &LowGain, &MidGain, &HighGain);
    unpkpart[i].tsLg = LowGain;
    unpkpart[i].tsMg = MidGain;
    unpkpart[i].tsHg = HighGain;

    /* three to one compression */
    unpak31(data->part[i].masked, &LowGain, &MidGain, &HighGain);
    unpkpart[i].maLg = LowGain;
    unpkpart[i].maMg = MidGain;
    unpkpart[i].maHg = HighGain;

    /* two to one compression */
    unpak21(data->part[i].forward, &LowGain, &HighGain);
    unpkpart[i].fsLg = LowGain;
    unpkpart[i].fsHg = HighGain;

    /* two to one compression */
    unpak21(data->part[i].backward, &LowGain, &HighGain);
    unpkpart[i].bsLg = LowGain;
    unpkpart[i].bsHg = HighGain;

    /* two to one compression */
    unpak21(data->part[i].bfratio, &LowGain, &HighGain);
    unpkpart[i].ratioLg = LowGain;
    unpkpart[i].ratioHg = HighGain;

    /* increment the average structure */
    sumpart.sptt += unpkpart[i].sptt;
    sumpart.spat += unpkpart[i].spat;
    sumpart.tsLg += unpkpart[i].tsLg;
    sumpart.tsMg += unpkpart[i].tsMg;
    sumpart.tsHg += unpkpart[i].tsHg;
    sumpart.maLg += unpkpart[i].maLg;
    sumpart.maMg += unpkpart[i].maMg;
    sumpart.maHg += unpkpart[i].maHg;
    sumpart.fsLg += unpkpart[i].fsLg;
    sumpart.fsHg += unpkpart[i].fsHg;
    sumpart.bsLg += unpkpart[i].bsLg;
    sumpart.bsHg += unpkpart[i].bsHg;
    sumpart.ratioLg += unpkpart[i].ratioLg;
    sumpart.ratioHg += unpkpart[i].ratioHg;
    }

  /* average the masp data */
  avgpart.sptt = sumpart.sptt / 144;
  avgpart.spat = sumpart.spat / 144;
  avgpart.tsLg = sumpart.tsLg / 144;
  avgpart.tsMg = sumpart.tsMg / 144;
  avgpart.tsHg = sumpart.tsHg / 144;
  avgpart.maLg = sumpart.maLg / 144;
  avgpart.maMg = sumpart.maMg / 144;
  avgpart.maHg = sumpart.maHg / 144;
  avgpart.fsLg = sumpart.fsLg / 144;
  avgpart.fsHg = sumpart.fsHg / 144;
  avgpart.bsLg = sumpart.bsLg / 144;
  avgpart.bsHg = sumpart.bsHg / 144;
  avgpart.ratioLg = sumpart.ratioLg / 144;
  avgpart.ratioHg = sumpart.ratioHg / 144;

  /* calculate the ratios */
  if (avgpart.maLg > 0) {
    TMLow = avgpart.tsLg / avgpart.maLg;
    TMLowDec = ((avgpart.tsLg*1000) / avgpart.maLg) % 1000;
  } else {
    TMLow = 9999;
    TMLowDec = 999;
  }
  if (avgpart.maMg > 0) {
    TMMid = avgpart.tsMg / avgpart.maMg;
    TMMidDec = ((avgpart.tsMg*1000) / avgpart.maMg) % 1000;
  } else {
    TMMid = 9999;
    TMMidDec = 999;
  }
  if (avgpart.maHg > 0) {
    TMHigh = avgpart.tsHg / avgpart.maHg;
    TMHighDec = ((avgpart.tsHg*1000) / avgpart.maHg) % 1000;
  } else {
    TMHigh = 9999;
    TMHighDec = 999;
  }


  /* print the data */
  printf("MASP PARTICLE DATA  (Rej): %5u (DOF): %5u    %3u    %2u/%2u/%4u  %2u:%2u:%2u\n",
         data->rejected,
         data->dofReject,
         time.julian,
         time.month,
         time.day,
         time.year,
         time.hh,
         time.mm,
         time.ss);
  printf("Miscellaneous --------------------------------------------------------------\n");
  printf("     (Fract Time): %5u  (VCA Gain): %4u  (ADC Offset): %4u  (Status): %1u \n",
         data->fract,
         data->gain,
         data->adc,
         data->status);
  printf("----------------------------------------------------------------------------\n");
  printf("     Average Total/Masked  (Low): %4u.%3u (Mid): %4u.%3u  (High): %4u.%3u\n",
         TMLow, TMLowDec,
         TMMid, TMMidDec,
         TMHigh, TMHighDec);
  printf("----------------------------------------------------------------------------\n");
  printf("Trnst Arrvl   Total Scatter   Masked_Signal    Forward   Backward      Ratio\n");
  printf(" Time  Time   Low  Mid High   Low  Mid High   Low High   Low High   Low High\n");
  printf("----------------------------------------------------------------------------\n");
  printf("%5u %5u  %4u %4u %4u  %4u %4u %4u  %4u %4u  %4u %4u  %4u %4u\n",
         avgpart.sptt,
         avgpart.spat,
         avgpart.tsLg, avgpart.tsMg, avgpart.tsHg,
         avgpart.maLg, avgpart.maMg, avgpart.maHg,
         avgpart.fsLg, avgpart.fsHg,
         avgpart.bsLg, avgpart.bsHg,
         avgpart.ratioLg, avgpart.ratioHg);
  printf("----------------------------------------------------------------------------\n");
  for (i = 0 ; i < 12 ; i++) {
    printf("%5u %5u  %4u %4u %4u  %4u %4u %4u  %4u %4u  %4u %4u  %4u %4u\n",
           unpkpart[i].sptt,
           unpkpart[i].spat,
           unpkpart[i].tsLg, unpkpart[i].tsMg, unpkpart[i].tsHg,
           unpkpart[i].maLg, unpkpart[i].maMg, unpkpart[i].maHg,
           unpkpart[i].fsLg, unpkpart[i].fsHg,
           unpkpart[i].bsLg, unpkpart[i].bsHg,
           unpkpart[i].ratioLg, unpkpart[i].ratioHg);
  }

}

/* ################################################################ */
void unpak31 (unsigned short signal, 
              unsigned short *low,
              unsigned short *mid,
              unsigned short *high)
/* ################################################################ */
/*      Revision History:                                           */
/*                          03/07/94 KSB created                    */
/*                                                                  */
/*      This routine unpacks three to one compressed data           */
/*                                                                  */
/*      Returns:                                                    */
/*                Nothing                                           */
/*                                                                  */
/* ################################################################ */
{
  /* declare and calculate the range */
  unsigned short range = (signal & 0xc000) >> 14;

  /* zero the signals */
  *low = 0;
  *mid = 0;
  *high = 0;

  /* do the decompression */
  switch (range) {
    case 0:
      *high = (unsigned short) ((signal & 0x01ff) << 3);
      *mid  = (unsigned short) ((signal & 0x3e00) >> 6);
      break;
    case 1:
      *mid  = (unsigned short) ((signal & 0x03ff) << 2);
      *low  = (unsigned short) ((signal & 0x3c00) >> 6);
      break;
    case 2:
      *low  = (unsigned short) (signal & 0x0fff);
      break;
  }
}

/* ################################################################ */
void unpak21 (unsigned short signal, 
              unsigned short *low,
              unsigned short *high)
/* ################################################################ */
/*      Revision History:                                           */
/*                          03/07/94 KSB created                    */
/*                                                                  */
/*      This routine unpacks two to one compressed data             */
/*                                                                  */
/*      Returns:                                                    */
/*                Nothing                                           */
/*                                                                  */
/* ################################################################ */
{
  /* declare and calculate the range */
  unsigned short range = (signal & 0x8000) >> 15;

  /* zero the signals */
  *low = 0;
  *high = 0;

	/* do the decompression */
  switch (range) {
    case 0:
      *high = (unsigned short) ((signal & 0x07ff) << 2);
      *low  = (unsigned short) ((signal & 0x3e00) >> 8);
      break;
    case 1:
      *low  = (unsigned short) (signal & 0x0fff);
      break;
  }
}


/* ####################################################################### */
void hist_isr()
/* ####################################################################### */
/* Revision History:                                                       */
/*                    04/10/94 KSB created                                 */
/*                                                                         */
/* This routine will start the histogram plotting routine                  */
/*                                                                         */
/* Returns:                                                                */
/*           Nothing                                                       */
/*                                                                         */
/* ####################################################################### */
{
  /* first we need to start the timer again so it will call again */
  wdStart(hist_wid, (sysClkRateGet()*histo_int), (FUNCPTR) hist_isr, 0);

  /* give the semaphore to */
  if ((print_map & 0x200) == 0x200)
    semGive(hist_sid);

}

/* ################################################################ */
void drawHistos()
/* ################################################################ */
/*      Revision History:                                           */
/*                          04/07/94 KSB created                    */
/*                                                                  */
/*      This routine draws histograms                               */
/*                                                                  */
/*      Returns:                                                    */
/*                Nothing                                           */
/*                                                                  */
/* ################################################################ */
{
  unsigned short counts[4][35];
  unsigned short left, right;
  unsigned short max_cts = 0;
  unsigned short line_val = 1;
  char left_label[12], right_label[12];
  int i,j,k;
  unsigned short totall = 0;
  unsigned short totalr = 0;
  int volts;
  int ones;
  int dec;

  while (1)
    {
    semTake(hist_sid, WAIT_FOREVER);

    /* should we exit? */
    if (param_map & 0x8000) {
      if (WARNING) printf("HISTOS is exiting\n");
      return;
      }


    /* re-initialize variables */
    max_cts = 0;
    line_val = 1;
    totall = 0;
    totalr = 0;

    /* move the inbound data to the local buffer */
    memcpy((char *)counts, (char *)totals, 2*4*35);

    /* clear totals */
    memset((char *)totals, 0, sizeof(totals));

    /* figure which parameters will be graphed, and where */
    if ( (histo_map & 0x1) ==  0x1)
      {
      left = 0;
      strcpy(left_label, " Total -+--");
      if ( (histo_map & 0x2) == 0x2) {
        right = 1;
        strcpy(right_label, " Masked +--");
      } else if ( (histo_map & 0x4) == 0x4) {
        right = 2;
        strcpy(right_label, " Forward --");
      } else if ( (histo_map & 0x8) == 0x8) {
        right = 3;
        strcpy(right_label, " Backward -");
      } else {
        right = 4;
        strcpy(right_label, "--------+--");
        }
      }
    else if ( (histo_map & 0x2) == 0x2) {
      left = 1;
      strcpy(left_label, " Masked +--");
      if ( (histo_map & 0x4) == 0x4) {
        right = 2;
        strcpy(right_label, " Forward --");
      } else if ( (histo_map & 0x8) == 0x8) {
        right = 3;
        strcpy(right_label, " Backward -");
      } else {
        right = 4;
        strcpy(right_label, "--------+--");
        }
      }
    else if ( (histo_map & 0x4) == 0x4) {
      left = 2;
      strcpy(left_label, " Forward --");
      if ( (histo_map & 0x8) == 0x8) {
        right = 3;
        strcpy(right_label, " Backward -");
      } else {
        right = 4;
        strcpy(right_label, "--------+--");
        }
      }
    else if ( (histo_map & 0x8) == 0x8) {
      left = 3;
      strcpy(left_label, "Backward -");
      right = 4;
      strcpy(right_label, "--------+--");
      }
    else {
      left = 4;
      strcpy( left_label, "-----------");
      right = 4;
      strcpy(right_label, "-----------");
      }
	
		
    /* find the maximum number of counts */
    for (j = 0; j < 35; j++)
      {
      if (counts[left][j] > max_cts) max_cts = counts[left][j];
      if (counts[right][j] > max_cts) max_cts = counts[right][j];
      totall += counts[left][j];
      totalr += counts[right][j];
      }
	
	
    /* figure how much each line is worth...keep it simple */
    if (max_cts <= 20)           { line_val =    1;
    } else if (max_cts <= 100)   { line_val =    5;
    } else if (max_cts <= 200)   { line_val =   10;
    } else if (max_cts <= 500)   { line_val =   25;
    } else if (max_cts <= 1000)  { line_val =   50;
    } else if (max_cts <= 2000)  { line_val =  100;
    } else if (max_cts <= 5000)  { line_val =  250;
    } else if (max_cts <= 10000) { line_val =  500;
    } else if (max_cts <= 20000) { line_val = 1000;
    } else if (max_cts <= 40000) { line_val = 2000;
    } else if (max_cts <= 60000) { line_val = 3000;
    } else if (max_cts <= 80000) { line_val = 4000;
    } else                       { line_val = 5000;
    }
	
    if(histo_lock > 0) line_val=histo_lock/20;
	
    /* print the first lines */
    cls();
    printf("     +-%11s--+----+----+%9u++-%11s--+----+----+--%2u:%2u:%2u+\n",
           left_label,totall, right_label,
           time_now.hour, time_now.minute, time_now.second);

    /* print the 20 data lines */
    for ( k = 20 ; k > 0 ; k--)
      {
      /* print the value label and first separator */
      if (!(k % 2))
        printf("%5i|",k*line_val);
      else
        printf("     |");

      /* print the data for the left plot */
      if ( left == 4 )
        printf("                                   ");
      else
        for (j = 0; j < 35; j++)
          if ( counts[left][j] >= (k*line_val))
            printf("#");
          else
            printf(" ");

      /* print the second separator */
      printf("|");

      /* print the data for the right plot */
      if ( right == 4 )
        printf("                                   ");
      else
        for (j = 0; j < 35; j++)
          if (counts[right][j] >= (k * line_val))
            printf("#");
          else
            printf(" ");

      /* finish the line */
      printf("|\n");
      }

    /* print the bottom lines */
    printf("    -+00--+05--+10--+15--+20--+25--+30--++00--+05--+10--+15--+20--+25--+30--++\n");

    printf("ALT:");
//	for (l=0; l<14; l++) printf("%c", cnav_dat.pAlt[l]);
//	for (l=0; l<6; l++) printf("%c", dnav_dat.palt[l]);
//	for (l=0; l<14; l++) printf("%c", fnav_dat.pAlt[l]);

    printf("  TEMP:");
//	for (l=0; l<14; l++) printf("%c", cnav_dat.temp[l]);
//	for (l=0; l<6; l++) printf("%c", dnav_dat.temp[l]);
//	for (l=0; l<14; l++) printf("%c", fnav_dat.temp[l]);

//	printf("  DEW:");
//	for (l=0; l<14; l++) printf("%c", cnav_dat.dewP[l]);
//	for (l=0; l<8; l++) printf("%c", dnav_dat.dptGE[l]);

    printf("MD:%4u  MH:%4u", MD_info.filenum, MH_info.filenum);

/*
  for ( i = 8 ; i < 12 ; i++) {
    if (das_hk.adc[i] >= 0x806) {
      volts = ((((das_hk.adc[i] - 0x806)*1000)/412));
      if (volts > 60000) volts = 0;
      ones = volts/1000;
      dec = volts - ones*1000;
      printf(" %d)%1d.%03u",i-7,ones,dec);
    } else {
      volts = ((((0x806 - das_hk.adc[i])*1000)/412));
      if (volts > 60000) volts = 0;
      ones = volts/1000;
      dec = volts - ones*1000;
      printf(" %d)-%1d.%03u",i-7,ones,dec);
      }
    }
*/
    printf("\n");
    }	

}

/* ################################################################ */
void sizeBins()
/* ################################################################ */
/*      Revision History:                                           */
/*                          04/07/94 KSB created                    */
/*                                                                  */
/*      This routine creates the requested bin limits               */
/*                                                                  */
/*      Returns:                                                    */
/*                Nothing                                           */
/*                                                                  */
/* ################################################################ */
{
  int j;

  if (histo_bin == 0)
    for (j = 0; j < 36; j++)
      bins[j] = j * 118;

  if (histo_bin == 1)
    {
    for (j = 0; j < 31; j++)
      bins[j] = maspLim[j];

    for (j = 31; j < 36; j++)
      bins[j] = 999999;
    }

  if (histo_bin == 2)
    for (j = 0; j < 36; j++)
      bins[j] = j * 1755;
printf("histo_bin=%d\n", histo_bin);

    for (j = 0; j < 31; j++)
      printf("%d ", bins[j]);
printf("\n");
}

/* ################################################################ */
void buildHisto(struct MASP_PART_PKT *masp_dat)
/* ################################################################ */
/*      Revision History:                                           */
/*                          04/07/94 KSB created                    */
/*                                                                  */
/*      This routine sums the packet data in the proper bins        */
/*                                                                  */
/*      Returns:                                                    */
/*                Nothing                                           */
/*                                                                  */
/* ################################################################ */
{
  int i,j;
  unsigned short LowGain, MidGain, HiGain;
  unsigned long total, masked, forward, backward;
  unsigned short ttime;
  unsigned short ratio;

  /* look at each particle in the packet */
  for (i = 0; i < 144; i++)
    {
    ttime = (masp_dat->part[i].ttimer & 0x7fff)-1;

    /* reject for bad transit times */
    /* VT = d */
    /* d = 25e-6 m */
    /* d/V = T */
    /* 25.0e-6 / 25 = 1000 ns = 50 counts */
    /* 25.0e-6 / 250 = 100 ns = 5 counts */
    if (ttime > 5 && ttime < 50) {
      /* unpack the data */
      unpak31(masp_dat->part[i].total, &LowGain, &MidGain, &HiGain);
      if (HiGain > 0)
        total = (unsigned long)(HiGain);
      else if (MidGain > 0)
        total = (unsigned long)(MidGain * 15);
      else
        total = (unsigned long)(LowGain * 15 * 15);

      unpak31(masp_dat->part[i].masked, &LowGain, &MidGain, &HiGain);
      if (HiGain > 0)
        masked = (unsigned long)(HiGain);
      else if (MidGain > 0)
        masked = (unsigned long)(MidGain * 13);
      else
        masked = (unsigned long)(LowGain * 13 * 13);

      unpak21(masp_dat->part[i].forward, &LowGain, &HiGain);
      if (HiGain > 0)
        forward = (unsigned long)(HiGain);
      else
        forward = (unsigned long)(LowGain * 16);

      unpak21(masp_dat->part[i].backward, &LowGain, &HiGain);
      if (HiGain > 0)
        backward = (unsigned long)(HiGain);
      else
        backward = (unsigned long)(LowGain * 30);

      /* bin the high gains */
      ratio = 0;
      if (forward > 0)
        ratio = (masked * 1000) / forward;

      if (ratio >= mt_ar)
        {
        for (j = 0; j < 35; j++)
          {
          if ((bins[j] <= total) && (total < bins[j+1]))
            totals[0][j]++;

          if ((bins[j] <= masked) && (masked < bins[j+1]))
            totals[1][j]++; 

          if ((bins[j] <= forward) && (forward < bins[j+1])) {
            totals[2][j]++;
            if (j < 32)
              histo[j]++;		// WINDS
            }

          if ((bins[j] <= backward) && (backward < bins[j+1]))
            totals[3][j]++;
          }
        if (total > bins[35])
          totals[0][34]++;

        if (masked > bins[35])
          totals[1][34]++;

        if (forward > bins[31]) // WINDS
          histo[30]++;

        if (forward > bins[35]) {
          totals[2][34]++;
          }

        if (backward > bins[35])
          totals[3][34]++;
        }
      }
    }
}



// ***********************************************************************
// user_Iface.cc
// 
// This routine will handle the user interface
//
// Written by Keith Barr
// Copyright 1995
// National Center for Atmospheric Research
//
// Revision History:
//   950407, ksb, major revision, C++, menu driven
// ***********************************************************************
void user_Iface()
{
  unsigned char rx;
  int pid;
  int menu = 0;			// flag specifiying current menu
  int displaying = 0;	// flag specifying if the display is doing something
  int mode_flag = 0;	// does mode need to be sent to probes?
  int use_def;			// use default value
  unsigned short utas;	// user set tas

  // open the serial port for reading
  pid = open("/tyCo/0", O_RDONLY, 0666);

  // set up the port to act like a terminal
  ioctl(pid, FIOSETOPTIONS, OPT_TERMINAL);

  // clear the port
  ioctl(pid, FIOFLUSH, 0);

  // setup default output
  print_map = 0x0;

  // wait for display to finish
  taskDelay(30);

  // keep looping until we exit
  while ((param_map & 0x8000) == 0) {
    // output the current menu
    if (!displaying) {
      // wait for display to finish
      taskDelay(100);

      // show the menu
      showMenu(menu);

      // print a prompt
      printf("maspDas> ");
    }

    // wait for input
    do {
      read(pid, (char *)&rx, 1);
    } while (rx == '\n');

    // convert input to upper case
    rx = toupper(rx);

    // process input
    if (menu == 0) {
      switch(rx) {
        case 'M':	// MASP data
          menu = 10;
          print_map = 0x0;
          break;

        case 'N':	// Navigation data
          print_map ^= 0x80;
          break;

        case 'D':	// DAS Housekeeping
          print_map ^= 0x40;
          break;

        case 'P':	// Packet information
          print_map ^= 0x20;
          break;

        case 'R':	// SEA seral dump
          param_map ^= 0x4;
          break;

        case 'W':	// Toggle warnings
          WARNING ^= 0x1;
          break;

        case 'H':	// Halt all displays and return to main menu
          menu = 0;
          print_map = 0x0;
          break;

        case 'E':	// Exit?
          // multiple levels for safety!
          read(pid, (char *)&rx, 1);
          rx = toupper(rx);
          if (rx == 'X') {
            read(pid, (char *)&rx, 1);
            rx =  toupper(rx);
            if (rx == 'I') {
              read(pid, (char *)&rx, 1);
              rx =  toupper(rx);
              if (rx == 'T') {
                printf("EXITING...please reboot before rerunning\n");
                param_map ^= 0x8000;
              } // T?
            } // I?
          } // X?

        default:	// unknown parameter
          break;
      } // Main Menu switch
    } else if (menu == 10) {
      switch (rx) {
        case 'P':	// MASP ASCII Particle Data
          print_map ^= 0x1;
          break;
			
        case 'K':	// MASP ASCII Housekeeping Data
          print_map ^= 0x2;
          break;

        case 'S':	// MASP Hex servo data
          print_map ^= 0x4;
          break;

        case 'G':	// MASP Histograms
          menu = 11;
          print_map = 0x0;
          break;

        case 'C':	// MASP 5 second calibration
          param_map ^= 0x1;
          break;

        case 'L':	// MASP 16Hz cal tick
          param_map ^= 0x10;
          pulse_now();
          break;

        case 'T':	// set true airspeed
          // get the current carriage return
          do {
            read(pid, (char *) &rx, 1);
          } while (rx != '\n');

          // reset the tas
          utas = 0;

          // get the input
          printf("Enter True Airspeed (return to use nav data): ");
          do {
            read(pid, (char *)&rx, 1);
            if ((rx >= 0x30) && (rx <= 0x39)) {
              utas = utas*10 + (unsigned short)(rx - 0x30);
            }
          } while (rx != '\n');

          // figure out what to do
          if (utas > 0) { // if user entered a valid speed, use it
            tas = utas;
            printf("True Airspeed set to %u m/s.\n", tas);
            param_map |= 0x2;
          } else { // otherwise, use nav data
            tas = 0;
            printf("Using Nav-Data for TAS\n");
            param_map &= 0xfffd;
          }
          break;

        case 'X':	// set minimum transit time
          // we are changing mode parameters here
          mode_flag =  1;

          // get the current carriage return
          do {
            read(pid, (char *) &rx, 1);
          } while (rx != '\n');

          // reset the min ttime
          minTtime = 0;

          // get the input
          printf("Enter Minimum Transit time (return for default): ");
          do {
            read(pid, (char *)&rx, 1);
            if ((rx >= 0x30) && (rx <= 0x39)) {
              minTtime = minTtime*10 + (unsigned short)(rx - 0x30);
            }
          } while (rx != '\n');

          // figure out what to do
          if (minTtime == 0) { // use default
            minTtime = MIN_TTIME;
          }
						
          // tell the user
          printf("minTtime set to %u.\n", minTtime);

          break;

        case 'R':	// set trigger threshold
          // we are changing mode parameters here
          mode_flag =  1;

          // get the current carriage return
          do {
            read(pid, (char *) &rx, 1);
          } while (rx != '\n');

          // reset the trigger threshold
          thresh = 0;

          // get the input
          printf("Enter Trigger Threshold (return for default): ");
          do {
            read(pid, (char *)&rx, 1);
            if ((rx >= 0x30) && (rx <= 0x39)) {
              thresh = thresh*10 + (unsigned short)(rx - 0x30);
            }
          } while (rx != '\n');

          // figure out what to do
          if (thresh == 0) { // use default
            thresh = TRIG_THRESH;
          }
						
          // tell the user
          printf("trigger threshold  set to %u.\n", thresh);

          break;

        case 'A':	// masked DOF gain
          // we are changing mode parameters here
          mode_flag =  1;

          // get the current carriage return
          do {
            read(pid, (char *) &rx, 1);
          } while (rx != '\n');

          // reset the gain
          maGain = 0;

          // get the input
          printf("Enter Masked Rejection Gain (return for default): ");
          do {
            read(pid, (char *)&rx, 1);
            if ((rx >= 0x30) && (rx <= 0x39)) {
              maGain = maGain*10 + (unsigned short)(rx - 0x30);
            }
          } while (rx != '\n');

          // figure out what to do
          if (maGain == 0) { // use default
            maGain = MASK_DOF_GAIN;
          }
						
          // tell the user
          printf("Masked Rejection Gain set to %u.\n", maGain);

          break;

        case 'B':	// total DOF gain
          // we are changing mode parameters here
          mode_flag =  1;

          // get the current carriage return
          do {
            read(pid, (char *) &rx, 1);
          } while (rx != '\n');

          // reset the gain
          tsGain = 0;

          // get the input
          printf("Enter Total Rejection Gain (return for default): ");
          do {
            read(pid, (char *)&rx, 1);
            if ((rx >= 0x30) && (rx <= 0x39)) {
              tsGain = tsGain*10 + (unsigned short)(rx - 0x30);
            }
          } while (rx != '\n');

          // figure out what to do
          if (tsGain == 0) { // use default
            tsGain = TOTAL_DOF_GAIN;
          }
						
          // tell the user
          printf("Total Rejection Gain set to %u.\n", tsGain);

          break;

        case 'M':	// set maximum MASP packets
          // we are changing mode parameters here
          mode_flag =  1;

          // get the current carriage return
          do {
            read(pid, (char *) &rx, 1);
          } while (rx != '\n');

          max_masp_pkts = 0;
          use_def = 1;

          // get the input
          printf("Enter maximum MASP packets(return for default): ");
          do {
            read(pid, (char *)&rx, 1);
            if ((rx >= 0x30) && (rx <= 0x39)) {
              max_masp_pkts=max_masp_pkts*10+(unsigned short)(rx-0x30);
              use_def = 0;
            }
          } while (rx != '\n');

          // figure out what to do
          if ((use_def) &&(max_masp_pkts == 0)) { // use default
            max_masp_pkts = MAX_MASP_PKTS;
          }
						
          // tell the user
          printf("Maximum MASP Packets set to %u.\n", max_masp_pkts);

          break;

        case 'H':	// Halt all displays and return to main menu
          menu = 0;
          print_map = 0x0;
          break;

        case 'Q':	// move up one menu level
          menu = 0;
          print_map = 0x0;
          break;

        default:		// unknown entry
          break;

      } // Masp Menu switch

    } else if (menu == 11) {
      switch (rx) {
        case 'G':	// histogram plotting
          // if we are entering the display range, get the interval
          if ((print_map & 0x200) == 0x200) {	// already on
            // turn it off
            print_map ^= 0x200;
            // reset interval
            histo_int = HISTO_INT;
          } else {
            // get the current carriage return
            do {
              read(pid, (char *) &rx, 1);
            } while (rx != '\n');
	
            // reset the histogram interval
            // zero can cause a crash
            histo_int = 1;

            // get the input
            printf("Enter averaging interval(return for default): ");
            do {
              read(pid, (char *)&rx, 1);
              if ((rx >= 0x30) && (rx <= 0x39)) {
                histo_int=(histo_int-1)*10+(unsigned short)(rx-0x30)+1;
              }
            } while (rx != '\n');

            // remove extra count
            histo_int -= 1;
	
            // figure out what to do
            if (histo_int == 0) { // use default
              histo_int = HISTO_INT;
            }
							
            // tell the user
            printf("Histogram interval set to %u.\n", histo_int);

            // turn it on
            print_map ^= 0x200;
          }

          break;

        case 'L':	// lock histograms at
          // get the current carriage return
          do {
            read(pid, (char *) &rx, 1);
          } while (rx != '\n');

          // reset the trigger threshold
          histo_lock = 0;

          // get the input
          printf("Enter Maximum Y-Value (return for floating): ");
          do {
            read(pid, (char *)&rx, 1);
            if ((rx >= 0x30) && (rx <= 0x39)) {
              histo_lock=histo_lock*10+(unsigned short)(rx-0x30);
            }
          } while (rx != '\n');

          // figure out what to do
          if (histo_lock == 0) { // use default
            printf("Histograms floating\n");
          } else {
            printf("Histograms locked at %u.\n", histo_lock);
          }

          break;

        case 'R':	// set minimum acceptance ratio
          // get the current carriage return
          do {
            read(pid, (char *) &rx, 1);
          } while (rx != '\n');

          // reset the trigger threshold
          mt_ar = 0;

          // get the input
          printf("Enter Acceptance Ratio * 1000 (return for default): ");
          do {
            read(pid, (char *)&rx, 1);
            if ((rx >= 0x30) && (rx <= 0x39)) {
              mt_ar=mt_ar*10+(unsigned short)(rx-0x30);
            }
          } while (rx != '\n');

          // figure out what to do
          if (mt_ar == 0) { // use default
            mt_ar = MASK_TOTAL_AR;
          }
						
          // tell the user
          printf("Minimum Acceptance Ratio set to %u.\n", mt_ar);

          break;


        case 'S':	// select histogram binning
          // get the current carriage return
          do {
            read(pid, (char *) &rx, 1);
          } while (rx != '\n');

          // reset the trigger threshold
          histo_bin = 0;

          // get the input
          printf("Enter histogram binning (0-5): ");
          do {
            read(pid, (char *)&rx, 1);
            if ((rx >= 0x30) && (rx <= 0x39)) {
              histo_bin=histo_bin*10+(unsigned short)(rx-0x30);
            }
          } while (rx != '\n');

          // figure out what to do
						
          // tell the user
          printf("Using histo-binning %u.\n", histo_bin);

          // set up the bins
          sizeBins();

          break;

        case 'T':	// total detector
          histo_map ^= 0x1;
          break;

        case 'M':	// masked detector
          histo_map ^= 0x2;
          break;

        case 'F':	// forward detector
          histo_map ^= 0x4;
          break;

        case 'B':	// backward detector
          histo_map ^= 0x8;
          break;

        case 'H':	// Halt all displays and return to main menu
          menu = 0;
          print_map = 0x0;
          break;

        case 'Q':	// move up one menu level
          menu = 10;
          print_map = 0x0;
          break;

        default:		// unknown entry
          break;

      } // MASP histogram switch

    }

    // is something using the screen?
    if (print_map == 0x0) {
      displaying = 0;
    } else {
      printf("50\E[;H\E[2J");
      displaying = 1;
    }

    // should we update probe modes?
    if (mode_flag) {
      printf("sending mode\n");
      sendMode();
      mode_flag = 0;
      ++modeSentCount %= 256;
    }

  } // end user interface while

  // tell the user and stop what we can
  printf("User Interface is exiting!\n");
  wdCancel(death_wid);
  cls_files();

}

// ***********************************************************************
// showMenu.cc
// 
// This routine will display the requested menu
//
// Written by Keith Barr
// Copyright 1995
// National Center for Atmospheric Research
//
// Revision History:
//   950407, ksb, major revision, C++, menu driven
// ***********************************************************************
void showMenu(int menu)
{
  // clear the screen and home the cursor
  printf("50\E[;H\E[2J");

	// display the requested menu
  if (menu == 0) {
    printf(".-----------------------------------------------.\n");
    printf("|                 Main Menu                     |\n");
    printf("|-----------------------------------------------|\n");
    printf("| (M) Change MASP parameters and view MASP data |\n");
    printf("|                                               |\n");
    printf("|                                               |\n");
    printf("| (N) View navigation data                      |\n");
    printf("| (D) View data system housekeeping data        |\n");
    printf("| (P) View received packet information          |\n");
//		if (param_map & 0x4) {
//			printf("| (R) 10-second serial stream dump to SEA off   |\n");
//		} else {
//			printf("| (R) 10-second serial stream dump to SEA on    |\n");
//		}
    printf("|-----------------------------------------------|\n");
    if (WARNING) {
      printf("| (W) Turn off warnings                         |\n");
    } else {
      printf("| (W) Turn on warnings                          |\n");
    }
    printf("| (H) Halt all displays and return to main menu |\n");
    printf("|-----------------------------------------------|\n");
    printf("| (EXIT) Exit program (must reboot to re-run)   |\n");
    printf("`-----------------------------------------------'\n");
  } else if (menu == 10) {
    printf(".-----------------------------------------------.\n");
    printf("|                 MASP Menu                     |\n");
    printf("|-----------------------------------------------|\n");
    printf("| (P) View ASCII particle data                  |\n");
    printf("| (K) View ASCII housekeeping data              |\n");
//		printf("| (S) View Hex Cal-laser servo data             |\n");
    printf("| (G) Histograms                                |\n");
    printf("|-----------------------------------------------|\n");
    if ((param_map & 0x1) == 0x1) {
      printf("| (C) Turn off 5-second calibration mode        |\n");
    } else {
      printf("| (C) Turn on 5-second calibration mode         |\n");
    }
    if ((param_map & 0x10) == 0x10) {
      printf("| (L) Turn off 16 Hz cal-laser pulse            |\n");
    } else {
      printf("| (L) Turn on 16 Hz cal-laser pulse             |\n");
    }
    printf("|-----------------------------------------------|\n");
    printf("| (T) Set true airspeed                   %5i |\n", tas);
    printf("| (X) Set minimum transit time            %5i |\n", minTtime);
    printf("| (M) Set maximum packet count            %5i |\n",max_masp_pkts);
    printf("| (R) Set trigger threshold               %5i |\n", thresh);
    printf("| (A) Set Mask DOF Rejection              %5i |\n", maGain);
    printf("| (B) Set Total DOF Rejection             %5i |\n", tsGain);
    printf("|-----------------------------------------------|\n");
    printf("| (H) Halt all displays and return to main menu |\n");
    printf("| (Q) Return to previous menu                   |\n");
    printf("`-----------------------------------------------'\n");

  } else if (menu == 11) {
    printf(".-----------------------------------------------.\n");
    printf("|            MASP Histogram Menu                |\n");
    printf("|-----------------------------------------------|\n");
    printf("| (G) Display Histograms                        |\n");
    printf("|-----------------------------------------------|\n");
    if ((histo_map & 0x1) == 0x1) {
      printf("| (T) Total detector                         on |\n");
    } else {
      printf("| (T) Total detector                        off |\n");
    }
    if ((histo_map & 0x2) == 0x2) {
      printf("| (M) Masked detector                        on |\n");
    } else {
      printf("| (M) Masked detector                       off |\n");
    }
    if ((histo_map & 0x4) == 0x4) {
      printf("| (F) Forward detector                       on |\n");
    } else {
      printf("| (F) Forward detector                      off |\n");
    }
    if ((histo_map & 0x8) == 0x8) {
      printf("| (B) Backward detector                      on |\n");
    } else {
      printf("| (B) Backward detector                     off |\n");
    }
    printf("|-----------------------------------------------|\n");
    printf("| (L) Histogram Y-axis Maximum            %5i |\n",histo_lock);
    printf("| (R) Minimum Acceptance Ratio            %5i |\n",mt_ar);
    printf("| (S) Bin Selection                       %5i |\n",histo_bin);
    printf("|-----------------------------------------------|\n");
    printf("| (H) Halt all displays and return to main menu |\n");
    printf("| (Q) Return to previous menu                   |\n");
    printf("`-----------------------------------------------'\n");
  }
}


