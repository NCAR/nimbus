/* Vmio14Count.cc

   Counter class for the OR VMIO12 with the VMIO14 daughter board.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 

*/

#include <Vmio14Count.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Vmio14Count::Vmio14Count (Vmio12 *vio, int chan)

// Constructor.
{
  int cio_port;
  char dir;                     // data direction value
  char pol;                     // data polarity value

// Copy the Vmio12 class pointer.
  vmio12 = vio;

// Get the cio number from the channel.
  cio_num = vmio12->cioNumber (chan);
  printf ("Vmio14Count: chan = %4X, cio_num = %d.\n", chan, cio_num);

// Verify that the port is port b.
  if (vmio12->cioPort (chan) != CIO_PORT_B_INDEX) {
    fprintf (stderr, "Vmio14Count: Invalid counter port requested, %d.\n", 
             vmio12->cioPort (chan));
    exit (ERROR);
  }

// Bits c0 and c1 control the vmio14 daughter card direction for ports a and b
// respectively.  Set the port b bit high for input.
  pol = vmio12->getPolarity (cio_num, CIO_PORT_C_INDEX);		
  dir = vmio12->getDirection (cio_num, CIO_PORT_C_INDEX);

  pol &= ~CIO_C1_BIT;                      // set c1 non-inverting
  dir &= ~CIO_C1_BIT;                      // set c1 output
  vmio12->setPolarity (cio_num, CIO_PORT_C_INDEX, pol);
  vmio12->setDirection (cio_num, CIO_PORT_C_INDEX, dir);

// Set the c1 bit high.
  vmio12->writePort (cio_num, CIO_PORT_C_INDEX, CIO_C1_BIT | CIO_C1_BIT_WRITE);

// Configure the port;
  vmio12->setPolarity (cio_num, CIO_PORT_B_INDEX, (char)0x00);	// non-invert
  vmio12->setDirection (cio_num, CIO_PORT_B_INDEX, (char)0xFF);	// all input

// Enable the counter/timers #1 and #2.
  vmio12->enableCounter1 (cio_num);	// enable the counters		
  vmio12->enableCounter2 (cio_num);		
  vmio12->setTimeConstant1 (cio_num, CIO_MAX_COUNT);	// load time const regs
  vmio12->setTimeConstant2 (cio_num, CIO_MAX_COUNT);
  vmio12->loadCounter1 (cio_num);	// load counters from time constant
  vmio12->loadCounter2 (cio_num);	
  last_count1 = CIO_MAX_COUNT;
  last_count2 = CIO_MAX_COUNT;
}
/*****************************************************************************/

int Vmio14Count::count1 ()

// Reads counter #1. Returns the computed number of counts since the last read.
{
  int count;
  int cur_count;

  cur_count = vmio12->readCounter1 (cio_num);
//  printf ("last_count1 = %d. cur_count = %d\n", last_count1, cur_count);
  if ((count = last_count1 - cur_count) < 0)
    count += CIO_MAX_COUNT;		// counter rolled over

  last_count1 = cur_count;

  return count;
}
/*****************************************************************************/

int Vmio14Count::count2 ()

// Reads counter #2. Returns the computed number of counts since the last read.
{
  int count;
  int cur_count;

  cur_count = vmio12->readCounter2 (cio_num);
  if ((count = last_count2 - cur_count) < 0)
    count += CIO_MAX_COUNT;		// counter rolled over

  last_count2 = cur_count;

  return count;
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/
