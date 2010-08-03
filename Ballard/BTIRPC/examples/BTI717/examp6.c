
/**
*
*  BTI717 DRIVER EXAMPLE 6  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP6.C -- Multiple Core Example
*
**/

/**
*
*  This program opens Card Number 0 and configures all
*  ARINC 717 cores to receive on all BiPhase channels.
*  Program reports when each BiPhase channel is in sync.
*
**/

#include "BTICard.h"
#include "BTI717.h"
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
	HCARD	hCard;                              /*Handle to card*/
	HCORE	hCores[MAX_CORES] = {0};            /*Handle to cores*/
	BOOL	b717Core[MAX_CORES] = {FALSE};      /*Flag indicating 717 core*/
	INT     corecount, corenum, chan;
	ERRVAL  errval;
	INT     ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP6  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI717 Example 6                                                  *");
	consoleprintf("\n    *  \"Program for multiple core example.\"                              *");
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
*  Find all ARINC 717 cores on the board with a BiPhase receive channel.
*
**/

	corecount = 0;

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		b717Core[corenum] = FALSE;
		errval = BTICard_CoreOpen(&hCores[corenum],corenum,hCard);
		if (errval) break;

		for (chan=0;chan<MAX_CHANNELS;chan++)
		{
			if (BTI717_ChIsType(CHCFG717_BIPHASERCV, chan, hCores[corenum]))
			{
				b717Core[corenum] = TRUE;
				corecount++;
				break;
			}
		}
	}

	if (!corecount)
	{
		consoleprintf("\nError:  Unable to find any ARINC 717 cores on card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing %d ARINC 717 core(s) on card #%d",corecount,CARDNUM);
	consoleprintf("\n");

/**
*
*  Configure all receive channels  for auto speed detection.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		if (b717Core[corenum])
		{
			BTICard_CardReset(hCores[corenum]);			/* Reset the core. */

			for (chan=0;chan<MAX_CHANNELS;chan++)
			{
				if (BTI717_ChIsType(CHCFG717_BIPHASERCV, chan, hCores[corenum]))
				{
					errval = BTI717_ChConfig(CHCFG717_BIPHASERCV | CHCFG717_AUTOSPEED,chan,hCores[corenum]);
					if (errval < 0)
					{
						consoleprintf("\nError:  An error was encountered (%i) while configuring channel %d.",errval,chan);
						consoleprintf("\n        on core %d (%s)",corenum,BTICard_ErrDescStr(errval,hCard));
						consoleprintf("\n");
						BTICard_CardClose(hCard);
						consoleexit(1);
					}
				}
			}
		}
	}

/**
*
*   Start each 717 core to begin
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		if (b717Core[corenum])	BTICard_CardStart(hCores[corenum]);
	}

/**
*
*  Display data on the screen.
*
**/

	consoleprintf("\nPress any key to exit....\n\n");

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		if (b717Core[corenum])
		{
			for (chan=0;chan<MAX_CHANNELS;chan++)
			{
				if (BTI717_ChIsType(CHCFG717_BIPHASERCV, chan, hCores[corenum]))
				{
					consoleprintf("\nSynchronizing channel %d on core %d to bit stream....",chan,corenum);

					while (!BTI717_SyncTest(chan,hCores[corenum]))
					{
						ch = getch();
						if (ch != ERR) break;
					}

					if (BTI717_SyncTest(chan,hCores[corenum]))	consoleprintf("In Sync!\n");
				}
			}
		}
	}

/**
*
*  Stop the card.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		BTICard_CardStop(hCores[corenum]);
	}

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
