/* PpsGps.h
 
   PPS GPS class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef PPSGPS_H
#define PPSGPS_H

#include <string.h>
#include <vxWorks.h>
#include <logLib.h>
 
#include <dsmctl.h>
#include <header.h>
#include <messageDefs.h>
 
#include <IsioPort.h>

const char PPSGPS_ALIGN_CHAR1 = (char)0xFF;	// sample alignment char '0x81'
const char PPSGPS_ALIGN_CHAR2 = (char)0x81;	// sample alignment char '0x81'
const short PPSGPS_ALIGN_FLAG = 0xFF81;	// sample alignment flag '81FF'
const char PPSGPS_LEN_CHAR =	(char)0x3D;	// data length
const int PPSGPS_SMP_LEN =	sizeof(struct Ppsgps_blk);
const int PPSGPS_SMP_ONE =	1;		// 2 characters=1 word
const int PPSGPS_SMP_FOUR =	4;		// 4 characters=2 words
const int PPSGPS_BAUD =	19200;		// baud rate
const int PPSGPS_BITS =	8;		// data bits
const int PPSGPS_PARITY =	SER_ODD;	// parity
const float PPSGPS_STOP =	1.0;		// stop bits
const int PPSGPS_FLOW =	FALSE;		// no flow control

class PpsGps : public IsioPort {

public:
  PpsGps (const char *base, const int port,
            void (*statMsg)(char* msg),
            void (*set_time)(const int year, const int month, const int day,
                    const int hour, const int minute, const int second));

  void checkStatus();				// check receiver and port 
  void secondAlign();				// performs 1 sec data alignment
  void isr();					// isr for this class
  const char* buffer() {return (const char*)&ppsgps_blk[gtog];}

private:
  void (*statusMsg)(char* msg);           // status msg handler
  void (*setTime)(const int year, const int month, const int day,
                  const int hour, const int minute, const int second);

  float convertDouble(char *dataP);
  void decodeAndSetTime(Ppsgps_blk *gpsP);

  int timeRqst;				// Set time on tfp board?
  int align_cnt;			// data aligned flag
  int full;				// buffer full flag
  int ptog;				// put toggle index
  int gtog;				// get toggle index
  Ppsgps_blk ppsgps_blk[TOG];		// toggling local buffers
  char msg[DSM_MSG_STRING_SIZE];	// message to send to the sun 
};
#endif
