/* Masp.cc
 
   MASP interface class.
 
   Original Author: Mike Spowart 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/
 
# include <Masp.h>
 
/******************************************************************************
** Public Functions
******************************************************************************/
 
Masp::Masp (unsigned char *intf_base, Bc635Vme &tp, 
            void (*stat_msg)(char* msg_str)) : tfp (tp)
 
// Constructor.
 
{
 
  int i,j; 
  pcnt=0;     /* number of packets received since start */
  rfindx=0; /* buffer number of next packet */
  rcSum = 0;  /* number of checksums errors read */
  acSum = 0;  /* number of checksums errors calculated */
  bpSum = 0;  /* number of bad packets received */

  thresh = TRIG_THRESH; /* MASP trigger threshold */
  maGain = MASK_DOF_GAIN;
  tsGain = TOTAL_DOF_GAIN;
  minTtime = MIN_TTIME; /* MASP minimum accepted t-time */
  max_masp_pkts = MAX_MASP_PKTS;

  /* initialize the holder for the number of packets since the last write */
  mh_pkts = 0;
  ms_pkts = 0;
  md_pkts = 0;
 
  /* load the time buffer */
  get_time();
 
  /* get last masp set, to give masp time to start */
  last_masp = sec_o_yr;
 
// Create the Masp message queue
   masp_qid = msgQCreate(30,2,MSG_Q_FIFO);

// Create the particle and record buffer class.
  for (j = 0; j < TOG; j++) {
    ms_buf[j] = (SerialBuf*)malloc (sizeof (SerialBuf));
    ms_buf[j]->SerialBuf (sizeof(SERVO_BLOCK));
    md_buf[j] = (SerialBuf*)malloc (sizeof (SerialBuf));
    md_buf[j]->SerialBuf (sizeof(DAT_BLOCK));
 
  } 

  ms_gtog = 0;
  ms_ptog = 0; 
  md_gtog = 0;
  md_ptog = 0; 

}
/*****************************************************************************/
void Masp::collect ()
 
// Copies data from the interface buffers into the local buffers.

{ 
  MaspHouse* rec; 
  /* read the message queue */
  if ((iflag = msgQReceive(masp_qid,(char*)&rfindx,2,WAIT_FOREVER)) > 0) {

    /* Check for read checksum errors -- data type dependent code */ 
    if ( rfindx < 7) {
      /* take a look at the flag in the register */
      if ( ( (stat=(int)dpr->rx_flags[rfindx]) > 1) && (pcnt > 0) ) { 
        printf("MASP rx_flag shows checksum error !\n"); 
        rcSum += 1; 
        /* increment housekeeping data */ 
      } 
      /* set up a pointer to the data buffer */ 
      data_pk = (unsigned short *)&dpr->rxb[rfindx]; 
    } 
    else { 
      printf("Warning...rfindx = %i\n",rfindx); 
    }
 
    /* get the data block size, type, frac time, and real time */
    p_size = *(data_pk + 1);
    p_type = *(data_pk + 2);
    f_time = *(data_pk + 3);
    m_time = *(unsigned long *)(data_pk + 4);
    p_cksm = *(data_pk + p_size - 1);

    rec = (MaspHouse*)gbuffer();
 
    /* now copy the data from the VMEMA to the appropriate buffer */
    if ((p_type == 1) && (p_size ==64)) {                /* masp hkng */
      /* copy data from vmema to buffer (from->to)*/
      bcopyWords( (char*)data_pk, (char*)rec, (int)p_size);

      // do we have a bad time?
      if (rec->hbuf[ptog].time == 0x15555) sendMode();

      /* reset the receive buffer flag */
      dpr->rx_flags[rfindx] = 0;
      dpr->dsp_irq = 1;
 
      /* increment the packets buffered counter */
      mh_pkts++;
 
      /* calculate the checksum */
      if (p_cksm != (checksum((ushort*)rec, p_size))) {
        printf("MH Checksum Error Detected\n");
        acSum += 1;
      }
 
      /* increment last packet time */
      last_masp = sec_o_yr;
      pcnt++;
    } 
    else if ((p_type == 2) && (p_size == 1020)) {      /* masp data */
      /* copy data from vmema to buffer */
      bcopyWords( (char*)data_pk, (char*)md_buf[md_ptog], (int)p_size);
 
      /* reset the receive buffer flag */
      dpr->rx_flags[rfindx] = 0;
      dpr->dsp_irq = 1;
 
      /* increment the counter */
      md_pkts++;
 
      /* calculate the checksum */
      if (p_cksm != (checksum((ushort*)md_buf[md_ptog], p_size))) {
        printf("MD Checksum Error Detected\n");
        acSum += 1;
      }
      /* increment last packet time */
      last_masp = sec_o_yr;
      pcnt++;
    } 
    else if ((p_type == 5) && (p_size == 184)) {       /* masp servo */
      /* copy data from vmema to buffer */
      bcopyWords( (char*)data_pk, (char*)ms_buf[ms_ptog], (int)p_size);
 
      /* reset the receive buffer flag */
      dpr->rx_flags[rfindx] = 0;
      dpr->dsp_irq = 1;
 
      /* increment the counter */
      ms_pkts++;
 
      /* calculate the checksum */
      if (p_cksm != (checksum((ushort*)ms_buf[ms_ptog], p_size))) {
        printf("MS Checksum Error Detected\n");
        acSum += 1;
      }
 
      /* increment last packet time */
      last_masp = sec_o_yr;
      pcnt++;
    } 
    else {             /* unknown */
      /* tell anyone watching */
      printf("Unknown packet: Type %i, Size %i, Index %i\n",
             p_type, p_size, rfindx);
      bpSum++;
      /* clear the buffer */
      if (rfindx < 7) {               /* MASP data */
        dpr->rx_flags[rfindx] = 0;
        dpr->dsp_irq = 1;       /* force buffer check */
      }
 
    }
  } 
  else {
    printf("MESSAGE QUEUE ERROR:  IFLAG=%i\n",iflag);
  }
  if (ms_bufFull()) {
    ms_ptog = ++ms_ptog % TOG;
  }
  if (md_bufFull()) {
    md_ptog = ++md_ptog % TOG;
  }
} 
/*****************************************************************************/
void Masp::secondAlign ()
 
