/* Noaa.h

   Noaa instruments  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef NOAA_H
#define NOAA_H

#include <vxWorks.h>
#include <logLib.h>

#include <dsmctl.h>
#include <header.h>
#include <messageDefs.h>

#include <IsioPort.h>

class Noaa : public IsioPort {

public:
  Noaa (const char *base, const int port, void (*statMsg)(const char* msg));
  void checkStatus();				// check serial port status
  void isr();					// isr for this class
  const char* buffer() {return (const char*)&noaa_blk;}

private:
  const NOAA_DATA_LEN =            29;             // data string length
 
  const NOAA_SYNC0_IDX =           0;              // string index for gain
  const char NOAA_SYNC0 =          (char)0x2A;     // string index for gain
  const NOAA_SYNC1_IDX =           1;              // string index for gain
  const char NOAA_SYNC1 =          (char)0x2A;     // string index for gain
 
// Port configuration.
  const NOAA_BAUD =             9600;           // baud rate
  const NOAA_BITS =             8;              // data bits
  const NOAA_PARITY =           SER_NONE;       // parity
  const float NOAA_STOP =       1.0;            // stop bits
  const NOAA_FLOW =             FALSE;          // no flow control
 
 
  Noaa_blk noaa_blk;	         		// local data block
  void (*statusMsg)(const char* msg);           // status msg handler

  char buf[NOAA_DATA_LEN];			// raw data buffer
  char msg[DSM_MSG_STRING_SIZE];       		// status message string
};

#endif
