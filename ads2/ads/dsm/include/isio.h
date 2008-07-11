/* isio.h
   Defines for the FORCE ISIO-2 serial interface board.
 
   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/
 
#ifndef ISIO_H
#define ISIO_H
 
/* Command defines */
#define GLOBINI    0x0000               /* i/o buffer config */
#define CHINIT2    0x0002               /* DUSCC init */
#define CLRBUF     0x0005               /* clear input buffer */
#define ASYNINI    0x0006               /* async init */
#define RECON      0x0007               /* enable DUSCC receiver */
#define RECOFF     0x0008               /* disable DUSCC receiver */
#define PUTFRM     0x0037               /* output sync frame */
#define OUTFLAG    0x0100               /* output channel command */
#define SETOFFS    0x0003               /* base address offset def */
#define RESET      0x0009               /* firmware reset */
#define GETCH      0x0010               /* get single char */
#define PUTCH      0x0014               /* output 1 or 2 chars */
#define GETSTR     0x0030               /* get delimited string */
#define PUTSTR     0x0032               /* put a delimited string */
#define GETCNT     0x0033               /* get counted string */
#define PUTCNT     0x0035               /* output a counted string */
#define INTFLG     0x1000               /* complete with interrupt */
  
#define RST_MSK    0x0400               /* cpu stat mask for _bim[].bimcr1 */
#define BIM_INT_ENABLE	0x10		/* bim control reg intrpt enable bit */

/* Communication characters */
#define SYN        0x16                       	/* sync char */
#define STX        0x82                       	/* start of text */
#define ETX        0x03                       	/* end of text */
#define EOT        0x84                       	/* end of transmission */
#define ENQ        0x85                       	/* enquire */
#define ACK        0xB0                       	/* acknowledge */
#define DLE        0x10                       	/* data link escape */

/* Data structures */
 
/* BIM and status register.
   Odd byte is the BIM, even byte is the ISIO status. */
typedef struct {
		  unsigned char res1;
/*0001*/          unsigned char bimcr1;       	/* control regs */
		  unsigned char res2;
/*0003*/          unsigned char bimcr2;
		  unsigned char res3;
/*0005*/          unsigned char bimcr3;
		  unsigned char res4;
/*0007*/          unsigned char bimcr4;
		  unsigned char res5;
/*0009*/          unsigned char bimvr1;       	/* vector regs */
		  unsigned char res6;
/*000B*/          unsigned char bimvr2;
		  unsigned char res7;
/*000D*/          unsigned char bimvr3;
		  unsigned char res8;
/*000F*/          unsigned char bimvr4;
} BimOdd;

/* Command ram */
typedef struct {

/*0000*/          short cmd;                   	/* command */
/*0002*/          unsigned short parm1;        	/* 1st parameter word */
/*0004*/          unsigned long ptr1;          	/* 1st pointer */
/*0008*/          unsigned long ptr2;          	/* 2nd pointer */
/*000C*/          unsigned short parm2;        	/* 2nd parameter word */
/*000E*/          unsigned short ipres;        	/* reserved for IPC */
} CmdRam;

/* Abort interrupt structure */
typedef struct {
 
/*0000*/        char res3;                     	/* unused */
/*0001*/        char abint;                     /* abort int location */
} Iabort;
 
/* Firmware reset structure */
typedef struct {
 
/*0000*/        char res4;                      /* unused */
/*0001*/        char reset;                     /* reset location */
} Ireset;
 
/* ISIO overall structure */
typedef struct {
 
/*0000*/          BimOdd bim[0x100];   		/* bim structure */
/*1000*/          Iabort iabort[0x400]; 	/* abort locations */
/*1800*/          Ireset ireset[0x400]; 	/* reset locations */
/*2000*/          unsigned short version;       /* board & rev type */
/*2002*/          char _ires1[0x5FFE];          /* reserved area */
/*8000*/          CmdRam cmdram[16];    	/* command RAMs */
/*8100*/          short abort;                  /* channel # to abort */
/*8102*/          unsigned short sysfail;      	/* for SYSFAIL message */
/*8104*/          unsigned short revision;     	/* board and fware rev */
/*8106*/          unsigned short echo;         	/* channel echo flags */
/*8108*/          unsigned short parity;       	/* parity error flags */
/*810A*/          unsigned short brdet;        	/* break detected flags */
/*810C*/          unsigned short frame;        	/* framing error flags */
/*810E*/          unsigned char res2[0x06F2];  	/* reserved area */
 
/* Warning the following buffer defs may be reconfigured by the
   GLOBINI command. */
 
/*8800*/          char ibufs[16][0x0800];     	/* chan 1-16 buffers */
                  char xbufs[0x10000-0x0800]; 	/* extra space */
} Isio;

/* Structure to define the 68562 register interface as used by the isio
   chini2 command.  This structure does not define the 68562 directly. */
 
typedef struct {
  char cmr1;                    /* command mode reg 1 */
  char cmr2;                    /* command mode reg 2 */
  char syn1;                    /* secondary address 1 */
  char syn2;                    /* secondary address 2 */
  char tpr;                     /* transmitter parameter register */
  char ttr;                     /* transmitter timing register */
  char rpr;                     /* receiver parameter register */
  char rtr;                     /* receiver timint register */
  char omr;                     /* output and misc register */
  char ctcr;                    /* counter/timer control register */
  char ctpl;                    /* counter/timer preset low */
  char ctph;                    /* counter/timer preset high */
  char ccr1;                    /* channel command register first command */
  char ccr2;                    /* channel command register second command */
  char pcr;                     /* pin configuration register */
} ChiniRegs;
 
#endif

