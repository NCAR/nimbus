/* Rdma.h

   Rdma  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef RDMA_H
#define RDMA_H

#include <vxWorks.h>
#include <logLib.h>

#include <dsmctl.h>
#include <header.h>
#include <messageDefs.h>

#include <IsioPort.h>

#define HK	0
#define CNT	1


const int RDMA_HK_LEN =           200;            // data string length
const int SCAN_ARRAY_SIZE =	  200;		  // size of scan array
const int RDMA_DATA_LEN = 	 2000;
 
// Port configuration.
const int RDMA_BAUD =             9600;           // baud rate
const int RDMA_BITS =             8;              // data bits
const int RDMA_PARITY =           SER_NONE;       // parity
const float RDMA_STOP =           1.0;            // stop bits
const int RDMA_FLOW =             FALSE;          // no flow control
 
class Rdma : public IsioPort {

public:
  Rdma (const char *base, const int port, void (*statMsg)(char* msg));
  void secondAlign();           // performs 1 sec data alignment
  void checkStatus();		// check serial port status
  void isr();			// isr for this class
  void initRdma();
  const char* buffer();

private:
 
  int   ptog;                                   // put toggle index
  int   gtog;                                   // get toggle index 
  int   idx;					// scan data array index
  Rdma_blk rdma_blk;				// local data block
  void (*statusMsg)(char* msg);                 // status msg handler

  short dataAvailFlag[2][TOG];			// HK or CNT
  char hk_buf[TOG][120];			// raw data buffer
  char cnt_buf[TOG][RDMA_DATA_LEN];		// raw data buffer
  char msg[DSM_MSG_STRING_SIZE];       		// status message string
  char term_str[3];                             // read termintation string

};

#endif
