/* MassSpec.h

   Mass Spec  instrument  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef MASSPEC_H
#define MASSPEC_H

#include <vxWorks.h>
#include <logLib.h>

#include <dsmctl.h>
#include <header.h>
#include <messageDefs.h>

#include <IsioPort.h>

class MassSpec : public IsioPort {

public:
  MassSpec (const char *base, const int port, void (*statMsg)(const char* msg));
  void checkStatus();				// check serial port status
  void isr();					// isr for this class
  void secondAlign();                           // performs 1 sec data alignment
  const char* buffer() {return (const char*)&mass_blk[gtog];}

private:

// Port configuration.
  const MASSPEC_BAUD =             9600;           // baud rate
  const MASSPEC_BITS =             8;              // data bits
  const MASSPEC_PARITY =           SER_NONE;       // parity
  const float MASSPEC_STOP =       1.0;            // stop bits
  const MASSPEC_FLOW =             FALSE;          // no flow control
 
  int ptog;                                     // put toggle index
  int gtog;                                     // get toggle index 
  Mass_blk mass_blk[TOG];         		// local data block
  void (*statusMsg)(const char* msg);           // status msg handler

  char buf[sizeof(Mass_blk)];		// raw data buffer
  char msg[DSM_MSG_STRING_SIZE];       		// status message string
};

#endif
