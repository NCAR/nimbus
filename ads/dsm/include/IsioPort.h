/* IsioPort.h
   Force ISIO-2 serial port class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef ISIOPORT_H
#define ISIOPORT_H

#include <vxWorks.h>
#include <taskLib.h>
#include <sysLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <header.h>
#include <isio.h>
#include <messageDefs.h>
#include <scn68562.h>

class IsioPort {
public:
  IsioPort (const char *base, const int port, const int baud, const int bits,
            const int parity, const float stop, const int flow, 
            void (*stat_msg)(char *msg));
            
  int checkPort();				// check receiver and port 
  char *getData();				// get data from the isio dpram
  int getcnt (char *const str, const int len);	// get counted string
  void igetcnt (const int len);			// get count with interrupt
  void getstr (const char *const term_str);	// get string
  void igetstr (const char *const term_str);	// get string with interrupt
  int putcnt (const char *const str, const int len);	// put counted string
  int pputcnt (const char *const str, const int len);	// put cnt str, no wait
  int clrbuf();					// clear rx input buffer
  int recoff();					// disable port receiver
  int recon();					// enable port receiver
  void enableInterrupt (const int vect, const int level); // enable port ints
  int abort (const int chan);			// abort ops on a channel
  void baudRate(const int baud);		// change baud rate


protected:
  char term_str[3];

private:
  int initPort();				// initialize the port
  int chini2 (const ChiniRegs *const regs);	// init port duscc
  int setoffs (const int);			// set board offset for a chan
  void (*statusMsg)(char* msg);                 // status msg handler

  volatile Isio *isio;				// --> isio board
  int port_num;					// port number
  int rx_chan;					// rx channel number
  int tx_chan;					// tx channel number
  int baud_rate;
  int data_bits;
  int flow_ctl;
  int parity_type;
  float stop_bits; 
  char msg[DSM_MSG_STRING_SIZE];                // status message string
};

#endif

