/* UvHyg.h
 
   UV Hygrometer class.
 
   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef UVHYG_H
#define UVHYG_H

#include <vxWorks.h>
#include <logLib.h>
 
#include <dsmctl.h>
#include <header.h>
#include <messageDefs.h>
 
#include <IsioPort.h>
#include <UvBlk.h>

const char UVH_ALIGN_CHAR = 	(char)0x2A;	// sample alignment char '*'
const short UVH_ALIGN_FLAG = 	0x2A2A;		// sample alignment flag '**'
const int UVH_SMP_LEN = 	sizeof(struct Uv_smp) + sizeof(UVH_ALIGN_FLAG);
const int UVH_BAUD =       	19200;           // baud rate
const int UVH_BITS =      	8;              // data bits
const int UVH_PARITY =    	SER_NONE;       // parity
const float UVH_STOP =	1.0;            // stop bits
const int UVH_FLOW =      	FALSE;          // no flow control

class UvHyg : public IsioPort {

public:
  UvHyg (const char *base, const int port, void (*statMsg)(char* msg));
  void checkStatus();			// check receiver and port 
  void secondAlign();			// performs 1 sec data alignment
  void isr();				// isr for this class

  const char *buffer()		{ return uv_blk[gtog].buffer(); }
  int index()			{ return uv_blk[gtog].index(); }
  void display()		{ uv_blk[gtog].display(); }

private:
  void (*statusMsg)(char* msg);                 // status msg handler

  int align_cnt;				// data aligned flag
  int full;					// buffer full flag
  int ptog;					// put toggle index
  int gtog;					// get toggle index
  UvBlk uv_blk[TOG];				// toggling local buffers
  char msg[DSM_MSG_STRING_SIZE];       		// message to send to the sun 
};
#endif
