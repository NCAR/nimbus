/*
   Spp100.h
   SPP100 probe class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
*/

#ifndef SPP100_H
#define SPP100_H

#include <vxWorks.h>
#include <logLib.h>
#include <string.h>

#include <dsmctl.h>
#include <header.h>
#include <messageDefs.h>
#include <IsioPort.h>
#include <TapeHeader.h>

const short SPP_ACK_ACK =    0x1b01;   	      // Init acknowledge (was 0606)
const short SPP_INIT_ACK =   108;	      // Init ack no. bytes

#define SPP100_LENGTH   53 	              // No. words in setup pkt.
#define SPP100_FIRST_LEN 24		      // No. bytes to range
// #define SPP100_INIT_LEN 10 
#define SPP200_FIRST_LEN 16	
// #define SPP200_INIT_LEN  6 
#define SPP200_LENGTH   47
#define SPP300_FIRST_LEN 20
// #define SPP300_INIT_LEN  7
#define SPP300_LENGTH   48
#define SPP_SMP_10      76
#define SPP_SMP_20      116
#define SPP_SMP_30      156
#define SPP_SMP_200_30  154
#define SPP_SMP_40      196

#define SPP_SEND_DATA {0x1b, 0x02, 0x1d00}
                                       	// ESC, id, cksum

// Port configuration defines.

// #define SPP_BAUD		19200		// baud rate
#define SPP_BAUD		38400		// baud rate
#define SPP_BITS		8		// data bits
#define SPP_PARITY		SER_NONE	// parity
#define SPP_STOP		1.0		// stop bits
#define SPP_FLOW		FALSE		// no flow control

class Spp : public IsioPort {
public:
  Spp (const char *base, TapeHeader& hdr, int probetype, void (*statMsg)(char* msg));
  void	checkStatus ();			// check receiver and port 
  void	secondAlign();			// performs 1 sec data alignment
  void	getSppData();			// get probe data
  int	initSpp (int range);		// setup Spp probe	
  void	isr ();				// isr for this class
  int	firstSample()	{idx = 0; return TRUE;}
  int	nextSample()	{return (idx < RATE_10 - 1)? ++idx: FALSE;}
  const char* buffer();
  int	index()		{return idx;}

private:
  int setup_range;
  int setup_bins;
  int smp_length;
  ushort computeCheckSum (unsigned char *pkt, int len); // compute check sum
  ushort swapushort(ushort x)	{ return(x << 8 | ((x >> 8) & 0x00ff)); }

  void (*statusMsg)(char* msg_str);		// status msg handler
  int   ptog;                                   // put toggle index
  int   gtog;                                   // get toggle index

  struct DMT_blk {
    char  spp_data[sizeof(DMT100_blk)];
  };
  typedef struct DMT_blk DMT_blk;

  struct Spp_blk {
    struct DMT_blk spp[RATE_10];         // block of 10 structs
  };
  typedef struct Spp_blk Spp_blk;

  Spp_blk  spp_blk[TOG];	       		// latest data received
  DMT_blk  spp_reply;
  char	out[sizeof(Spp100_blk)];
  short reply_pkt[SPP100_LENGTH];
  int range;
  int full;
  int idx;
  int good_pkt;
  int which_probe;
  int spp_first_len;
  char msg_str[DSM_MSG_STRING_SIZE];		// status message string
  int new_data;
  int chan;
  int length;
  int length_back;
//  int init_pkt[SPP100_LENGTH];

  struct Init_blk {
    ushort  id;                                 // ESC,id
    ushort  model;                              // model
    ushort  firmware;                           // firmware
    ushort  trig_thresh;                        // trigger threshold
    ushort  transRej;                           // transRej
    ushort  chanCnt;                            // chanCnt
    ushort  dofRej;                             // dofRej
    ushort  range;                              // range
    ushort  avTranWe;                           // avgTransWeight
    ushort  attAccept;                          // attAccept
    ushort  divFlag;                            // divisorflag 0=/2, 1=/4
    ushort  ct_method;                          // ct_method; 1=sum/peak
    ushort  max_width;                          // max_width threshold
    ushort  chksum;                             // cksum
  };
  typedef struct Init_blk Init_blk;

  struct Init100_blk {
    ushort  id;               		        // ESC,id
    ushort  model;				// model
    ushort  trig_thresh;			// trigger threshold
    ushort  transRej;                      	// transRej
    ushort  chanCnt;                       	// chanCnt
    ushort  dofRej;                        	// dofRej
    ushort  range;                         	// range
    ushort  avTranWe;				// avgTransWeight
    ushort  attAccept;				// attAccept
    ushort  divFlag;				// divisorflag 0=/2, 1=/4
    ushort  ct_method;                     	// ct_method; 1=sum/peak
    ushort  max_width;                     	// max_width threshold
    ushort  OPCthreshold[OPCCHAN];		// OPCthreshold[OPCCHAN]
    ushort  chksum;                        	// cksum
  };
  typedef struct Init100_blk Init100_blk;

  struct Init200_blk {
    ushort  id;                            	// ESC,id
    ushort  model;				// model
    ushort  trig_thresh;			// trigger threshold
    ushort  chanCnt;                       	// chanCnt
    ushort  range;                         	// range
    ushort  avTranWe;				// avgTransWeight
    ushort  divFlag;				// divisorflag 0=/2, 1=/4
    ushort  max_width;                     	// max_width threshold
    ushort  OPCthreshold[OPCCHAN];		// OPCthreshold[OPCCHAN]
    ushort  spares[4];
    ushort  chksum;                        	// cksum
  };
  typedef struct Init200_blk Init200_blk;

  struct Init300_blk {
    ushort  id;                            	// ESC,id
    ushort  model;				// model
    ushort  trig_thresh;			// trigger threshold
    ushort  chanCnt;                       	// chanCnt
    ushort  dofRej;                        	// dofRej
    ushort  range;                         	// range
    ushort  avTranWe;				// avgTransWeight
    ushort  divFlag;				// divisorflag 0=/2, 1=/4
    ushort  max_width;                     	// max_width threshold
    ushort  OPCthreshold[OPCCHAN];		// OPCthreshold[OPCCHAN]
    ushort  spares[3];
    ushort  chksum;                        	// cksum
  };
  typedef struct Init300_blk Init300_blk;

  Init100_blk setup100_pkt;
  Init200_blk setup200_pkt;
  Init300_blk setup300_pkt;
};

#endif
