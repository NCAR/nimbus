
/**
*
*  BTI708 DRIVER EXAMPLE 1  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP1.C -- Transmitter example
*
**/

/**
*
*  This program opens Card Number 0, and configures the
*  first compatible core to configure the first transmit
*  channel to transmit a sequence of 32 messages at a
*  rate of 200Hz.  The messages are automatically
*  assigned the numbers 0 through 31.
*
*  An Event Log frequency of 16 is specifed, which will
*  cause the channel to issue an Event Log List entry
*  every 16th message (message numbers 15, and 31).
*  The program uses these events to update he data for
*  the half of messages that were just transmitted.
*
*  The program continues in this manner until a key is
*  pressed.
*
**/

#include "BTICard.h"
#include "BTI708.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

#define	FRAMEFREQ	  200          /*Frame frequency of 200Hz*/
#define LOGFREQ		  16           /*Generate a Log Event after every 16th message*/
#define	FRAMECOUNT	  32           /*Transmit a total of 32 messages*/

void consoleprintf(const char *, ...);
//void consoleentry(void);
void consoleexit(INT);

#define consoleprintf	printf

int main(void)
{
	HCARD  hCard;
	HCORE  hCore;
	ERRVAL errval;
	INT    corenum;
	INT    xmtchan=0;
	INT    j,jj;
	UINT16 Data[100];
	UINT16 Type;
	UINT32  Info;
	UINT16 framenum;
	INT    count = 0;
	INT    ch;

	//consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP1  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI708 Example 1                                                  *");
	consoleprintf("\n    *  \"Program for transmitter example.\"                                *");
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

/**
*
*  Find the first ARINC 708 core with a transmit channel.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (xmtchan=0;xmtchan<MAX_CHANNELS;xmtchan++)	if (BTI708_ChIsXmt(xmtchan,hCore)) break;

		if (xmtchan != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || xmtchan == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No ARINC 708 transmit channels present in cardnum %d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing ARINC 708 core #%d",corenum);
	consoleprintf("\nUsing transmit channel #%d",xmtchan);
	consoleprintf("\n");

/**
*
*  Configure transmit channel.
*
**/

	BTICard_CardReset(hCore);

	errval = BTI708_ChConfig(CHCFG708_DEFAULT,xmtchan,hCore);

	if (errval)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        channel #%d on core #%d.",xmtchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Build the transmit channel messages.
*
**/

	errval = BTI708_XmtBuild(MSGCRT708_DEFAULT,FRAMEFREQ,LOGFREQ,FRAMECOUNT,xmtchan,hCore);

	if (errval)
	{
		consoleprintf("\nError:  An error was encountered (%i) while building",errval);
		consoleprintf("\n        channel #%d on core #%d.",xmtchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Create and write the data for the messages.
*
**/

	for (j=0;j<FRAMECOUNT;j++)					/*Loop through all the messages*/
	{
		for (jj=0;jj<100;jj++) Data[jj] = j + (count * FRAMECOUNT);
		BTI708_MsgDataWr(Data,j,xmtchan,hCore);	/*Write data to the Device*/
	}
	count++;

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
*  Loop until a key is hit.
*
**/

	consoleprintf("\nPress any key to exit...");
	consoleprintf("\n\n");

	while (1)
	{
		ch = getch();
		if (ch != ERR) break;

		if (BTICard_EventLogRd(&Type,&Info,NULL,hCore))	//Read entry from Event Log list
		{
			framenum = (UINT16)Info;

			if (framenum == ((FRAMECOUNT/2)-1))			//First half transmitted
			{
				consoleprintf("\rUpdating frames 0 - %d",framenum);
				for (j=0;j<=(FRAMECOUNT/2)-1;j++)
				{
					for (jj=0;jj<100;jj++) Data[jj] = j + (count * FRAMECOUNT);
					BTI708_MsgDataWr(Data,j,xmtchan,hCore);
				}
			}
			else if (framenum == (FRAMECOUNT-1))		//Second half transmitted
			{
				consoleprintf("\rUpdating frames %d - %d",(FRAMECOUNT/2),framenum);
				for (j=(FRAMECOUNT/2);j<=framenum;j++)
				{
					for (jj=0;jj<100;jj++) Data[jj] = j + (count * FRAMECOUNT);
					BTI708_MsgDataWr(Data,j,xmtchan,hCore);
				}
				count++;
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

/*
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
*/
