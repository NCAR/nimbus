
/**
*
*  BTI429 DRIVER EXAMPLE 5  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP5.C -- Transmit and Receive List Buffer example
*
**/

/**
*
*  This program opens the Card Number 0, finds the first
*  compatible core, and configures transmit and receive
*  list buffers.  The program transmits one message filled
*  with ramping data in the associated list buffer.  The
*  receive list buffer is read and displays the data to the
*  screen.
*
**/

#include "BTICard.h"
#include "BTI429.h"
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
	HCARD hCard;
	HCORE hCore;
	ERRVAL errval;
	INT corenum;
	INT rcvchan=0;
	INT xmtchan=0;
	BOOL rcvfound, xmtfound;
	UINT32 j;
	MSGSTRUCT429 xmt_airspeed, rcv_airspeed;
	LISTADDR xmtlistaddr, rcvlistaddr;
	INT ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP5  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI429 Example 5                                                  *");
	consoleprintf("\n    *  \"Program for transmit and receive list buffers.\"                  *");
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

	consoleprintf("\nUsing ARINC 429 card #%d",CARDNUM);

/**
*
*  Find the first ARINC 429 core with both a receive and a transmit channel.
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
			rcvfound = BTI429_ChIsRcv(rcvchan,hCore);
			if (rcvfound) break;
		}

		if (rcvfound)
		{
			for (xmtchan=0;xmtchan<MAX_CHANNELS;xmtchan++)	/* Search for transmit channel. */
			{
				xmtfound = BTI429_ChIsXmt(xmtchan,hCore);
				if (xmtfound) break;
			}
		}

		if (rcvfound && xmtfound)	break;
	}

	if (errval || (corenum == MAX_CORES) || !rcvfound || !xmtfound)
	{
		consoleprintf("\nError:  No compatible ARINC 429 core present in card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing ARINC 429 core #%d",corenum);
	consoleprintf("\nUsing receive channel #%d",rcvchan);
	consoleprintf("\nUsing transmit channel #%d",xmtchan);
	consoleprintf("\n");

/**
*
*  Configure rcvchan for automatic speed detection with
*  the self-test wraparound enabled.
*  Configure xmtchan to transmit at low speed with
*  the self-test wraparound enabled.
*
**/

	BTICard_CardReset(hCore);            /*Reset the card*/

	errval = BTI429_ChConfig(CHCFG429_AUTOSPEED|CHCFG429_SELFTEST,rcvchan,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        channel #%d on core #%d.",rcvchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	errval = BTI429_ChConfig(CHCFG429_SELFTEST,xmtchan,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        channel #%d on core #%d.",xmtchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Before any messages can be received, filters must be set
*  for the receive channel.
*
**/

	rcv_airspeed.addr = BTI429_FilterSet(MSGCRT429_DEFAULT,0206,SDIALL,rcvchan,hCore);

/**
*
*  Create a message for each of the labels we are transmitting.
*
**/

	xmt_airspeed.addr = BTI429_MsgCreate(MSGCRT429_DEFAULT,hCore);

/**
*
*  Next, a schedule is built by creating explicit message timing.
*
**/

	BTI429_SchedMsg(xmt_airspeed.addr,xmtchan,hCore);
	BTI429_SchedGap(1218,xmtchan,hCore);

/**
*
*  Create the transmit and receive list buffers.
*
*  The transmit list buffer will be a circular buffer,
*  which means it should be preloaded with data, and
*  it will continuously transmit that data.
*
*  The receive list buffer will be a FIFO, which we will
*  read out of and display to the screen.
*
**/

	xmtlistaddr = BTI429_ListXmtCreate(LISTCRT429_CIRCULAR,256,xmt_airspeed.addr,hCore);
	rcvlistaddr = BTI429_ListRcvCreate(LISTCRT429_FIFO,16,rcv_airspeed.addr,hCore);

/**
*
*  Initialize the transmit list buffer.
*
**/

	for (j=0x00000;j<0x1000000;j+=0x10000)
	{
		BTI429_ListDataWr((0x00000086 | j),xmtlistaddr,hCore);
	}

/**
*
*  Start operation of the card.
*
**/

	BTICard_CardStart(hCore);

/**
*
*  Display data on the screen.
*
**/

	consoleprintf("\nPress any key to exit....\n\n");

	while(1)
	{
		ch = getch();
		if (ch != ERR) break;

		rcv_airspeed.data = BTI429_ListDataRd(rcvlistaddr,hCore);

		if (rcv_airspeed.data) consoleprintf("\rAirSpeed=%08lX ",rcv_airspeed.data);
	}

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
