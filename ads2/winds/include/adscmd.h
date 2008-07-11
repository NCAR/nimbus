/* adscmd.h
   Display computer interface defines.
   If this file is modified, the same named file on the display computer
   must be modified to match.
*/

#ifndef ADSCMD_H
#define ADSCMD_H
 
#define PKT_FLG    0x5A5A              /* command packet start word */
 
#define INVALID        -1              /* invalid(top) menu selection */
#define SEC_CMD    0x0010              /* one second command packet */
 
#define TAPE_CMD   0x0020              /* packet is for a tape command */
#define REC_D0     0x0000              /* record on drive 0 */
#define REC_D1     0x0001              /* record on drive 1 */
#define STOP_REC   0x0002              /* stop recording command */
#define UNLD_D0    0x0003              /* unload drive 0 */
#define UNLD_D1    0x0004              /* unload drive 1 */
#define RQST_STAT  0x0005              /* tape status requested */
 
#define P2D_CMD    0x0030              /* packet is for pms 2d control */
#define SET_RATE   0x0001              /* set 2d record rate */
#define DISAB_2D   0x0000              /* disable a probe */
#define ENAB_2D    0x00C2              /* enable a probe */
#define INOP_2D    0x00FF              /* probe not present */
 
#define P1D_CMD    0x0040	       /* set pms 1d range command */
#define TIME_CMD   0x0050              /* packet is for a time set command */
#define HDR_CMD    0x0060              /* prepare for new header command */
#define DME_CMD    0x0070              /* dme channel list command */
#define DGO_CMD    0x0080              /* digital output command */
#define PKT_CMD    0x0090              /* isio command channel reset request*/
#define EVT_CMD    0x00A0              /* event marker command */
#define GPS_MAG_CMD 0x00B0             /* init Magnavox GPS command */
#define OPH3_CMD   0x00C0		/* init ophir3 command */

#define SYNC_DISK  3333                /* sync disk before power down */
 
/* Messages to send to the Display Computer. */
 
#define TPE_STAT  0                     /* tape status message */
#define P2D_STAT  1                     /* pms 2d status message */
#define P1D_STAT  2                     /* pms 1d status message */
#define DME_STAT  3                     /* dme status message */
#define CMD_STAT  4                     /* other commands response message */
#define TIME_STAT 5	 		/* time set status message */
#define HDR_RCVD  7                     /* header received message */
#define RUN_ERR   0x10                  /* execution error message */
#define HDR_ERR   0x11                  /* error reading the tape header */
#define PKT_ERR   0x12                  /* misaligned packet received */
 
#define LOADED   0                     /* drive is loaded */
#define LOADING  1                     /* drive is in process of loading */
#define UNLDING  2                     /* drive is in process of unloading */
#define UNLOADED 3                     /* drive is unloaded */
#define NOT_PRES 4                     /* drive is not present */
#define FAILED   5                     /* drive has failed */
#define BUSY     6                     /* drive is busy */
 
#define DRV_0    0                     /* drive 0 identification */
#define DRV_1    1                     /* drive 1 identification */
#define DRV_NONE -1                    /* no drive identification */
 
#define RT2D_OFF        0               /* 2d recording off */
#define RT2D_1          1               /* 1 rec/sec recording rate */
#define RT2D_2          2               /* 2 rec/sec recording rate */
#define RT2D_4          4               /* 4 rec/sec recording rate */
#define RT2D_8          8               /* 8 rec/sec recording rate */
#define RT2D_MAX        100             /* max recording rate */
 
#define TAS2D_AUTO      0               /* auto tas value */
#define TAS2D_5         5               /* 5 m/s manual tas */
#define TAS2D_10        10              /* 10 m/s manual tas */
#define TAS2D_20        20              /* 20 m/s manual tas */
#define TAS2D_40        40              /* 40 m/s manual tas */
#define TAS2D_80        80              /* 80 m/s manual tas */
#define TAS2D_THRESH    40              /* threshold to disable manual tas */
#define TAS2D_MAN_NOTICE 300            /* secs between manual tas alerts */
#define TAS2D_DEFAULT	TAS2D_10	/* default tas setting */
 
#define RANGE_NONE	-2		/* non-ranging probe */
#define RANGE_0         0               /* pms 1d range 0 */
#define RANGE_1         1               /* pms 1d range 1 */
#define RANGE_2         2               /* pms 1d range 2 */
#define RANGE_3         3               /* pms 1d range 3 */
 
#define ASASP_OFF       0               /* asasp off */
#define ASASP_ON        1               /* asasp on */

#define MX_PKT_DATA 6		 	/* max # packet data words */
#define MX_PKT_MSG  60		 	/* max len of msg packet message */
#define MX_CMD_MSG  40			/* max len of cmd packet message */

/* Ads ethernet state */
#define CHECK_ADS	0		/* check the ads connection state */
#define START_ADS	1	     	/* run the ads startup routine */
#define CONNECT_ADS	2		/* set connected to ads state */
#define HDR_RCVD_ADS	3		/* set header receives by ads state */

/* Time set indices. */
#define DATE_SIZE 6
#define YEAR_IDX  0
#define MONTH_IDX 1
#define DAY_IDX   2
#define HOUR_IDX  3
#define MIN_IDX   4
#define SEC_IDX   5

/* Structure of the command packet received from the Display Computer. */
 
struct CMDPKT      {
                     int flag;
                     int type;
                     int action;
                     int data[MX_PKT_DATA];
                     char msg[MX_CMD_MSG];
                   };
 
/* Structure of the message packet sent to the Display Computer. */
 
struct MSGPKT      {
                     int flag;
                     int type;
                     int hour;
                     int minute;
                     int sec;
                     int data[MX_PKT_DATA];
                     char msg[MX_PKT_MSG];
                   };

#endif
