/* Ophir3.h

   Ophir 3 radiometer class.
 
   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <vxWorks.h>
#include <logLib.h>

#include <header.h>
#include <dsmctl.h>
#include <messageDefs.h>

#include <IsioPort.h>
#include <Ophir3Blk.h>

#ifndef OPHIR3_H
#define OPHIR3_H

const int OPH3_DATA_LEN = 	55;		// data string length
const int OPH3_ARRAY_LEN =	60;		// data array size

const int OPH3_GAIN_IDX = 	0;		// string index for gain
const int OPH3_BB_IDX = 	2;		// string index for bb_pos
const int OPH3_ZERO_INTVL = 	300;		// # secs between zero checks
const int OPH3_ZERO_DUR =   	5;		// zero check duration in secs

const str2 OPH3_START_CAL = 	"X";		// start calibration
const str2 OPH3_SET_NULL =	"Z";		// select zero 
const str2 OPH3_SET_CTL_BB =	"b";		// select controlled black body

const char OPH3_BB_NORM =	'b';		// normal operation bb code
const char OPH3_BB_ZERO =	'z';		// detector zero bb code
const char OPH3_BB_COOL =	'c';		// full cool bb code
const char OPH3_BB_HEAT =	'h';		// full heat bb code
const char OPH3_GAIN0 =		'0';		// gain 1 char 
const char OPH3_GAIN1 =		'1';		// gain 1 char 
const char OPH3_GAIN2 =		'2';		// gain 2 char 
const char OPH3_GAIN3 =		'3';		// gain 3 char 

// Port configuration.
const int OPHIR3_BAUD =         9600;          	// baud rate
const int OPHIR3_BITS =         8;             	// data bits
const int OPHIR3_PARITY =       SER_NONE;      	// parity
const float OPHIR3_STOP =      	1.0;           	// stop bits
const int OPHIR3_FLOW =         FALSE;         	// no flow control

class Ophir3 : public Ophir3Blk, public IsioPort {

public:
  Ophir3 (const char *base, const int port, void (*statMsg)(char* msg));
  void initOphir();				// start interrupts 
  void checkStatus();				// check serial port status
  void secondAlign();				// Change to new second
  void isr();					// isr for this class
  const char *buffer();
  void startCal();				// start calibration sequence
  void checkZero();				// send zero command 

  void displayRaw() 	{printf ("%s\n", buf[gtog]);}	// display raw buffer


private:
  void (*statusMsg)(char* msg);                 // status msg handler

  int	ptog, gtog;
  int	zero_cnt;				// zero check interval counter
  int	zero_flag;				// zero check in progress flag
  char	bb_state;				// current black body state

  char	buf[TOG][OPH3_DATA_LEN];		// raw data buffer

};

#endif
