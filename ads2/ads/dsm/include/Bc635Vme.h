/* Bc635Vme.h
   Class for interfacing the BC635VME time and frequency processor.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef BC635VME_H
#define BC635VME_H


#include <stdio.h>
#include <string.h>
#include <logLib.h>
#include <sysLib.h>
#include <taskLib.h>

typedef struct {
  short id;                           	// id (default 0xfef4) 
  short device;                       	// device type (default 0xf350)
  short stat_ctl;               	// status/control register, read/write 
  short unused1;
  short unused2;
  short timereq;                      	// time request, read only
  short time0;                        	// request time 0, read only
  short time1;                        	// request time 1, read only
  short time2;                        	// request time 2, read only
  short time3;                        	// request time 3, read only
  short time4;                        	// request time 4, read only
  short event0;                       	// event 0, read only
  short evt_stb1;                     	// read event 1, write strobe 1
  short evt_stb2;                     	// read event 2, write strobe 2
  short evt_stb3;                     	// read event 3, write strobe 3
  short event4;                       	// event 4, read only
  short unlock;                       	// release capture lockout, read only
  short ack;                          	// data acknowledge, read/write
  short cr0;                          	// control reg 0, read/write
  char unused3;
  char iofifo;                       	// read output fifo, write input fifo
  short mask;                         	// interrupt mask, read/write
  short intstat;                      	// interrupt status, read/write
  short vector;                       	// interrupt vector, read/write
  short level;                        	// interrupt level, read/write
  short unused4[8];
} Bc635Regs; 

#define BCD_DIGIT_MASK		0x000F		// mask to read 1 bcd digit 
#define BCD_2_DIGIT_MASK	0x00FF		// mask to read 2 bcd digits 
#define BCD_DIGIT_SHIFT		4		// bits to shift 1 bcd digits
#define BCD_2_DIGIT_SHIFT	8		// bits to shift 2 bcd digits
#define BCD_3_DIGIT_SHIFT	12		// bits to shift 3 bcd digits
#define BCD_4_DIGIT_SHIFT	16		// bits to shift 4 bcd digits
#define BC635_STROBE_ENAB	0x10		// enable coincidence strobe
#define BC635_STROBE_MINOR	0x20		// set minor strobe mode
#define BC635_CLOCK_RATE	10000000	// set minor strobe mode
#define BC635_MAX_PACKET	20		// max fifo packet length
#define BC635_CALENDAR_SIZE	13		// length of calendar array
#define BC635_LEAP_YEAR_MOD	4		// leap year modulus
#define BC635_PATH_LEAP_YEAR	0x02		// path packet leap year bit
#define SOH			'\x01'		// start of header char
#define ETB			'\x17'		// end transmission block char

class Bc635Vme {
public:
  Bc635Vme (char*);					// constructor
  inline int year() 		{return bc_year;}	// get dec time values
  inline int month() 		{return bc_month;}
  inline int day() 		{return bc_day;}
  inline int hour() 		{return ourHour;}
  inline int minute() 		{return ourMinute;}
  inline int second() 		{return ourSecond;}
//  inline int hour() 		{return bc_hour;}
//  inline int minute() 		{return bc_minute;}
//  inline int second() 		{return bc_second;}
  inline int msec() 		{return bc_msec;}
  int readmsec();
  inline int ihour() 		{return bc_hour;}
  inline int iminute() 		{return bc_minute;}
  inline int isecond() 		{return bc_second;}
  inline int bcdYear() 		{return bcd_year;}	// get bcd time values
  inline int bcdMonth()		{return bcd_month;}
  inline int bcdDay() 		{return bcd_day;}
  inline int bcdHour() 		{return bcd_hour;}
  inline int bcdMinute()	{return bcd_minute;}
  inline int bcdSecond() 	{return bcd_second;}
  inline int julianDay()	{return bc_julian_day;}
  inline int newSecond() 	{return (int)(bc_reg->intstat & (short)0x08);}
  inline void clearNewSecond() 	{bc_reg->intstat = (short)0x08;}
  inline void disableJamSync() 	{path_a |= 0x04;}

  void syncUp();				// startup routine
  void masterSyncUp();				// startup routine
  void readTime();				// read full time from intfc
  int readMsec();				// read only msec from intfc
  void setPeriodicOutput (int rate, int width);	// program periodic output
  void selectTimeCodeMode();			// select irig B sync
  void selectFreeRunMode();			// select free running mode 
  void select1PPSMode();			// select 1PPS sync
  void selectRealTimeClockMode();		// use RTC for time
  void selectModulatedInput();			// select modualted input mode
  void selectModulatedOutput();			// select modualted output mode
  void setPath();				// send default path packet
  void loadDA();				// load default D/A value
  void setRealTimeClock (int new_year, int new_month, int new_day,
                         int new_hour, int new_minute, int new_second);
  void syncRealTimeClock ();			// sync rtc to cur major time
  void setMajorTime (int new_year, int new_month, int new_day,
                     int new_hour, int new_minute, int new_second);
  void jamSync ();				// force minor time to zero
  void enableMinorStrobeInt(int, int, int); 	// enable strobe interrupts
  void strobeIsr ();

private:
  void setMinorStrobeTime();		// set strobe time for next interval
  void writeFifo (char*);		// write a packet to the fifo
  void computeJulianDay();		// julian day from year, month, day
  void decodeJulianDay();		// month, day from year, julian day
  void computeBcdDate();		// bcd date from decimal date
  void setLeapYear();			// enable/disable leap year calcs

  volatile Bc635Regs *bc_reg;
  char packet[BC635_MAX_PACKET];	// fifo packet
  int strobe_msec_intv;			// coincidence strobe interval
  char path_a;				// current path packet data a
  char path_b;				// current path packet data b

  int bc_year;                          // current decimal time variables
  int bc_month;
  int bc_day;
  int bc_hour;
  int bc_minute;
  int bc_second;
  int bc_msec;
  int bc_julian_day;

  int bcd_year;                          // current bcd time variables
  int bcd_month;
  int bcd_day;
  int bcd_hour;
  int bcd_minute;
  int bcd_second;

  int	ourHour, ourMinute, ourSecond;	// Since TFP can't keep time, we'll keep our own.
};

#endif

