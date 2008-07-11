/* Vmio27Count.cc

   OP VMIO27/28 Counter class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 

*/

#include <Vmio27Count.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Vmio27Count::Vmio27Count (Vmio12 &vio, int chan) : vmio12 (vio)

// Constructor.
{
  char dir;                     // data direction value
  char pol;                     // data polarity value

// Get the corresponding cio number. 
  cio_num = vmio12.cioNumber (chan);

// Configure the cio.
  vmio12.setPolarity (cio_num, CIO_PORT_B_INDEX, (char)0x00);  // non-inverting
  vmio12.setDirection (cio_num, CIO_PORT_B_INDEX, (char)0xFF); // port b input
  vmio12.setPolarity (cio_num, CIO_PORT_A_INDEX, (char)0x00);  // non-inverting
  vmio12.setDirection (cio_num, CIO_PORT_A_INDEX, (char)0x00); // port a output
  vmio12.setPolarity (cio_num, CIO_PORT_C_INDEX, (char)0x00);  // non-inverting
  vmio12.setDirection (cio_num, CIO_PORT_C_INDEX, (char)0x00); // port c output

  mode12 = 0;
  mode34 = 0;
}
/*****************************************************************************/

void Vmio27Count::setMode (int ctr_num, char mode)

// Sets the mode of a counter pair.  Counters 1 and 2 operate in the same
// mode and counters 3 and 4 in the same mode.
{
  switch (ctr_num) {
    case VMIO27_CTR_1:
    case VMIO27_CTR_2:
      mode12 = mode;
      break;

    case VMIO27_CTR_3:
    case VMIO27_CTR_4:
      mode34 = mode << VMIO27_NUM_MODE_BITS;
      break;

    default:
      fprintf (stderr, "Vmio27Count: Unknown counter number requested, %d.\n",
               ctr_num);
      exit(ERROR);
  }
  mode = mode12|mode34;
  printf ("mode = 0x%X\n", mode);
  vmio12.writePort (cio_num, CIO_PORT_A_INDEX, mode12 | mode34);
//resetCounters();
}
/*****************************************************************************/

void Vmio27Count::resetCounters()

// Resets all the counters by setting WE, RD, and C3 active on the chips.
{
  vmio12.writePort (cio_num, CIO_PORT_C_INDEX, 	// rst + wr + rd
                    VMIO27_RST_SEL & VMIO27_WR_ENAB & VMIO27_RD_ENAB); 
  vmio12.writePort (cio_num, CIO_PORT_C_INDEX, 	// lsb + wr 
                    VMIO27_LSB_SEL & VMIO27_WR_ENAB); 
  vmio12.writePort (cio_num, CIO_PORT_C_INDEX, VMIO27_LSB_SEL);	// lsb 
}
/*****************************************************************************/

int Vmio27Count::readCount (int ctr_num)

// Reads the current counter value.
{
  char cs;			// chip select
  char lsb;
  char lmsb;
  char msb;
  char mode;

// Shift the counter number up into the chip select bits.
  cs = (ctr_num << VMIO27_NUM_MODE_BITS) << VMIO27_NUM_MODE_BITS;

// Read the lsb.
  mode = cs | mode34 | mode12;
  printf ("mode = 0x%X\n", mode);
  vmio12.writePort (cio_num, CIO_PORT_A_INDEX, cs | mode34 | mode12); // cs 

  printf ("val = 0x%X\n", VMIO27_LSB_SEL);
  vmio12.writePort (cio_num, CIO_PORT_C_INDEX, VMIO27_LSB_SEL); // setup lsb 

  printf ("val = 0x%X\n", VMIO27_LSB_SEL & VMIO27_RD_ENAB);
  vmio12.writePort (cio_num, CIO_PORT_C_INDEX, 	                // enable rd 
                    VMIO27_LSB_SEL & VMIO27_RD_ENAB); 
  lsb = vmio12.readPort (cio_num, CIO_PORT_B_INDEX);		// read lsb

  printf ("val = 0x%X\n", VMIO27_LMSB_SEL);
  vmio12.writePort (cio_num, CIO_PORT_C_INDEX, VMIO27_LMSB_SEL);// setup lsb+1 

  printf ("val = 0x%X\n", VMIO27_LMSB_SEL & VMIO27_RD_ENAB);
  vmio12.writePort (cio_num, CIO_PORT_C_INDEX, 	                // enable rd 
                    VMIO27_LMSB_SEL & VMIO27_RD_ENAB); 
  lmsb = vmio12.readPort (cio_num, CIO_PORT_B_INDEX);		// read lsb+1

  printf ("val = 0x%X\n", VMIO27_MSB_SEL);
  vmio12.writePort (cio_num, CIO_PORT_C_INDEX, VMIO27_MSB_SEL);// setup msb 
  printf ("val = 0x%X\n", VMIO27_MSB_SEL & VMIO27_RD_ENAB);
  vmio12.writePort (cio_num, CIO_PORT_C_INDEX, 	                // enable rd 
                    VMIO27_MSB_SEL & VMIO27_RD_ENAB); 
  msb = vmio12.readPort (cio_num, CIO_PORT_B_INDEX);		// read msb

  vmio12.writePort (cio_num, CIO_PORT_C_INDEX, VMIO27_LSB_SEL); // disable rd

// Return the composite data value.
  return (msb << 16) + (lmsb << 8) + lsb;
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/
