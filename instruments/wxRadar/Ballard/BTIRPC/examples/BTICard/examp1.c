
/**
*
*  BTICard DRIVER EXAMPLE 1  (10/23/2009)
*  Copyright (c) 2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP1.C -- Report BTIDriver devices
*
**/

/**
*
*  This example uses a special feature of the BTICard_CardOpen(...)
*  function to search the computer for registered Ballard BTIDriver
*  devices.  The program loops through the array of discovered
*  devices and outputs some generic card information.
*
**/

#include <BTICard.h>
#include <BTI422.h>
#include <BTI429.h>
#include <BTI708.h>
#include <BTI717.h>
#include <BTI1553.h>
#include <BTICSDB.h>
#include <BTIDIO.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define	CARDNUM      0
#define MAX_CORES    4
#define MAX_CHANNELS 32
#define MAX_PORTS    16
#define	MAX_BANKS    4

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(void)
{
	HCARD  hCard;					/*Handle to card*/
	HCORE  hCore;					/*Handle to core*/
	ERRVAL errval;					/*Error return value*/
	UINT16 j, jj;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP1  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2009  Ballard Technology, Inc.  Everett, WA, USA.       *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTICard Example 1                                                 *");
	consoleprintf("\n    *  \"Report BTIDriver devices\"                                        *");
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

	consoleprintf("\nUsing BTICard card #%d",CARDNUM);

/**
*
*  Loop through each core on the card
*  Report all protocols found
*
**/

	for (j=0;j<MAX_CORES;j++)
	{
		// Open the core
		hCore = -1;

		if (ERR_NONE == BTICard_CoreOpen(&hCore,j,hCard))
		{
			// Report all channels
			for (jj=0; jj<MAX_CHANNELS; jj++)
			{
				if      (BTI429_ChIs429(jj, hCore))		consoleprintf("\nFound 429 channel #%d on core #%d",jj,j);
				else if (BTI708_ChIs708(jj, hCore))		consoleprintf("\nFound 708 channel #%d on core #%d",jj,j);
				else if (BTI717_ChIs717(jj, hCore))		consoleprintf("\nFound 717 channel #%d on core #%d",jj,j);
				else if (BTI1553_ChIs1553(jj, hCore))	consoleprintf("\nFound 1553 channel #%d on core #%d",jj,j);
				else if (BTICSDB_ChIsCSDB(jj, hCore))	consoleprintf("\nFound CSDB channel #%d on core #%d",jj,j);
			}

			// Report all ports
			for (jj=0; jj<MAX_PORTS; jj++)
			{
				if (BTI422_PortIs422(jj, hCore))
					consoleprintf("\nFound 422 port #%d on core #%d",jj,j);
			}

			// Report all dio banks
			for (jj=0; jj<MAX_BANKS; jj++)
			{
				if (BTIDIO_BankIsDIO(jj, hCore))
					consoleprintf("\nFound DIO bank #%d on core #%d",jj,j);
			}
		}
	}

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
