/* GreyVme.h
 
   NCAR/RAF Vme based PMS 2D Grey scale probe interface class.
 
   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef GREYVME_H
#define GREYVME_H

#include <vxWorks.h>
#include <sysLib.h>
#include <taskLib.h>
#include <logLib.h>

#include <SerialBuf.h>

#include <dsmctl.h>
#include <header.h>
#include <messageDefs.h>

#define GREY_GBL_CTL    	0x0000 	// global ctl struct adr
#define GREY_PRB_0_CTL  	0x0040 	// probe 0 ctl struct adr
#define GREY_PRB_1_CTL  	0x0060 	// probe 1 ctl struct adr
#define GREY_GBL_BUF_0   	0x0100 	// probe1 global data buffer
#define GREY_GBL_BUF_1   	0x2100 	// probe2 global data buffer 
#define GREY_DPR_SEM   		0x4000 	// dp ram semaphore
#define GREY_BIM_VCT    	0x8000 	// bim vector register
#define GREY_BIM_CTL    	0x8002 	// bim control register
#define GREY_RESET		0xC000  // software reset register
//#define GREY_DEFAULT_CONTROL	0x0101	// default probe control word
#define GREY_DEFAULT_CONTROL	0x0000	// default probe control word
#define GREY_INT_DISABLE	0x00	// bim ctl reg interrupt disable 
#define GREY_INT_ENABLE		0x18	// bim ctl reg interrupt enable 
#define GREY_INT_MASK		0x1f    // interrupt pending/clear mask
#define GREY_BYTES_PER_CHG_SMP  2	// # bytes per charge sample
#define GREY_MAX_SLICE		128	// max number of slices per particle

typedef struct {                	// Probe control struct
  long id;                      	// probe id, 'G1', 'G2'
  short type;                   	// 0 = none, 1 = GA1, 2 = GA2
  short res;                    	// resolution(microns),25 50 100 200
  short chg_spc;                	// charge spacing (mm), 0 = none
  short chg_loc;                	// charge ring dist from beam (mm)
  short control;                	// control byte to send to the probe
  short spare;
} GreyPctl;

typedef struct {                	// global control struct
  short full[GREY_NUM_PROBES];  	// buffer full lengths
  short hour;                   	// set interface hour
  short minute;                 	// set interface minute
  short sec;                    	// set interface second 
  short set_time;                   	// set time flag 
  short tas;                    	// current true air speed (m/s)
  short go;                     	// control structs initialized 
} GreyGctl;

class GreyVme {
public:
  GreyVme (unsigned char *intf_base, void (*stat_msg)(char* msg_str));
  int initProbe (int channel, char *id, int type, int res, int chg_spc, 
                 int chg_loc);
  void enableProbe (char *id);         	// enable a probe
  void disableProbe (char *id);        	// disable a probe
  void setTas (int tas);               	// set true air speed
  void setTasMode (int mode);        	// set auto or fixed tas mode
  void setControl (char *id, int ctl);	// set probe ctl byte
  void setTime (int hour, int minute, int sec);
  void startSampling();			// set the interface go flag
  void collect();                  	// collect particle blocks from intfc

  inline void enableInterrupt (int vect, int level)
	{*bim_vct = (short)vect; *bim_ctl = (short)(GREY_INT_ENABLE + level);}
  inline void disableInterrupt(){*bim_ctl = GREY_INT_DISABLE;}
  inline int bufFull()          {return buf[gtog]->bufFull();}
  inline int length()           {return buf[gtog]->bufIndex();}
  inline char *buffer()         {return buf[gtog]->getBuf();}
  inline void releaseBuf()      {buf[gtog]->releaseBuf();gtog = 1 - gtog;}
  inline void clearInterrupt()	{*bim_ctl &= GREY_INT_MASK;}        

// Individual particle collecting diagnostic routines, not for regular sampling.
  void collectParticle (int channel);  	// collect individual particles
  inline GreyParticle *particle (int channel) 
				{return (GreyParticle*)part[channel]->getBuf();}
  inline int partLength (int channel)   {return part[channel]->bufIndex();}
  inline void releasePart (int channel) {part[channel]->releaseBuf();}
  inline void releaseSem() 		{*dpr_sem = 1;}

private:
  int probeIndex (char *id);		// get a probe's array index
  inline void getSem() 	{for (*dpr_sem = 0; *dpr_sem & 0x01; *dpr_sem = 0);}
  inline void reset()		{*sw_rst = 0;}

  void (*statusMsg)(char* msg_str);     // status msg handler
  SerialBuf *buf[TOG];			// local particle block buffers 
  SerialBuf *part[TOG];			// local particle buffers 
  GreyGctl *gctl;			// global control struct
  GreyPctl *pctl[GREY_NUM_PROBES];	// probe control structs
  unsigned char *gbuf[GREY_NUM_PROBES];	// interface data buffers
  int ptype[GREY_NUM_PROBES];		// probe types
  short *dpr_sem;			// dual-port ram semaphore
  short *bim_vct;			// bim vector register
  short *bim_ctl;			// bim control register
  short *sw_rst;			// software reset register
  
  struct GreyParticle *cur_part;       	// current particle pointer
  struct GreyParticle *dp_part[GREY_NUM_PROBES]; // current particle in dp ram
  char msg[DSM_MSG_STRING_SIZE];	// error & status msg string
  str4 probe_names[GREY_NUM_PROBES];    // active probe names
  int ptog;                             // current buffer put toggle
  int gtog;                             // current buffer get toggle
  int tas_mode;				// true air speed mode
};
#endif
