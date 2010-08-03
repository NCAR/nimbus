
/**
*
*  BTICSDB DRIVER EXAMPLE 9  (10/23/2009)
*  Copyright (c) 2005-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP9.C -- Asynchronous serial example
*
**/

/**
*
*  This example demonstrates asynchronous serial communication through
*  the first CSDB transmit and receive channels. The receive channel is
*  placed in asynchronous mode so that it receives all data without the
*  need to synchronize to a CSDB bus. The transmit channel is placed in
*  asynchronous mode and a non-continuous message is created. Both transmit
*  and receive channels use a FIFO list buffer.
*
**/

#include "BTICard.h"
#include "BTICSDB.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM           0
#define	MAX_CORES         4
#define	MAX_CHANNELS     32

#define BYTESPERBLOCK     1
#define BLOCKSPERFRAME    1
#define FRAMESPERSECOND 200
#define LISTSIZE        100

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(int argc, char* argv[])
{
	HCARD hCard;
	HCORE hCore;
	ERRVAL errval;
	INT input;
	INT corenum;
	INT rcvchan = 0;
	INT xmtchan = 0;
	BOOL rcvfound, xmtfound;
	MSGADDR msgaddr;
	MSGADDR msgdefault;
	BYTE xmtbuf;
	BYTE rcvbuf;
	BOOL done = 0;
	LISTADDR xmtlistaddr,rcvlistaddr;
	INT i,count;
	LPCSTR initstring = "Ballard Technology, OmniBus Serial Module.";

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP9  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2005-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTICSDB Example 9                                                 *");
	consoleprintf("\n    *  \"Program for Asynchronous serial mode.\"                           *");
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

	consoleprintf("\nUsing CSDB card #%d",CARDNUM);

/**
*
*  Find the first CSDB core with both a receive and a transmit channel.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		rcvfound = 0;
		xmtfound = 0;

		errval = BTICard_CoreOpen(&hCore,corenum,hCard);	/* Open a handle to the core. */
		if (errval) break;

		for (rcvchan=0;rcvchan<MAX_CHANNELS;rcvchan++)		/* Search for receive channel. */
		{
			rcvfound = BTICSDB_ChIsRcv(rcvchan,hCore);
			if (rcvfound) break;
		}

		if (rcvfound)
		{
			for (xmtchan=0;xmtchan<MAX_CHANNELS;xmtchan++)	/* Search for transmit channel. */
			{
				xmtfound = BTICSDB_ChIsXmt(xmtchan,hCore);
				if (xmtfound) break;
			}
		}

		if (rcvfound && xmtfound)	break;
	}

	if (errval || (corenum == MAX_CORES) || !rcvfound || !xmtfound)
	{
		consoleprintf("\nError:  No compatible CSDB core present in card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing CSDB core #%d",corenum);
	consoleprintf("\nUsing receive channel #%d",rcvchan);
	consoleprintf("\nUsing transmit channel #%d",xmtchan);
	consoleprintf("\n");

/**
*
*  Configure rcvchan for high speed.
*  Configure xmtchan to transmit at high speed.
*
**/

	BTICard_CardReset(hCore);

	errval = BTICSDB_ChConfig(CHCFGCSDB_ASYNCMODE,BITRATECSDB_HIGHSPEED,BYTESPERBLOCK,FRAMESPERSECOND,BLOCKSPERFRAME,rcvchan,hCore); /*Configure rcvchan*/

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        channel #%d on core #%d.",rcvchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	errval = BTICSDB_ChConfig(CHCFGCSDB_ASYNCMODE,BITRATECSDB_HIGHSPEED,BYTESPERBLOCK,FRAMESPERSECOND,BLOCKSPERFRAME,xmtchan,hCore); /*Configure xmtchan*/

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        channel #%d on core #%d.",xmtchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*	Create and schedule a non-continuous message.
*
**/

	msgaddr = BTICSDB_MsgCreate(MSGCRTCSDB_NONCONT,hCore);

	errval = BTICSDB_SchedMsg(msgaddr,xmtchan,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while scheduling the message.",errval);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*	Create a default fitler to receive all data.
*
**/

	msgdefault	= BTICSDB_FilterDefault(MSGCRTCSDB_DEFAULT,rcvchan,hCore);

/**
*
*	Create and initialize the transmit list. Create the receive list.
*
**/

	xmtlistaddr = BTICSDB_ListXmtCreate(LISTCRTCSDB_FIFO,LISTSIZE,msgaddr,hCore);

	if (xmtlistaddr == 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring the transmit list.",errval);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	for (i = 0; (initstring[i] != '\0') && (i != LISTSIZE); i++)
	{
		xmtbuf = initstring[i];
		BTICSDB_ListDataWr(&xmtbuf,BYTESPERBLOCK,xmtlistaddr,hCore);
	}

	if(BTICSDB_MsgValidSet(msgaddr,hCore)) consoleprintf("Message was previously set as valid.");

	rcvlistaddr = BTICSDB_ListRcvCreate(LISTCRTCSDB_FIFO,LISTSIZE,msgdefault,hCore);

/**
*
*	Start Device.
*
**/

	BTICard_CardStart(hCore);

	consoleprintf("\n\n");

	while (!done)
	{
		count = BTICSDB_ListDataRd(&rcvbuf,BYTESPERBLOCK,rcvlistaddr,hCore);
		if (count)
		{
			if (rcvbuf == 0x0d)             //Carriage Return
			{
				consoleprintf("\n");
			}
			else
			{
				consoleprintf("%c",rcvbuf);
			}
		}

		input = getch();

		if (input != ERR)
		{
			if (input == 0x1B)              // ESC key
			{
				done = 1;
			}
			else if(input == 0xE0)          // Function key
			{
				input = getch();        // Function keys have extra keypress
			}
			else
			{
				xmtbuf = input;
				if (BTICSDB_ListDataWr(&xmtbuf,BYTESPERBLOCK,xmtlistaddr,hCore) != BYTESPERBLOCK) consoleprintf("\nError writing to list.\n");
				if (BTICSDB_MsgValidSet(msgaddr,hCore)) consoleprintf("\nMessage was previously set as valid.");
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
