/* Aerosol.h

   Aerosol instrument  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef AEROSOL_H
#define AEROSOL_H

#include <vxWorks.h>
#include <logLib.h>

#include <dsmctl.h>
#include <header.h>
#include <messageDefs.h>

#include <IsioPort.h>

class Aerosol : public IsioPort {

public:
  Aerosol (const char *base, const int port, void (*statMsg)(const char* msg));
  void checkStatus();				// check serial port status
  void isr();					// isr for this class
  const char* buffer() {return (const char*)&aerosol_blk;}

private:
  const AEROSOL_NUM_PARMS =	   5;		// No. Aerosol floats

// Port configuration.
  const AEROSOL_BAUD =             9600;           // baud rate
  const AEROSOL_BITS =             8;              // data bits
  const AEROSOL_PARITY =           SER_NONE;       // parity
  const float AEROSOL_STOP =       1.0;            // stop bits
  const AEROSOL_FLOW =             FALSE;          // no flow control
 
 
  Aerosol_blk aerosol_blk;         		// local data block
  void (*statusMsg)(const char* msg);           // status msg handler

  char buf[128];				// raw data buffer
  char msg[DSM_MSG_STRING_SIZE];       		// status message string
};

#endif
