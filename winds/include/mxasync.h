/* mxasync.h
   Header file for mxasync defines.  This file must be preceeded by
   header.h so that MX_P1D and MX_EVT are defined.
*/

#ifndef MXASYNC_H
#define MXASYNC_H

#include <header.h>

#define A24D16_BASE        0xFF000000  /* vmebus base seen from MD-CPU320 */
#define A24D16_BASE_V37    0xF0000000  /* vmebus base seen from GMS V37 */
#define SET_VME_BTO_MASK   0x1800      /* bcsr1 vmebus timeout mask */
#define SET_VME_SLAVE_BASE 0x00400007  /* CPU320 bcsr2 value */
#define SET_VME_SLAVE_MAX  0x00000050  /* CPU320 bcsr3 value */
#define BASE_NUM_SYNC_BLKS 6	       /* base number of sync blocks sampled */

/* ISR vector and address defines */

#define AVC_BASE_VCT 0x18             	/* autovectors base vector for 68155 */

#define SPUR_VCT 0x18                 	/* spurious interrupt vector # */
#define SPUR_ADR 0x0060               	/* spurious interrupt vector address */
 
#define AVC1_VCT 0x19                 	/* autovector 1 vector # */
#define AVC1_ADR 0x0064               	/* autovector 1 vector address */

#define AVC2_VCT 0x1A                 	/* autovector 2 vector # */
#define AVC2_ADR 0x0068               	/* autovector 2 vector address */
 
#define AVC3_VCT 0x1B                 	/* autovector 3 vector # */
#define AVC3_ADR 0x006C               	/* autovector 3 vector address */
 
#define AVC7_VCT 0x1F                 	/* autovector 7 vector # */
#define AVC7_ADR 0x007C               	/* autovector 7 vector address */
 
#define ISIO1_BIM1_VCT  0x51           	/* ISIO #1 bim 1 vector */
#define ISIO1_BIM1_ADR  0x0144 		/* ISIO #1 bim 1 vector address */

#define ISIO1_BIM2_VCT  0x52           	/* ISIO #1 bim 2 vector */
#define ISIO1_BIM2_ADR  0x0148 		/* ISIO #1 bim 2 vector address */

#define ISIO1_BIM3_VCT  0x53           	/* ISIO #1 bim 3 vector */
#define ISIO1_BIM3_ADR  0x014C 		/* ISIO #1 bim 3 vector address */

#define ISIO1_BIM4_VCT  0x54           	/* ISIO #1 bim 4 vector */
#define ISIO1_BIM4_ADR  0x0150 		/* ISIO #1 bim 4 vector address */

#define IRS_VCT1 0x55			/* 1st IRS isr vector # */
#define IRS_ADR1 0x0154 		/* 1st IRS isr vector address */
 
#define IRS_VCT2 0x56			/* 2nd IRS isr vector # */
#define IRS_ADR2 0x0158 		/* 2nd IRS isr vector address */
 
#define TR10_ADR 0x00A8			/* trap #10 vector address */
#define TR11_ADR 0x00AC			/* trap #11 vector address */
 
#define BIM_CTL 0x11      		/* isio bim enable ints, level 1 */
#define LRNRX 		0               /* read loran C isio channel # */
#define LRNTX 		1               /* write loran C isio channel # */
#define TIME_RX		2		/* time output read channel # */
#define TIME_TX		3		/* time output write channel # */
#define NATSRX 		4		/* read NATS isio channel # */
#define NATSTX		5		/* write NATS isio channel # */	
#define UVHYG_RX	6		/* read UV hygrometer isio chan # */
#define UVHYG_TX	7		/* write UV hygrometer isio chan # */	
#define GPS_MAG_RX 	8               /* read Magnavox GPS isio channel # */
#define GPS_MAG_TX 	9       	/* write Magnavox GPS isio channel # */
#define GPS_TRIM_RX 	10              /* read GPS isio channel # */
#define GPS_TRIM_TX 	11       	/* write GPS isio channel # */
#define OPH3_RX		12		/* ophir 3 read channel */
#define OPH3_TX		13		/* ophir 3 write channel */
#define LIDAR_RX	02		/* read lidar isio channel # */
#define LIDAR_TX	03		/* write lidar isio channel # */
/* LIDAR_TX, RX modified for the toga-coare project */

