/* APN232.cc

   APN232 radar altimeter interface class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 

*/

#include <APN232.h>


/* -------------------------------------------------------------------- */
APN232::APN232 (Vmio12 *vio, int chan) : vmio12 (vio) 
{
  char dir;                     // data direction value
  char pol;                     // data polarity value

// Get the corresponding cio number and cio port for the channel.
  cio_num = vmio12->cioNumber (chan);
  cio_port = vmio12->cioPort (chan);

// Get the current port c polarity and direction. 
  pol = vmio12->getPolarity (cio_num, CIO_PORT_C_INDEX);		
  dir = vmio12->getDirection (cio_num, CIO_PORT_C_INDEX);

// Bit c2 is used to toggle between reading the low byte and the high byte from
// the APN232 daughter card.
  if ((cio_port == CIO_PORT_A_INDEX) || (cio_port == CIO_PORT_B_INDEX)) {
    pol &= ~(CIO_C2_BIT | CIO_C3_BIT);	// set c2, c3 non-inverting
    dir &= ~(CIO_C2_BIT | CIO_C3_BIT); 	// set c2, c3 for output
  }
  else {
    fprintf (stderr, "APN232: Illegal cio port requested, %d.\n", cio_port);
    exit (ERROR);
  }

  vmio12->setPolarity (cio_num, CIO_PORT_C_INDEX, pol);
  vmio12->setDirection (cio_num, CIO_PORT_C_INDEX, dir);

// Configure the port for inputs.
  vmio12->setPolarity (cio_num, cio_port, (char)0x00);	// non-inverting
  vmio12->setDirection (cio_num, cio_port, (char)0xFF);	// all input
}

/* -------------------------------------------------------------------- */
short APN232::readPort()
{
  short	val = 0, prevVal;

  // Set c3 high as a diagnostic to provide a window for the read.
  vmio12->writePort(cio_num, CIO_PORT_C_INDEX, CIO_C3_BIT | CIO_C3_BIT_WRITE);


  // Read until you have two consecutive identical samples.
  do
    {
    prevVal = val;

    // Read the high order byte. c2 is high.
    vmio12->writePort(cio_num, CIO_PORT_C_INDEX, CIO_C2_BIT | CIO_C2_BIT_WRITE);
    val = (short)vmio12->readPort(cio_num, cio_port) << 8;

    // Read the low order byte. c2 is low.
    vmio12->writePort (cio_num, CIO_PORT_C_INDEX, CIO_C2_BIT_WRITE);
    val |= (short)vmio12->readPort(cio_num, cio_port);
    }
  while (val != prevVal);


  // Set c3 low.
  vmio12->writePort(cio_num, CIO_PORT_C_INDEX, CIO_C3_BIT_WRITE);

  return(val);

}

/* END APN232.CC */
