
/**
*
*  BTI708 DRIVER EXAMPLE 8  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP8.C -- IRIG timer example
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
*  The program will display the IRIG time and elapsed time from
*  receive message structure 0, until a key is pressed.
*
**/

#include "BTICard.h"
#include "BTI708.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define	CARDNUM		  0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

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
	BOOL   timetagIsBCD;
	UINT32  j,jj;
	UINT16 Data[100];
	MSGFIELDS708 msgfields;
	BTIIRIGTIME  irigtime;
	struct tm    *curtime;
	time_t       long_time;
	INT    ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP8  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2003-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI708 Example 8                                                  *");
	consoleprintf("\n    *  \"Program for IRIG timer example.\"                                 *");
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

	errval = BTI708_ChConfig(CHCFG708_ELAPSE|CHCFG708_SELFTEST,rcvchan,hCore);

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
*  Configure the IRIG timer. (DEFAULT -> bit rate = IRIGB, internal, slave/reciever)
*  Sequential monitor will store IRIG time in sequetial records
*
**/

	errval = BTICard_IRIGConfig(IRIGCFG_DEFAULT,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        the IRIG timer on core #%d.",corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	timetagIsBCD = (TIMETAG_FORMAT_BCD == BTICard_TimerStatus(hCore)) ? TRUE : FALSE;

/**
*
*  Synchronize the on core IRIG timer with the computer local time
*
**/

	time( &long_time );								/* Get time as long integer. */
	curtime = localtime( &long_time );				/* Convert to local time. */

	irigtime.days  = (UINT16)curtime->tm_yday;		/* Day of year (0 - 365; January 1 = 0) */
	irigtime.hours = (UINT16)curtime->tm_hour;		/* Hours after midnight (0 - 23) */
	irigtime.min   = (UINT16)curtime->tm_min;		/* Minutes after hour (0 - 59) */
	irigtime.sec   = (UINT16)curtime->tm_sec;		/* Seconds after minute (0 - 59) */
	irigtime.msec  = 0;								/* The tm structure does not include milliseconds. */
	irigtime.usec  = 0;								/* The tm structure does not include microseconds. */

	errval = BTICard_IRIGWr(&irigtime,hCore);
	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while writting",errval);
		consoleprintf("\n        the IRIG time on core #%d.",corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
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
*  Loop until a key is hit.
*
**/

	consoleprintf("\nPress any key to exit.");
	consoleprintf("\n");
	consoleprintf("\n");

	while (1)
	{
		ch = getch();
		if (ch != ERR) break;

		BTI708_MsgBlockRd(&msgfields, 0, rcvchan, hCore);

		if (!timetagIsBCD)
		{
			//Convert binary timetag to BCD
			BTICard_IRIGTimeBinToBCD(&msgfields.timetagh,
									 &msgfields.timetag,
									 msgfields.timetagh,
									 msgfields.timetag);

			//Convert binary elapsetime to BCD
			BTICard_IRIGTimeBinToBCD(0,
									 &msgfields.elapsetime,
									 0,
									 msgfields.elapsetime);
		}

		consoleprintf("\r");

		consoleprintf("TT: %02u:%02u %02u.%03u.%03u    ",
				BTICard_IRIGFieldGetHours(msgfields.timetagh,msgfields.timetag),
				BTICard_IRIGFieldGetMin(msgfields.timetagh,msgfields.timetag),
				BTICard_IRIGFieldGetSec(msgfields.timetagh,msgfields.timetag),
				BTICard_IRIGFieldGetMillisec(msgfields.timetagh,msgfields.timetag),
				BTICard_IRIGFieldGetMicrosec(msgfields.timetagh,msgfields.timetag));

		consoleprintf("Elapse Time: %02ums ",BTICard_IRIGFieldGetMillisec(0x0000, msgfields.elapsetime));

		consoleprintf("    ");
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
