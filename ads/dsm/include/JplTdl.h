/* JplTdl.h

   Jpl TDL  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef JPLTDL_H
#define JPLTDL_H

#include <vxWorks.h>
#include <logLib.h>

#include <dsmctl.h>
#include <header.h>
#include <messageDefs.h>

#include <IsioPort.h>

const int JPL_DATA_LEN =            29;             // data string length
const int JPL_SYNC0_IDX =           0;              // string index for gain
const char JPL_SYNC0 =          (char)0x2A;         // string index for gain
const int JPL_SYNC1_IDX =           1;              // string index for gain
const char JPL_SYNC1 =          (char)0x2A;         // string index for gain
 
// Port configuration.
const int JPLTDL_BAUD =             9600;           // baud rate
const int JPLTDL_BITS =             8;              // data bits
const int JPLTDL_PARITY =           SER_NONE;       // parity
const float JPLTDL_STOP =           1.0;            // stop bits
const int JPLTDL_FLOW =             FALSE;          // no flow control
 
class JplTdl : public IsioPort {

public:
  JplTdl (const char *base, const int port, void (*statMsg)(char* msg));
  void secondAlign();           // performs 1 sec data alignment
  void checkStatus();				// check serial port status
  void isr();					// isr for this class
  void zero();
  void initTdl();
  const char* buffer() {return (char*)&jpltdl_blk[gtog];}

private:
 
  int   ptog;                                   // put toggle index
  int   gtog;                                   // get toggle index 
  int   idx;
  Jpltdl_blk jpltdl_blk[TOG];			// local data block
  void (*statusMsg)(char* msg);                 // status msg handler

  char buf[JPL_DATA_LEN];			// raw data buffer
  char msg[DSM_MSG_STRING_SIZE];       		// status message string
  char term_str[3];                             // read termintation string
};

#endif
