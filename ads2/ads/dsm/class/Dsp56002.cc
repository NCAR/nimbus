/* Dsp56002.cc
   Class for interfacing the RAF Motorola 56002 DSP board.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <Dsp56002.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
Dsp56002::Dsp56002 (char *base, AnalogTable &at) : ana_tbl (at)
 
// Class constructor.
{
  int i,j;
  base_adr = base;				// copy the base address
  dsp_tbl = (DspTable*)(base_adr + DSP_TABLE_OFFSET);
  dsp_sem = (short*)(base_adr + DSP_SEMAPHORE_OFFSET);
  dsp_buf_1 = base_adr + DSP_BUFFER_1_OFFSET;
  dsp_buf_2 = base_adr + DSP_BUFFER_2_OFFSET;
  dsp_buf_3 = base_adr + DSP_BUFFER_3_OFFSET;
  dsp_buf_4 = base_adr + DSP_BUFFER_4_OFFSET;
  dsp_buf_5 = base_adr + DSP_BUFFER_5_OFFSET;
  dsp_buf_6 = base_adr + DSP_BUFFER_6_OFFSET;

  assignTable();				// copy the dsp table to memory

// Create the data buffer classes.
  for (j = 0; j < TOG; j++) {
    if (ana_tbl.lenhigh()&& (high_rate() == 5000)) {
      for (i=0; i < NUM_HIGH_CHAN; i++) {
        buf_5000[j][i] = new SerialBuf (sizeof(Mcr_rec));  
      }
    }
    else {
      for (i=0; i < NUM_HIGH_CHAN; i++) 
        buf_5000[j][i] = (SerialBuf*)NULL;
    }

    if (ana_tbl.lenhigh() && (high_rate() == 1000)) {
      buf_1000[j] = new SerialBuf (ana_tbl.lenhigh() * RATE_1000);
    }
    else
      buf_1000[j] = (SerialBuf*)NULL;

    if (ana_tbl.len250()) {
      buf_250[j] = new SerialBuf (ana_tbl.len250() * RATE_250);
    }
    else
      buf_250[j] = (SerialBuf*)NULL;

    if (ana_tbl.len25()) {
      buf_25[j] = new SerialBuf (ana_tbl.len25() * RATE_25);
    }
    else
      buf_25[j] = (SerialBuf*)NULL;

    if (ana_tbl.len5()) {
      buf_5[j] = new SerialBuf (ana_tbl.len5() * RATE_5);
    }
    else
      buf_5[j] = (SerialBuf*)NULL;
  }

// Compute the number of bytes to move at each 5 hz interrupt.
  
  if (high_rate() == 1000) 
  hz5_lenhigh = ana_tbl.lenhigh() * RATE_1000 / 5;  
  else
  hz5_lenhigh = ana_tbl.lenhigh() * RATE_5000 / 5;  // 14 * 5000/5
  hz5_len250 = ana_tbl.len250() * RATE_250 / 5;
  hz5_len25 = ana_tbl.len25() * RATE_25 / 5;
  hz5_len5 = ana_tbl.len5() * RATE_5 / 5; 

  ptog = 0;
  gtog = 0;
}
/*****************************************************************************/

int Dsp56002::getDspData ()

