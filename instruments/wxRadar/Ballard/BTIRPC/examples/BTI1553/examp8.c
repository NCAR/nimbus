
/**
*
*  BTI1553 DRIVER EXAMPLE 8  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP8.C -- Errors in RT Transmissions
*
**/

/**
*
*  This example requires a Ballard 1553 C version Device or higher.
*  It configures Card Number 0 as an RT and configures a response
*  time error for transmit commands of subaddress 2.  Each time the
*  subaddress is accessed, the response time is incremented by
*  0.1us.
*
**/

#include "BTICard.h"
#include "BTI1553.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

#ifndef	TRUE
#define	TRUE 1
#endif

#ifndef	FALSE
#define	FALSE 0
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

	UINT16  GapLength;				/*Gap in units of 0.1us*/

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP8  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI1553 Example 8                                                 *");
	consoleprintf("\n    *  \"Errors in RT Transmissions\"                                      *");
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
*  Find the first MIL-STD-1553 core and C or D version channel.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (channum=0;channum<MAX_CHANNELS;channum++)
		{
			if ( BTI1553_ChGetInfo(INFO1553_ERRORGEN,channum,hCore) ) break;
		}

		if (channum != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || channum == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No MIL-STD-1553 version C or D channels present in card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing MIL-STD-1553 core #%d",corenum);
	consoleprintf("\nUsing MIL-STD-1553 channel #%d",channum);
	consoleprintf("\n");

/**
*
*  Configure the channel for remote terminal mode.
*
**/

	BTICard_CardReset(hCore);

	errval = BTI1553_RTConfig(RTCFG1553_DEFAULT,1,channum,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring for",errval);
		consoleprintf("\n        remote terminal mode on channel #%d on core #%d.",channum,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Define the type and location of the gap.
*
**/

	GapLength = 120;                    /*Set initial gap length to 12us (Each GapLength=0.1 microseconds)*/

	BTI1553_ErrorDefine(ERRDEF1553_GAP,    /*Generate a gap*/
						GapLength,         /*Set the length of the gap*/
						0,                 /*Unused parameter*/
						ERRDEF1553_SWD1,   /*Gap preceeds the status word*/
						0,                 /*Unused parameter*/
						channum,           /*Channel number*/
						hCore);

	BTI1553_ErrorTagRT(TRUE,SUBADDRESS,1,XMT,2,channum,hCore);
	BTI1553_ErrorCtrl(ERRCTRL1553_ONCE | ERRCTRL1553_TAGMSG,channum,hCore);

/**
*
*  Start the card to begin responding to transmissions.
*
**/

	BTICard_CardStart(hCore);

/**
*
*  Loop until a key is hit.
*
**/

	consoleprintf("\nPress any key to exit....\n\n");

	while (1)
	{
		ch = getch();
		if (ch != ERR) break;

		if (BTI1553_ErrorSent(channum,hCore) /*AND ANOTHER ERROR DESIRED*/)
		{
			BTI1553_ErrorDefine(ERRDEF1553_GAP,   /*Change the length of the gap*/
							++GapLength,          /*Set the length of the new gap*/
							0,                    /*Unused parameter*/
							ERRDEF1553_SWD1,      /*Gap preceeds the status word*/
							0,                    /*Unused parameter*/
							channum,              /*Channel number*/
							hCore);

			BTI1553_ErrorCtrl(ERRCTRL1553_ONCE | ERRCTRL1553_TAGMSG,channum,hCore);
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
