/* Vmio12.cc

   OR VMIO12 digital input/output module interface class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 

*/

#include <Vmio12.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Vmio12::Vmio12 (char *base_adr)

// Constructor.
{
  int j;

  vmio = (Vmio*)base_adr;			// interface address

// Reset, configure all ports as inputs, and enable the cios.
  for (j = 0; j < VMIO12_NUM_CIOS; j++) {
    resetCio (j);
    setDirection (j, CIO_PORT_C_INDEX, (unsigned char)0xFF);
    setDirection (j, CIO_PORT_B_INDEX, (unsigned char)0xFF);
    setDirection (j, CIO_PORT_C_INDEX, (unsigned char)0xFF);
    enableCio (j);
  }
}
/*****************************************************************************/

void Vmio12::resetCio (int cio_num)

// Issues a software reset to the specified cio.
{
  unsigned char temp;

  temp = vmio->cio[cio_num].ctl;	// verify in register select state
  vmio->cio[cio_num].ctl = CIO_MICR;		// select master int control reg
  vmio->cio[cio_num].ctl = CIO_MICR_RESET;	// set reset bit
  vmio->cio[cio_num].ctl = CIO_MICR;		// select master int control reg
  vmio->cio[cio_num].ctl = 0x00;		// clear the reset bit

// After the reset bit is cleared, the MICR register needs to be selected
// again to clear the reset ??
  vmio->cio[cio_num].ctl = CIO_MICR;		// select master int control reg
  temp = vmio->cio[cio_num].ctl;		// read the register
  vmio->cio[cio_num].ctl = CIO_MICR;		// select master int control reg
  temp = vmio->cio[cio_num].ctl;		// read the register
}
/*****************************************************************************/

void Vmio12::enableCio (int cio_num)

// Set the port enable bits for the cio.
{
  vmio->cio[cio_num].ctl = CIO_MCCR;            // select master int control reg
  vmio->cio[cio_num].ctl = 
    CIO_MCCR_A_ENABLE | CIO_MCCR_B_ENABLE | CIO_MCCR_C_ENABLE;	
}
/*****************************************************************************/

void Vmio12::enableCioInt (int cio_num, int vector)

// Set the port enable bits for the cio interrupts.
{
  int temp;

  vmio->cio[cio_num].ctl = CIO_A_MSR;            // select port A spec. reg
  vmio->cio[cio_num].ctl = 0x40;

  vmio->cio[cio_num].ctl = CIO_B_MSR;            // select port B spec. reg
  vmio->cio[cio_num].ctl = 0x40;

  vmio->cio[cio_num].ctl = CIO_MICR;            // select master int control reg
  vmio->cio[cio_num].ctl = CIO_MICR_INT_ENABLE;

  vmio->cio[cio_num].ctl = CIO_A_HSR;		// set strobe handshake
  vmio->cio[cio_num].ctl = CIO_A_STROBE_HS;

  vmio->cio[cio_num].ctl = CIO_A_PCSR;		// enable port A interrupts
  vmio->cio[cio_num].ctl = 0x20;

  vmio->cio[cio_num].ctl = CIO_A_PCSR;		// enable port A interrupts
  vmio->cio[cio_num].ctl = CIO_A_PORT_EN;
  vmio->cio[cio_num].ctl = CIO_A_PCSR;          // disable port A interrupts
  vmio->cio[cio_num].ctl = 0x20;


  vmio->cio[cio_num].ctl = CIO_A_IVEC;		// interrupt vector
  vmio->cio[cio_num].ctl = vector;
  vmio->cio[cio_num].ctl = vector;

  vmio->cio[cio_num].ctl = CIO_A_IVEC; 
  temp = vmio->cio[cio_num].ctl;

}
/*****************************************************************************/

void Vmio12::clearCioInt (int cio_num)

// Set the port enable bits for the cio interrupts.
{
  int temp;

  vmio->cio[cio_num].ctl = CIO_A_PCSR;          // disable port A interrupts
  vmio->cio[cio_num].ctl = 0x20;
}
/*****************************************************************************/
 
unsigned char Vmio12::getDirection (int cio_num, int cio_port)
 
// Returns the data direction register for the specified port. 
{
  unsigned char temp;
 
  temp = vmio->cio[cio_num].ctl;           // verify in register select state
 
  switch (cio_port) {
    case CIO_PORT_A_INDEX:
      vmio->cio[cio_num].ctl = CIO_A_DDR;       // port a data direction reg
      break;
    case CIO_PORT_B_INDEX:
      vmio->cio[cio_num].ctl = CIO_B_DDR;       // port b data direction reg
      break;
    case CIO_PORT_C_INDEX:
      vmio->cio[cio_num].ctl = CIO_C_DDR;       // port c data direction reg
      break;
    default:
      fprintf (stderr, "Vmio12: Unknown cio port requested, %d.\n", cio_port);
      exit (ERROR);
  }
  return vmio->cio[cio_num].ctl;               	// return the register
}
/*****************************************************************************/
 
