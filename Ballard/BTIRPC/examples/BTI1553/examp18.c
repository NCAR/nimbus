
/**
*
*  BTI1553 DRIVER EXAMPLE 18  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP18.C -- Engineering Unit Example
*
**/

/**
*
*  This example configures Card Number 0 as a Bus Monitor
*  terminal.  A terminal address filter is applied to only
*  record messages for TA #1. It prints the time-tag,
*  command words (if any), and status words (if any) for
*  each monitor record. An engineering unit conversion is
*  applied to the first data word in messages with Cwd1=0x0843.
*  This program can be used to monitor data from example 2 or
*  example 5.
*
*  The engineering unit conversion scales the unsigned field by the
*  resolution then adds the offset (i.e., unsigned BNR).
*
**/

#include "BTICard.h"
#include "BTI1553.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

#define	RESOLUTION  2		/* Field Resolution */
#define OFFSET      0		/* Field Offset */

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

	UINT16 seqbuf[2048];
	UINT32  seqcount;
	UINT32  blkcnt;
	LPSEQRECORD1553 pRec1553;
	SEQFINDINFO sfinfo;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP18  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI1553 Example 18                                                *");
	consoleprintf("\n    *  \"Engineering Unit Example\"                                        *");
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
		consoleprintf("\nError:  No MIL-STD-1553 channels present in cardnum %d.",CARDNUM);
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
*  Set the Terminal Address Filter to only monitor TA #1
*
**/

	errval = BTI1553_MonFilterTA(0x00000002, channum, hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while setting monitor",errval);
		consoleprintf("\n        filters on channel #%d on core #%d.",channum,corenum);
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
*  Start the card to begin transmitting, receiving and monitoring messages.
*
**/

	BTICard_CardStart(hCore);

/**
*
*  Read the entire contents of the monitor and display the results.
*  The engineering unit conversion is applied to the first data word in the
*  message with Cwd1=0x0843.
*
**/

	consoleprintf("\nPress any key to exit....\n\n");

	while(1)
	{
		ch = getch();
		if (ch != ERR) break;

		seqcount = BTICard_SeqBlkRd(seqbuf,2048,&blkcnt,hCore);

		BTICard_SeqFindInit(seqbuf,seqcount,&sfinfo);

		while(!BTICard_SeqFindNext1553(&pRec1553,&sfinfo))
		{
			consoleprintf("\n");

			consoleprintf(" Ch:%02u",(pRec1553->activity & MSGACT1553_CHMASK) >> MSGACT1553_CHSHIFT);
			consoleprintf(" Time:%lu",pRec1553->timestamp);

			if (pRec1553->activity & MSGACT1553_RCVCWD1) consoleprintf(" Cwd1:%04X",pRec1553->cwd1);
			if (pRec1553->activity & MSGACT1553_RCVCWD2) consoleprintf(" Cwd2:%04X",pRec1553->cwd2);
			if (pRec1553->activity & MSGACT1553_RCVSWD1) consoleprintf(" Swd1:%04X",pRec1553->swd1);
			if (pRec1553->activity & MSGACT1553_RCVSWD2) consoleprintf(" Swd2:%04X",pRec1553->swd2);

			if (pRec1553->error & MSGERR1553_NORESP)        // No Response
			{
				consoleprintf(" No response!");
			}
			else if (pRec1553->error & MSGERR1553_ANYERR)   // Error
			{
				consoleprintf(" Error!");
			}
			else if (pRec1553->cwd1 == 0x0843)              // Success!
			{
				consoleprintf(" Field:%04X", ((UINT16)BTICard_ValGetBits(pRec1553->data[0],0,7) * RESOLUTION) + OFFSET);
			}
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
