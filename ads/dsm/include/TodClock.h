/* TodClock.h
   Class for MVME162 battery backed time of day clock.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef TODCLOCK_H
#define TODCLOCK_H

#include <stdio.h>

#define TOD_BASE 	0xfffc1ff8

typedef struct {
  char control;
  unsigned char second;
  unsigned char minute;
  unsigned char hour;
  unsigned char day;
  unsigned char date;
  unsigned char month;
  unsigned char year;
} TodClockRegs; 

#define TOD_WRITE		0x80		// write registers control 
#define TOD_READ		0x40		// read registers control  
#define TOD_RUN			0x00		// run control  

class TodClock {
public:
  TodClock ();					// constructor
  int second () 		{return tod_second;}
  int minute () 		{return tod_minute;}
  int hour () 			{return tod_hour;}
  int day () 			{return tod_day;}
  int date () 			{return tod_date;}
  int month () 			{return tod_month;}
  int year () 			{return tod_year;}	// get time values

  void readTime ();				// read time from regs.
  void setTime (int new_year, int new_month, int new_date, 
                int new_hour, int new_min, int new_sec);

private:
  volatile TodClockRegs *tod_regs;

  int tod_second;
  int tod_minute;
  int tod_hour;
  int tod_day;
  int tod_date;
  int tod_month;
  int tod_year;                          // current time variables
};

#endif

