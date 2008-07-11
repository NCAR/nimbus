/*
   GpsTans3.h
   Trimble Tans III GPS receiver class.
 
   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
*/

#ifndef GPSTANS3_H
#define GPSTANS3_H

#include <vxWorks.h>
#include <logLib.h>
#include <string.h>

#include <header.h>
#include <messageDefs.h>
#include <IsioPort.h>

// GPS packet ids. 
#define GPS3_INFO		0x3E	 	// user information packet id
#define GPS3_TIME   		0x41           	// gps time packet id 
#define GPS3_SAT_SELECT 	0x44           	// gps satellite selection pkt 
#define GPS3_HEALTH   		0x46           	// gps health packet id 
#define GPS3_POS_FIX    	0x4A          	// gps position packet id 
#define GPS3_IO_OPTIONS		0x55		// current io options packet id
#define GPS3_VEL_FIX    	0x56          	// gps velocity packet id 
#define GPS3_FIX_INFO   	0x57          	// information about last fix
#define GPS3_RESET		0xA7		// receiver reset (power on)
#define GPS3_PKT_STATUS 	0xFF          	// command packet status
 
#define GPS3_INIT   	{0x10, 0x30, 0x05, 0x01, 0x09, 0x01, 0xF0, 0x10, 0x03, 0x10, 0x35, 0x06, 0x0E, 0x01, 0x04, 0x08, 0xDF, 0x10, 0x03, 0x10, 0x22, 0x03, 0x00, 0xFD, 0x10, 0x03} 
                                       	// alt input/output - msl geoid
                                       	// ecef output off 
                                       	// velocity - enu on, ecef off 
                                       	// output gps time asap when calc
					// force auto mode

#define GPS3_RQST_HEALTH	{0x10, 0x26, 0x02, 0xFE, 0x10, 0x03}
#define GPS3_SET_ALT {0x10, 0x2A, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x03}
 
#define GPS3_TYPE_IDX		1	// index of packet type byte
#define GPS3_LEN_IDX		2	// index of packet byte count
#define GPS3_DATA_IDX		3	// index of of first data byte
#define GPS3_ADDSTAT_IDX 	3      	// index of additional status byte
#define GPS3_SAT_IDX    	4      	// index of of first satellite byte
#define GPS3_TIME_IDX		3	// index of of first time byte
#define GPS3_WEEK_IDX		7	// index of of first week byte
#define GPS3_UTC_IDX		9	// index of of first utc offset byte
#define GPOSLEN    		20     	// # gps position data bytes
#define GVELLEN    		20     	// # gps velocity data bytes
#define GPSREC     		64     	// max rx packet length 
 
// Time conversion defines.
#define SECS_PER_DAY      	86400
#define SECS_PER_HOUR      	3600
#define SECS_PER_MIN         	60
#define DAYS_PER_YEAR       	365
#define DAYS_PER_LEAP_YEAR  	366
#define DAYS_PER_WEEK         	7
#define CALENDAR_SIZE        	13
#define LEAP_YEAR_MOD         	4
#define GPS3_BASE_YEAR        	80

// Port configuration defines.
#define GPS3_TANS_BAUD		9600		// baud rate
#define GPS3_TANS_BITS		8		// data bits
#define GPS3_TANS_PARITY	SER_NONE	// parity
#define GPS3_TANS_STOP		1.0		// stop bits
#define GPS3_TANS_FLOW		FALSE		// no flow control

class GpsTans3 : public IsioPort {
public:
  GpsTans3 (const char *base, const int port, 
            void (*stat_msg)(char* msg_str),
            void (*set_time)(const int year, const int month, const int day, 
                    const int hour, const int minute, const int second));
           

  const char* buffer() {return (const char*)&gps_blk;}	// get current data blk
  void checkStatus ();				// check receiver and port 
  void setAltitude (const float alt);		// send altitude to the rx
  void isr ();					// isr for this class

private:
  int initTans ();				// init the gps rx
  void requestHealth ();			// send request health packet

  int computeCheckSum (char *pkt);              // compute packet check sum
  int stuffDle (char *src, char *dest);		// dle stuff a command string
  int calcDateTime (int week, int secs, int gps_off);	// compute date & time

  void (*statusMsg)(char* msg_str);		// status msg handler
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
