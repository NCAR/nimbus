/* vmema.h - This file describes the physical structure of the MASP - VMEbus
             interface

   Original Author:  Kim A. Weaver
   Copyright 1993
   National Center for Atmospheric Research
   Boulder, Colorado

   Written in support of ASHOE/MAESA

   Revision History:
      10/--/1993 kaw Created
*/


#ifndef VMEMA_H
#define VMEMA_H

/* Define the VMEMA interface card structures */
#define A24D16_BASE    	0xF0000000  	/* VMEbus base seen from V17 */
#define TFP_BASE        0x0fff1000      /* time-freq processor base */
#define DPR_A		VMEMA_BASE+A24D16_BASE
#define DPR_B		VMEMA_BASE+A24D16_BASE+0x4000
#define BIC_CTL_REG1	0xC000		/* Bus interruptor control regs */
#define BIC_CTL_REG2	0xC002		/* Bus interruptor control regs */
#define SW_RESET	0xE000		/* Read for reset */
#define SP1_VCT_NUM	0x70		/* SPORT 1 ISR vector number */
#define SP3_VCT_NUM	0x71		/* SPORT 3 ISR vector number */
#define SP2_VCT_NUM	0x72		/* SPORT 2 ISR vector number */
#define SP4_VCT_NUM	0x73		/* SPORT 4 ISR vector number */
#define MX_PHYS         0x8000          /* max physical record size, 32768 */

/* Define the SPORT interface structure */
struct TX_BUF { unsigned short	tb[512]; };
struct RX_BUF { unsigned short	rb[1024]; };

/* Define the interface card's memory structure */
struct DPR
{
	struct RX_BUF rxb[7];		/* Define the 7 RX buffers */
	struct TX_BUF txb;		/* Define the TX buffer */
	unsigned short rx_flags[7];	/* The Rx flags */
	unsigned short cmd_stat;	/* Command status register */
	unsigned short spare[502];	
	unsigned short dsp_irq;		/* Read to cause dsp irq */
};

/* Describe the isr interface structure */
struct VMEMA_CTL
{
	struct DPR 	*dpr;		/* pointer to the interface's DPR */
	int		flag;		/* Set when packet arrives */
	int		nxt_buf;	/* The first buffer to check */
};

#endif
