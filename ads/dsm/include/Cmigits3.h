/*
   Cmigits3.h
   Cmigits3 class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
*/

#ifndef CMIGITS_H
#define CMIGITS_H

#include <vxWorks.h>
#include <logLib.h>
#include <string.h>
#include <dsmctl.h>

#include <header.h>
#include <messageDefs.h>
#include <IsioPort.h>

// Packet ids.
#define SYNC1 	        (char)0xFF   	// Message sync word DEL/SOH
#define SYNC2 		(char)0x81    	// Message sync word 0x81FF
#define SYNC		(unsigned short)0xFF81
#define DEL_SOH		0xFF81
#define CMIGITS_HDR	0x03
#define DISCONNECT	0x02
#define CONNECT		0x04
#define TIMEMARK	0x3
#define NAVSOLUTION	3501
#define PARAMETERCTL	3504
#define C3_STATUS	3500
#define IRS_RESET	0x0
 
#define CMIGITS_RESET	{0x81FF, 0x0, 0x0, 0x02, 0x8201}

// Port configuration defines.
const int CMIGITS_BAUD =       38400;		// baud rate
const int CMIGITS_BITS =       8;		// data bits
const int CMIGITS_PARITY =     SER_ODD;	// parity
const float CMIGITS_STOP =     1.0;		// stop bits
const int CMIGITS_FLOW =       FALSE;		// no flow control

class Cmigits3: public IsioPort {
public:
  Cmigits3 (const char *base, const int port, void (*stat_msg)(char* msg_str));
  char *buffer();
  int  Cmig_sem;
  void checkStatus ();				// check receiver and port 
  void readIsr ();					// isr for this class
  void initCmigits3 ();
  void secondAlign ();
  void gatherData ();

private:

  void (*statusMsg)(char* msg_str);		// status msg handler
  int   ptog;                                   // put toggle index
  int   gtog;                                   // get toggle index
  int   state, pcktLen;
  unsigned short   num_data[TOG];
  int   newSecond;
  char term_str[2];	
  Cmigits3_blk cmigits3_blk;    		// latest gps data received/
  char msg_str[DSM_MSG_STRING_SIZE];		// status message string
  char data_buffer[TOG][4000], *buffer_add;
  ushort swapushort(ushort x)   { return(x << 8 | ((x >> 8) & 0x00ff)); }
};

#endif
