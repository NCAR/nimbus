/* Vm3118.cc
   Class for interfacing the VMIC VM3118 A/D board.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions: 2/14/95 - Converted Dsp5602.cc to Vm3118.cc - R.H.

*/

#include <Vm3118.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
Vm3118::Vm3118 (char *base, AnaAuxTable &at) : ana_aux_table (at)
 
// Class constructor.
{
  int j,i;
  cntrl = (Reg3118*)base;
  adc_buf = (short*)(base + VM3118_BUFFER_OFFSET);

  initVm3118();

// Create the data buffer classes.
  for (j = 0; j < TOG; j++) {
    if (ana_aux_table.len25()) {
      buf_25[j] = new SerialBuf (ana_aux_table.len25() * RATE_25);
    }
    else
      buf_25[j] = (SerialBuf*)NULL;

    if (ana_aux_table.len5()) {
      buf_5[j] = new SerialBuf (ana_aux_table.len5() * RATE_5);
    }
    else
      buf_5[j] = (SerialBuf*)NULL;
  }

  ptog = 0;
  gtog = 0;
  hertz50_cnt = 0;
}
/*****************************************************************************/

int Vm3118::getAdcData ()

// Checks for and gets data from the Vm3118.  Returns true if data was acquired.
// This routine is called at a 25 hertz rate.
{
  int j;
  short local_buf[VM3118_MAX_CHAN];     	// temp local buffer

  if (!dataReady())
    return FALSE;
  
// Copy the data from the Vm3118 and put it in the correct sampling order.
// Collect data from the 25 hertz channels.
  for (j = 0, ana_aux_table.firstChannel(); j < ana_aux_table.count25(); 
       j++, ana_aux_table.nextChannel()) { 
    local_buf[j] = *(adc_buf + (ana_aux_table.address() & VM3118_ADDRESS_MASK));
  }
  buf_25[ptog]->copyToBuf ((char*)local_buf, ana_aux_table.len25());

// If this is a 5 hertz sample period, collect data from the 5 hz chans.
  if (!(hertz50_cnt % RATE_10)) {
    for (j = 0; j < ana_aux_table.count5(); j++, ana_aux_table.nextChannel()) { 
      local_buf[j] = *(adc_buf + (ana_aux_table.address() & 
                                  VM3118_ADDRESS_MASK));
    }
    buf_5[ptog]->copyToBuf ((char*)local_buf, ana_aux_table.len5());
  }
  return TRUE;
}
/*****************************************************************************/
 
void Vm3118::initVm3118()
{
  int channel;

// Set all channel gains to 1 initially.
  for (channel = 0; channel < VM3118_MAX_CHAN; channel++) {
    cntrl->csr = (VM3118_CSR_GAIN_MODE + channel);	// select set gain mode
   *(adc_buf + channel) = VM3118_GAIN_1;
  }

// Set the gains of any channels which have a gain of 10.
  for (channel = ana_aux_table.firstChannel(); channel; 
       channel = ana_aux_table.nextChannel()) {
    switch (ana_aux_table.gain()) {
      case 1:
        break;
      case 10:
        *(adc_buf + (ana_aux_table.address() & VM3118_ADDRESS_MASK)) = 
        VM3118_GAIN_10;
        break;
      default:
        fprintf (stderr, "Vm3118: Invalid gain, %2d, on channel %2d\n",
                 ana_aux_table.gain(), ana_aux_table.address());
        exit (ERROR);
    }
  }

// Init control registers.
  cntrl->csr = VM3118_CSR_LED_OFF | VM3118_CSR_TWOS_COMP |
               VM3118_CSR_SINGLE_SCAN; 
}
/*****************************************************************************/

void Vm3118::hertz1_isr ()
 
// 1 hertz alignment isr. Releases and toggles buffers.
// At the start of a new second this routine is called prior to hertz5_isr.
{
  buf_25[gtog]->releaseBuf();
  buf_5[gtog]->releaseBuf();

  gtog = ptog;                                  // toggle buffers
  ptog = ++ptog % TOG;
}
/*****************************************************************************/

