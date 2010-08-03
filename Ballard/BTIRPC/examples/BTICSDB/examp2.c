
/**
*
*  BTICSDB DRIVER EXAMPLE 2  (10/23/2009)
*  Copyright (c) 2005-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP2.C -- Receiver example
*
**/

/**
*
*  This program opens Card Number 0, and configures the
*  first compatible core to receive continuous message
*  and a burst message into unique Message Records. All
*  other messages are received into a default Message Record.
*
**/

#include "BTICard.h"
#include "BTICSDB.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM          0
#define	MAX_CORES        4
#define	MAX_CHANNELS    32

#define BLOCKSPERFRAME  18
#define BYTESPERBLOCK    6
#define FRAMESPERSECOND 10

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(int argc, char* argv[])
{
	HCARD hCard;
	HCORE hCore;
	ERRVAL errval;
	INT corenum;
	INT rcvchan = 0;
	MSGADDR msgdefault;
	MSGADDR msgcont;
	MSGADDR msgburst;
	BYTE datadefault[BYTESPERBLOCK];
	BYTE datacont[BYTESPERBLOCK];
	BYTE databurst[BYTESPERBLOCK];
	INT ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP2  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2005-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTICSDB Example 2                                                 *");
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

	consoleprintf("\nUsing CSDB card #%d",CARDNUM);

/**
*
*  Find the first CSDB core with a receive channel.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (rcvchan=0;rcvchan<MAX_CHANNELS;rcvchan++)	if (BTICSDB_ChIsRcv(rcvchan,hCore)) break;

		if (rcvchan != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || rcvchan == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No CSDB receive channels present in cardnum %d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing CSDB core #%d",corenum);
	consoleprintf("\nUsing receive channel #%d",rcvchan);
	consoleprintf("\n");

/**
*
*  Configure receiver channel for default operation.
*
**/

	BTICard_CardReset(hCore);

	errval = BTICSDB_ChConfig(CHCFGCSDB_DEFAULT,BITRATECSDB_LOWSPEED,BYTESPERBLOCK,FRAMESPERSECOND,BLOCKSPERFRAME,rcvchan,hCore);

	if (errval)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        channel #%d on core #%d.",rcvchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Before any messages can be received, filters must be set
*  for the receive channel.  First, the default filter is
*  created.  Then, two filters are set for the two messages
*  of interest.
*
**/

	msgdefault	= BTICSDB_FilterDefault(MSGCRTCSDB_DEFAULT,rcvchan,hCore);
	msgcont		= BTICSDB_FilterSet(MSGCRTCSDB_DEFAULT,0x12,SIALL,rcvchan,hCore);
	msgburst	= BTICSDB_FilterSet(MSGCRTCSDB_DEFAULT,0x31,SIALL,rcvchan,hCore);

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

	while(1)
	{
		ch = getch();
		if (ch != ERR) break;

		/*Read message records*/
		BTICSDB_MsgDataRd(datadefault,BYTESPERBLOCK,msgdefault,hCore);
		BTICSDB_MsgDataRd(datacont,BYTESPERBLOCK,msgcont,hCore);
		BTICSDB_MsgDataRd(databurst,BYTESPERBLOCK,msgburst,hCore);

		/*Display values*/
		consoleprintf("\r");
		consoleprintf("Cont: Addr=%08lX, Status=%08lX   ",datacont[0],datacont[1]);
		consoleprintf("Burst: Addr=%08lX, Status=%08lX",databurst[0],databurst[1]);
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
