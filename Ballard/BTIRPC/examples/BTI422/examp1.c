
/**
*
*  BTI422 DRIVER EXAMPLE 1  (10/23/2009)
*  Copyright (c) 2004-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP1.C -- Transmit and receive example
*
**/

/**
*
*  This program opens Card Number 0, and configures the
*  first serial port at 115,200 bps with 8 bit data,
*  no parity, 1 stop bit. Each key pressed is transmitted
*  and each character received is displayed.
*
**/

#include "BTICard.h"
#include "BTI422.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_PORTS     16

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(int argc, char* argv[])
{
	HCARD hCard;
	HCORE hCore;
	ERRVAL errval;
	INT corenum;
	INT port=0;
	BOOL done;
	BYTE ch;
	INT keyhit;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP1  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2004-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI422 Example 1                                                  *");
	consoleprintf("\n    *  \"Program for transmit and receive example.\"                       *");
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

	consoleprintf("\nUsing ARINC 422 card #%d",CARDNUM);

/**
*
*  Find the first core with a serial port.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (port=0;port<MAX_PORTS;port++)
		{
			if (BTI422_PortIs422(port, hCore))	break;
		}

		if (port != MAX_PORTS) break;
	}

	if (errval || corenum == MAX_CORES || port == MAX_PORTS)
	{
		consoleprintf("\nError:  No serial ports present in cardnum %d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing serial core #%d",corenum);
	consoleprintf("\nUsing port #%d",port);
	consoleprintf("\n");

/**
*
*  Configure port.
*
**/

	BTICard_CardReset(hCore);

	errval = BTI422_PortConfig(PORTCFG422_PARITYNONE,BITRATE422_115200BPS,port,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring port %d.",errval,port);
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

	consoleprintf("\nPress ESC key to exit....\n\n");
	done = false;

	while (!done)
	{
		if (BTI422_FIFODataRd(&ch,1,NULL,port,hCore))
		{
			if(ch == 0x0d)         // Carriage return
			{
				consoleprintf("\n");
			}
			else
			{
				consoleprintf("%c",ch);
			}
		}

		keyhit = getch();
		if (keyhit != ERR)
		{
			ch = (BYTE)keyhit;

			if (ch == 0x1B)        // ESC key
			{
				done = true;
			}
			else if(ch == 0xE0)    // Function key
			{
				keyhit = getch();     // Function keys have extra keypress
			}
			else
			{
				BTI422_FIFODataWr(&ch,1,0,port,hCore);
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