unsigned char Vmio12::getPolarity (int cio_num, int cio_port)
 
// Returns the data polarity register for the specified port. 
{
  unsigned char temp;
 
  temp = vmio->cio[cio_num].ctl;           // verify in register select state
 
  switch (cio_port) {
    case CIO_PORT_A_INDEX:
      vmio->cio[cio_num].ctl = CIO_A_DPPR;       // port a data polarity reg
      break;
    case CIO_PORT_B_INDEX:
      vmio->cio[cio_num].ctl = CIO_B_DPPR;       // port b data polarity reg
      break;
    case CIO_PORT_C_INDEX:
      vmio->cio[cio_num].ctl = CIO_C_DPPR;       // port c data polarity reg
      break;
    default:
      fprintf (stderr, "Vmio12: Unknown cio port requested, %d.\n", cio_port);
      exit (ERROR);
  }
  return vmio->cio[cio_num].ctl;               	// return the register
}
/*****************************************************************************/
 
void Vmio12::setDirection (int cio_num, int cio_port, unsigned char dir)
 
// Sets the data direction for the specified port. A 1 in a bit of dir
// sets the corresponding port bit as an input, a 0 an output.
{
  unsigned char temp;
 
  temp = vmio->cio[cio_num].ctl;           // verify in register select state
 
  switch (cio_port) {
    case CIO_PORT_A_INDEX:
      vmio->cio[cio_num].ctl = CIO_A_DDR;       // port a data direction reg
      break;
    case CIO_PORT_B_INDEX:
      vmio->cio[cio_num].ctl = CIO_B_DDR;       // port b data direction reg
      break;
    case CIO_PORT_C_INDEX:
      vmio->cio[cio_num].ctl = CIO_C_DDR;       // port c data direction reg
      break;
    default:
      fprintf (stderr, "Vmio12: Unknown cio port requested, %d.\n", cio_port);
      exit (ERROR);
  }
  vmio->cio[cio_num].ctl = dir;                 // write the register
}
/*****************************************************************************/
 
void Vmio12::setPolarity (int cio_num, int cio_port, unsigned char pol)
 
// Sets the bit poalrity for the specified port. A 1 in a bit of pol
// sets the corresponding port bit as inverting, a 0 as non-inverting.
{
  unsigned char temp;
 
  temp = vmio->cio[cio_num].ctl;           // verify in register select state
 
  switch (cio_port) {
    case CIO_PORT_A_INDEX:
      vmio->cio[cio_num].ctl = CIO_A_DPPR;       // port a data polarity reg
      break;
    case CIO_PORT_B_INDEX:
      vmio->cio[cio_num].ctl = CIO_B_DPPR;       // port b data polarity reg
      break;
    case CIO_PORT_C_INDEX:
      vmio->cio[cio_num].ctl = CIO_C_DPPR;       // port c data polarity reg
      break;
    default:
      fprintf (stderr, "Vmio12: Unknown cio port requested, %d.\n", cio_port);
      exit (ERROR);
  }
  vmio->cio[cio_num].ctl = pol;                 // write the register
}
/*****************************************************************************/

void Vmio12::enableCounter1 (int cio_num)

// Set the enable bits for counter #1.
{
  unsigned char temp;

  temp = vmio->cio[cio_num].ctl;           // assure register select state

// Set for continuous cycle mode, enable external count input, 
  vmio->cio[cio_num].ctl = CIO_CTMSR1;	// select mode specification reg
  vmio->cio[cio_num].ctl = CIO_CTMSR_CONT_CYCLE | CIO_CTMSR_ECE;

// Set the software gate control bit.
  vmio->cio[cio_num].ctl = CIO_CTCSR1;	// select ct cmd/status reg
  vmio->cio[cio_num].ctl = CIO_CTCSR_GCB;

// Enable the counter timer in the master configuration control register.
  vmio->cio[cio_num].ctl = CIO_MCCR; 	// select master int control reg
  temp = vmio->cio[cio_num].ctl;	// read the register
  vmio->cio[cio_num].ctl = CIO_MCCR; 	// select master int control reg
  vmio->cio[cio_num].ctl = temp | CIO_MCCR_CT1_ENABLE; 	// write the reg
}
/*****************************************************************************/

void Vmio12::enableCounter2 (int cio_num)

// Set the enable bits for counter #2.
{
  unsigned char temp;

  temp = vmio->cio[cio_num].ctl;           // assure register select state

// Set for continuous cycle mode, enable external count input, 
  vmio->cio[cio_num].ctl = CIO_CTMSR2;	// select mode specification reg
  vmio->cio[cio_num].ctl = CIO_CTMSR_CONT_CYCLE | CIO_CTMSR_ECE;

// Set the software gate control bit.
  vmio->cio[cio_num].ctl = CIO_CTCSR2;	// select ct cmd/status reg
  vmio->cio[cio_num].ctl = CIO_CTCSR_GCB;

// Enable the counter timer in the master configuration control register.
  vmio->cio[cio_num].ctl = CIO_MCCR; 	// select master int control reg
  temp = vmio->cio[cio_num].ctl;	// read the register
  vmio->cio[cio_num].ctl = CIO_MCCR; 	// select master int control reg
  vmio->cio[cio_num].ctl = temp | CIO_MCCR_CT2_ENABLE; 	// write the reg
}
/*****************************************************************************/