// Called at the start of a second to align the indices and toggle buffers.
{
  unsigned int seconds = 0;
  unsigned short cksum = 0;
  unsigned short t_chk = 0;
  int i, minute, second;
  int t_diff;
  int ttog;

  minute = tfp.minute() % 10;
  second = tfp.second();

  if((minute == 0) & (second == 0))
    cal_flag = 1; 

  /* initialize the MASP time packet */
  masp_time.sync_flag = 0x4450;
  masp_time.length = 0x7;
  masp_time.type = 0x544d;
 
  /* update the clock structure */
  seconds = get_time();
 
  /* rewrite register just in case */
  *(unsigned short *) (VMEMA_BASE+A24D16_BASE+BIC_CTL_REG2) = 0x63;

  /* fill in the data to be sent to the MASP */
  masp_time.seconds_yr = seconds;

  /* calculate the checksum */
  masp_time.cksum=checksum(( unsigned short *)&masp_time,masp_time.length);
 
  /* check to see if we can send data to MASP...if so, send it */
  i=0;
  while(dpr->cmd_stat > 0 && i < 5) {
//    taskDelay(2);
    taskDelay(sysClkRateGet() / 50);
    i++;
  }
 
  if ( dpr->cmd_stat == 0 ) {
    bcopyWords((char*)&masp_time,(char*)&dpr->txb,masp_time.length);
    dpr->dsp_irq = 3;
  }
 
  /* how long since we have seen data from the masp */
  t_diff = sec_o_yr - last_masp;
 
  /* check to see if masp is hung */
  if ( (t_diff%10 == 0) && (t_diff > 9) ) {
    /* tell user */
    printf("No data from MASP in %d seconds!\n",sec_o_yr-last_masp);
  }
 
  if ((t_diff > 1) && (t_diff%3 == 0) && (t_diff < 86400)) {
    printf("sending XON to MASP ");
    for (i=0;i<7;i++) {
      printf("%i  ",dpr->rx_flags[i]);
    }
    dpr->txb.tb[0] = 0x4450;
    dpr->txb.tb[1] = 0x4;
    dpr->txb.tb[2] = 0x4f4e;
    dpr->txb.tb[3] = 0x93a2;
    dpr->dsp_irq = 2;
 
    sendMode();
  }
  ttog = (ptog + 1) % TOG;
  gtog = ptog;
  ptog = ttog;
}
 
/*****************************************************************************/	
void Masp::masp_isr()
{
  static short n_buf = 0;
  int i;
 
  /* clear the innterrupt on the VMEMA card */
  *(unsigned short *) (VMEMA_BASE+A24D16_BASE+BIC_CTL_REG2) = 0x43;
  *(unsigned short *) (VMEMA_BASE+A24D16_BASE+BIC_CTL_REG2) = 0x63;
 
 
  for ( i = 0 ; i < 7 ; i++) {
    if (dpr->rx_flags[n_buf]) {
      msgQSend(masp_qid, (char*)&n_buf, 2, NO_WAIT, MSG_PRI_NORMAL);
      i=6;
    }
    n_buf++;
    n_buf %= 7;
  }
 
} /* end of masp_isr */
/*****************************************************************************/	
void Masp::setTas (int tas)
 
