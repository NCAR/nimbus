/* Counter.cc

   Counter sampling class.

   Original Author: Chris Webster
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <NewCounter.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
/* -------------------------------------------------------------------- */
Counter::Counter(CounterTable &ct, IPcounter *ip) : cnt_tbl (ct)
{
  int j;
  int stat;

  IPcnt = ip;

// Create the Vmio14Count digital input port classes.
  for (stat = cnt_tbl.firstEntry(); stat; stat = cnt_tbl.nextEntry()) {

  }

// Create the data buffer classes.
  for (j = 0; j < TOG; j++) {

    if (cnt_tbl.len25()) {
      buf_25[j] = (SerialBuf *)malloc(sizeof(SerialBuf));
      buf_25[j]->SerialBuf(cnt_tbl.len25() * RATE_25);
    }
    else
      buf_25[j] = (SerialBuf *)NULL;

    if (cnt_tbl.len5()) {
      buf_5[j] = (SerialBuf *)malloc(sizeof(SerialBuf));
      buf_5[j]->SerialBuf(cnt_tbl.len5() * RATE_5);
    }
    else
      buf_5[j] = (SerialBuf *)NULL;

    if (cnt_tbl.len1()) {
      buf_1[j] = (SerialBuf *)malloc(sizeof(SerialBuf));
      buf_1[j]->SerialBuf(cnt_tbl.len1());
    }
    else
      buf_1[j] = (SerialBuf *)NULL;
  }

  ptog = 0;
  gtog = 0;

}

/* -------------------------------------------------------------------- */
void Counter::getData (int hz_cnt)

// Checks for and gets data from the data classes.
// This routine is called at a 50 hz rate.
{
  int j;
  int idx = 0;
  unsigned long data;

// If this is the start of a new second, release the last second's buffers
// and toggle the buffers.
  if (!hz_cnt) {
    if (cnt_tbl.cnt25())		// release 25 hz
      buf_25[gtog]->releaseBuf();
    if (cnt_tbl.cnt5())			// release 5 hz
      buf_5[gtog]->releaseBuf();
    if (cnt_tbl.cnt1())			// release 1 hz
      buf_1[gtog]->releaseBuf();

    gtog = ptog;			// toggle buffers
    ptog = ++ptog % TOG;
  }

  IPcounter->readCounters();

  if (hz_cnt % 10 == 0)
    IPcounter->adjustMaster();

// Read 25 hz data.
  for (j = 0; j < cnt_tbl.cnt25; ++j, cnt_tbl.nextEntry()) {
    data = IPcnt->counter(cnt_tbl.portNumber());

    if (cnt_tbl.portType() == CTR_PORT_16)
      data <<= 16;

    buf_25[ptog]->copyToBuf((char *)&data, cnt_tbl.portType() / 8);
  }

}
