/* Ozone.h

   Noaa ozone instrument  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef OZONE_H
#define OZONE_H

#include <vxWorks.h>
#include <logLib.h>

#include <dsmctl.h>
#include <header.h>
#include <messageDefs.h>

#include <IsioPort.h>

const int OZONE_DATA_LEN =            21;         // data string length
const int OZONE_SYNC0_IDX =           0;          // string index for gain
const char OZONE_SYNC0 =          (char)0x2A;     // string index for gain
const int OZONE_SYNC1_IDX =           1;          // string index for gain
const char OZONE_SYNC1 =          (char)0x2A;     // string index for gain

// Port configuration.
const int OZONE_BAUD =             9600;          // baud rate
const int OZONE_BITS =             8;             // data bits
const int OZONE_PARITY =           SER_NONE;      // parity
const float OZONE_STOP =           1.0;           // stop bits
const int OZONE_FLOW =             FALSE;         // no flow control
 
class Ozone : IsioPort {

public:
  Ozone (const char *base, const int port, void (*statMsg)(const char* msg));
  void checkStatus();				// check serial port status
  void isr();					// isr for this class
  const char* buffer() {return (const char*)&ozone_blk;}

private:
 
  Ozone_blk ozone_blk;	         		// local data block
  void (*statusMsg)(const char* msg);           // status msg handler

  char buf[OZONE_DATA_LEN];			// raw data buffer
  char msg[DSM_MSG_STRING_SIZE];       		// status message string
};

#endif
