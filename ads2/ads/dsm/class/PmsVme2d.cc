/* 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/
#include <PmsVme2d.h>

/******************************************************************************
** Public Functions
******************************************************************************/

PmsVme2d::PmsVme2d (unsigned char *intf_base, void (*stat_msg)(char* msg_str)) 
                
{
  int j;

//  printf("Begin Constructor\n");
// Set interface pointers.

  interfaceInstalled = false;
  probeInterface((char *)intf_base);

// Create the particle and record buffer class.
  for (j = 0; j < TOG; j++) {
    buf[j] = new SerialBuf (sizeof(P2d_rec) * 7);
  }
// Initialize buffer control variables.
  memset ((char*)probe_names, 0, sizeof(probe_names));
  gtog = 0;
  ptog = 0;
  p_cnt = 0;
  c_cnt = 0;
  rec_cnt = 0;
  hvps_type = FALSE;

  if (interfaceInstalled == false)
    return;

  statusMsg = stat_msg;                         // status message handler
  gctl = (Pms2dGctl*)(intf_base + PMS2D_GBL_CTL);
  pctl[0] = (Pms2dPctl*)(intf_base + PMS2D_PRB_0_CTL);
  pctl[1] = (Pms2dPctl*)(intf_base + PMS2D_PRB_1_CTL);
  g2rec[0] = (P2d_data*)(intf_base + PMS2D_GBL_BUF_0);
  g2rec[1] = (P2d_data*)(intf_base + PMS2D_GBL_BUF_1);
  h2rec[0] = (P2d_hdr*)(intf_base + PMS2D_HDR_BUF_0);
  h2rec[1] = (P2d_hdr*)(intf_base + PMS2D_HDR_BUF_1);
  dpr_sem_0 = (short*)(intf_base + PMS2D_DPR_SEM_0);
  dpr_sem_1 = (short*)(intf_base + PMS2D_DPR_SEM_1);
  dpr_sem_2 = (short*)(intf_base + PMS2D_DPR_SEM_2);
  bim_vct = (short*)(intf_base + PMS2D_BIM_VCT);
  bim_ctl = (short*)(intf_base + PMS2D_BIM_CTL);
  sw_rst = (short*)(intf_base + PMS2D_RESET);

// Issue a software reset to the interface.
//  reset();
  taskDelay (sysClkRateGet() / 10);		// delay for reset

  for (j = 0; j < PMS2D_NUM_PROBES; j++) 
    ptype[j] = GNONE;

// Set a default true air speed.
  max_tas = STD_2D_MAX_TAS;

  setTasMode (TAS2D_AUTO);
//  printf("Have set Mode\n");
  setTas (TAS2D_DEFAULT);
//  printf("Have set Tas\n");
  setRate (RATE2D_MAX);                         

}
/******************************************************************************/

void PmsVme2d::initProbe (int channel, char *id, int type, int res)

// Initializes a probe for sampling.
// Called to initialize a probe based on the probe type defined by the tape
// header, P2D_P_STR or P2D_C_STR, or P2D_H_STR.

{ 
  if (interfaceInstalled == false)
    return;

// C probes and HVPS probes are both sampled through the C interface channel.
  if(type == 1) {
    c_cnt++;
    sprintf (msg, "C%1d", c_cnt);
  }
  else if (type == 2) {
    p_cnt++;
    sprintf (msg, "P%1d", p_cnt);                // build the probe id string
  }
  else if (type == 3) {
    c_cnt++;
    sprintf (msg, "H%1d", c_cnt);                // build the probe id string
    hvps_type = TRUE;
 
// If a HVPS probe is being sampled through this interface, the tas is limited
// to HVPS_MAX_TAS.  NOTE: This limits the tas for all probes on this
// interface.
    max_tas = HVPS_MAX_TAS;
  }
  else {
    sprintf (msg, "Unknown Pms 2D probe type initialization requested, %s.\n",
             type);
    statusMsg (msg);
    return;
  }
//  printf("2D VME Go\n");
  getSem_0();
  pctl[channel]->id = *(short*)msg;
  pctl[channel]->type = (short)type;
  pctl[channel]->res = (short)res;
  releaseSem_0();
  ptype[channel] = type;
  strcpy (probe_names[channel], msg);            // save the id string
  printf("probe_names[%d] = %s\n",channel,probe_names[channel]);
  printf("id = %s\n",msg);
} 
/******************************************************************************/

void PmsVme2d::enableProbe (char *id)

// Eables a probe.
{
  int channel;

  if (interfaceInstalled == false)
    return;

  if ((channel = probeIndex (id)) != ERROR) {
    pctl[channel]->type = ptype[channel];
//    sprintf (msg, "%s probe enabled.\n", id);
  }
  else
    sprintf (msg, "Unknown PMS 2D probe enable requested. --> %s.\n", id);

  statusMsg (msg);
}

/******************************************************************************/
void PmsVme2d::disableProbe (char *id)

// Disables a probe.
{
  int channel;

  if (interfaceInstalled == false)
    return;

  if ((channel = probeIndex (id)) != ERROR) {
    pctl[channel]->type = GNONE;
    sprintf (msg, "%s probe disabled.\n", id);
  }
  else
    sprintf (msg, "Unknown PMS 2D probe disable requested. --> %s.\n", id);

  statusMsg (msg);
}

/******************************************************************************/
void PmsVme2d::setTas (int tas)

// Sets the true air speed if the tas_mode is TAS2D_AUTO.
{
  if (interfaceInstalled == false)
    return;

  if (hvps_type) {
    tas = 45;
  }
  if (tas_mode == TAS2D_AUTO) {
    volatile short	newTas = (short)tas * (255 / 125) + 1;
    getSem_2();
    gctl->tas = newTas;
    releaseSem_2();
    }

}
/******************************************************************************/
void PmsVme2d::setTasMode (int mode)
 
