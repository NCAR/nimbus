/* Mca.h

   Mca  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef MCA_H
#define MCA_H

#include <vxWorks.h>
#include <logLib.h>

#include <dsmctl.h>
#include <header.h>
#include <messageDefs.h>

#include <IsioPort.h>

#define MCA_DATA_LEN         532		// No. bytes
#define MCA_STATUS_LEN       20
#define MCA_CMD_LEN          5
 
// Port configuration.
const int MCA_BAUD =             9600;           // baud rate
const int MCA_BITS =             8;              // data bits
const int MCA_PARITY =           SER_NONE;       // parity
const float MCA_STOP =           1.0;            // stop bits
const int MCA_FLOW =             FALSE;          // no flow control
 
class Mca : public IsioPort {

public:
  Mca (const char *base, const int port, void (*statMsg)(char* msg));
  void secondAlign();           // performs 1 sec data alignment
  void checkStatus();		// check serial port status
  void isr();			// isr for this class
  void initMca();
  void reqstData();
  const char* buffer() {return (char*)&mca_blk[gtog];}

private:
 
  int   ptog;                                   // put toggle index
  int   gtog;                                   // get toggle index 
  int   idx;					// scan data array index
  Mca_blk mca_blk[TOG];			        // local data block
  Mca_status mca_status[TOG];		        // local status block
  void (*statusMsg)(char* msg);                 // status msg handler

  char buf[MCA_DATA_LEN];			// raw data buffer
  char msg[DSM_MSG_STRING_SIZE];       		// status message string
  char command[5];

};

#endif
