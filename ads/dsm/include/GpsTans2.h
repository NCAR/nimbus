/*
   GpsTans2.h
   Trimble Tans III GPS receiver class.
 
   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
*/

#ifndef GPSTANS2_H
#define GPSTANS2_H

#include <vxWorks.h>
#include <logLib.h>
#include <string.h>

#include <header.h>
#include <messageDefs.h>

#include <IsioPort.h>

// GPS packet ids. 
#define GPS_TIME   		0x41   	// gps time packet id 
#define GPS_SAT_SELECT 		0x44   	// gps satellite selection packet id 
#define GPS_RESET		0x45	// receiver reset (power on)
#define GPS_HEALTH   		0x46   	// gps health packet id 
#define GPS_POS_FIX    		0x4A   	// gps position packet id 
#define GPS_MACHINE_CODE	0x4B 	// gps machine code/status packet id
#define GPS_IO_OPTIONS		0x55	// current io options packet id
#define GPS_VEL_FIX    		0x56   	// gps velocity packet id 
#define GPS_FIX_INFO   		0x57   	// information about last fix
 
#define GPS_INIT   	{0x10, 0x35, 0x0E, 0x02, 0x00, 0x00, 0x10, 0x03, 0x10, 0x22, 0x00, 0x10, 0x03} 
                                       	// alt input/output - msl geoid
                                       	// ecef output off 
                                       	// velocity - enu on, ecef off 
                                       	// output gps time asap when calc
					// force auto mode

#define GPS_RQST_HEALTH	{0x10, 0x26, 0x10, 0x03}
#define GPS_SET_ALT {0x10, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x10, 0x03}
 
#define GPS_TYPE_IDX	1		// index of packet type byte
#define GPS_DATA_IDX	2		// index of of first data byte
#define GPS_ADDSTAT_IDX	3		// index of additional status byte
#define GPS_SAT_IDX	3		// index of of first satellite byte
#define GPS_TIME_IDX	2		// index of of time byte
#define GPS_WEEK_IDX	6		// index of of week byte
#define GPS_UTC_IDX	8		// index of of utc offset byte
#define GPOSLEN    20                  	// # gps position data bytes
#define GVELLEN    20                  	// # gps velocity data bytes
#define GPSREC     64                 	// max rx packet length 
 
// Time conversion defines.
#define SECS_PER_DAY      	86400
#define SECS_PER_HOUR      	3600
#define SECS_PER_MIN         	60
#define DAYS_PER_YEAR       	365
#define DAYS_PER_LEAP_YEAR  	366
#define DAYS_PER_WEEK         	7
#define CALENDAR_SIZE        	13
#define LEAP_YEAR_MOD         	4
#define GPS_BASE_YEAR        	80

// Port configuration defines.
#define GPS_TANS_BAUD		9600		// baud rate
#define GPS_TANS_BITS		8		// data bits
#define GPS_TANS_PARITY		SER_ODD		// parity
#define GPS_TANS_STOP		1.0		// stop bits
#define GPS_TANS_FLOW		TRUE		// flow control

class GpsTans2 : public IsioPort {
public:
  GpsTans2 (const char *base, const int port, 
            void (*stat_msg)(char* msg_str),
            void (*set_time)(const int year, const int month, const int day, 
                     const int hour, const int minute, const int second));

  const char* buffer() {return (char*)&gps_blk;} // get current data blk
  void checkStatus ();				// check receiver and port 
  void setAltitude (const float alt);		// send altitude to the rx
  void isr ();					// isr for this class

private:
  int initTans ();				// init the gps rx
  int stuffDle (char *src, char *dest);		// dle stuff a command string
  int calcDateTime (int week, int secs, int gps_off);	// compute date & time

  void (*statusMsg)(char* msg_str);	        // status msg handler
  void (*setTime)(const int year, const int month, const int day,
                  const int hour, const int minute, const int second);
           
  Gps_blk gps_blk;                		// latest gps data received/
  unsigned char gps_buf[GPSREC];		// isr buffer
  int buf_idx;					// isr buffer index
  int odd_dle;					// odd dle count flag
  int rx_rqst;					// receiver initialize request
  int rx_init;					// receiver initialized flag
  int time_rqst;				// time set requested flag
  int time_set;					// time set flag
  int year;					// gps time variables
  int month;
  int day;
  int hour;
  int minute;
  int second;
  int sec_cnt;					// second counter
  char msg_str[DSM_MSG_STRING_SIZE];		// status message string
};

#endif
