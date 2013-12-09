
/**
*
*  BTI708 DRIVER EXAMPLE 11  (10/23/2009)
*  Copyright (c) 2003-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP11.C -- Variable Bit Length example
*
**/

/**
*
*  This program opens Card Number 0, and configures the
*  first transmit channel on the first 708 Core to transmit
*  a sequence of 32 messages at a rate of 200Hz. The messages
*  are automatically assigned the numbers 0 through 31.
*
*  The transmitted and received bit lenghts are displayed on
*  the console.
*
*  The user can then control the bit length using the LEFT and
*  RIGHT arrow keys.
*
*  The program continues in this manner until a key is
*  pressed.
*
**/

#include "BTICard.h"
#include "BTI708.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

#define	FRAMEFREQ	  100          /*Frame frequency of 200Hz*/
#define LOGFREQ		  16           /*Generate a Log Event after every 16th message*/
#define	FRAMECOUNT	  32           /*Transmit a total of 32 messages*/

#define BIT_HIGH	  1856
#define BIT_LOW		  1

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(void)
{
	HCARD  hCard;
	HCORE  hCore;
	ERRVAL errval;
	INT    corenum;
	INT    xmtchan=0;
	INT    rcvchan=0;
	BOOL   xmtfound;
	BOOL   rcvfound;
	INT    cfgflag;
	INT    j,jj;
	UINT16 Data[116];
	UINT16 bitlength = 1600;
	BOOL   done;
	INT    ch;

	UINT16 seqbuf[2048];
	UINT32 seqcount;
	UINT32 blkcnt;
	LPSEQRECORD708 pRec708;
	SEQFINDINFO sfinfo;

/**
*
*  Create a display window.
*
**/

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP11  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2003-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI708 Example 11                                                 *");
	consoleprintf("\n    *  \"Program for variable bit length example.\"                        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n");

/**
*
*  Open the card with the specified card number.
*  An error value is returned which is tested
*  to determine if an error occurred.
*
**/

	errval = BTICard_CardOpen(&hCard,CARDNUM);

	if (errval)
	{
		consoleprintf("\nError:  Either card number %u is not present, or",CARDNUM);
		consoleprintf("\n        an error occurred (%i) opening the card.",errval);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		consoleexit(1);
	}

/**
*
*  Find the first ARINC 708 core with a receive and transmit channel that supports variable bit length.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		rcvfound = 0;
		xmtfound = 0;

		errval = BTICard_CoreOpen(&hCore,corenum,hCard);	/* Open a handle to the core. */
		if (errval) break;

		for (rcvchan=0;rcvchan<MAX_CHANNELS;rcvchan++)		/* Search for receive channel. */
		{
			rcvfound = BTI708_ChIsRcv(rcvchan,hCore);
			if (rcvfound) break;
		}

		if (rcvfound)
		{
			for (xmtchan=0;xmtchan<MAX_CHANNELS;xmtchan++)	/* Search for transmit channel. */
			{
				xmtfound = (BTI708_ChIsXmt(xmtchan,hCore) && BTI708_ChGetInfo(INFO708_VARBIT,xmtchan,hCore));
				if (xmtfound) break;
			}
		}

		if (rcvfound && xmtfound)	break;
	}

	if (errval || (corenum == MAX_CORES) || !rcvfound || !xmtfound)
	{
		consoleprintf("\nError:  No compatible ARINC 708 core present in card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\n\tUsing ARINC 708 core #%d",corenum);
	consoleprintf("\n\tUsing receive channel #%d",rcvchan);
	consoleprintf("\n\tUsing variable bit length transmit channel #%d",xmtchan);
	consoleprintf("\n");

/**
*
*  Configure receive channel.
*
**/

	BTICard_CardReset(hCore);

	cfgflag = CHCFG708_DEFAULT | CHCFG708_SEQALL | CHCFG708_SELFTEST | CHCFG708_VARBIT;

	errval = BTI708_ChConfig(cfgflag,rcvchan,hCore);

	if (errval)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        channel #%d on core #%d.",rcvchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Build the receive channel messages.
*
**/

	errval = BTI708_RcvBuild(MSGCRT708_DEFAULT,0,1,rcvchan,hCore);

	if (errval)
	{
		consoleprintf("\nError:  An error was encountered (%i) while building",errval);
		consoleprintf("\n        channel #%d on core #%d.",rcvchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Configure xmtchan for variable bit length and self test
*
**/

	cfgflag = CHCFG708_DEFAULT | CHCFG708_SELFTEST | CHCFG708_VARBIT;

	errval = BTI708_ChConfig(cfgflag,xmtchan,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        channel #%d on core #%d.",xmtchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Build the transmit channel messages.
*
**/

	errval = BTI708_XmtBuild(MSGCRT708_DEFAULT,FRAMEFREQ,LOGFREQ,FRAMECOUNT,xmtchan,hCore);

	if (errval)
	{
		consoleprintf("\nError:  An error was encountered (%i) while building",errval);
		consoleprintf("\n        channel #%d on core #%d.",xmtchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Create and write the data for the messages.
*
**/

	/* Initialize the data array */
	for (jj=0;jj<116;jj++) Data[jj] = jj;

	bitlength = (bitlength>BIT_HIGH) ? BIT_HIGH : bitlength;
	bitlength = (bitlength<BIT_LOW)  ? BIT_LOW  : bitlength;

	for (j=0;j<FRAMECOUNT;j++)					/*Loop through all the messages*/
	{
		/* Write the variable length data to the device */
		errval = BTI708_MsgVarDataWr(Data,bitlength,j,xmtchan,hCore);
		if (errval)
		{
			consoleprintf("\nError:  An error was encountered (%i) while writing variable",errval);
			consoleprintf("\n        bit length data on channel #%d on core #%d.",xmtchan,corenum);
			consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
			consoleprintf("\n");
			BTICard_CardClose(hCard);
			consoleexit(1);
		}
	}

/**
*
*  Configure the Sequential Monitor.
*
**/

	errval = BTICard_SeqConfig(SEQCFG_DEFAULT,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        the sequential monitor on core #%d.",corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Start operation of the card.
*
**/

	BTICard_CardStart(hCore);

/**
*
*  Move to display window.
*
**/

	move(16,0);

/**
*
*  Change bitlength values when arrow keys are pressed. Exit when ESC is pressed.
*
**/

	consoleprintf("\n\tOPTIONS:");
	consoleprintf("\n\tARROW RIGHT = Add 1 to Transmit bit length");
	consoleprintf("\n\tARROW LEFT  = Subtract 1 from Transmit bit length");
	consoleprintf("\n\tESC         = Exit program");
	consoleprintf("\n\n");

	for (done=0;!done;)
	{
		seqcount = BTICard_SeqBlkRd(seqbuf,sizeof(seqbuf)/sizeof(seqbuf[0]),&blkcnt,hCore);

		BTICard_SeqFindInit(seqbuf,seqcount,&sfinfo);

		while(!BTICard_SeqFindNext708(&pRec708,&sfinfo))
		{
			consoleprintf("\tTime Stamp: %08lX\n",pRec708->timestamp);
			consoleprintf("\tActivity:   %04X\n", pRec708->activity);


			consoleprintf("\n\t(Tx) Bit Length: %4d",bitlength);
			consoleprintf("\n\t(Rx) Bit Length: %4d",pRec708->bitcount);

			move(22,0);
		}

		ch = getch();
		if (ch != ERR)
		{
			switch (ch)
			{
				case 27:	/* ESC */
				{
					done=1;
					break;
				}
				case KEY_LEFT:	/* LEFT  */
				case KEY_RIGHT:	/* RIGHT */
				{
					if (ch==KEY_RIGHT)	bitlength = (bitlength+1>BIT_HIGH) ? BIT_HIGH : bitlength+1;
					else        bitlength = (bitlength-1<BIT_LOW)  ? BIT_LOW  : bitlength-1;

					for (j=0;j<FRAMECOUNT;j++)					/*Loop through all the messages*/
					{
						/* Write the variable length data to the device */
						errval = BTI708_MsgVarDataWr(Data,bitlength,j,xmtchan,hCore);
						if (errval)
						{
							consoleprintf("\nError:  An error was encountered (%i) while writing variable",errval);
							consoleprintf("\n        bit length data on channel #%d on core #%d.",xmtchan,corenum);
							consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
							consoleprintf("\n");
							BTICard_CardClose(hCard);
							consoleexit(1);
						}
					}

					break;
				}
			}
		}
	}

/**
*
*  Move to bottom of screen.
*
**/

	move(32,0);

/**
*
*  Stop the card.
*
**/

	BTICard_CardStop(hCore);

/**
*
*  The card MUST be closed before exiting the program.
*
**/

	BTICard_CardClose(hCard);

	consoleexit(0);
	return(0);
}

void consoleentry(void)
{
	initscr();
	cbreak();
	keypad(stdscr,TRUE);
	noecho();
	nonl();
	intrflush(stdscr,FALSE);
	timeout(0);
	scrollok(stdscr,TRUE);
}

void consoleexit(INT value)
{
	consoleprintf("\n");
	endwin();
	exit(value);
}

void consoleprintf(const char * str, ...)
{
	CHAR buf[1024];
	va_list arg_maker;

	va_start(arg_maker,str);
	vsnprintf(buf,1024,str,arg_maker);
	va_end(arg_maker);
	printw(buf);
	refresh();
}
