/* AdcAoa.cc

   Air data computer angle-of-attack interface class.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <AdcAoa.h>

/******************************************************************************
** Public Functions
******************************************************************************/

AdcAoa::AdcAoa (IP429 *ip, int rxchan) 
         
// Constructor.
{
  ip429 = ip;
  rx_chan = rxchan;

  ptog = 0;
  gtog = 0;
  idx1 = 0;
  new_sec = FALSE;
  memset ((char*)adcaoa_blk, 0, sizeof (adcaoa_blk));
  cur_aoa = 0;

}
/*****************************************************************************/
 
void AdcAoa::secAlign ()

// Called at the start of a second to align the indices and toggle buffers.
{
  new_sec = TRUE;			// set the new second flag
  gtog = ptog;                   	// toggle the get buffer index 
}
/*****************************************************************************/

void AdcAoa::readadcIsr ()

// Sampling isr. 
{
  int arinc_data;
  int temp;

// Read the data words from the arinc interface.
  *(short*)(((char*)&arinc_data) + sizeof(short)) = ip429->data1 (rx_chan);
  *(short*)&arinc_data = ip429->data2 (rx_chan);

// Process the data. 
  temp = arinc_data & ARINC_LABEL_MASK;

  if (new_sec) {					// check for new second
    togglePut();					// toggle put buffer
    new_sec = FALSE;
  }

  switch (arinc_data & ARINC_LABEL_MASK) {  	// switch on the arinc label
// The 1 Hz parameters 
    case 0244:
      if (idx1 < RATE_1)
        adcaoa_blk[ptog].aoa = arinc_data;
      break;
  }

}
/******************************************************************************
** Private Functions
******************************************************************************/

void AdcAoa::togglePut ()

// Fills in any missing samples due to walking clocks, resets the buffer
// indices, and toggles the put index.
{
  int dtog;
  dtog = ++ptog % TOG;

  adcaoa_blk[ptog] = adcaoa_blk[dtog];

/*  for (; idx1 < RATE_1; idx1++) {
    adcaoa_blk[ptog].aoa= adcaoa_blk[dtog].aoa;
  }
*/

  idx1 = 0;
  ptog = ++ptog % TOG;
}
/*****************************************************************************/

