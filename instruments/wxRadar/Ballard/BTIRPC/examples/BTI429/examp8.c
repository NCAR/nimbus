
/**
*
*  BTI429 DRIVER EXAMPLE 8  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP8.C -- Polling the Event Log List
*
**/

/**
*
*  This program opens Card Number 0, finds the first
*  compatible core, and configures the first receive
*  channel to receive 2 GPS messages into separate
*  Message Records; and receive all other messages
*  into a default Message Record.
*
*  Each of the messages is also configured to generate an
*  Event Log List entry.  When an entry is read from the list,
*  the associated data is read.
*
**/

#include "BTICard.h"
#include "BTI429.h"
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
	ERRVAL  errval;
	INT     corenum;
	INT     rcvchan=0;
	UINT16  Type;
	UINT32   Info;
	MSGADDR msgaddr;
	MSGSTRUCT429 msgdefault;
	MSGSTRUCT429 latitude;
	MSGSTRUCT429 longitude;
	INT ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP8  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI429 Example 8                                                  *");
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

	consoleprintf("\nUsing ARINC 429 card #%d",CARDNUM);

/**
*
*  Find the first ARINC 429 core with a receive channel.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (rcvchan=0;rcvchan<MAX_CHANNELS;rcvchan++)	if (BTI429_ChIsRcv(rcvchan,hCore)) break;

		if (rcvchan != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || rcvchan == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No ARINC 429 receive channels present in cardnum %d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing ARINC 429 core #%d",corenum);
	consoleprintf("\nUsing receive channel #%d",rcvchan);
	consoleprintf("\n");

/**
*
*  Configure rcvchan for automatic speed detection.
*
**/

	BTICard_CardReset(hCore);

	errval = BTI429_ChConfig(CHCFG429_AUTOSPEED,rcvchan,hCore);   /*Configure channel*/

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
*  Each message is configured to create Event Log list entries.
*
**/

	msgdefault.addr = BTI429_FilterDefault(MSGCRT429_LOG,rcvchan,hCore);
	latitude.addr   = BTI429_FilterSet(MSGCRT429_LOG,0310,SDIALL,rcvchan,hCore);
	longitude.addr  = BTI429_FilterSet(MSGCRT429_LOG,0311,SDIALL,rcvchan,hCore);

/**
*
*  Initialize data values to zero.
*
**/

	msgdefault.data = 0;
	latitude.data   = 0;
	longitude.data  = 0;

/**
*
*  Configure the Event Log list.
*
**/

	errval = BTICard_EventLogConfig(LOGCFG_ENABLE,1024,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        the event log list on core #%d.",corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
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

	while(1)
	{
		ch = getch();
		if (ch != ERR) break;

		if (BTICard_EventLogRd(&Type,&Info,NULL,hCore))	//Read entry from Event Log list
		{
			msgaddr = Info;								//Info value is message address

			if (msgaddr == msgdefault.addr)
			{
				msgdefault.data = BTI429_MsgDataRd(msgaddr,hCore);
			}
			else if (msgaddr == latitude.addr)
			{
				latitude.data = BTI429_MsgDataRd(msgaddr,hCore);
			}
			else if (msgaddr == longitude.addr)
			{
				longitude.data = BTI429_MsgDataRd(msgaddr,hCore);
			}
		}

		consoleprintf("\r");
		consoleprintf("Default=%08lX " ,msgdefault.data);
		consoleprintf("Latitude=%08lX ",latitude.data);
		consoleprintf("Longitude=%08lX",longitude.data);
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
