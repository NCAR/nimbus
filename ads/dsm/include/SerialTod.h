/* SerialTod.h
 
   Serial Time-Of-Day class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef SERIALTOD_H
#define SERIALTOD_H

#include <vxWorks.h>
#include <logLib.h>
#include <string.h>
 
#include <dsmctl.h>
#include <header.h>
#include <messageDefs.h>
#include <Bc635Vme.h> 
#include <IsioPort.h>

typedef struct {
  int hours;
  int minutes;
  int seconds;
  int days;
} SerialTodData;

const int SERTOD_BAUD =      	9600;           // baud rate
const int SERTOD_BITS =      	8;              // data bits
const int SERTOD_PARITY =    	SER_NONE;       // parity
const float SERTOD_STOP =	1.0;            // stop bits
const int SERTOD_FLOW =      	FALSE;          // no flow control
const int SERTOD_LEN = 27;

class SerialTod : public IsioPort {

public:
  SerialTod (const char *base,const int port,void (*statMsg)(char* msg),
             Bc635Vme *);
  void sendtod();				// send time-of-day 

private:
  void (*statusMsg)(char* msg);           // status msg handler

  SerialTodData st_data;                       // time data
  int *st_datap;
  Bc635Vme *tfp;                        // time-freq processor class
  int ser_port;
  char term_str[1];
  char *datap;
};
#endif