// Sets the true air speed.
{
 
  /* fill in the true airspeed to be sent to the MASP */
  masp_time.tas = (short)tas * 255 / 125;
}
/*****************************************************************************/
 
void Masp::control (int action, int value)
 
// Handles commands to the Masp.
{
  switch (action) {
    case MASP_SET_PARAM:             // enable calibration 
      setParam (value);
      break;
    break;
  }
}
/*****************************************************************************/
void Masp::setParam (unsigned short param)
 
// Sets the param_map.
{
 
  param_map = param;
}

/*****************************************************************************/
void Masp::probeInit ()
 
// Sets the param_map.
{
  int i;

  *(unsigned short *) (VMEMA_BASE+A24D16_BASE+SW_RESET) = 0x00;
  *(unsigned short *) (VMEMA_BASE+A24D16_BASE+BIC_CTL_REG1) =SP1_ADR;

/* initialize the ISR - VN 0x76, level 3 (written by KAW) */
   dpr = (struct DPR *) (DPR_A);
 
/* clear the buffers */
   for (i=0;i<7;i++) dpr->rx_flags[i]=0;
   dpr->dsp_irq = 1;       /* force a buffer check */
 
}
 
/******************************************************************************
** Private Functions
******************************************************************************/

/* ####################################################################### */
void Masp::calibrate()
/* ####################################################################### */
/*                                                                         */
/* This routine will put the MASP in calibration mode, and bring it back   */
/*      to data mode                                                       */
/*                                                                         */
/* ####################################################################### */
{
  int i,ignoreFlag;
  ignoreFlag = 1;

  /* initialize the MASP mode packet */
  masp_mode.sync_flag = 0x4450;
  masp_mode.length = 0x12;
  masp_mode.type = 0x4d44;
  masp_mode.level[0] = MODE_LEVEL_1;
  masp_mode.level[1] = MODE_LEVEL_2;
  masp_mode.level[2] = MODE_LEVEL_3;	/* laser Amplitudes */
  masp_mode.width[0] = MODE_WIDTH_1;
  masp_mode.width[1] = MODE_WIDTH_2;
  masp_mode.width[2] = MODE_WIDTH_3;	/* Pulse Durations */
  masp_mode.ki = KI;
  masp_mode.kp = KP;

  if (ignoreFlag != 1){
    /* change the threshold */
    masp_mode.trig_thresh = thresh;
    masp_mode.min_ttime = minTtime;
    masp_mode.maGain = maGain;
    masp_mode.tsGain = tsGain;
    masp_mode.mode = 0x1;		
    masp_mode.max_pkts = 50;	
    masp_mode.cksum = checksum((unsigned short *)&masp_mode,masp_mode.length);
  }
  /* should we exit? */
  if (param_map & 0x8000) {
    printf("CALIBRATE is exiting\n");
    return;
  }

  if (ignoreFlag != 1){
    if ((param_map & 0x1) || ( cal_flag == 1) ) {
//      taskDelay(20);	/* 200 msec */
      taskDelay(sysClkRateGet() / 5);

      /* set calibration flag high */
      cal_flag = 1;

      /* we enter the routine with the mode packet set for calibrate */
      /* so all we have to do is send it */
      i=0;
      while ( dpr->cmd_stat > 0 && i < 10) {
        printf("blocked while sending cal packet\n");
//        taskDelay(2);
        taskDelay(sysClkRateGet() / 50);
        i++;
      }

      if ( dpr->cmd_stat == 0) {
        bcopyWords ( (char *)&masp_mode,(char *)&dpr->txb, masp_mode.length);
        dpr->dsp_irq = 3;
      }

      /* wait a bit and turn the cal_flag off */
//      taskDelay(60);
      taskDelay(sysClkRateGet() / 2);
      cal_flag = 0;

      /* lets reset the throttle rate */
      masp_mode.mode = 0x0;		/* 1 = Calibrate, 0 = data */
      masp_mode.max_pkts = max_masp_pkts;
      masp_mode.cksum=checksum((unsigned short *)&masp_mode,masp_mode.length);
      i=0;
      while ( dpr->cmd_stat > 0 && i < 10) {
        printf("blocked while sending cal packet\n");
        taskDelay(sysClkRateGet() / 50);
        i++;
      }

      if ( dpr->cmd_stat == 0) {
        bcopyWords ( (char *)&masp_mode,(char *)&dpr->txb, masp_mode.length);
        dpr->dsp_irq = 3;
      }


      /* run pulse now in case the cal shut the mode off */
      pulse_now();
    }
  }/*End of ignore loop*/
  if (ignoreFlag == 1)
    sendMode();
}


