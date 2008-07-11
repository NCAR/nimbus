/* Counter.cc

   Counter sampling class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <Counter.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
Counter::Counter (DigitalTable &ct, Vmio12 *vmio12[]) : cnt_tbl (ct)
 
// Class constructor.
{
  int j;
  int stat;

// Null class pointers.
  for (j = 0; j < DIG_MAX_CHAN; j++)
    vmio14_cnt[j] = (Vmio14Count*)0;

// Create the Vmio14Count digital input port classes.
  for (stat = cnt_tbl.firstEntry(); stat; stat = cnt_tbl.nextEntry()) {

// Get port number.
    j = cnt_tbl.portNumber();

    if (!(int)vmio14_cnt[j]) {	// check if class is already created
      vmio14_cnt[j] = new Vmio14Count (vmio12[cnt_tbl.boardNumber()], 
                                  cnt_tbl.address());
    }
  }

// Create the data buffer classes.
  for (j = 0; j < TOG; j++) {

    if (cnt_tbl.len25()) {
      buf_25[j] = new SerialBuf (cnt_tbl.len25() * RATE_25);
    }
    else
      buf_25[j] = (SerialBuf*)NULL;

    if (cnt_tbl.len5()) {
      buf_5[j] = new SerialBuf (cnt_tbl.len5() * RATE_5);
    }
    else
      buf_5[j] = (SerialBuf*)NULL;

    if (cnt_tbl.len1()) {
      buf_1[j] = new SerialBuf (cnt_tbl.len1());
    }
    else
      buf_1[j] = (SerialBuf*)NULL;
  }
  ptog = 0;
  gtog = 0;
}
/*****************************************************************************/

void Counter::getData (int hz_cnt)

// Checks for and gets data from the data classes.
// This routine is called at a 25 hz rate.
{
  int j;
  int idx = 0;
  unsigned short data;

// If this is the start of a new second, release the last second's buffers
// and toggle the buffers.
  if (!hz_cnt) {
    if (cnt_tbl.cnt25())          	// release 25 hz
      buf_25[gtog]->releaseBuf();
    if (cnt_tbl.cnt5())            	// release 5 hz
      buf_5[gtog]->releaseBuf();
    if (cnt_tbl.cnt1())            	// release 1 hz
      buf_1[gtog]->releaseBuf();

    gtog = ptog;                                // toggle buffers
    ptog = 1 - ptog;
  }

// Get 25 hz data.
  for (j = 0, cnt_tbl.firstEntry(); j < cnt_tbl.cnt25(); 
       j++, cnt_tbl.nextEntry()) {

// Determine which counter on the port is to be read, and read the data.
    if (!cnt_tbl.counterNumber()) 
      data = (unsigned short)vmio14_cnt[cnt_tbl.portNumber()]->count1();
    else 
      data = (unsigned short)vmio14_cnt[cnt_tbl.portNumber()]->count2();

    buf_25[ptog]->copyToBuf ((char*)&data, sizeof (data));
  }

// Check if time for 5 hertz sampling. 
  if (hz_cnt % RATE_5)
    return;

// Get 5 hz data.
  for (j = 0; j < cnt_tbl.cnt5(); j++, cnt_tbl.nextEntry()) {
    if (!cnt_tbl.counterNumber()) 
      data = (unsigned short)vmio14_cnt[cnt_tbl.portNumber()]->count1();
    else
      data = (unsigned short)vmio14_cnt[cnt_tbl.portNumber()]->count2();

    buf_5[ptog]->copyToBuf ((char*)&data, sizeof (data));
  }

// Check if time for 1 hertz sampling. 
  if (hz_cnt)
    return;

// Get 1 hz data.
  for (j = 0; j < cnt_tbl.cnt1(); j++, cnt_tbl.nextEntry()) {
    if (!cnt_tbl.counterNumber()) 
      data = (unsigned short)vmio14_cnt[cnt_tbl.portNumber()]->count1();
    else
      data = (unsigned short)vmio14_cnt[cnt_tbl.portNumber()]->count2();

    buf_1[ptog]->copyToBuf ((char*)&data, sizeof (data));
  }
}
/*****************************************************************************/
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

