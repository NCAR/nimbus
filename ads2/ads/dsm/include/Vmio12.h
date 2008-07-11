/* Vmio12.h
 
   OR VMIO12 digital input/output module interface class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef VMIO12_H
#define VMIO12_H
 
#include <stdio.h>
#include <stdlib.h>

#define VMIO12_NUM_CIOS		4	// number of cios per board
#define CIO_NUM_PORTS		3	// number of a, b and c ports per cio
#define CIO_NUM_AB_PORTS	2	// number of a and b ports per cio
#define VMIO12_NUM_AB_PORTS	8	// number of a and b ports per board
#define VMIO12_CHAN_MASK	0x0F	// address channel mask
#define CIO_PORT_C_INDEX	0	// port c register index
#define CIO_PORT_B_INDEX	1	// port b register index
#define CIO_PORT_A_INDEX	2	// port a register index 
#define CIO_MAX_COUNT		65535	// maximum counter value

// Register select addresses.
#define CIO_MICR		0x00	// master interrupt control register
#define CIO_MCCR		0x01	// master configuration control reg
#define CIO_A_IVEC		0x02	// interrupt vector register
#define CIO_A_PCSR		0x08	// port command and status register
#define CIO_CTCSR1		0x0A	// ctr/timer #1 command & status reg
#define CIO_CTCSR2		0x0B	// ctr/timer #2 command & status reg
#define CIO_CCR1_MSB		0x10	// ctr/timer #1 current count msb reg
#define CIO_CCR1_LSB		0x11	// ctr/timer #1 current count lsb reg
#define CIO_CCR2_MSB		0x12	// ctr/timer #2 current count msb reg
#define CIO_CCR2_LSB		0x13	// ctr/timer #2 current count lsb reg
#define CIO_TCR1_MSB		0x16	// ctr/timer #1 time constant msb reg
#define CIO_TCR1_LSB		0x17	// ctr/timer #1 time constant lsb reg
#define CIO_TCR2_MSB		0x18	// ctr/timer #2 time constant msb reg
#define CIO_TCR2_LSB		0x19	// ctr/timer #2 time constant lsb reg
#define CIO_CTMSR1		0x1C	// ctr/timer #1 mode specification reg
#define CIO_CTMSR2		0x1D	// ctr/timer #2 mode specification reg
#define CIO_CUR_VEC		0x1F    // current interrupt vector
#define CIO_A_MSR		0x20	// port a specification register
#define CIO_A_HSR		0x21	// port a handshake register
#define CIO_A_DPPR		0x22	// port a data path polarity register
#define CIO_A_DDR		0x23	// port a data direction register
#define CIO_B_MSR		0x28	// port b specification register
#define CIO_B_DPPR		0x2A	// port b data path polarity register
#define CIO_B_DDR		0x2B	// port b data direction register
#define CIO_C_DPPR		0x05	// port c data path polarity register
#define CIO_C_DDR		0x06	// port c data direction register

// Master interrupt control register defines.
#define CIO_MICR_RESET		0x01
#define CIO_MICR_INT_ENABLE	0x80

// Master configuration control register defines.
#define CIO_MCCR_A_ENABLE	0x04
#define CIO_MCCR_C_ENABLE	0x10
#define CIO_MCCR_CT2_ENABLE	0x20
#define CIO_MCCR_CT1_ENABLE	0x40
#define CIO_MCCR_B_ENABLE	0x80

// Bandy setup for interrupt service.
#define CIO_A_STROBE_HS		0x40
#define CIO_A_PORT_EN		0xC0
// Counter/timer command and status register defines.
#define CIO_CTCSR_TCB		0x02	// trigger command bit, loads time const
#define CIO_CTCSR_GCB		0x04	// gate command bit, software gate
#define CIO_CTCSR_RCC		0x08	// read counter control bit

// Counter/timer mode specification register defines.
#define CIO_CTMSR_CONT_CYCLE	0x80	// continuous cycle
#define CIO_CTMSR_ECE		0x20	// external count enable bit
#define CIO_CTMSR_REB		0x04	// retrigger enable bit

// Bits C0 and C1 are used to control the data direction of ports a and b.
#define CIO_C0_BIT		0x01	// control c0 bit in port c
#define CIO_C1_BIT		0x02	// control c1 bit in port c
#define CIO_C2_BIT		0x04	// control c2 bit in port c
#define CIO_C3_BIT		0x08	// control c3 bit in port c

// The upper 4 bits of port c data reg are for write enabling the lower 4 bits.
// To change a lower bit, the corresponding upper bit must be set to 0.
#define CIO_C_BIT_MASK		0x0F 	// mask upper 4 bits
#define CIO_C0_BIT_WRITE	0xE0	// c0 bit write enable mask
#define CIO_C1_BIT_WRITE	0xD0	// c1 bit write enable mask
#define CIO_C2_BIT_WRITE	0xB0	// c2 bit write enable mask
#define CIO_C3_BIT_WRITE	0x70	// c3 bit write enable mask
     				
 
// Define the Z8536 registers as seen from the Vmebus.
typedef struct {
  char unused0;
  unsigned char data;
  char unused1[14];
} CioPort;

typedef struct {
  CioPort port[CIO_NUM_PORTS];		// data registers
  char unused0;
  unsigned char ctl;          		// control registers
  char unused1[14];
} Cio;

typedef struct {
  Cio cio[VMIO12_NUM_CIOS];            // board registers 
} Vmio;
 
class Vmio12 {
public:
  Vmio12 (char *base_adr);			// constructor
  void resetCio (int cio_num);			// software reset of a cio
  void enableCio (int cio_num);			// enable cio ports
  void enableCioInt (int cio_num, int vector);	// enable cio interrupts
  void clearCioInt (int cio_num);		// clear cio interrupts
  unsigned char getDirection (int cio_num, int cio_port);
  unsigned char getPolarity (int cio_num, int cio_port);  
  void setDirection (int cio_num, int cio_port, unsigned char dir);
  void setPolarity (int cio_num, int cio_port, unsigned char pol); 
  void enableCounter1 (int cio_num);		// enable counter/timer #1
  void enableCounter2 (int cio_num);		// enable counter/timer #2
  void setTimeConstant1 (int cio_num, int constant);	   // set ct1 tcr regs
  void setTimeConstant2 (int cio_num, int constant);	   // set ct2 tcr regs
  void loadCounter1 (int cio_num);		// load time constant into ct1
  void loadCounter2 (int cio_num);		// load time constant into ct2
  int readCounter1 (int cio_num);		// read counter #1 current cnt 
  int readCounter2 (int cio_num);		// read counter #2 current cnt 
  int cioPort (int chan);			// decode port index from chan 
  int cioPortStatus (int chan);			// return port interrupt status

  int cioNumber (int chan)		// decode cio index from chan 
               		{return (chan / CIO_NUM_AB_PORTS) % VMIO12_NUM_CIOS;} 
  unsigned char readPort (int cio_num, int cio_port) 
			{return vmio->cio[cio_num].port[cio_port].data;}
  void writePort (int cio_num, int cio_port, unsigned char data) 
			{
// printf("writePort: cio_num=%d, cio_port=%d, data=%x\n", cio_num, cio_port, data);
vmio->cio[cio_num].port[cio_port].data = data;
// printf("writePort: 0x%x\n", vmio->cio[cio_num].port[cio_port].data);
}
 
private:
  Vmio *vmio;					// board pointer 
};

#endif
