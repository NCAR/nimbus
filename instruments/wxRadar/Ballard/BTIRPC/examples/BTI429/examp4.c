
/**
*
*  BTI429 DRIVER EXAMPLE 4  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP4.C -- Explicit message timing
*
**/

/**
*
*  This program opens Card Number 0, finds the first
*  compatible core, and builds a transmit schedule
*  using explicit message timing. The self-test
*  wraparound is enabled and filters are set to
*  receive the messages.  The minimum and maximum elapsed
*  time for each message is displayed.
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
	MSGSTRUCT429 xmt_airspeed, xmt_altitude, rcv_airspeed, rcv_altitude;
	MSGFIELDS429 airspeed_record, altitude_record;
	INT ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP4  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI429 Example 4                                                  *");
	consoleprintf("\n    *  \"Program for explicit message timing.\"                            *");
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
*  Configure rcvchan to receive at low speed with the
*  Min/Max Elapse Time option also enabled.
*  Note that the self-test wraparound is also enabled.
*
*  Configure xmtchan to transmit at low speed.
*  Note that the self-test wraparound is also enabled.
*  The external line drivers are disabled, and xmtchan is
*  connected internally to all the receive channels.
*
**/

	BTICard_CardReset(hCore);                   /*Reset the card*/

	errval = BTI429_ChConfig(CHCFG429_MAXMIN|CHCFG429_SELFTEST,rcvchan,hCore);

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
*  for the receive channel.  Two filters are set for the
*  two messages of interest.
*
**/

	rcv_airspeed.addr = BTI429_FilterSet(MSGCRT429_DEFAULT,0206,SDIALL,rcvchan,hCore);
	rcv_altitude.addr = BTI429_FilterSet(MSGCRT429_DEFAULT,0212,SDIALL,rcvchan,hCore);

/**
*
*  Create a message for each of the labels we are transmitting.
*
**/

	xmt_airspeed.addr = BTI429_MsgCreate(MSGCRT429_DEFAULT,hCore);
	xmt_altitude.addr = BTI429_MsgCreate(MSGCRT429_DEFAULT,hCore);

/**
*
*  Next, a schedule is built by creating explicit message timing.
*
**/

	BTI429_SchedMsg(xmt_altitude.addr,xmtchan,hCore);
	BTI429_SchedMsg(xmt_airspeed.addr,xmtchan,hCore);
	BTI429_SchedGap(495,xmtchan,hCore);
	BTI429_SchedMsg(xmt_altitude.addr,xmtchan,hCore);
	BTI429_SchedGap(531,xmtchan,hCore);

/**
*
*  Those messages that are to be transmitted are initialized.
*
**/

	BTI429_MsgDataWr(0x0000008A,xmt_altitude.addr,hCore);
	BTI429_MsgDataWr(0x00000086,xmt_airspeed.addr,hCore);

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

		BTI429_MsgBlockRd(&airspeed_record,rcv_airspeed.addr,hCore);
		BTI429_MsgBlockRd(&altitude_record,rcv_altitude.addr,hCore);

		consoleprintf("\r");

		consoleprintf("AirSpeed: min/max=%2ums/%2ums ",
		(airspeed_record.mintime / 1000),
		(airspeed_record.maxtime / 1000));

		consoleprintf("Altitude: min/max=%2ums/%2ums ",
		(altitude_record.mintime / 1000),
		(altitude_record.maxtime / 1000));

		consoleprintf("         ");
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
