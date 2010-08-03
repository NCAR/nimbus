
/**
*
*  BTI717 DRIVER EXAMPLE 1  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP1.C -- Receiver example
*
**/

/**
*
*  This program opens Card Number 0, and configures the
*  first BiPhase channel as a receiver.  After the
*  channel gets in sync, the program displays the first
*  word of each subframe (sync words).
*
**/

#include "BTICard.h"
#include "BTI717.h"
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
	UINT16 syncword1,syncword2,syncword3,syncword4;
	INT ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP1  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI717 Example 1                                                  *");
	consoleprintf("\n    *  \"Program for receiver example.\"                                   *");
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

	consoleprintf("\nUsing ARINC 717 card #%d",CARDNUM);

/**
*
*  Find the first ARINC 717 core with a BiPhase receive channel.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (rcvchan=0;rcvchan<MAX_CHANNELS;rcvchan++)
		{
			if (BTI717_ChIsType(CHCFG717_BIPHASERCV, rcvchan, hCore))	break;
		}

		if (rcvchan != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || rcvchan == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No ARINC 717 BiPhase receive channels present in cardnum %d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing ARINC 717 core #%d",corenum);
	consoleprintf("\nUsing BiPhase receive channel #%d",rcvchan);
	consoleprintf("\n");

/**
*
*  Configure channel for auto speed detection.
*
**/

	BTICard_CardReset(hCore);

	errval = BTI717_ChConfig(CHCFG717_BIPHASERCV | CHCFG717_AUTOSPEED,rcvchan,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring channel %d.",errval,rcvchan);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
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
*  Display data on the screen.
*
**/

	consoleprintf("\nPress any key to exit....\n\n");
	consoleprintf("\nSynchronizing to bit stream....");

	while (!BTI717_SyncTest(rcvchan,hCore))
	{
		ch = getch();
		if (ch != ERR) break;
	}

	if (BTI717_SyncTest(rcvchan,hCore))
	{
		consoleprintf("In Sync!\n\n");
		while(1)
		{
			ch = getch();
			if (ch != ERR) break;

			BTI717_SubFrmWordRd(&syncword1,1,1,rcvchan,hCore);
			BTI717_SubFrmWordRd(&syncword2,2,1,rcvchan,hCore);
			BTI717_SubFrmWordRd(&syncword3,3,1,rcvchan,hCore);
			BTI717_SubFrmWordRd(&syncword4,4,1,rcvchan,hCore);

			consoleprintf("\r");
			consoleprintf("Sync1=%04o ",syncword1);
			consoleprintf("Sync2=%04o ",syncword2);
			consoleprintf("Sync3=%04o ",syncword3);
			consoleprintf("Sync4=%04o ",syncword4);
			consoleprintf("(Octal)");
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
