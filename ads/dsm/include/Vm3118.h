/* Vm3118.h
   Class for interfacing the VM3118 ADC.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions: 2/15/95 - Changed to work with the VMIC VM3118 ADC - R.H.

*/

#ifndef VM3118_H
#define VM3118_H

#include <vxWorks.h>
#include <logLib.h>

#include <AnaAuxTable.h>
#include <SerialBuf.h>

#define VM3118_BUFFER_OFFSET	0x0080	// board offset to data buffer
#define VM3118_MAX_CHAN		64	// max channels on the board
#define VM3118_ADDRESS_MASK	0x00FF	// channel address mask

#define VM3118_GAIN_1		0	// set channel gain to 1
#define VM3118_GAIN_10		1	// set channel gain to 10

#define VM3118_CSR_LED_OFF	0x8000	// turn off self test led
#define VM3118_CSR_TWOS_COMP	0x2000	// output data in 2's complement
#define VM3118_CSR_DATA_READY	0x1000	// conversion complete bit
#define VM3118_CSR_START_SCAN	0x0100	// start conversion sequence bit
#define VM3118_CSR_SINGLE_SCAN	0x0080	// operate in single scan mode
#define VM3118_CSR_GAIN_MODE  	0x00C0 	// Put vm3118 into gain load mode

typedef struct
{
  short bid;                    	// board ID Register (1 word) 
  short csr;                    	// control Status Register 
  short bcr;                    	// buffer Control Register
  short spare1[5];
  short icr;                    	// interrupt Control Register
  short spare2[3];
  short ivr;                    	// interrupt Vector Register
  short spare3[3];
  short tr0;                    	// interval Timer Register 0
  short tr1;                    	// interval Timer Register 1
  short dcr;                    	// data Counter Register
  short tcr;                    	// timer/Counter Control Register
}Reg3118;

class Vm3118 {
public:
  Vm3118 (char* base, AnaAuxTable &at);
  void initVm3118();
  int getAdcData();			// read data from the interface board
  void hertz1_isr();			// 1 hertz alignment isr

  inline void startConversion (int hz50_cnt)	
				{cntrl->csr |= VM3118_CSR_START_SCAN; 
				 hertz50_cnt = hz50_cnt;}
  inline char *buf25() 		{return buf_25[gtog]->getBuf();}
  inline char *buf5() 		{return buf_5[gtog]->getBuf();}

private:
  inline int dataReady()	{return cntrl->csr & VM3118_CSR_DATA_READY;}

  AnaAuxTable &ana_aux_table;		// analog sample table
  Reg3118 *cntrl;
  short *adc_buf;

  SerialBuf *buf_25[TOG];		// 25 hz data buffers
  SerialBuf *buf_5[TOG];		// 5 hz data buffers
  int gtog;				// get buffer index
  int ptog;				// put buffer index
  int hertz50_cnt;			// 50 hertz count of the current sample
};

#endif