#define TOG	   2			/* toggling buffer sizes */

#define SMPDATA	   0		      	/* sampled data block type */
#define GREYDATA   1		      	/* grey scale data block type */
#define TWODATA	   2		      	/* pms 2d data block type */
#define MSGDATA	   3		      	/* message block type */

#define TPE_DONE   0		      	/* tape command successfully issued */
#define TPE_BUSY   1		      	/* command not issued, drive is busy */
#define TPE_FAIL   2		      	/* command not issued, drive failure */
#define TPE_MAX_TRIES	5		/* max tries to issue a tape command */

/* new recording state defines for the v17 scsi driver. */
#define RCD_NOT_READY	0		/* drive is not ready for recording */
#define RCD_LEADER_RQST	1		/* leader write requested */
#define RCD_READY	2		/* drive is ready for recording */
#define RCD_ENABLED	3		/* recording enabled on the drive */
#define RCD_DISABLED	4		/* recording disabled on the drive */

#define LRNREC     630                 /* loran C rec len, 14 45 byte strings*/
#define LRNBLK     182                 /* p.r. loran block length */
#define LRNSTR     45                  /* loran C string length */
#define LRNDLM     0x0D0A              /* loran string delimiter */
#define GPSDLM     0x1003              /* gps string delimiter, DLE-ETX */
#define P2DRATE    2                   /* 2D data interval to display comp. */
#define DEFTAS     20                  /* default tas word */
#define GPSREC     132                 /* max gps packet length */
#define LDRREC     20                  /* tape leader message record length */
#define SUN_WAIT   600                 /* # secs to wait for sun to send hdr */
/* delete the next define, once the scsi driver has been verified. */
#define DRV_WAIT   180                 /* # secs to wait for drive 0 to load */
 
#define GPOSLEN    20                  /* # gps position data bytes */
#define GVELLEN    20                  /* # gps velocity data bytes */
 
/* Define the GPS packet ids */
 
#define GPS_ALT    0x2A                /* 2d altitude command */
#define GPS_TIME   0x41                /* gps time packet id */
#define GPS_STLS   0x44                /* gps satellite selection packet id */
#define GPS_PWR    0x45                /* gps power up packet id */
#define GPS_HLTH   0x46                /* gps health packet id */
#define GPS_POS    0x4A                /* gps position packet id */
#define GPS_MCOD   0x4B                /* gps machine code/status packet id */
#define GPS_VEL    0x56                /* gps velocity packet id */
 
/* Define the initialization packets sent to the GPS */
 
#define GPS_NPK    4                   /* number of initialization packets */
#define GPS_PLN    6                   /* maximum packet length */
 
                                       /* alt input/output - msl geoid */
                                       /* ecef output off */
                                       /* velocity - enu on, ecef off */
                                       /* output gps time asap when calc */
#define GPS_PK0    {0x35, 0x0E, 0x02, 0x00, 0x00}
#define GPS_PK1    {0x22, 0x00}        /* force automatic mode */
#define GPS_PK2    {0x26}              /* request current health */
#define GPS_PK3    {0x24}              /* request current mode */
 
#define GPS_LN0    5                   /* length of packet 0 */
#define GPS_LN1    2                   /* length of packet 1 */
#define GPS_LN2    1                   /* length of packet 2 */
#define GPS_LN3    1                   /* length of packet 3 */
 
#define SDID       0x8681              /* sampled data id word */

#define NATS_HDR_SUFFIX ".nats"		/* nats header file suffix */
#define MX_DOCYCLE 2                    /* max number of digital output cycles