void Vmio12::setTimeConstant1 (int cio_num, int constant)

// Loads the CIO_TCR1_MSB and CIO_TCR1_LSB registers.
{
  vmio->cio[cio_num].ctl = CIO_TCR1_LSB;	// select lsb reg
  vmio->cio[cio_num].ctl = (unsigned char)constant;	// load lsb
  vmio->cio[cio_num].ctl = CIO_TCR1_MSB;	// select msb reg
  vmio->cio[cio_num].ctl = (unsigned char)(constant >> 8); // load msb
}
/*****************************************************************************/

void Vmio12::setTimeConstant2 (int cio_num, int constant)

// Loads the CIO_TCR2_MSB and CIO_TCR2_LSB registers.
{
  vmio->cio[cio_num].ctl = CIO_TCR2_LSB;	// select lsb reg
  vmio->cio[cio_num].ctl = (unsigned char)constant;	// load lsb
  vmio->cio[cio_num].ctl = CIO_TCR2_MSB;	// select msb reg
  vmio->cio[cio_num].ctl = (unsigned char)(constant >> 8); // load msb
}
/*****************************************************************************/
 
void Vmio12::loadCounter1 (int cio_num)
 
// Loads the current time constant value into counter/timer #1 by setting
// the trigger command bit in the counter/timer command & status register. 
{
  vmio->cio[cio_num].ctl = CIO_CTCSR1;        // select ct cmd/status reg
  vmio->cio[cio_num].ctl = CIO_CTCSR_GCB | CIO_CTCSR_TCB; // write the reg
}
/*****************************************************************************/
 
void Vmio12::loadCounter2 (int cio_num)
 
// Loads the current time constant value into counter/timer #2 by setting
// the trigger command bit in the counter/timer command & status register. 
{
  vmio->cio[cio_num].ctl = CIO_CTCSR2;        	// select ct cmd/status reg
  vmio->cio[cio_num].ctl = CIO_CTCSR_GCB | CIO_CTCSR_TCB; // write the reg
}
/*****************************************************************************/
 
int Vmio12::readCounter1 (int cio_num)
 
// Reads the current count from counter #1.
{
  unsigned int value;

// Latch the current count registers.
  vmio->cio[cio_num].ctl = CIO_CTCSR1;          // select ct cmd/status reg
  vmio->cio[cio_num].ctl = CIO_CTCSR_GCB | CIO_CTCSR_RCC; 

  vmio->cio[cio_num].ctl = CIO_CCR1_MSB;	// select msb register
  value = (unsigned int)vmio->cio[cio_num].ctl;	// read msb
  vmio->cio[cio_num].ctl = CIO_CCR1_LSB;	// select lsb register
  value = (value << 8) | (unsigned int)vmio->cio[cio_num].ctl;	// read lsb

  return (int)value;
}
/*****************************************************************************/
 
int Vmio12::readCounter2 (int cio_num)
 
// Reads the current count from counter #2.
{
  unsigned int value;
 
// Latch the current count registers.
  vmio->cio[cio_num].ctl = CIO_CTCSR2;          // select ct cmd/status reg
  vmio->cio[cio_num].ctl = CIO_CTCSR_GCB | CIO_CTCSR_RCC; 

  vmio->cio[cio_num].ctl = CIO_CCR2_MSB;        // select msb register
  value = (unsigned int)vmio->cio[cio_num].ctl;	// read msb
  vmio->cio[cio_num].ctl = CIO_CCR2_LSB;        // select lsb register
  value = (value << 8) | (unsigned int)vmio->cio[cio_num].ctl;	// read lsb
 
  return (int)value;
}
/*****************************************************************************/
 

int Vmio12::cioPort (int chan) 

// Decodes the cio port index from the channel number.
// Channel numbers 0-7. Port a for even channels, port b for odd channels.
// Channel numbers 8-15, are for c2, c3, of port c.
{
  if ((chan & VMIO12_CHAN_MASK) < VMIO12_NUM_AB_PORTS)
    return ((chan + 1) % CIO_NUM_AB_PORTS) + 1;
  else
    return CIO_PORT_C_INDEX;
}
/*****************************************************************************/


int Vmio12::cioPortStatus (int chan)

// Returns the cio port status from the channel number.
// Channel numbers 0-7. Port a for even channels, port b for odd channels.
// Channel numbers 8-15, are for c2, c3, of port c.
{
  int temp;

  vmio->cio[chan].ctl = CIO_CUR_VEC;          
  temp = vmio->cio[chan].ctl;
  vmio->cio[chan].ctl = CIO_A_IVEC;          
  temp = vmio->cio[chan].ctl;

  vmio->cio[chan].ctl = CIO_A_PCSR;          
  temp = vmio->cio[chan].ctl;
  return temp;
}
