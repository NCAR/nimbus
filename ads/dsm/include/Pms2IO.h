/* Pms2IO.h
 
   MVME110 with NCAR I/O channel Pms 2D interface class.
 
   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef PMS2IO_H
#define PMS2IO_H

#include <SerialBuf.h>

#include <header.h>
#include <dsmctl.h>
#include <messageDefs.h>

#define STD_2D_MAX_TAS		125	// max tas for std 2d probes
#define HVPS_MAX_TAS		90	// max tas for hvps probes

// 2d global command struct.
typedef struct {
  short tas;                                  	// true air speed 
  unsigned char rec2d[MX_P2D];                  // select 2d probes 
} G2cmd;
 
// 2D global time struct.
typedef struct {
  short timeflag;			// new time flag
  short hour;                 		
  short minute;               	
  short second;                  
} G2time;

class Pms2IO {
public:
  Pms2IO (char *gcm2, char *gtime2, char *grec2, 
          void (*stat_msg)(char* msg_str)); 
  void initProbe (char* type);		// initialize a probe
  void enableProbe (char *id);		// enable a probe
  void disableProbe (char *id);		// disable a probe
  void setTas (int tas);		// set true air speed
  void setTasMode (int mode);		// set true air speed mode 
  void setRate (int rate);		// set max data record rate
  void setTime (int hour, int minute, int second); // set intfc time
  void collect (int hz_cnt);		// get global data 

  inline int bufFull()		{return buf[gtog]->bufFull();}
  inline char *buffer()    	{return buf[gtog]->getBuf();}
  inline int length()    	{return buf[gtog]->bufIndex();}
  inline void releaseBuf()	{buf[gtog]->releaseBuf();gtog =++gtog % TOG;}

private:
  int probeIndex (char*);		// get index of a probe
  int setProbeControl (int, int);	// write a command to a probe
  int makeDummyData (struct P2d_rec*);	// init a rec with dummy data
  void (*statusMsg)(char* msg_str);     // status msg handler

  SerialBuf *buf[TOG];              	// toggling physical buffers
  int ptog;                             // put buffer index
  int gtog;                             // get buffer index

  G2cmd *g2cmd;				// global command struct
  G2time *g2time;			// global time struct
  P2d_rec *g2rec;			// global data buffer
  str4 probe_names[MX_P2D];		// active probe names
  int rec_rate;				// record rate
  int rec_cnt;				// record count
  int c_cnt;                     	// number of C probes
  int p_cnt;                     	// number of P probes
  int tas_mode;				// tas mode auto or fixed value
  int max_tas;				// max tas value
  char msg[DSM_MSG_STRING_SIZE];
};

#endif
