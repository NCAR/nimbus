/* Synchro.h
 
   IP-Synchro Industry Pack driver.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef SYNCHRO_H
#define SYNCHRO_H

#include <vxWorks.h>
#include <logLib.h>
#include <sysLib.h>
#include <taskLib.h>
#include <SerialBuf.h>
#include <SynchroTable.h>

#define SYNCHRO_DATA_REG_OFFSET	0x1C		// byte offset to the data regs
#define SYNCHRO_CTL 0x02c0			// control word
#define SYNCHRO_NUM_BYTES 2

 
// Define the control registers.
 
 
// Define the board data registers.
typedef struct {
  short ctl;				// control word 
  short vector;
  short xmled;
  short status;
  short xmctl;
  short chan_A_pos_cmp;
  short chan_B_pos_cmp;
  short status2;
  short dummy1;
  short chan_A_int_clr;
  short chan_B_int_clr;
  short chan_A_bit_int_clr;
  short chan_B_bit_int_clr;
  short dummy2;
  short chan_A_pos_data;
  short chan_B_pos_data;
} SynchroReg;
 
class Synchro {
public:
  Synchro (char *base_adr, SynchroTable &at);	// constructor
  void getData (int hz_cnt);
  void test();
  inline char *buf25(){return buf_25[gtog]->getBuf();}
  inline char *buf5()           {return buf_5[gtog]->getBuf();}
  inline char *buf1()           {return buf_1[gtog]->getBuf();}
 
private:
  SynchroTable &synchro_table;		// Synchro sample table
  volatile SynchroReg *ip_reg;		// Synchro registers
  int ctl; 
  SerialBuf *buf_25[TOG];               // 25 hz data buffers
  SerialBuf *buf_5[TOG];                // 5 hz data buffers
  SerialBuf *buf_1[TOG];                // 1 hz data buffers
  int gtog;                             // get buffer index
  int ptog;                             // put buffer index
};

#endif