// Checks for and gets data from the dsp.  Returns true if data was acquired.
{
  static int j = 0;
  int i,k;
  int cnt;
  int i2, i3, i4;			// sample indices
  char *dsp_buf;
  static short tbuf[5000];

  if (!dataReady())
    return FALSE;

  dsp_buf = base_adr + (dsp_tbl->buf_adr << 1); 

// Make sure the buffer address received from the DSP is valid.
  if ((dsp_buf != dsp_buf_1) && (dsp_buf != dsp_buf_2) &&
      (dsp_buf != dsp_buf_3) && (dsp_buf != dsp_buf_4) &&
      (dsp_buf != dsp_buf_5) && (dsp_buf != dsp_buf_6)) {
    logMsg ("Dsp56002: Invalid data buffer address received, 0x%X\n", 
            (int)dsp_buf, 0,0,0,0,0);
    clearDataReady();
    return FALSE;
  }
//  j = 0;
// For Mcr, hz5_lenhigh = 14000
/*
  k = hz5_lenhigh & j;
  printf("k = %d\n",k);
  if ((k == 0) && (high_rate() == 5000)) {
*/
  if (((hz5_lenhigh & j) == 0) && (high_rate() == 5000)) {
// Pull in the data from the dsp.
    for (i=0; i < ana_tbl.lenhigh()/2; i++) {
      buf_5000[ptog][i]->copyToBuf (dsp_buf + j, hz5_lenhigh/NUM_HIGH_CHAN);
      j += hz5_lenhigh/NUM_HIGH_CHAN;
    }
/*
    Mcr_rec *m = (Mcr_rec *)buf_5000[ptog][0];
    printf("%d %d %d %d %d %d %d %d %d %d\n",m->data[90],m->data[91],
           m->data[92],m->data[93], m->data[94],m->data[95],m->data[96],
           m->data[97],m->data[98], m->data[99]);
*/
    return TRUE;
  }
  else {
    if (hz5_lenhigh && (high_rate() == 1000)) {			// 1000 hz
      buf_1000[ptog]->copyToBuf (dsp_buf + j, hz5_lenhigh);
      j += hz5_lenhigh;
    }

    if (hz5_len250) {				// 250 hz
      buf_250[ptog]->copyToBuf (dsp_buf + j, hz5_len250);
      j += hz5_len250;
    }

    if (hz5_len25) {				// 25 hz
      buf_25[ptog]->copyToBuf (dsp_buf + j, hz5_len25);
      j += hz5_len25;
    }

    if (hz5_len5) {				// 5 hz
      buf_5[ptog]->copyToBuf (dsp_buf + j, hz5_len5);
    }
    j = 0;
  }
  clearDataReady();
  return TRUE;
}

/*****************************************************************************/
void Dsp56002::hertz1_isr ()
 
// 1 hertz alignment isr. Releases and toggles buffers.
// At the start of a new second this routine is called prior to hertz5_isr.
{
  int i;
  if (high_rate() == 5000)                              // release high hz
    for (i=0; i < NUM_HIGH_CHAN; i++) 
      buf_5000[gtog][i]->releaseBuf();
  if (high_rate() == 1000)                               // release 250 hz
    buf_1000[gtog]->releaseBuf();
  if (hz5_len250)                               // release 250 hz
    buf_250[gtog]->releaseBuf();
  if (hz5_len25)                                // release 25 hz
    buf_25[gtog]->releaseBuf();
  if (hz5_len5)                                 // release 5 hz
    buf_5[gtog]->releaseBuf();

  gtog = ptog;                                  // toggle buffers
  ptog = 1 - ptog;

}

/*****************************************************************************/
int Dsp56002::checkTable ()

// Checks for a valid running flag.

{
  if (dsp_tbl->flag != (short)ANA_RUNNING_FLAG) {
    assignTable();
    return FALSE;
  }
  return TRUE;
}
/*****************************************************************************/
 
void Dsp56002::setDataReady ()
 
// Sets the data ready flag in the dp ram for debugging purposes.
{
  get_sem();
  dsp_tbl->data_ready = TRUE;
  rel_sem();
}
/*****************************************************************************/

void Dsp56002::assignTable ()

// Assign the dsp sample table to the dsp memory.
{
  get_sem();
  *dsp_tbl = ana_tbl.dspTable();		
  dsp_tbl->flag = ANA_SETUP_FLAG;        // new setup flag
  rel_sem();
//  logMsg ("Setup table assigned to the DSP.\n", 0,0,0,0,0,0);
}
/*****************************************************************************/

void Dsp56002::assignCalTable ()

// Assign the dsp sample table to the dsp memory.
{
  get_sem();
  *dsp_tbl = ana_tbl.calTable();		
  dsp_tbl->flag = ANA_SETUP_FLAG;        // new setup flag
  rel_sem();
}
/******************************************************************************
** Private Functions
******************************************************************************/

int Dsp56002::dataReady ()
 
// Checks for available data in the dp ram.
{
  int data_rdy;

  get_sem();
  data_rdy = dsp_tbl->data_ready;
  rel_sem();

  return data_rdy;
}
/*****************************************************************************/
 
void Dsp56002::clearDataReady ()
 
// Clears the data ready flag in the dp ram.
{
  get_sem();
  dsp_tbl->data_ready = FALSE;
  rel_sem();
}
/*****************************************************************************/
