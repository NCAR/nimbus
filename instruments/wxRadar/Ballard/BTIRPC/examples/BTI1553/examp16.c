
/**
*
*  BTI1553 DRIVER EXAMPLE 16  (10/23/2009)
*  Copyright (c) 2003-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP16.C -- RT Response Time
*
**/

/**
*
*  This example configures Card Number 0 as an RT to receive
*  the BC-RT data transmitted by the bus controller in
*  Example 2. The programmable RT response time is set and then
*  controlled by the up and down arrow keys. The program runs
*  until the ESC key is pressed.
*
**/

#include "BTICard.h"
#include "BTI1553.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

#define MAX_RESPTIME  255
#define MIN_RESPTIME  77

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

	INT     resptime;
	BOOL    done;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP16  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2003-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI1553 Example 16                                                *");
	consoleprintf("\n    *  \"RT Response Time example.\"                                       *");
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
*  Find the first MIL-STD-1553 core and channel that supports
*  programmable RT response time.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (channum=0;channum<MAX_CHANNELS;channum++)
		{
			if ( BTI1553_ChIs1553(channum,hCore) )
			{
				if (BTI1553_ChGetInfo(INFO1553_RTRESPTIME,channum,hCore))	break;
			}
		}

		if (channum != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || channum == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No 1553 channels that support programmable RT");
		consoleprintf("\n        response time present in cardnum %d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing MIL-STD-1553 core #%d",corenum);
	consoleprintf("\nUsing programmable RT response time 1553 channel #%d",channum);
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
*  Set the Programmable RT Response Time
*
**/

	resptime = 80;
	errval = BTI1553_RTResponseTimeSet(resptime,1,channum,hCore);
	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while setting the",errval);
		consoleprintf("\n        RT response time on channel #%d on core #%d.",channum,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Start the card to begin receiving messages.
*
**/

	BTICard_CardStart(hCore);

/**
*
*  Change response time values when arrow keys are pressed. Exit when ESC is pressed.
*
**/

	consoleprintf("\n\tOPTIONS:");
	consoleprintf("\n\tARROW UP    = Add 10 to resptime");
	consoleprintf("\n\tARROW DOWN  = Subtract 10 from resptime");
	consoleprintf("\n\tESC         = exit program");
	consoleprintf("\n\n");

	for (done=0;!done;)
	{
		consoleprintf("\r\tresptime: %03X (Hex)",resptime);
		ch = getch();
		if (ch != ERR)
		{
			switch (ch)
			{
				case 27:	/* ESC */
				{
					done=1;
					break;
				}
				case KEY_UP:	/* UP   */
				case KEY_DOWN:	/* DOWN */
				{
					if (ch==KEY_UP)	resptime = (resptime+10>MAX_RESPTIME) ? MAX_RESPTIME : resptime+10;
					else        resptime = (resptime-10<MIN_RESPTIME) ? MIN_RESPTIME : resptime-10;

					BTI1553_ChStop(channum,hCore);

					errval = BTI1553_RTResponseTimeSet(resptime,1,channum,hCore);
					if (errval < 0)
					{
						consoleprintf("\nError:  An error was encountered (%i) while setting the",errval);
						consoleprintf("\n        RT response time on channel #%d on core #%d.",channum,corenum);
						consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
						consoleprintf("\n");
						done=1;
					}

					BTI1553_ChStart(channum,hCore);

					break;
				}
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
