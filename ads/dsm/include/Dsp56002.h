/* Dsp56002.h
   Class for interfacing the RAF Motorola 56002 DSP board.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef DSP56002_H
#define DSP56002_H

#include <vxWorks.h>
#include <logLib.h>

#include <AnalogTable.h>
#include <SerialBuf.h>

#define DSP_TABLE_OFFSET	0x0200		// offset to the sample table
#define DSP_BUFFER_OFFSET	0x26E0		// offset to the output buffer
#define DSP_SEMAPHORE_OFFSET	0x8000		// offset to the dp semaphore
#define DSP_BUFFER_1_OFFSET	0x5400		// offset to data buffer #1
#define DSP_BUFFER_2_OFFSET	0x2AE0		// offset to data buffer #2
#define DSP_BUFFER_3_OFFSET	0x4940		// offset to data buffer #1
#define DSP_BUFFER_4_OFFSET	0x1280		// offset to data buffer #2
#define DSP_BUFFER_5_OFFSET	0x0C30		// offset to data buffer #1
#define DSP_BUFFER_6_OFFSET	0x39E0		// offset to data buffer #2

// Dual-port ram semaphore macros.
#define get_sem() for (*dsp_sem = 0; *dsp_sem & 0x01; *dsp_sem = 0)
#define rel_sem() *dsp_sem = 1

class Dsp56002 {
public:
  Dsp56002 (char*, AnalogTable&);		// constructor
  int getDspData();				// get data from the dsp
  void hertz1_isr();				// 1 hertz alignment isr
  int checkTable();				// check for valid table
  void setDataReady();				// set data ready flag
  void assignTable();				// copy smp table to dsp memory
  void assignCalTable();			// copy cal table to dsp memory

  char *buf5000(int i)		{return buf_5000[gtog][i]->getBuf();}
  char *buf1000()		{return buf_1000[gtog]->getBuf();}
  char *buf250()		{return buf_250[gtog]->getBuf();}
  char *buf25() 		{return buf_25[gtog]->getBuf();}
  char *buf5() 			{return buf_5[gtog]->getBuf();}
  int high_rate()		{return ana_tbl.high_rate();}

  AnalogTable &ana_tbl;				// analog sample table
  int gtog;					// get buffer index
  int ptog;					// put buffer index
  SerialBuf *buf_5000[TOG][NUM_HIGH_CHAN];	// 5000 hz data buffers
 
private:
  int dataReady ();				// check the data ready flag
  void clearDataReady ();			// clear the data ready flag

  DspTable *dsp_tbl;				// --> dsp sample table 
  char *base_adr;				// --> dsp board base
  char *dsp_buf_1;				// data buffer #1 address
  char *dsp_buf_2;				// data buffer #2 address
  char *dsp_buf_3;				// data buffer #1 address
  char *dsp_buf_4;				// data buffer #2 address
  char *dsp_buf_5;				// data buffer #1 address
  char *dsp_buf_6;				// data buffer #2 address
  short *dsp_sem;				// --> dsp dp ram semaphore

  SerialBuf *buf_1000[TOG];			// 1000 hz data buffers
  SerialBuf *buf_250[TOG];			// 250 hz data buffers
  SerialBuf *buf_25[TOG];			// 25 hz data buffers
  SerialBuf *buf_5[TOG];				// 5 hz data buffers

  int hz5_lenhigh;		// number of 5000 hz bytes to move at 5 hz intr
  int hz5_len250;		// number of 250 hz bytes to move at 5 hz intr
  int hz5_len25;		// number of 25 hz bytes to move at 5 hz intr
  int hz5_len5;			// number of 5 hz bytes to move at 5 hz intr
};

#endif

