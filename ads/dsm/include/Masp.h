/* Masp.h -- contains the necessary DAS structures and defines

   Original Author:  Mike Spowart
   Copyright 1994
   National Center for Atmospheric Research
   Boulder, Colorado


*/

#ifndef MASP_H
#define MASP_H

#include <vxWorks.h>
#include <sysLib.h>
#include <taskLib.h>
#include <logLib.h>

# include    <mv162.h>
# include  <sockLib.h>
# include  <tickLib.h>
# include  <hostLib.h>
# include  <msgQLib.h>
# include    <wdLib.h>
# include <dosFsLib.h>
# include       <iv.h>
# include    <types.h>
# include   <socket.h>
# include       <in.h>
# include   <string.h>
# include <stdioLib.h>
# include   <intLib.h>
# include    <tyLib.h>
# include    <ioLib.h>
# include    <ctype.h>
# include <rebootLib.h>
# include   <semLib.h>
   
#include <Bc635Vme.h>
#include <SerialBuf.h>
#include <header.h>
#include <dsmctl.h>
#include <messageDefs.h>
#include <dsmVmeDefs.h>

/* define how many packets of data to block before writing */
# define DAT_BLOCK 1020*2*16           /* must be a factor or 512 */
# define HK_BLOCK 64*2*256             /* must be a factor of 3600 */
# define SERVO_BLOCK 184*2*94          /* must be a factor of 12 */

/* define the MASP constants */
# define INTERVAL      10       /* minutes between cal. cycles */
# define MODE_LEVEL_1  5
# define MODE_LEVEL_2  5
# define MODE_LEVEL_3  5
# define MODE_WIDTH_1  5
# define MODE_WIDTH_2  5
# define MODE_WIDTH_3  5
# define KI            2000
# define KP            2000
# define TRIG_THRESH   400              /* trigger threshold */
# define MIN_TTIME     3                /* minimum transit time */
# define MAX_MASP_PKTS 5                /* doesn't apply to cal cycles! */
# define MASK_TOTAL_AR 1400             /* masked acceptance ratio * 1000 */
# define MASK_DOF_GAIN 16               /* Masked Ap DOF Rejection Gain */
# define TOTAL_DOF_GAIN 0               /* Total Ap DOF Rejection Gain */
 
/* Define the VMEMA interface card structures */
#define A24D16_BASE     0xF0000000      /* VMEbus base seen from V17 */
#define DPR_A           VMEMA_BASE+A24D16_BASE
#define DPR_B           VMEMA_BASE+A24D16_BASE+0x4000
#define BIC_CTL_REG1    0xC000          /* Bus interruptor control regs */
#define BIC_CTL_REG2    0xC002          /* Bus interruptor control regs */
#define SW_RESET        0xE000          /* Read for reset */

#define MASP_PKT_SZ     1020

/* Define the SPORT interface structure */
struct TX_BUF { unsigned short  tb[512]; };
struct RX_BUF { unsigned short  rb[1024]; };
 
/* Define the interface card's memory structure */
struct DPR
{
        struct RX_BUF rxb[7];           /* Define the 7 RX buffers */
        struct TX_BUF txb;              /* Define the TX buffer */
        unsigned short rx_flags[7];     /* The Rx flags */
        unsigned short cmd_stat;        /* Command status register */
        unsigned short spare[502];
        unsigned short dsp_irq;         /* Read to cause dsp irq */
};
 
/* Describe the isr interface structure */
struct VMEMA_CTL
{
        struct DPR      *dpr;           /* pointer to the interface's DPR */
        int             flag;           /* Set when packet arrives */
        int             nxt_buf;        /* The first buffer to check */
};

struct TIME_PACKET {
	unsigned short sync_flag;	/* sync word */
	unsigned short length;		/* the packet length */
	unsigned short type;		/* the packet type */
	unsigned long  seconds_yr;	/* the seconds of the year */
	unsigned short tas;		/* the true airspeed */
	unsigned short cksum;		/* checksum */
};

struct MODE_PACKET {
	unsigned short sync_flag;	/* sync word */
	unsigned short length;		/* the packet length */
	unsigned short type;		/* the packet type */
	unsigned short mode;		/* the mode (0=calib, 1=data) */
	unsigned short level[3];	/* the laser amplitude levels */
	unsigned short width[3];	/* the pulse durations */
	unsigned short ki;	
	unsigned short kp;
	unsigned short trig_thresh;
	unsigned short max_pkts;	/* the maximum number of packets */
	unsigned short min_ttime;	/* sets the minimum transit time */
	unsigned short maGain;		/* Masked Ap gain for rejection */
	unsigned short tsGain;		/* Masked Ap gain for rejection */
	unsigned short cksum;		/* checksum */
};

