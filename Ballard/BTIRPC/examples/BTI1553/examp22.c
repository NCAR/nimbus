
/**
*
*  BTI1553 DRIVER EXAMPLE 22  (10/23/2009)
*  Copyright (c) 2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP22.C -- Wrapping subaddresses across different RTs
*
**/

/**
*
*  This example configures 2 RTs, one with a receive and one
*  with a transmit subaddress to illustrate RT to RT wrapping
*  of data.
*
**/

#include <BTICard.h>
#include <BTI1553.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define	CARDNUM		  0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(void)
{
	HCARD  hCard;				/*Handle to card*/
	HCORE  hCore;				/*Handle to core*/
	ERRVAL errval;				/*Error return value*/
	INT    corenum;				/*Core number*/
	INT    channum=0;			/*Channel number*/
	INT    ch;

	MSGADDR RTMsgs[2];			/*RT data pointers*/

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP22  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2009  Ballard Technology, Inc.  Everett, WA, USA.       *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI1553 Example 22                                                *");
	consoleprintf("\n    *  \"Wrapping subaddresses across different RTs\"                      *");
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
*	Configure the channel for remote terminal mode. Two remote
*	terminals are configured.
*
**/

	BTICard_CardReset(hCore);		/*Reset the core*/

	errval = BTI1553_RTConfig(RTCFG1553_DEFAULT, 1, channum,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring RT",errval);
		consoleprintf("\n        mode on channel #%d on core #%d RT number %d",channum,corenum,1);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	errval = BTI1553_RTConfig(RTCFG1553_DEFAULT, 2, channum,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring RT",errval);
		consoleprintf("\n        mode on channel #%d on core #%d RT number %d",channum,corenum,2);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/***
*
*	Get the message addresses (message pointers) of each RT.
*	For test purposes, the transmit subaddress is overwritten with the receive
*	subaddress, however, any subaddress can be pointed to any other.
*
***/

	RTMsgs[0] = BTI1553_RTGetMsg(SUBADDRESS, 1, RCV, 1, channum, hCore);
	RTMsgs[1] = BTI1553_RTGetMsg(SUBADDRESS, 2, XMT, 1, channum, hCore);

/***
*
*	Overwrite RT 2's message pointer with the RT 1's message pointer
*
***/

	BTI1553_DescPtrWr(RTMsgs[0], SUBADDRESS, 2, XMT, 1, channum, hCore);

/***
*
*	Start the Ballard hardware. The hardware will run without host intervention until halted.
*
***/

	BTICard_CardStart(hCore);

	consoleprintf("\nPress any key to exit....\n\n");

	while(1)
	{
		ch = getch();
		if (ch != ERR) break;
	}

/**
*
*  Stop the card.
*
**/

	BTICard_CardStop(hCore);

/**
*
*  The card MUST be closed before continuing
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
