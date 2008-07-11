/* maspDas.h -- contains the necessary DAS structures and defines

   Original Author:  Keith S. Barr
   Copyright 1994
   National Center for Atmospheric Research
   Boulder, Colorado

   Written in support of ASHOE/MAESA

   Revision History:
      07/20/1994 ksb Created
*/

#ifndef MASPDAS_H
#define MASPDAS_H
   
#include <masp.h>
#include <aircraft.h>

/* define socket port numbers...must be consistent with Sun side! */
# define DAS_PORT_NUM 1101
# define SUNHOST "kiwi"
# define ETHERNET 0             /* 1 for yes, 0 for no */

/* define the base address for the IP cards */
# define IP_A_BASE 0xfff58000
# define IP_B_BASE 0xfff58100
# define IP_C_BASE 0xfff58200
# define IP_D_BASE 0xfff58300

/* define how many packets of data to block before writing */
# define BLOCK_COUNT 16         /* must be a factor or 512 */
# define HK_BLOCK 60            /* must be a factor of 3600 */
# define SERVO_BLOCK 1          /* must be a factor of 12 */

struct TIME_OF_DAY {
	unsigned int year;      /* years A.D. */        
	unsigned int month;     /* month */     
	unsigned int date;      /* date of month */
	unsigned int day;       /* day of week...Sunday=1, Saturday=7 */
	unsigned int julian;	/* Julian day of the year */
	unsigned int hour;      /* GMT hours */
	unsigned int minute;    /* minutes after the hour */
	unsigned int second;    /* seconds after the minute */
	unsigned int control;   /* control word */
	unsigned int sec_o_yr;  /* seconds after Jan 1 00:00:00 */
};

struct TIME_PACKET {
	unsigned short sync_flag;	/* sync word */
	unsigned short length;		/* the packet length */
	unsigned short type;		/* the packet type */
	unsigned short spare;		/* for alignment */
	unsigned long  seconds_yr;	/* the seconds of the year */
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
	unsigned short tas;			/* the true airspeed */
	unsigned short cksum;		/* checksum */
};

struct FILE_DATA {
	int pcnt;			/* no. of pkts in current data file */
	int file_id;			/* data file descriptor */
	char filename[15];		/* filename */
	char leader[3];			/* the identifier for the file */
	char drive[9];			/* the directory to save in */
	int max_count;			/* the number of saves before new */
	int filenum;			/* current file number */
	int max_files;			/* the maximum number of files */
};

struct CNV_TIME {
	unsigned short julian;		/* julian date */
	unsigned short month;		/* month */
	unsigned short day;		/* date */
	unsigned short year;		/* year */
	unsigned long  gmts;		/* GMT seconds of today */
	unsigned short hh;		/* hour (24) */
	unsigned short mm;		/* minutes */
	unsigned short ss;		/* seconds */
};

struct L_UNPACKED_MASP {
	unsigned long sptt;		/* transit time */
	unsigned long spat;		/* arival time */
	unsigned long tsLg;		/* total scatter low gain */
	unsigned long tsMg;		/* total scatter mid gain */
	unsigned long tsHg;		/* total scatter high gain */
	unsigned long maLg;		/* masked aperature low gain */
	unsigned long maMg;		/* masked aperature mid gain */
	unsigned long maHg;		/* masked aperature high gain */
	unsigned long fsLg;		/* forward scatter low gain */
	unsigned long fsHg;		/* forward scatter high gain */
	unsigned long bsLg;		/* backward scatter low gain */
	unsigned long bsHg;		/* backward scatter high gain */
	unsigned long ratioLg;		/* ratio low gain */
	unsigned long ratioHg;		/* ratio high gain */
};

struct UNPACKED_MASP {
	unsigned short sptt;		/* transit time */
	unsigned short spat;		/* arival time */
	unsigned short tsLg;		/* total scatter low gain */
	unsigned short tsMg;		/* total scatter mid gain */
	unsigned short tsHg;		/* total scatter high gain */
	unsigned short maLg;		/* masked aperature low gain */
	unsigned short maMg;		/* masked aperature mid gain */
	unsigned short maHg;		/* masked aperature high gain */
	unsigned short fsLg;		/* forward scatter low gain */
	unsigned short fsHg;		/* forward scatter high gain */
	unsigned short bsLg;		/* backward scatter low gain */
	unsigned short bsHg;		/* backward scatter high gain */
	unsigned short ratioLg;		/* ratio low gain */
	unsigned short ratioHg;		/* ratio high gain */
};

/* do some prototyping */
int data_to_disk(char *data, long int num_bytes, unsigned short file_type,
                 struct FILE_DATA *file_data);
void data_to_screen(char *pData, int size, int type); 
unsigned short checksum(unsigned short *data, unsigned short size);
void print_pack(int p_size, int rfindx, unsigned short p_type,
                unsigned long m_time, unsigned short ftime,
                int pcnt, int read, int calc, int bad);
