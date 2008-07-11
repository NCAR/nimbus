/* Pms1Vme.cc

   NCAR Vme based Pms 1d interface class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 

*/

#include <Pms1Vme.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Pms1Vme::Pms1Vme (char *base_adr)

// Constructor.
{
  int j;
  int k;

  bzero ((char*)v1d_chan, sizeof (v1d_chan));	// zero the control structs
  v1d_intf = (struct Vme1dIntf*)base_adr;	// interface address
  tog = 0;
  active = 0;					// class is not yet active
  control = 0;
}
/*****************************************************************************/

int Pms1Vme::initChan (int chan, int rate, int bins, int range, int spacing)

// Initialize a channel control struct.  Returns 1 if successful, 0 otherwise.
{
  int control;					// constructed control word
  int j;

// Reset the interface card whenever a channel is initialized.  If more than
// one channel is initialized, unnecessary resets will occur, but it doesn't 
// matter.
  v1d_intf->cmd_stat = VME1D_INTF_RESET;

// Verify the channel number, and previous initialization.
  if ((chan < 0) || (chan >= MX_P1DV2) || (int)(v1d_chan[chan].rate))
    return 0;					

// Verify the sample rate.
  switch (rate) {
    case RATE_10:
    case RATE_5:
    case RATE_1:
      break;
    default:
      return 0;					// invalid rate
  };

// Verify the number of bins, and set the block sizes.
  switch (bins) {
    case BINS_16:
      v1d_chan[chan].smp_size = sizeof (struct P16v2_blk);
      v1d_chan[chan].blk_size = sizeof (struct P16v2_blk) * rate;
      control = VME1D_16_MASK;
      break;
    case BINS_32:
      v1d_chan[chan].smp_size = sizeof (struct P32v2_blk); 
      v1d_chan[chan].blk_size = sizeof (struct P32v2_blk) * rate;
      control = VME1D_32_MASK;
      break;
    case BINS_64:
      v1d_chan[chan].smp_size = sizeof (struct P64v2_blk); 
      v1d_chan[chan].blk_size = sizeof (struct P64v2_blk) * rate;
      control = VME1D_64_MASK;
      break;
    default:
      return 0;					// invalid number of bins
  }

// Allocate memory for a local bin data blocks, and spacing blocks if spacing
// is enabled.
  for (j = 0; j < TOG; j++) {
    if (!(int)(v1d_chan[chan].blk[j] = (char*)malloc(v1d_chan[chan].blk_size)))
      return 0; 
    if (spacing && !(int)(v1d_chan[chan].space[j] = 
                          (char*)malloc(sizeof (struct P1dSpc_blk) * rate))) 
      return 0; 
//  printf ("v1d_chan[0].blk[%1d] = 0x%X\n", v1d_chan[0].blk[j]);
  }

// Load the remainder of the channel control struct.
  v1d_chan[chan].rate = rate;
  v1d_chan[chan].bins = bins;
  v1d_chan[chan].spacing = spacing;

// Build the interface control word.
  if (spacing)
    control |= VME1D_SPC_MASK;

  v1d_chan[chan].control = control << (chan * VME1D_CHAN_BITS);

// Initialize the channel range;
  setRange (chan, range);

// Clear the previously set interface reset bit.
  v1d_intf->cmd_stat = 0;

  active = 1;					// class is active
  return 1;
}
/*****************************************************************************/

int Pms1Vme::setRange (int chan, int range)

// Sets the range for the specified channel.
// Returns 1 if successful, 0 otherwise.
{
// Make sure the probe is a ranging probe, and the range is valid.
  if ((v1d_chan[chan].range >= RANGE_0) &&
    (range >= RANGE_0) && (range <= RANGE_3)) {
    v1d_chan[chan].range = range;			// store the range
    v1d_chan[chan].range_ctl_bits = range;		// build range ctl word
    v1d_chan[chan].range_ctl_bits <<= chan * VME1D_CHAN_BITS;	
    return 1;
  }
  return 0;
}
/*****************************************************************************/

char* Pms1Vme::binBlk (int chan)

// Returns a pointer to the last local bin buffer filled.
{
  struct P16v2_blk *pb;
  int j;

//pb = (struct P16v2_blk*)v1d_chan[chan].blk[(tog + 1) % TOG];
//for (j = 0; j < 16; j++)
//  printf ("%4X ", pb->bins[j]);
//putchar ('\n');
  return v1d_chan[chan].blk[(tog + 1) % TOG];
}
/*****************************************************************************/

char* Pms1Vme::spcBlk (int chan)

// Returns a pointer to the last local spacing buffer filled.
{
  return v1d_chan[chan].space[(tog + 1) % TOG];
}
/*****************************************************************************/

void Pms1Vme::pms1VmeIsr (int hz10_cnt)

