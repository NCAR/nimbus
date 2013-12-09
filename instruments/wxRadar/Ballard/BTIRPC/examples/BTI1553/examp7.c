
/**
*
*  BTI1553 DRIVER EXAMPLE 7  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP7.C -- Errors in BC Transmissions - Scheduled Messages
*
**/

/**
*
*  This example requires a Ballard 1553 C version Device or higher.
*  It configures Card Number 0 as a BC and creates a schedule
*  containing a receive command each for terminal address 1
*  and terminal address 2.  Both receive commands send 2 data
*  words to the remote terminal.  The example then alternates
*  a bit count error between the first data word of both
*  commands.
*
**/

#include "BTICard.h"
#include "BTI1553.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

#define	FRAMETIME	10000			/*Frame time in microseconds*/

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

	MSGADDR BCMsgs[2];
	INT     ErrorCount = 0;
	UINT16  InitialData1;
	UINT16  InitialData2;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP7  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI1553 Example 7                                                 *");
	consoleprintf("\n    *  \"Errors in BC Transmissions - Scheduled Messages\"                 *");
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
		consoleprintf("\nError:  No MIL-STD-1553 version C or D channels present in cardnum %d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing MIL-STD-1553 core #%d",corenum);
	consoleprintf("\nUsing MIL-STD-1553 channel #%d",channum);
	consoleprintf("\n");

/**
*
*  Configure the channel for bus controller mode.
*
**/

	BTICard_CardReset(hCore);                                       /*Reset the core*/

	errval = BTI1553_BCConfig(BCCFG1553_DEFAULT,channum,hCore);     /*Configure the core*/

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring for",errval);
		consoleprintf("\n        bus controller mode on channel #%d on core #%d.",channum,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Create the message records.
*
**/

	BCMsgs[0] = BTI1553_BCCreateMsg(MSGCRT1553_DEFAULT,0x0842,0,&InitialData1,hCore);
	BCMsgs[1] = BTI1553_BCCreateMsg(MSGCRT1553_DEFAULT,0x1042,0,&InitialData2,hCore);

/**
*
*  Schedule the messages to create the desired bus controller
*  schedule.
*
**/

	BTI1553_BCSchedFrame(FRAMETIME,channum,hCore);
	BTI1553_BCSchedMsg(BCMsgs[0],channum,hCore);
	BTI1553_BCSchedMsg(BCMsgs[1],channum,hCore);

/**
*
*  Define the type of error generation.  One of the bus
*  controller messages is tagged to generate errors once.
*
**/

	BTI1553_ErrorDefine(ERRDEF1553_CNTBIT,3,2,0,0,channum,hCore);
	BTI1553_ErrorTagBC(TRUE,BCMsgs[0],channum,hCore);
	BTI1553_ErrorCtrl(ERRCTRL1553_ONCE | ERRCTRL1553_TAGMSG,channum,hCore);

/**
*
*  Start the card to begin transmissions.
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
/**
*
*  When the error is generated once, tag another message for
*  error generation and enable another single error.
*
**/

		if (BTI1553_ErrorSent(channum,hCore) /*AND ANOTHER ERROR DESIRED*/)
		{
			if ((++ErrorCount) & 1)
			{
				BTI1553_ErrorTagBC(FALSE,BCMsgs[0],channum,hCore);
				BTI1553_ErrorTagBC(TRUE, BCMsgs[1],channum,hCore);
			}
			else
			{
				BTI1553_ErrorTagBC(TRUE, BCMsgs[0],channum,hCore);
				BTI1553_ErrorTagBC(FALSE,BCMsgs[1],channum,hCore);
			}

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
