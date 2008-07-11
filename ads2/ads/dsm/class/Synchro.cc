/* Synchro.cc

   Green Springs IP-Synchro Industry Pack driver.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 

*/

#include <Synchro.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Synchro::Synchro (char *base_adr, SynchroTable &at) : synchro_table (at)

// Constructor.
{
  int j; 
// Set the register pointers.
   ip_reg = (SynchroReg*)base_adr;

// Init the control register for 16 bit resolution and no interrupts.
  ctl = SYNCHRO_CTL;
  ip_reg->ctl = ctl;

// Create the data buffer classes.
  for (j = 0; j < TOG; j++) {
 
    if (synchro_table.len25()) {
      buf_25[j] = new SerialBuf (synchro_table.len25() * RATE_25);
    }
    else
      buf_25[j] = (SerialBuf*)NULL;
 
    if (synchro_table.len5()) {
      buf_5[j] = new SerialBuf (synchro_table.len5() * RATE_5);
    }
    else
      buf_5[j] = (SerialBuf*)NULL;
 
    if (synchro_table.len1()) {
      buf_1[j] = new SerialBuf (synchro_table.len1());
    }
    else
      buf_1[j] = (SerialBuf*)NULL;
  }
  ptog = 0;
  gtog = 0;
}
/*****************************************************************************/

void Synchro::getData (int hz_cnt)

// Reads data from channel A or B.
{
  int j;
  unsigned short data;
// If this is the start of a new second, release the last second's buffers
// and toggle the buffers.
  if (!hz_cnt) {
    if (synchro_table.cnt25())                // release 25 hz
      buf_25[gtog]->releaseBuf();
    if (synchro_table.cnt5())                 // release 5 hz
      buf_5[gtog]->releaseBuf();
    if (synchro_table.cnt1()){                 // release 1 hz
      buf_1[gtog]->releaseBuf();
    }
    gtog = ptog;                                // toggle buffers
    ptog = ++ptog % TOG;
  }
// Get 25 hz data.
  for (j = 0, synchro_table.firstChannel(); j < synchro_table.cnt25();
       j++, synchro_table.nextChannel()) { 
    if(j==0)  data = ip_reg->chan_A_pos_data; 
    else data = ip_reg->chan_B_pos_data;
    buf_25[ptog]->copyToBuf ((char *)&data, sizeof (data));
  }
// Check if time for 5 hertz sampling.
  if (hz_cnt % RATE_5)
    return;
// Get 5 hz data. 
  for (j = 0; j < synchro_table.cnt5(); j++, synchro_table.nextChannel()) {
    if(j==0) data = ip_reg->chan_A_pos_data;
    else data = ip_reg->chan_B_pos_data;
    buf_5[ptog]->copyToBuf ((char *)&data, sizeof (data));
  }
// Check if time for 1 hertz sampling.
  if (hz_cnt)
    return;
// Get 1 hz data. 
  for (j = 0; j < synchro_table.cnt1(); j++, synchro_table.nextChannel()) {
    if(j==0) data = ip_reg->chan_A_pos_data; 
    else data = ip_reg->chan_B_pos_data;
    data = 2000;
    buf_1[ptog]->copyToBuf ((char *)&data, sizeof (data));
  }
 
}
/*****************************************************************************/
void Synchro::test()

// Read data from channel A and B for test with cksynchro
{
  float fine, course, alt;
//  fine = (float)(ip_reg->chan_A_pos_data)*360.0*84.6666666/65536.;
//  course = (float)(ip_reg->chan_B_pos_data)*360.0*84.6666666/65536.;
  fine = (float)(ip_reg->chan_A_pos_data)*360.0*11.1/65536.;
  course = (float)(ip_reg->chan_B_pos_data)*360.0*277.8/65536.;
  alt = (float)((int)(course/4000.0))*4000.0 + fine;
  printf ("hgmf = %f  hgmc = %f  hgme = %f (ft)\n", 
          fine, course, alt);

}
/*****************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/
