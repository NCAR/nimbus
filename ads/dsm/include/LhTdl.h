/* LhTdl.h

   LHTDL  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef LHTDL_H
#define LHTDL_H

#include <vxWorks.h>
#include <logLib.h>

#include <dsmctl.h>
#include <header.h>
#include <messageDefs.h>

#include <IsioPort.h>

// Port configuration.
const int LHTDL_DATA_LEN =         60;
const int LHTDL_BAUD =             9600;           // baud rate
const int LHTDL_BITS =             8;              // data bits
const int LHTDL_PARITY =           SER_NONE;       // parity
const float LHTDL_STOP =           1.0;            // stop bits
const int LHTDL_FLOW =             FALSE;          // no flow control
 
class LhTdl : public IsioPort {

public:
  LhTdl (const char *base, const int port, void (*statMsg)(char* msg));
  void secondAlign();           // performs 1 sec data alignment
  void checkStatus();				// check serial port status
  void isr();					// isr for this class
  void zero();
  void initTdl();
  const char* buffer() {return (char*)&lhtdl_blk[gtog];}

private:
 
  int   ptog;                                   // put toggle index
  int   gtog;                                   // get toggle index 
  int   idx;
  LhTdl_blk lhtdl_blk[TOG];			// local data block
  void (*statusMsg)(char* msg);                 // status msg handler

  char buf[LHTDL_DATA_LEN];			// raw data buffer
  char msg[DSM_MSG_STRING_SIZE];       		// status message string
  char term_str[3];                             // read termintation string
};

#endif
