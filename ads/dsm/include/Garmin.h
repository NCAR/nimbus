/*
   Garmin.h
   Garmin GPS receiver class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
*/

#ifndef GPSGARMIN_H
#define GPSGARMIN_H

#include <vxWorks.h>
#include <logLib.h>
#include <string.h>
#include <dsmctl.h>

#include <header.h>
#include <messageDefs.h>
#include <IsioPort.h>

// GPS packet ids.
#define GARMIN_START  0x24 
#define GPGGA 	"$GPGGA"          	// receive GGA sentence
#define GPRMC 	"$GPRMC"          	// receive RMC sentence
#define GPGLL 	"$GPGLL"          	// receive GLL sentence
 
// Time conversion defines.
#define SECS_PER_DAY      	86400
#define SECS_PER_HOUR      	3600
#define SECS_PER_MIN         	60
#define DAYS_PER_YEAR       	365
#define DAYS_PER_LEAP_YEAR  	366
#define DAYS_PER_WEEK         	7
#define CALENDAR_SIZE        	13
#define LEAP_YEAR_MOD         	4
#define GARMIN_BASE_YEAR        	80

// Port configuration defines.
const int GARMIN_BAUD =	        9600;		// baud rate
const int GARMIN_BITS =         8;		// data bits
const int GARMIN_PARITY =       SER_NONE;	// parity
const float GARMIN_STOP =       1.0;		// stop bits
const int GARMIN_FLOW =         FALSE;		// no flow control

class Garmin: public IsioPort {
public:
  Garmin (const char *base, const int port, void (*stat_msg)(char* msg_str),
          void (*set_time)(const int year, const int month, const int day,
                const int hour, const int minute, const int second));
  const char* buffer() {return (char*)&garmin_blk[gtog];}
  void checkStatus ();				// check receiver and port 
  void isr ();					// isr for this class
  void initGarmin ();
  void secondAlign ();
  int day()      {return garmin_day;}     // get year field
  int month()    {return garmin_month;}   // get month field
  int year()     {return garmin_year;}    // get day field
  int alt()	 {return garmin_height;}  // get altitude private:

  void (*statusMsg)(char* msg_str);		// status msg handler
  void (*setTime)(const int year, const int month, const int day,
                  const int hour, const int minute, const int second);
  int   ptog;                                   // put toggle index
  int   gtog;                                   // get toggle index
  char term_str[3];	
  Garmin_blk garmin_blk[TOG];          		// latest gps data received/
  char msg_str[DSM_MSG_STRING_SIZE];		// status message string
  int garmin_day;
  int garmin_month;
  int garmin_year;
  int garmin_hour;
  int garmin_minute;
  int garmin_second;
  int garmin_height;
  int time_rqst;
  int date_rqst;
  int date_time;
};

#endif
