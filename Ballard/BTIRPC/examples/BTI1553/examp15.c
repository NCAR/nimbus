
/**
*
*  BTI1553 DRIVER EXAMPLE 15  (10/23/2009)
*  Copyright (c) 2003-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP15.C -- IRIG Timer example (bus monitor).
*
**/

/**
*
*  This example configures Card Number 0 as a Bus Monitor
*  terminal.  It prints the time-tag (in IRIG format),
*  command words (if any), and status words (if any) for each
*  monitor record.
*
**/

#include "BTICard.h"
#include "BTI1553.h"
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#define	CARDNUM       0
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
	HCARD   hCard;					/*Handle to card*/
	HCORE   hCore;					/*Handle to core*/
	ERRVAL  errval;					/*Error return value*/
	INT     corenum;				/*Core number*/
	INT     channum=0;				/*Channel number*/
	INT     ch;
	BOOL	timetagIsBCD;

	UINT16 seqbuf[2048];
	UINT32  seqcount;
	UINT32  blkcnt;
	LPSEQRECORD1553 pRec1553;
	LPSEQRECORDMORE1553 pRecMore;
	SEQFINDINFO sfinfo;

	BTIIRIGTIME  irigtime;
	struct tm    *curtime;
	time_t       long_time;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP15  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2003-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI1553 Example 15                                                *");
	consoleprintf("\n    *  \"IRIG Timer example (bus monitor).\"                               *");
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

	consoleprintf("\nUsing BTI1553 card #%d",CARDNUM);

/**
*
*  Find the first MIL-STD-1553 core and channel.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (channum=0;channum<MAX_CHANNELS;channum++)
			if (BTI1553_ChIs1553(channum,hCore)) break;

		if (channum != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || channum == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No MIL-STD-1553 channels present in card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing MIL-STD-1553 core #%d",corenum);
	consoleprintf("\nUsing MIL-STD-1553 channel #%d",channum);
	consoleprintf("\n");

/**
*
*  Configure the channel for monitor mode.
*
**/

	BTICard_CardReset(hCore);

	errval = BTI1553_MonConfig(MONCFG1553_DEFAULT,channum,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring for",errval);
		consoleprintf("\n        monitor mode on channel #%d on core #%d.",channum,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Configure the sequential record.
*
**/

	errval = BTICard_SeqConfig(SEQCFG_DEFAULT,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        the sequential record on core #%d.",corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Configure the IRIG timer. (DEFAULT -> bit rate = IRIGB, internal, slave/receiver)
*  Sequential monitor will store IRIG time in sequential records
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

	consoleprintf("\n\nStart Time:%02u:%02u %02u.%03u.%03u \n",
			irigtime.hours,
			irigtime.min,
			irigtime.sec,
			irigtime.msec,
			irigtime.usec);

/**
*
*  Start the card to begin receiving messages.
*
**/

	BTICard_CardStart(hCore);

/**
*
*  Read the entire contents of the monitor and display the results.
*
**/

	consoleprintf("\nPress any key to exit....\n\n");

	while(1)
	{
		ch = getch();
		if (ch != ERR) break;

		/* Empty entire contents of sequential monitor into a buffer. */
		seqcount = BTICard_SeqBlkRd(seqbuf,2048,&blkcnt,hCore);

		BTICard_SeqFindInit(seqbuf,seqcount,&sfinfo);

		while(!BTICard_SeqFindNext1553(&pRec1553,&sfinfo))
		{
			/* Get extended data for this record. */
			BTICard_SeqFindMore1553(&pRecMore,pRec1553);

			if (!timetagIsBCD)
			{
				//Convert binary timetag to BCD
				BTICard_IRIGTimeBinToBCD(&(pRecMore->timestamph),
										 &(pRec1553->timestamp),
										 pRecMore->timestamph,
										 pRec1553->timestamp);
			}

			consoleprintf("\n");

			consoleprintf(" Ch:%02u",(pRec1553->activity & MSGACT1553_CHMASK) >> MSGACT1553_CHSHIFT);
			consoleprintf(" Time:%02u:%02u %02u.%03u.%03u    ",
					BTICard_IRIGFieldGetHours(   pRecMore->timestamph, pRec1553->timestamp),
					BTICard_IRIGFieldGetMin(     pRecMore->timestamph, pRec1553->timestamp),
					BTICard_IRIGFieldGetSec(     pRecMore->timestamph, pRec1553->timestamp),
					BTICard_IRIGFieldGetMillisec(pRecMore->timestamph, pRec1553->timestamp),
					BTICard_IRIGFieldGetMicrosec(pRecMore->timestamph, pRec1553->timestamp));

			if (pRec1553->activity & MSGACT1553_RCVCWD1) consoleprintf(" Cwd1:%04X",pRec1553->cwd1);
			if (pRec1553->activity & MSGACT1553_RCVCWD2) consoleprintf(" Cwd2:%04X",pRec1553->cwd2);
			if (pRec1553->activity & MSGACT1553_RCVSWD1) consoleprintf(" Swd1:%04X",pRec1553->swd1);
			if (pRec1553->activity & MSGACT1553_RCVSWD2) consoleprintf(" Swd2:%04X",pRec1553->swd2);

			if (pRec1553->error & MSGERR1553_NORESP) consoleprintf(" No response!");
			if (pRec1553->error & MSGERR1553_ANYERR) consoleprintf(" Error!");
		}
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