/* Physical record information structure. */
struct PRSMP {
  unsigned char buf[TOG][MX_PHYS];  	/* toggling buffers */
  int lr_len;             		/* log. record len in bytes */
  int pr_len;             		/* phys record lens in bytes */
  int pr_full[TOG];       		/* phys record full flag */
  int hdr_off;            		/* offset to header block */
  int dme_off;            		/* offset to dme block */
  int dout_off;           		/* offset to digout block */
  int evt_off[MX_EVT];    		/* offsets to event blocks */
  strnam evt_locn[MX_EVT]; 		/* acrft locations of events */
  int num_evt;	     			/* number of defined events */
  int gps_mag_off;         		/* offset to Magnavox gps blk */
  int gps_trim_off;         		/* offset to Trimble gps blk */
  int hskp_off;            		/* offset to hskp block */
  int ins_off;            		/* offset to ins block */
  int irs_off[MX_IRS];    		/* offset to irs blocks */
  int lrnc_off;           		/* offset to loran C block */
  int oph3_off;           		/* offset to ophir 3 block */
  str12 pms1_names[MX_P1D];  		/* Pms 1d names, name_locn */
  int pms1_off[MX_P1D];   		/* offsets to pms 1d blocks */
  str12 p1vme_names[MX_P1DV2]; 		/* vme pms 1d names, locn */
  int p1vme_off[MX_P1DV2];  		/* offsets to vme pms 1d blks */
  int p1vme_len[MX_P1DV2];  		/* lengths of vme pms 1d blks */
  int p1spc_off[MX_P1DV2];  		/* offsets to vme pms 1d spacing blks */
  int p1spc_len[MX_P1DV2];  		/* lengths of vme pms 1d spacing blks */
  int ser_off;				/* offset of the generic serial blks */
  int ser_len;				/* length of the generic serial blks */
  int sdi_off;            		/* offset to sdi block */
  int uvhyg_off;           		/* offset to uv hygrometer block */
};

/* IRS interface control and data structure. */

struct Irs_ctl	   {
		     struct Irs_blk irs_blk[TOG]; /* toggling data buffers */
		   
         	     unsigned long sec_tick;	/* The 1 Hz isr writes this */
         	     unsigned long irs_sec;	/* Tick count when IRS leads */ 

		     struct Rx_control *rx_ctl; /* --> Arinc control struct */

		     int get;			/* irsblk buffer get toggle */
		     int put;			/* irsblk buffer put toggle */
		     int last_int;		/* The # of the last int */

	  	     int put_25;		/* where 25 Hz is going */	
	  	     int put_10;		/* where 10 Hz is going */	
	  	     int put_5;			/* where 5 Hz is going */	
	  	     int put_2;			/* where 2 Hz is going */	

		     int first_25;		/* set when searching for */
		     int first_10;		/* set when searching for */
		     int first_5;		/* set when searching for */
		     int first_2;		/* set when searching for */

 		     int indx_50;		/* 50 hz index */
 		     int indx_25;		/* 25 hz index */
 		     int indx_10;		/* 10 hz index */
 		     int indx_5;		/* 5 hz index */
 		     int indx_2;		/* 2 hz index */
      		   };

/* Lidar 1 second pickoff block. Warning this struct is not long word aligned
   due to the inclusion of the starting id short word. */


struct LidarBlk {
  short id;				/* '**' */
  short  hour;               		/* ADS time */
  short  minute;             		/* ADS time */
  short  sec;                		/* ADS time */
  short  spare1;             		/* spare */

  long pitch_angle[RATE_5];            /* Label 324 */
  long roll_angle[RATE_5];             /* Label 325 */
  long true_heading[RATE_5];           /* Label 314 */
  long inrt_vert_speed[RATE_5];        /* Label 365 */
  long ground_speed[RATE_5];           /* Label 312 */
  long velocity_ns[RATE_5];            /* Label 366 */
  long velocity_ew[RATE_5];            /* Label 367 */
  long present_lat[RATE_5];            /* Label 310 */
  long present_lon[RATE_5];            /* Label 311 */
  long inertial_alt[RATE_5];           /* Label 361 */
};

#endif
