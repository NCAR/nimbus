/* dsmVmeDefs.h
   Dsm VME bus defines.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
 
/* Define global memory addresses */

#ifndef DSMVMEDEFS_H
#define DSMVMEDEFS_H

#define A24D16_BASE     0xF0000000      	/* VMEbus base from mv162 */
#define MC_CHIP_BASE	0xFFF42000		/* mv162 MCchip	base addr */
#define M56002_1_BASE	0x00100000		/* M56002 DSP board #1 base */
#define M56002_2_BASE	0x00110000		/* M56002 DSP board #2 base */
#define G2BUF     	0x00200000              /* 2D global data buffer */
#define G2CMD      	0x00237e00              /* 2D global command buf adr*/
#define G2TIME      	0x00237f00              /* 2D global time struct adr */
#define VMEMA_BASE      0x00510000        	/* high speed serial base */
#define PMSVME2D_BASE_0 0x00800000		/* PmsVme2d intf #1 base */
#define PMSVME2D_BASE_1 0x00810000		/* PmsVme2d intf #2 base */
#define GREYVME_BASE_0  0x00600000		/* grey scale intf #1 base */
#define GREYVME_BASE_1  0x00610000		/* grey scale intf #2 base */
#define ARINC429_BASE   0x00700000        	/* arinc 429 intf base */
#define PMS2D_BASE_0  	0x00800000		/* pms 2d intf #1 base */
#define PMS2D_BASE_1  	0x00810000		/* pms 2d intf #2 base */
#define ISIO1_BASE   	0x00960000              /* isio base address */
#define VM3118_BASE	0x00F00000		/* vmic 3118 base */

#define M316_BASE       0x0fff0000              /* mvme316 I/O chan base adr */
#define M316_BIM        M316_BASE+0xbf00        /* mvme316 bim address */
#define M316_VIO        M316_BASE+0xe000        /* mvme316 I/O memory adr */
#define TFP_BASE	0x0fff1000		/* time-freq processor base */
#define VMIO12_BASE_0	0x0fff1E00		/* vmio12 digital I/O #0 base*/
#define VMIO12_BASE_1	0x0fff1F00		/* vmio12 digital I/O #0 base*/
#define VME1D_BASE_0    0x0fff8000      	/* interface #0 base address */
#define VME1D_BASE_1    0x0fff8100      	/* interface #1 base address */
#define SRAM_BASE	0xffe00000		/* battery backed sram adr */
#define DATA_MBOX  	0			/* data avail mailbox index */
#define SEC_MBOX   	1			/* 1s interrupt mailbox index */

/* Define addresses of classes which are stored in battery backes sram. */
#define SRAM_TAPEHEADER_BASE	SRAM_BASE
#define SRAM_TVMCONFIG_BASE	(SRAM_BASE + 0x10000)
#define SRAM_DSMCONFIG_BASE	(SRAM_BASE + 0x18000)
#define SRAM_VARCONFIG_BASE	(SRAM_BASE + 0x19000)
 
#define ADR_2D     0x22e000		/* address of the 2d program */

/* ISR vector and address defines */
 
#define AVC_BASE_VCT 0x18               /* autovectors base vector for 68155 */
 
#define SPUR_VCT 0x18                   /* spurious interrupt vector # */
#define SPUR_ADR 0x0060                 /* spurious interrupt vector address */
 
#define AVC1_VCT 0x19                   /* autovector 1 vector # */
#define AVC1_ADR 0x0064                 /* autovector 1 vector address */
 
#define AVC2_VCT 0x1A                   /* autovector 2 vector # */
#define AVC2_ADR 0x0068                 /* autovector 2 vector address */
 
#define AVC3_VCT 0x1B                   /* autovector 3 vector # */
#define AVC3_ADR 0x006C                 /* autovector 3 vector address */
 
#define AVC7_VCT 0x1F                   /* autovector 7 vector # */
#define AVC7_ADR 0x007C                 /* autovector 7 vector address */
 
