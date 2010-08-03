
/**
*
*  BTIDIO DRIVER EXAMPLE 5  (10/23/2009)
*  Copyright (c) 2005-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP5.C -- IRIG Timer example
*
**/

/**
*
*  This program opens Card Number 0 and configures the
*  first compatible core to monitor the input values of
*  the discretes of Bank A.  A time-tag for each message
*  is converted from the IRIG time and displayed.
*
**/

#include "BTICard.h"
#include "BTIDIO.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define	CARDNUM          0
#define	MAX_CORES        4
#define	MAX_BANKS        4

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(int argc, char* argv[])
{
	HCARD hCard;
	HCORE hCore;
	ERRVAL errval;
	INT corenum;
	INT banknum = 0;
	UINT32 bankcfg;
	UINT16 seqbuf[2048];
	UINT32 seqcount;
	UINT32 blkcnt;
	BOOL   timetagIsBCD;
	LPSEQRECORDDIO pRecDIO;
	SEQFINDINFO sfinfo;
	BTIIRIGTIME  irigtime;
	struct tm    *curtime;
	time_t       long_time;
	INT ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP5  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2005-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTIDIO Example 5                                                  *");
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

	consoleprintf("\nUsing DIO card #%d",CARDNUM);

/**
*
*  Find the first core with a DIO bank.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (banknum=0;banknum<MAX_BANKS;banknum++)
		{
			if (BTIDIO_BankIsDIO(banknum,hCore))	break;
		}

		if (banknum != MAX_BANKS) break;
	}

	if (errval || corenum == MAX_CORES || banknum == MAX_BANKS)
	{
		consoleprintf("\nError:  No DIO banks present in card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing DIO core #%d",corenum);
	consoleprintf("\nUsing Bank %c",banknum + 'A');
	consoleprintf("\n");

/**
*
*  Configure banknum as a shunt, input/output bank with
*  Sequential Monitor of all discretes
*
**/

	BTICard_CardReset(hCore);

	/*
	*  Threshold:   1.4V, TTL levels
	*  Sample Rate: 1ms clock, no divide value --> 1ms sampling
	*/
	bankcfg = BANKCFGDIO_SERIES | BANKCFGDIO_INPUT | BANKCFGDIO_SEQALL;
	errval = BTIDIO_BankConfig(bankcfg,THRESHOLDDIO_TTL,SAMPLERATEDIO_MS | 0x0001,banknum,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        bank #%d on core #%d.",banknum,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Configure the Sequential Monitor for continuous mode.
*
**/

	errval = BTICard_SeqConfig(SEQCFG_DEFAULT,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring the sequential record.",errval);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
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

	timetagIsBCD = (TIMETAG_FORMAT_BCD == BTICard_TimerStatus(hCore)) ? TRUE : FALSE;

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

	consoleprintf("\nPress any key to exit...");
	consoleprintf("\n");
	consoleprintf("\n");

	while (1)
	{
		ch = getch();

		if (ch != ERR) break;

		/* Initialize Seq Find and print the state and timetag of any new records*/
		seqcount = BTICard_SeqBlkRd(seqbuf,sizeof(seqbuf)/sizeof(seqbuf[0]),&blkcnt,hCore);

		BTICard_SeqFindInit(seqbuf,seqcount,&sfinfo);

		while(!BTICard_SeqFindNextDIO(&pRecDIO,&sfinfo))
		{

			if (!timetagIsBCD)
			{
				//Convert binary timetag to BCD
				BTICard_IRIGTimeBinToBCD(&(pRecDIO->timestamph),
										 &(pRecDIO->timestamp),
										 pRecDIO->timestamph,
										 pRecDIO->timestamp);
			}

			consoleprintf("\nBank %c:  DIN: ",pRecDIO->bank + 'A');

			consoleprintf("%c",(pRecDIO->state & 0x0080) ? '1' : '0');
			consoleprintf("%c",(pRecDIO->state & 0x0040) ? '1' : '0');
			consoleprintf("%c",(pRecDIO->state & 0x0020) ? '1' : '0');
			consoleprintf("%c",(pRecDIO->state & 0x0010) ? '1' : '0');

			consoleprintf("%c",(pRecDIO->state & 0x0008) ? '1' : '0');
			consoleprintf("%c",(pRecDIO->state & 0x0004) ? '1' : '0');
			consoleprintf("%c",(pRecDIO->state & 0x0002) ? '1' : '0');
			consoleprintf("%c",(pRecDIO->state & 0x0001) ? '1' : '0');

			consoleprintf(", Time: %02u:%02u %02u.%03u.%03u      ",
				BTICard_IRIGFieldGetHours(pRecDIO->timestamph,pRecDIO->timestamp),
				BTICard_IRIGFieldGetMin(pRecDIO->timestamph,pRecDIO->timestamp),
				BTICard_IRIGFieldGetSec(pRecDIO->timestamph,pRecDIO->timestamp),
				BTICard_IRIGFieldGetMillisec(pRecDIO->timestamph,pRecDIO->timestamp),
				BTICard_IRIGFieldGetMicrosec(pRecDIO->timestamph,pRecDIO->timestamp)
				);

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
