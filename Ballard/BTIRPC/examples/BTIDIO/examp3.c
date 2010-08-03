
/**
*
*  BTIDIO DRIVER EXAMPLE 3  (10/23/2009)
*  Copyright (c) 2005-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP3.C -- Polling the Event Log List
*
**/

/**
*
*  This program opens Card Number 0 and configures the
*  first compatible core to log positive edge events
*  of Discrete 0 in Bank A.
*
**/

#include "BTICard.h"
#include "BTIDIO.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM          0
#define	MAX_CORES        4
#define	MAX_BANKS        4

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
	INT ionum = 0;
	INT ch;
	UINT16 type;
	UINT32 info;
	INT event_banknum;
	BOOL done;
	UINT32 disccfg;
	UINT32 bankcfg;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP3  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2005-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTIDIO Example 3                                                  *");
	consoleprintf("\n    *  \"Polling the Event Log List.\"                                     *");
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
	consoleprintf("\nUsing Discrete %d",ionum);
	consoleprintf("\n");

/**
*
*  Configure banknum as a shunt, input bank with inverted input polarity.
*
**/

	BTICard_CardReset(hCore);

	/*
	*  Threshold:   10kOhm switching level
	*  Sample Rate: 1ms clock, no divide value --> 1ms sampling
	*  This example does not enable Event Logging at the Bank Level
	*/
	bankcfg = BANKCFGDIO_SHUNT | BANKCFGDIO_INPUT | BANKCFGDIO_POLIN_INV;
	errval = BTIDIO_BankConfig(bankcfg,THRESHOLDDIO_10KOHM,SAMPLERATEDIO_MS | 0x0001,banknum,hCore);

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
*  Configure ionum as an inout with event logging
*
**/

	disccfg = DISCCFGDIO_INOUT | DISCCFGDIO_POLIN_INV | DISCCFGDIO_LOGPOSEDGE | DISCCFGDIO_LOGFAULT;
	errval = BTIDIO_DiscreteConfig(disccfg,	ionum,banknum,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        discrete #%d on bank %c on core %d.",ionum,banknum+'A',corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Configure the Event Log to handle up to 30 events
*
**/

	errval = BTICard_EventLogConfig(LOGCFG_ENABLE,1024,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring the sequential record.",errval);
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
*  Display data on the screen.
*
**/

	consoleprintf("\nPress spacebar to toggle discrete %d.  Press escape to exit...\n\n",ionum);

	done = 0;
	while (!done)
	{
		ch = getch();

		if (ch != ERR)
		{
			/* if escape is pressed, exit loop */
			if (ch == 0x1B)
			{
				done = 1;
			}

			/* if spacebar is pressed, toggle the discrete */
			else if (ch == ' ')
			{
				BTIDIO_DiscreteWr(!BTIDIO_DiscreteRd(ionum,banknum,hCore),ionum,banknum,hCore);
			}

			/* Clear faults on user command */
			else if (ch == 'c' || ch == 'C')
			{
				BTIDIO_BankFaultClr(0x01,banknum,hCore);
			}
		}

		/* Read the Event Log */
		if (BTICard_EventLogRd(&type,&info,&event_banknum,hCore))
		{
			if (type == EVENTTYPE_DIOEDGE)
			{
				consoleprintf("\nBank %c had an edge (0x%04X)",event_banknum+'A',info);
			}
			else if (type == EVENTTYPE_DIOFAULT)
			{
				consoleprintf("\nBank %c had a fault (0x%04X)",event_banknum+'A',info);
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
