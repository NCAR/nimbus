/* GreyVme.cc
 
   NCAR/RAF Vme based PMS 2D Grey scale probe interface class.
 
   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/
#include <GreyVme.h>

/******************************************************************************
** Public Functions
******************************************************************************/

GreyVme::GreyVme (unsigned char *intf_base, void (*stat_msg)(char* msg_str)) 
                
{
  int j;

// Set interface pointers.
  statusMsg = stat_msg;                         // status message handler
  gctl = (GreyGctl*)(intf_base + GREY_GBL_CTL);
  pctl[0] = (GreyPctl*)(intf_base + GREY_PRB_0_CTL);
  pctl[1] = (GreyPctl*)(intf_base + GREY_PRB_1_CTL);
  gbuf[0] = intf_base + GREY_GBL_BUF_0;
  gbuf[1] = intf_base + GREY_GBL_BUF_1;
  dp_part[0] = (GreyParticle*)gbuf[0];
  dp_part[1] = (GreyParticle*)gbuf[1];
  dpr_sem = (short*)(intf_base + GREY_DPR_SEM);
  bim_vct = (short*)(intf_base + GREY_BIM_VCT);
  bim_ctl = (short*)(intf_base + GREY_BIM_CTL);
  sw_rst = (short*)(intf_base + GREY_RESET);
  releaseSem();

// Issue a software reset to the interface.
  reset();
  taskDelay (sysClkRateGet() / 10);		// delay for reset

// Create the particle and record buffer class.
  for (j = 0; j < TOG; j++) {
    buf[j] = new SerialBuf (MX_PHYS);
    part[j] = new SerialBuf (4096);
  }

// Initialize buffer control variables.
  memset ((char*)probe_names, 0, sizeof(probe_names));
  gtog = 0;
  ptog = 0;

  for (j = 0; j < GREY_NUM_PROBES; j++) 
    ptype[j] = GNONE;

// Set a default true air speed.
  setTasMode (TAS2D_AUTO);
  setTas (TAS2D_DEFAULT);
}
/******************************************************************************/

int GreyVme::initProbe (int channel, char *id, int type, int res, int chg_spc, 
                        int chg_loc) 

// Initializes a probe for sampling.
{
  getSem();
  pctl[channel]->id = *(long*)id;
  pctl[channel]->type = (short)type;
  pctl[channel]->res = (short)res;
  pctl[channel]->chg_spc = (short)chg_spc;
  pctl[channel]->chg_loc = (short)chg_loc;
  pctl[channel]->control = (short)GREY_DEFAULT_CONTROL;
  releaseSem();

  strcpy (probe_names[channel], id);         	// save the id string
  ptype[channel] = type;			// save the probe type 
} 
/******************************************************************************/

void GreyVme::enableProbe (char *id)

// Disables a probe.
{
  int channel;

  if ((channel = probeIndex (id)) != ERROR) {
    pctl[channel]->type = ptype[channel];
    sprintf (msg, "%s probe enabled.\n", id);
  }
  else
    sprintf (msg, "Unknown Grey Scale probe enable requested. --> %s.\n", id);

  statusMsg (msg);
}
/******************************************************************************/

void GreyVme::disableProbe (char *id)

// Disables a probe.
{
  int channel;

  if ((channel = probeIndex (id)) != ERROR) {
    pctl[channel]->type = GNONE;
    sprintf (msg, "%s probe disabled.\n", id);
  }
  else
    sprintf (msg, "Unknown Grey Scale probe disable requested. --> %s.\n", id);

  statusMsg (msg);
}
/******************************************************************************/

void GreyVme::setTas (int tas)

// Sets the true air speed if the tas_mode is TAS2D_AUTO.
{
  if (tas_mode == TAS2D_AUTO) {
    getSem();
    gctl->tas = (short)tas;
    releaseSem();
  }
}
/******************************************************************************/
 
void GreyVme::setTasMode (int mode)
 
// Set the true air speed mode to auto or a fixed tas mode.
{
  if ((tas_mode = mode) != TAS2D_AUTO) {
    getSem();
    gctl->tas = (short)mode;		// set fixed tas
    releaseSem();
  }
}
/******************************************************************************/

void GreyVme::setControl (char *id, int ctl)

