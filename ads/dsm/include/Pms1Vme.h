/* Pms1Vme.h
 
   NCAR Vme based Pms 1d interface class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef PMS1VME_H
#define PMS1VME_H
 
#include <vxWorks.h>
#include <logLib.h>
#include <stdlib.h>
#include <string.h>

#include <dsmctl.h>
#include <header.h>

#define VME1D_16_MASK   0x4             // unload 16 bins control word mask
#define VME1D_32_MASK   0x8             // unload 32 bins control word mask
#define VME1D_64_MASK   0xC             // unload 64 bins control word mask
#define VME1D_SPC_MASK  0x10            // unload spacing bins ctl word mask
#define VME1D_RNG_MASK  0xFFFC          // range bits mask
#define VME1D_FIFO_NOT_EMPTY 0x02       // fifo not empty mask
#define VME1D_INTF_RESET 0x8000         // interface reset mask
#define VME1D_CHAN_BITS 5               // # bits per channel in ctl word
#define VME1D_AUX_USED  8               // current number of aux chans used

// Define the board interface structure.
 
struct Vme1dIntf {
  short cmd_stat;                       // command(w), status (r) register
  short spare1[7];                      // unused
  short fifo_out;                       // output FIFO
  short spare2;                         // long word boundary
};
 
// Define the channel control structure.
 
struct Vme1dChan {
  char *blk[TOG];                       // --> toggling local data blocks
  char *space[TOG];                     // --> toggling local spacing blocks
  int idx;                              // current sample index
  int smp_size;                         // 1 sample block size in bytes
  int blk_size;                         // 1 second data block size in bytes
  int rate;                             // sample rate
  unsigned int bins;                    // number of bins to sample
  int range_ctl_bits;                   // interface range control bits
  int range;                            // probe range
  int spacing;                          // spacing enabled
  int control;                          // interface control word
};
 
class Pms1Vme {
public:
  Pms1Vme (char *base_adr);		// constructor
  int initChan (int chan, int rate, int bins, int range, int spacing);
  int setRange (int chan, int range);   // set range bits
  char* binBlk (int chan);           	// get the current bins block
  char* spcBlk (int chan);           	// get the current spacing block
  void pms1VmeIsr (int hz25_cnt);       // isr for this class
 
private:
  void sample (int hz25_cnt);        	// gets data from the intfc
  void command (int hz25_cnt);       	// issues intfc command
 
  struct Vme1dChan v1d_chan[MX_P1DV2];  // channel control structs
  struct Vme1dIntf *v1d_intf;           // --> board interface struct
  int active;                           // true if chans have been initialized
  int tog;                              // local buffer index
  short control;			// current control word
};

#endif