// Set the true air speed mode to auto or a fixed tas mode.
{
  if (interfaceInstalled == false)
    return;

  if ((tas_mode = mode) != TAS2D_AUTO) {
    volatile short	newTas = (short)mode * (255 / 125) + 1;

    getSem_2();
    gctl->tas = newTas;
    releaseSem_2();
    }

}

/******************************************************************************/
void PmsVme2d::setRate (int rate)
 
// Sets the max rate at which records are acquired from the controller.
{
  rec_rate = rate;
}
/******************************************************************************/

void PmsVme2d::setTime (int hour, int minute, int sec)

// Sets the interface time.
{
  if (interfaceInstalled == false)
    return;

  getSem_2();
  gctl->hour = (short)hour;
  gctl->minute = (short)minute;
  gctl->second = (short)sec;
  gctl->set_time = TRUE;
  releaseSem_2();
}
/******************************************************************************/

void PmsVme2d::startSampling ()

// Sets the interface go flag, which enables sampling.
{
  if (interfaceInstalled == false)
    return;

//  getSem_2();
  gctl->go = TRUE;
//  releaseSem_2();
}
/******************************************************************************/

short PmsVme2d::collect (int new_second)

// Copies data from the interface buffers into the local buffers.
{
  int channel;
  P2d_rec *rec;
  int gfull, idx;			// buffer full number of bytes 
  short rec_full = FALSE;

  if (interfaceInstalled == false)
    return rec_full;

// If this is the start of a new second, reset the record counter.
//  if (new_second) 
//    rec_cnt = 0;


  for (channel = 0; channel < PMS2D_NUM_PROBES; channel++)
    {
    if (channel == 0) {
      *dpr_sem_0 = 0;
      if (*dpr_sem_0 & 0x01)
        continue;
      gfull = gctl->full[0];
      releaseSem_0();
      }
    else {
      *dpr_sem_1 = 0;
      if (*dpr_sem_1 & 0x01)
        continue;
      gfull = gctl->full[1];
      releaseSem_1();
      }

//    logMsg("gfull = %d channel = %d\n",gfull,channel,0,0,0,0);

    if (!gfull)
      continue;

    // If the interface buffer is full, and the local buffer is not full,
    // copy in the data.
    if (!buf[ptog]->bufFull() && (rec_cnt < rec_rate)) {
      rec = (P2d_rec*)buf[ptog]->getBufPtr();
      buf[ptog]->copyToBuf ((char*)h2rec[channel], sizeof (P2d_hdr));
      buf[ptog]->copyToBuf ((char*)g2rec[channel], sizeof (P2d_data));
//      logMsg("2D collect rec_cnt = %d\n",rec_cnt,0,0,0,0,0);
      rec_cnt++;
/*
      logMsg("%x %x %x %x %x %x\n",g2rec[channel]->data[0],g2rec[channel]->data[1],
              g2rec[channel]->data[2],g2rec[channel]->data[3],g2rec[channel]->data[4],
              g2rec[channel]->data[5]);      
*/
      // Clear the buffer full flag.
      if (channel == 0) {
        getSem_0();
        gctl->full[0] = 0;
        releaseSem_0();
        }
      else {
        getSem_1();
        gctl->full[1] = 0;
        releaseSem_1();
        }

//      logMsg("Gfull = %d Channel = %d ptog = %d \n",gfull,channel,ptog,0,0,0);
//      logMsg("rec_rate = %d  rec_cnt = %d\n",rec_rate,rec_cnt,0,0,0,0);
//  if (new_second)
//      logMsg("min = %d sec = %d\n",rec->minute,rec->second,0,0,0,0);
      }

    // If this record is from an HVPS probe then overwrite the id word.

    idx = probeIndex ((char*)&rec->id);
    if (probe_names[idx][0] == 'H') {	// does name start with 'H' ?
      strncpy ((char*)&rec->id, probe_names[idx], strlen (probe_names[idx]));
      hvps_type = TRUE;
      }


    // Check for a full local buffer. Toggle buffers if so.
    if (buf[ptog]->bufFull()){
//      logMsg("2D bufFull  Channel = %d ptog = %d \n",channel,ptog,0,0,0,0);
      gtog = ptog;
      ptog = 1 - ptog;
      rec_full = TRUE;
      rec_cnt = 0;
    }
  }
  return rec_full;
}

/******************************************************************************
** Private Functions
******************************************************************************/

int PmsVme2d::probeInterface(char *intf_base)
{
  char testW = 1;

  if (vxMemProbe(intf_base, VX_WRITE, 1, &testW) == OK)
    interfaceInstalled = true;

printf("2D interface card: %d\n", interfaceInstalled);
}

/******************************************************************************/
int PmsVme2d::probeIndex (char *id)

// Returns the index corresponding the the passed in id, or ERROR if invalid.
{
  if (!strncmp (P1_STR, id, strlen(P1_STR)))
    return P1IDX;
  if (!strncmp (C1_STR, id, strlen(C1_STR)))
    return C1IDX;
  if (!strncmp (H1_STR, id, strlen(H1_STR)))
    return H1IDX;
  if (!strncmp (P2_STR, id, strlen(P2_STR)))
    return P2IDX;
  if (!strncmp (C2_STR, id, strlen(C2_STR)))
    return C2IDX;
  if (!strncmp (H2_STR, id, strlen(H2_STR)))
    return H2IDX;
  return ERROR;
}