#define TFP_VCT		0x50		/* time-freq processor vector */
#define TFP_ADR		0x0140		/* time-freq processor vector address */

#define ISIO1_BIM1_VCT  0x51            /* ISIO #1 bim 1 vector */
#define ISIO1_BIM1_ADR  0x0144          /* ISIO #1 bim 1 vector address */
 
#define ISIO1_BIM2_VCT  0x52            /* ISIO #1 bim 2 vector */
#define ISIO1_BIM2_ADR  0x0148          /* ISIO #1 bim 2 vector address */
 
#define ISIO1_BIM3_VCT  0x53            /* ISIO #1 bim 3 vector */
#define ISIO1_BIM3_ADR  0x014C          /* ISIO #1 bim 3 vector address */
 
#define ISIO1_BIM4_VCT  0x54            /* ISIO #1 bim 4 vector */
#define ISIO1_BIM4_ADR  0x0150          /* ISIO #1 bim 4 vector address */
 
#define IRS_VCT1 	0x55            /* 1st IRS isr vector # */
#define IRS_ADR1 	0x0154          /* 1st IRS isr vector address */
 
#define IRS_VCT2 	0x56           	/* 2nd IRS isr vector # */
#define IRS_ADR2 	0x0158         	/* 2nd IRS isr vector address */
 
#define IP429_VCT 	0x58       	/* IP 429-1 vector, lower 3 bits 0 */
#define IP429_ADR 	0x0160         	/* IP 429-1 address */
 
#define VM3118_VCT      0x59		/* VM3118 Vector # */
#define VM3118_ADR      0x0164  	/* VM3118 isr vector adr */

#define GREY_VCT0      	0x5A		/* Greyscale probe 0 vector # */
#define GREY_ADR0     	0x0168   	/* Greyscale probe 0 vector adr */

#define GREY_VCT1      	0x5B		/* Greyscale probe 1 vector # */
#define GREY_ADR1     	0x016C   	/* Greyscale probe 1 vector adr */

#define MVIP301_VCT	0x60		/* masp serial daughter card, mvip301 */
#define MVIP301_ADR	0x0180		/* masp serial daughter card, mvip301 */

#define GPS_VCT1 	0x61            /* 1st GPS isr vector # */
#define GPS_ADR1 	0x0184          /* 1st GPS isr vector address */
 
#define GPS_VCT2 	0x62           	/* 2nd GPS isr vector # */
#define GPS_ADR2 	0x0188         	/* 2nd GPS isr vector address */

#define DMS_VCT 	0x63           	/* DMS isr vector # */
#define DMS_ADR 	0x018C         	/* DMS isr vector address */

#define SP1_VCT         0x70            /* Masp SPORT 1 ISR vector number */
#define SP1_ADR         0x1c0           /* Masp SPORT 1 interrupt vector*/
#define SP3_VCT         0x71            /* Masp SPORT 3 ISR vector number */
#define SP3_ADR         0x1c4           /* Masp SPORT 3 interrupt vector*/
#define SP2_VCT         0x72            /* Masp SPORT 2 ISR vector number */
#define SP2_ADR         0x1c8           /* Masp SPORT 2 interrupt vector*/
#define SP4_VCT         0x73            /* Masp SPORT 4 ISR vector number */
#define SP4_ADR         0x1cc 	        /* Masp SPORT 4 interrupt vector*/

#define PMS2D_VCT0       0x5A            /* Pms2d vector # */
#define PMS2D_ADR0       0x0168          /* Pms2d vector adr */

/* Reserves 0x40 through 0x4F	*/
#define MC_CHIP_VCT	0x40		/* For Tic timer */
#define MC_CHIP_ADR	0x0100		/* For Tic timer */

#define TR10_ADR 0x00A8                 /* trap #10 vector address */
#define TR11_ADR 0x00AC                 /* trap #11 vector address */
 
#endif
 