void ConvertTime(long seconds, struct CNV_TIME *time);
unsigned int get_time();	/* the routine that gets time */
int create_socket();		/* create the sockets */
int create_msgQ();		/* create the message queue */
void time_isr();		/* the ISR that warns of time */
void mode_isr();		/* the ISR that starts a cal cycle */
void hist_isr();		/* the ISR that prints histograms */
void adc_isr();		/* the ISR that reads the adc */
void readADC();		/* the routine that reads the ADC @ 10 Hz */
void one_sec();			/* the one second jobs */
void calibrate();		/* change from data to cal. to data */
void das_hkng();		/* records DAS housekeeping */
void nav_stream();		/* routine to record the nav serial */
void user_Iface();		/* routine to watch for input on serial 0 */
void dead_das();		/* if this runs, we are dead...tell pilot */
void cls_files();		/* at the end of a flight, close the files */
void pulse_now();		/* toggles the 16Hz pulses */
void sendTime();
void cls();			/* clear the data screen */
void sendMode();		/* send a mode packet to masp */
void showMenu(int menu);		// prints the current menu

/* Define the message Queue */
MSG_Q_ID masp_qid;

/* Define the Watchdog IDs */
WDOG_ID time_wid;
WDOG_ID mode_wid;
WDOG_ID death_wid;
WDOG_ID hist_wid;
WDOG_ID adc_wid;


/* Define the Semaphore IDs */
SEM_ID time_sid;
SEM_ID mode_sid;
SEM_ID hist_sid;
SEM_ID adc_sid;

/* create the disk synchronization semaphores */
SEM_ID mh_disk;
SEM_ID ms_disk;
SEM_ID md_disk;
SEM_ID dh_disk;
SEM_ID nd_disk;

/* set up the global variables */
struct sockaddr_in sock_name;   /* internet style address structure */
struct DPR *dpr;                        /* a pointer to the Dual Ported RAM */
struct DPR *fpr;                        /* a pointer to the Dual Ported RAM */
struct TIME_OF_DAY time_now;    /* contains current time (1 second update) */
struct TIME_PACKET masp_time;   /* contains time to be sent to MASP */
struct MODE_PACKET masp_mode;   /* contains the mode data for the MASP */
struct DAS_HK_PACKET das_hk;    /* contains the DAS housekeeping data */
struct ER2_NAV_PACKET enav_dat;/* contains the ER2 serial stream data */
struct DC8_NAV_PACKET dnav_dat;	// contains the DC8 nav data
struct C130_NAV_PACKET cnav_dat;	// contains the C130 nav data
struct FALCON_NAV_PACKET fnav_dat;
struct MASP_XON_PKT xon_pkt;

int DAS_sock;                   /* data socket file descriptor */
int das_pri;                    /* contains the priority level for MASPDAS */
unsigned long last_masp;        /* time last masp packet was received */
unsigned short bad_save;        /* set to 1 if we had a bad save */
unsigned short data_cycle;      /* the number of times we have cycled power */
unsigned short  print_map;      /* bitmapped print/exit requests */
unsigned short  histo_map;      /* bitmapped histogram requests */
unsigned short  param_map = 0x2;/* bitmapped varios req.  SEA on */
unsigned short *pDigital;       /* bitmap of MVIP 521 output */
int pack_ex;                    /* the packet data exclusion flag */
unsigned short tas=200;             /* true airspeed */
unsigned long last_tas;         /* the time the last TAS was rx'd */
unsigned short cal_flag;        /* set to one while calibrating */
unsigned short cal_pkt_cnt;     /* number of packets seen this calibrate */
unsigned short thresh = TRIG_THRESH; /* MASP trigger threshold */
unsigned short maGain = MASK_DOF_GAIN;
unsigned short tsGain = TOTAL_DOF_GAIN;
unsigned short minTtime = MIN_TTIME; /* MASP minimum accepted t-time */
unsigned short totals[4][35];   /* array of counts for drawing histos */
unsigned short histo_int = HISTO_INT; /* histogram interval */
unsigned short histo_bin = HISTO_BIN; /* histogram bin info */
unsigned long bins[36];        /* histogram bin limits */
unsigned short mt_ar = MASK_TOTAL_AR;   /* mask/total acceptance ratio */
unsigned short max_masp_pkts = MAX_MASP_PKTS;
unsigned short histo_lock = 0;
unsigned short adc[16];
unsigned short adcCount[16];
int navPort;							/* handle to navigation serial port */

unsigned long maspLim[32];

unsigned short modeSentCount = 0;
unsigned short mhCommandCount;
unsigned long mhTime;

/* the data file structure */
struct FILE_DATA MH_info;
struct FILE_DATA MD_info;
struct FILE_DATA MS_info;
struct FILE_DATA DH_info;
struct FILE_DATA ND_info;

/* create the memory for the das housekeeping and for the navdata */
unsigned short *dhb[2];
unsigned short *dhNext;
int dh_pkts;
int dh_buf;

unsigned short *ndb[2];
unsigned short *ndNext;
int nd_pkts;
int nd_buf;

/* bitmap for the warning messages */
bool WARNING = TRUE;

// for Fail file information and creation
        char fail_flags[132];
        char fail_file[20];
        int fail_light = 0;
        int fail_id;


#endif
