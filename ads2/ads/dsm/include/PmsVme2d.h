/* PmsVme2d.h
 
   PMS 2-D VME interface.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef PMSVME2D_H
#define PMSVME2D_H

#include <vxWorks.h>
#include <vxLib.h>
#include <sysLib.h>
#include <taskLib.h>
#include <logLib.h>

#include <SerialBuf.h>

#include <header.h>
#include <dsmctl.h>
#include <messageDefs.h>
#include <dsmVmeDefs.h>

#define PMS2D_GBL_CTL            0x0000  // global ctl struct adr
#define PMS2D_PRB_0_CTL          0x0040  // probe 0 ctl struct adr
#define PMS2D_PRB_1_CTL          0x0060  // probe 1 ctl struct adr
#define PMS2D_GBL_BUF_0          0x1000  // probe0 global data buffer
#define PMS2D_GBL_BUF_1          0x2000  // probe1 global data buffer
#define PMS2D_HDR_BUF_0          0x0080  // probe0 header buffer
#define PMS2D_HDR_BUF_1          0x0100  // probe1 header buffer
#define PMS2D_DPR_SEM_0          0x4000  // dp ram semaphore
#define PMS2D_DPR_SEM_1          0x4002  // dp ram semaphore
#define PMS2D_DPR_SEM_2          0x4004  // dp ram semaphore
#define PMS2D_BIM_VCT            0x8000  // bim vector register
#define PMS2D_BIM_CTL            0x8002  // bim control register
#define PMS2D_RESET              0xC000  // software reset register
#define PMS2D_INT_DISABLE        0x00    // bim ctl reg interrupt disable
#define PMS2D_INT_ENABLE         0x08    // bim ctl reg interrupt enable
#define PMS2D_INT_MASK           0x0f    // interrupt pending/clear mask
 
#define STD_2D_MAX_TAS		125	// max tas for std 2d probes
#define HVPS_MAX_TAS		90	// max tas for hvps probes
#define PMS2D_DATA		4096	// no. bytes in 2D buffer
#define HOUSE_VAR		8	// no. housekeeping variables

// 2d global control struct.
typedef struct {
  short full[PMS2D_NUM_PROBES];			// full flags
  short hour;
  short minute;
  short second;
  short set_time;				// set time flag
  short tas;                                  	// true air speed 
  short go;
} Pms2dGctl;
 
// 2d probe control struct
typedef struct {
  short id;				// probe id: 'P1', 'P2', 'C1', 'H1',..
  short fill;
  short type;				// 0=none, 1 = C probe, 2 = P probe
					// 3 = HV probe
  short res;				// resolution(microns) 25,50,100,200
} Pms2dPctl;


class PmsVme2d {
public:
  PmsVme2d (unsigned char *intf_base, void (*stat_msg)(char* msg_str));
  void initProbe (int channel, char *id, int type, int res);
  void enableProbe (char *id);		// enable a probe
  void disableProbe (char *id);		// disable a probe
  void setTas (int tas);		// set true air speed
  void setTasMode (int mode);		// set true air speed mode 
  void setRate (int rate);              // set max data record rate
  void startSampling();                 // set the interface go flag
  void setTime (int hour, int minute, int second); // set intfc time
  short collect (int hz_cnt);		// get global data 
  inline void enableInterrupt (int vect, int level)
        {*bim_vct = (short)vect; *bim_ctl = (short)(PMS2D_INT_ENABLE + level);}
  inline void disableInterrupt(){*bim_ctl = PMS2D_INT_DISABLE;}
  inline void clearInterrupt()  {*bim_ctl &= PMS2D_INT_MASK;}

  inline int bufFull()		{return buf[gtog]->bufFull();}
  inline char *buffer()    	{return buf[gtog]->getBuf();}
  inline int length()    	{return buf[gtog]->bufIndex();}
  inline void releaseBuf()	{buf[gtog]->releaseBuf();}
//  inline void releaseBuf()	{buf[gtog]->releaseBuf();gtog = 1 - gtog;}

private:
  int probeIndex(char *id);		// get a probe's array index
  int probeInterface(char *);		// test for interface card.

  inline void getSem_0()       {for (*dpr_sem_0 = 0; *dpr_sem_0 & 0x01; 
                                  *dpr_sem_0 = 0);}
  inline void releaseSem_0()   {*dpr_sem_0 = 1;}

  inline void getSem_1()       {for (*dpr_sem_1 = 0; *dpr_sem_1 & 0x01; 
                                  *dpr_sem_1 = 0);}
  inline void releaseSem_1()   {*dpr_sem_1 = 1;}

  inline void getSem_2()       {for (*dpr_sem_2 = 0; *dpr_sem_2 & 0x01; 
                                  *dpr_sem_2 = 0);}
  inline void releaseSem_2()   {*dpr_sem_2 = 1;}

  inline void reset()           {*sw_rst = 0;}
 
  int setProbeControl (int, int);	// write a command to a probe
  int makeDummyData (struct P2d_rec*);	// init a rec with dummy data
  void (*statusMsg)(char* msg_str);     // status msg handler

  SerialBuf *buf[TOG];              	// toggling physical buffers
  SerialBuf *part[TOG];                 // local particle buffers

  int ptog;                             // put buffer index
  int gtog;                             // get buffer index
  int hvps_type;

  Pms2dGctl *gctl;			// global control struct
  Pms2dPctl *pctl[PMS2D_NUM_PROBES];	// global particle struct
  P2d_data *g2rec[PMS2D_NUM_PROBES];	// global data buffer
  P2d_hdr *h2rec[PMS2D_NUM_PROBES];	// global data buffer

  str4 probe_names[MX_P2D];		// active probe names
  int ptype[PMS2D_NUM_PROBES];          // probe types

  short *dpr_sem_0;                     // dual-port ram semaphore probe 0
  short *dpr_sem_1;                     // dual-port ram semaphore probe 1
  short *dpr_sem_2;                     // dual-port ram semaphore tas
  short *bim_vct;                       // bim vector register
  short *bim_ctl;                       // bim control register
  short *sw_rst;                        // software reset register

  int rec_rate;				// record rate
  int rec_cnt;				// record count
  int c_cnt;                     	// number of C probes
  int p_cnt;                     	// number of P probes
  int tas_mode;				// tas mode auto or fixed value
  int max_tas;				// max tas value
  char msg[DSM_MSG_STRING_SIZE];

  bool interfaceInstalled;

};

#endif
