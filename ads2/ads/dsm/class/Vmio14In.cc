/* Vmio14In.cc

   OR VMIO14/16 Digital Input class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 

*/

#include <Vmio14In.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Vmio14In::Vmio14In (Vmio12 *vio, int chan)

// Constructor.
{
  char dir;                     // data direction value
  char pol;                     // data polarity value

// Keep a copy the Vmio12 pointer.
  vmio12 = vio;

// Get the corresponding cio number and cio port for the channel.
  cio_num = vmio12->cioNumber (chan);
  cio_port = vmio12->cioPort (chan);
//  printf ("cio_num = %d, cio_port = %d, chan = %d\n", cio_num, cio_port, chan);

  pol = vmio12->getPolarity (cio_num, CIO_PORT_C_INDEX);		
  dir = vmio12->getDirection (cio_num, CIO_PORT_C_INDEX);

// Bits c0 and c1 control the vmio14 daughter card direction for ports a and b
// respectively.  Set the bits high for input. 
// If configuring port c, set c2 and c3 as non-inverting inputs.
  if (cio_port == CIO_PORT_A_INDEX) {
    pol &= ~CIO_C0_BIT;                      // set c0 non-inverting
    dir &= ~CIO_C0_BIT;                      // set c0 output
  }
  else if (cio_port == CIO_PORT_B_INDEX) {
    pol &= ~CIO_C1_BIT;                      // set c1 non-inverting
    dir &= ~CIO_C1_BIT;                      // set c1 output
  }
  else if (cio_port == CIO_PORT_C_INDEX) {
    pol &= ~(CIO_C2_BIT | CIO_C3_BIT);       // set c2, c3 non-inverting
    dir |= CIO_C2_BIT | CIO_C3_BIT;          // set c2, c3 input
  }
  else {
    fprintf (stderr, "Vmio14In: Illegal cio port requested, %d.\n", cio_port);
    exit (ERROR);
  }
  vmio12->setPolarity (cio_num, CIO_PORT_C_INDEX, pol);
  vmio12->setDirection (cio_num, CIO_PORT_C_INDEX, dir);

// Set the c0 or c1 bit high if configuring for ports a or b, and configure
// the port for inputs.
  if (cio_port == CIO_PORT_A_INDEX) {
    vmio12->writePort(cio_num, CIO_PORT_C_INDEX, CIO_C0_BIT | CIO_C0_BIT_WRITE);
    vmio12->setPolarity (cio_num, cio_port, (char)0x00);	// non-inverting
    vmio12->setDirection (cio_num, cio_port, (char)0xFF);	// all input
  }
  else if (cio_port == CIO_PORT_B_INDEX) {
    vmio12->writePort(cio_num, CIO_PORT_C_INDEX, CIO_C1_BIT | CIO_C1_BIT_WRITE);
    vmio12->setPolarity (cio_num, cio_port, (char)0x00);	// non-inverting
    vmio12->setDirection (cio_num, cio_port, (char)0xFF);	// all input
  }
}
/*****************************************************************************/
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/