/* ####################################################################### */
void Masp::sendMode()
/* ####################################################################### */
/*                                                                         */
/* This routine will update the mode parameters on both probes             */
/*                                                                         */
/* ####################################################################### */
{
  struct MODE_PACKET      masp_set;
  int i;

  /* build packets */
  masp_set.sync_flag   = 0x4450;
  masp_set.length      = 0x12;
  masp_set.type        = 0x4d44;
  masp_set.mode        = 0x0;		/* 1 = Calibrate, 0 = data */
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
  masp_set.cksum = checksum((unsigned short *)&masp_set,masp_set.length);

  /* send packet to probe */
  i=0;
  while ( dpr->cmd_stat > 0 && i < 10) {
//    taskDelay(3);
    taskDelay(sysClkRateGet() / 30);
    i++;
  }
		
  if ( dpr->cmd_stat == 0) {
    bcopyWords ( (char *)&masp_set,(char *)&dpr->txb, masp_set.length);
    dpr->dsp_irq = 3;
  }
}

/* ####################################################################### */
unsigned int Masp::get_time()
/* ####################################################################### */
/*                                                                         */
/*   This routine will fill the time_now structure with the current time   */
/*                                                                         */
/* ####################################################################### */
{

  unsigned int second;
  unsigned int minute;
  unsigned int hour;
  unsigned int day;
  unsigned int julian;
  unsigned int seconds_year;

  julian = 0;
  seconds_year = 0;

  hour = tfp.hour();
  minute = tfp.minute();
  second = tfp.second();
  day = tfp.julianDay();

  /* now calculate the seconds of the year */

  /* now add days to today */
  julian += day;

  /* now calculate seconds through last midnight */
  seconds_year = 3600 * 24 * (julian - 1);

  /* now add seconds for hours of today */
  seconds_year += (3600 * hour);

  /* now add seconds for minute */
  seconds_year += (60 * minute);

  /* now add seconds for second */
  seconds_year += (second);

  sec_o_yr = seconds_year;
}

/*****************************************************************************/	
/* ################################################################ */
unsigned short Masp::checksum(unsigned short *data, unsigned short size)
/* ################################################################ */
/*                                                                  */
/*     This routine calculates the checksum on a packet             */
/*                                                                  */
/* ################################################################ */
{
  unsigned short cksum = 0;
  int i;
	
  for ( i = 0 ; i < (size - 1) ; i++) {
    cksum += *data++;
  }
	
  return cksum;
}

/* ################################################################ */
void Masp::pulse_now()
/* ################################################################ */
/*                                                                  */
/*     This routine will toggle the 16Hz pulse on the MASP          */
/*                                                                  */
/* ################################################################ */
{
  struct MODE_PACKET hz_mode;
  int i;
 
  /* initialize the packet */
  hz_mode.sync_flag = 0x4450;	
  hz_mode.length = 0x12;
  hz_mode.type = 0x4d44;

  /* set the bitmapped mode word 0x2 is pulses on */
  if (param_map & 0x10) {
    hz_mode.mode = 0x2;	
  } else {
    hz_mode.mode = 0x0;
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

  hz_mode.cksum = checksum((unsigned short *)&hz_mode, hz_mode.length);

  /* we enter the routine with the mode packet set for calibrate */
  /* so all we have to do is send it */
  i=0;
  while ( dpr->cmd_stat > 0 && i < 10) {
    taskDelay(sysClkRateGet() / 50);
    i++;
  }
	
  if ( dpr->cmd_stat == 0) {
    bcopyWords ( (char *)&hz_mode,(char *)&dpr->txb, hz_mode.length);
    dpr->dsp_irq = 3;
  }
  
}

/*****************************************************************************/	
/* ################################################################ */
void Masp::unpak31 (unsigned short signal, 
              unsigned short *low,
              unsigned short *mid,
              unsigned short *high)
/* ################################################################ */
/*                                                                  */
/*      This routine unpacks three to one compressed data           */
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
          *high = (unsigned short) ((signal & 0x03ff) << 2);
          *mid  = (unsigned short) ((signal & 0x3c00) >> 6);
          break;
    case 1:
          *mid  = (unsigned short) ((signal & 0x07ff) << 1);
          *low  = (unsigned short) ((signal & 0x3800) >> 6);
          break;
    case 2:
          *low  = (unsigned short) (signal & 0x0fff);
          break;
  }
}

/* ################################################################ */
void Masp::unpak21 (unsigned short signal, 
              unsigned short *low,
              unsigned short *high)
/* ################################################################ */
/*                                                                  */
/*      This routine unpacks two to one compressed data             */
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
        *high = (unsigned short) ((signal & 0x07ff) << 1);
        *low  = (unsigned short) ((signal & 0x7800) >> 8);
        break;
  case 1:
        *low  = (unsigned short) (signal & 0x0fff);
        break;
  }
}

/*****************************************************************************/	