// Sampling isr for the Vme Pms 1d interface.
{
  int chan;

  if (!active)
    return;

// If it is a new second, toggle buffers, reset the interrupt counter,
// and reset the sample indices.
  if (!hz10_cnt) {
    tog = (tog + 1) % TOG;
    for (chan = 0; chan < MX_P1DV2; chan++)
      v1d_chan[chan].idx = 0;
  }

// Sample the probes.
///** Comment out to run without a new 1d interface.  
  sample (hz10_cnt);	

// Issue the sampling command for the next interrupt.
  command (hz10_cnt);
//**/
}
/******************************************************************************
** Private Functions
******************************************************************************/

void Pms1Vme::sample (int hz10_cnt)

// Reads data from the interface for the probes being sampled this interrupt.
// The ordering of data from the interface fifo is sizing and spacing data
// for all probes sampled, followed by the housekeeping data for all probes.
{
  struct P64v2_blk *p64;		// --> local bin sample
  struct P1dSpc_blk *pspc;		// --> local spacing sample
  int chan;				// channel index
  int i;
  int j;
  int k;

  if (!control)				// check if a command has been issued
    return;

// First read the particle sizing and spacing for each probe.
  for (chan = 0; chan < MX_P1DV2; chan++) {
 
// Check if this channel is being sampled on this interrupt.
    if (v1d_chan[chan].rate && !(hz10_cnt % (RATE_10 / v1d_chan[chan].rate))) {

// Set the local buffer pointer. The 64 bin pointer type is used for all data
// block types.
      p64 = (struct P64v2_blk*)(v1d_chan[chan].blk[tog] +
              (v1d_chan[chan].idx*v1d_chan[chan].smp_size));
      pspc = (struct P1dSpc_blk*)(v1d_chan[chan].space[tog] + 
               (v1d_chan[chan].idx * sizeof (struct P1dSpc_blk)));

// Put the range word into the sample block.
      p64->rng_flg = v1d_chan[chan].range;
// Read the bin data into the local buffer.
      for (j = 0; j < v1d_chan[chan].bins; j++) {
        p64->bins[j] = v1d_intf->fifo_out;
      }

// Read the spacing data if enabled.
      if (v1d_chan[chan].spacing) {
        for (j = 0; j < BINS_64; j++) {
          pspc->bins[j] = v1d_intf->fifo_out;
        }
      }
    }
  }

// Read the housekeeping data for each probe.
  for (chan = 0; chan < MX_P1DV2; chan++) {

// Check if this channel is being sampled on this interrupt.
    if (v1d_chan[chan].rate && !(hz10_cnt % (RATE_10 / v1d_chan[chan].rate))) {
 
// Set the local buffer pointer. 
      p64 = (struct P64v2_blk*)(v1d_chan[chan].blk[tog] + 
              (v1d_chan[chan].idx * v1d_chan[chan].smp_size));

      for (j = 0; j < VME1D_AUX_USED; j++) {
        p64->aux[j] = v1d_intf->fifo_out;
      }
/*
      logMsg ("hz10_cnt= %d, aux[3]= %x aux[4]= %x aux[5]= %X aux[6]= %x 
              aux[7]= %X\n", hz10_cnt, p64->aux[3], p64->aux[4], p64->aux[5], 
              p64->aux[6], p64->aux[7]);
*/
// Update the channel index for this probe.
      v1d_chan[chan].idx++;
    }
  }

// Make sure the fifo is empty.
  for (k = 0, j = v1d_intf->cmd_stat; (k < 64) && (j & VME1D_FIFO_NOT_EMPTY);
       k++, j = v1d_intf->cmd_stat) {
    i = (int)v1d_intf->fifo_out;
  }
  if (k)
    logMsg ("Pms1Vme: fifo flush count = %d, hz10_cnt = %d\n", 
            k, hz10_cnt, 0,0,0,0);
}
/*****************************************************************************/

void Pms1Vme::command (int hz10_cnt)

// Issues the intfc command for the probes to be sampled on the next interrupt.
{
  int chan;
  int i;
  int j;
  int k;

// Build the composite control word.
  for (chan = 0, control = 0; chan < MX_P1DV2; chan++) {

// Check if this channel is to be sampled on the next interrupt.
    if (v1d_chan[chan].rate && 
        !((hz10_cnt + 1) % (RATE_10 / v1d_chan[chan].rate))) {
      control |= v1d_chan[chan].control + v1d_chan[chan].range_ctl_bits;
    }
// If not, just OR in the range control bits.
    else
      control |= v1d_chan[chan].range_ctl_bits;
  }

// Write the command to the interface, and make sure it gets accepted.
  if (control) {
    v1d_intf->cmd_stat = control;             // issue the command
/**
    logMsg ("Pms1Vme: hz10_cnt= %d, control = 0x%4X\n",
              hz10_cnt, (int)control, 0,0,0,0);
**/
  }
}
/*****************************************************************************/
