
/**
*
*  BTI708 DRIVER EXAMPLE 3  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP3.C -- Sequential Monitor example
*
**/

/**
*
*  This program opens Card Number 0, and configures the
*  first compatible core to configure the first receive
*  channel and the first transmit channel to operate on
*  the self-test bus.  The tranmsitter is configured
*  with 128 messages that are initialized with
*  incrementing data.  The receive channel is configured
*  to record it's data to the Sequential Monitor.
*
*  An Event Log frequency of 0 is specifed, which will
*  not generate any Event Log List entries for thess channels.
*
*  The program will display the data from the Sequential Record,
*  until a key is pressed.
*
**/

#include "BTICard.h"
#include "BTI708.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

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
	UINT32  j,jj;
	UINT16 Data[100];
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
	consoleprintf("\n    *  EXAMP3  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI708 Example 3                                                  *");
	consoleprintf("\n    *  \"Program for Sequential Monitor example.\"                         *");
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
*  Find the first ARINC 708 core with both a receive and a transmit channel.
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
				xmtfound = BTI708_ChIsXmt(xmtchan,hCore);
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

	consoleprintf("\nUsing ARINC 708 core #%d",corenum);
	consoleprintf("\nUsing receive channel #%d",rcvchan);
	consoleprintf("\nUsing transmit channel #%d",xmtchan);
	consoleprintf("\n");

/**
*
*  Configure receive channel.
*
**/

	BTICard_CardReset(hCore);

	errval = BTI708_ChConfig(CHCFG708_SEQALL|CHCFG708_SELFTEST,rcvchan,hCore);

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
*  Configure transmit channel.
*
**/

	errval = BTI708_ChConfig(CHCFG708_SELFTEST,xmtchan,hCore);

	if (errval)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        channel #%d on core #%d.",xmtchan,corenum);
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
*  Build the transmit channel messages.
*
**/

	errval = BTI708_XmtBuild(MSGCRT708_DEFAULT,200,0,128,xmtchan,hCore);

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

	for (j=0;j<128;j++)							/*Loop through all the messages*/
	{
		for (jj=0;jj<100;jj++) Data[jj] = (j << 8) + jj;
		BTI708_MsgDataWr(Data,j,xmtchan,hCore);	/*Write data to the Device*/
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
*  Loop until a key is hit.
*
**/

	consoleprintf("\nPress any key to exit.");
	consoleprintf("\n\n");

	while (1)
	{
		ch = getch();
		if (ch != ERR) break;

		seqcount = BTICard_SeqBlkRd(seqbuf,sizeof(seqbuf)/sizeof(seqbuf[0]),&blkcnt,hCore);
		BTICard_SeqFindInit(seqbuf,seqcount,&sfinfo);

		while(!BTICard_SeqFindNext708(&pRec708,&sfinfo))
		{
			consoleprintf("Time Stamp: %08lX\n",pRec708->timestamp);
			consoleprintf("Activity:   %04X\n", pRec708->activity);

			// Display extended header data if it exists
			if (BTICard_SeqFindCheckVersion((LPUINT16)pRec708, SEQVER_1))
			{
				consoleprintf("Bitcount:   %03X\n", pRec708->bitcount);
			}

			for (j=0;j<(pRec708->datacount);++j)
			{
				if (!(j%10))
				{
					consoleprintf("\n%4u | ",j);
				}
				consoleprintf("%04X ",pRec708->data[j]);
			}

			move(19,0);
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