// Sets the probe control word.
{
  int channel;

  if ((channel = probeIndex (id)) != ERROR) {
    getSem();
    pctl[channel]->control = (short)ctl;
    releaseSem();
  }
  else
    sprintf (msg,"Unknown Grey Scale probe set control requested. --> %s.", id);

  statusMsg (msg);
}
/******************************************************************************/

void GreyVme::setTime (int hour, int minute, int sec)

// Sets the interface time.
{
  getSem();
  gctl->hour = (short)hour;
  gctl->minute = (short)minute;
  gctl->sec = (short)sec;
  gctl->set_time = TRUE;
  releaseSem();
}
/******************************************************************************/

void GreyVme::startSampling ()

// Sets the interface go flag, which enables sampling.
{
  getSem();
  gctl->go = TRUE;
  releaseSem();
}
/******************************************************************************/

void GreyVme::collect ()

// Copies data from the interface buffers into the local buffers.
{
  int channel;
  int gfull, cntr;			// buffer full number of bytes 
  GreyParticle *p;
  short *s;

  for (channel = 0; channel < GREY_NUM_PROBES; channel++)
    {
    getSem();
    gfull = gctl->full[channel];
    releaseSem();

// If the interface buffer is full, and the local buffer is not full, copy
// in the data.
    if (gfull)
      {
      logMsg("Gfull = %d Channel = %d ptog = %d \n",gfull,channel,ptog,0,0,0);

        gfull *= 2;			// convert buffer length to bytes
        if (!buf[ptog]->bufFull() && buf[ptog]->spaceAvail() >= gfull)
          {
          cur_part = (GreyParticle*)buf[ptog]->getBufPtr(); // keep ptr 
          buf[ptog]->copyToBuf ((char*)gbuf[channel], gfull);
          }
// If the buffer is full, mark the last particle as the end of the record.
// Toggle the buffers, and copy the data to the new buffer.
        else
          {
          buf[ptog]->setBufFull();
          ptog = 1 - ptog;

          if (!buf[ptog]->bufFull())
            {
            cur_part = (GreyParticle*)buf[ptog]->getBufPtr(); // keep ptr 
            buf[ptog]->copyToBuf ((char*)gbuf[channel], gfull);
            }
          }

        getSem();
        gctl->full[channel] = 0;
        releaseSem();
      }
    }
}

/******************************************************************************/
void GreyVme::collectParticle (int channel)

// Copies individual particles from the interface buffers into the local
// particle buffer.  This is used for diagnostics when inidividual particles
// are needed as they are collected from the probe.
{
  int size;
  int gfull;			// buffer full number of bytes 

// If there is a new particle in the buffer, pull it in. 
  if (dp_part[channel]->id[0] == 'G') {
    taskDelay (sysClkRateGet()/5);	// make sure particle is unloaded
    size = sizeof (GreyParticle) + 
           dp_part[channel]->slice_cnt * GREY_BYTES_PER_SLICE +
           dp_part[channel]->chg_len * GREY_BYTES_PER_CHG_SMP;
    part[channel]->copyToBuf ((char*)dp_part[channel], size);
    dp_part[channel]->id[0] = '\0';
//  printf ("sizeof (GreyParticle)=%d, slice_cnt=%d, chg_len=%d, size = %d\n", 
//    sizeof (GreyParticle), dp_part[channel]->slice_cnt,
//    dp_part[channel]->chg_len, size);
    (char*)dp_part[channel] += size;
  }

// Clear the buffer full flag if it is set.
  getSem();
  gfull = gctl->full[channel];
  if (gfull) {
    gctl->full[channel] = 0;
    dp_part[channel] = (GreyParticle*)gbuf[channel];
//  printf ("buffer acquired\n");
  }
  releaseSem();
}
/******************************************************************************
** Private Functions
******************************************************************************/

int GreyVme::probeIndex (char *id)

// Returns the index corresponding the the passed in id, or ERROR if invalid.
{
  int channel;

  for (channel = 0; channel < GREY_NUM_PROBES; channel++) {
    if (!strncmp (probe_names[channel], id, strlen(G1_STR)))
      return channel;
  }
  return ERROR;
}
/******************************************************************************/
