/* NoNoy.h

   NO_NOY  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef NONOY_H
#define NONOY_H

#include <vxWorks.h>
#include <logLib.h>

#include <dsmctl.h>
#include <header.h>
#include <messageDefs.h>

#include <IsioPort.h>

class NoNoy : IsioPort {

public:
  NoNoy (const char *base, const int port, void (*statMsg)(const char* msg));
  void checkStatus();				// check serial port status
  void isr();					// isr for this class
  const char* buffer() {return (const char*)&nonoy_blk;}

private:
  const NONOY_DATA_LEN = sizeof(float)*10 + 11 + sizeof(short)*5; // data length
 
  const NONOY_SYNC0_IDX =           0;              // string index 
  const char NONOY_SYNC0 =          (char)0x2A;     // string index 
  const NONOY_SYNC1_IDX =           1;              // string index 
  const char NONOY_SYNC1 =          (char)0x40;     // string index 
  const char NONOY_SYNC2 =          (char)0x4B;     // string index 
 
// Port configuration.
  const NONOY_BAUD =             9600;           // baud rate
  const NONOY_BITS =             8;              // data bits
  const NONOY_PARITY =           SER_NONE;       // parity
  const float NONOY_STOP =       1.0;            // stop bits
  const NONOY_FLOW =             FALSE;          // no flow control
 
 
  Nonoy_blk nonoy_blk;			// local data block
  void (*statusMsg)(const char* msg);           // status msg handler

  char buf[NONOY_DATA_LEN];			// raw data buffer
  char msg[DSM_MSG_STRING_SIZE];       		// status message string
};

#endif