typedef struct {
  MASP_HK_PKT hbuf[TOG];
} MaspHouse;

class Masp {
public:
  Masp (unsigned char *intf_base, Bc635Vme&, void (*stat_msg)(char* msg_str));
  void masp_isr();                      /* interrupt service routine */
  void collect();			/* Collect Masp data */
  void probeInit();			/* Initialize probe */
  void secondAlign();			/* perform one-second tasks */
  void setTas (int tas);                // set true air speed
  void setParam (unsigned short param); // set the param_map
  void calibrate();		/* change from data to cal. to data */
  void control (int action, int value);

  inline char *md_buffer()         {return md_buf[md_gtog]->getBuf();}
  inline int md_length()           {return md_buf[md_gtog]->bufIndex();}
  inline char *ms_buffer()         {return ms_buf[ms_gtog]->getBuf();}
  inline int ms_length()           {return ms_buf[ms_gtog]->bufIndex();}
  inline int ms_bufFull()          {return ms_buf[ms_gtog]->bufFull();}
  inline int md_bufFull()          {return md_buf[md_gtog]->bufFull();}
  inline void ms_releaseBuf()      {ms_buf[ms_gtog]->releaseBuf();
                                    ms_gtog =++ms_gtog % TOG;}
  inline void md_releaseBuf()      {md_buf[md_gtog]->releaseBuf();
                                    md_gtog =++md_gtog % TOG;}

  inline char   *gbuffer() 	 {return (char*)&masph.hbuf[gtog];}
  inline char   *pbuffer()	 {return (char*)&masph.hbuf[ptog];}
  struct MaspHouse masph;
  unsigned short cal_flag;	/* set to one while calibrating */

private:
  unsigned short checksum(unsigned short *data, unsigned short size);
  void ConvertTime(long seconds, struct CNV_TIME *time);
  unsigned int get_time(); 	/* the routine that gets time */
  int create_msgQ();		/* create the message queue */
  void mode_isr();		/* the ISR that starts a cal cycle */
  void pulse_now();		/* toggles the 16Hz pulses */
  void sendMode();		/* send a mode packet to masp */
  void unpak31 (unsigned short signal, unsigned short *low, 
              unsigned short *mid, unsigned short *high);
  void unpak21 (unsigned short signal, unsigned short *low, 
              unsigned short *high);
  Bc635Vme &tfp;                        // time-freq processor class
  void (*statusMsg)(char* msg_str);     // status msg handler
  unsigned int sec_o_yr;  /* seconds after Jan 1 00:00:00 */

  int iflag,stat;
  int pcnt;     	/* number of packets received since start */
  short rfindx; 	/* buffer number of next packet */
  int rcSum;  		/* number of checksums errors read */
  int acSum;  		/* number of checksums errors calculated */
  int bpSum;  		/* number of bad packets received */
 
  SerialBuf *ms_buf[TOG];                  // toggling physical buffers
  SerialBuf *md_buf[TOG];                  // toggling physical buffers
 
  int ms_ptog;                             // put buffer index
  int ms_gtog;                             // get buffer index
  int md_ptog;                             // put buffer index
  int md_gtog;                             // get buffer index
  int ptog;                                // put buffer index
  int gtog;                                // get buffer index
 
  /* create the holder for the number of packets since the last write */
  int mh_pkts;
  int ms_pkts;
  int md_pkts;
 
  unsigned short p_size, p_type, f_time, p_cksm;
  unsigned long m_time;
  unsigned short *data_pk;

  /* Define the message Queue */
  MSG_Q_ID masp_qid;

  /* Define the Watchdog IDs */
  WDOG_ID mode_wid;

  /* set up the global variables */
  struct DPR *dpr;                /* a pointer to the Dual Ported RAM */
  struct TIME_PACKET masp_time;   /* contains time to be sent to MASP */
  struct MODE_PACKET masp_mode;   /* contains the mode data for the MASP */

  unsigned int  last_masp;        /* time last masp packet was received */
  unsigned short  param_map;	  /* bitmapped varios req.  SEA on */
  unsigned short thresh;	  /* MASP trigger threshold */
  unsigned short maGain;
  unsigned short tsGain;
  unsigned short minTtime;	/* MASP minimum accepted t-time */
  unsigned short max_masp_pkts;
}; 

#endif
