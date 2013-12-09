
/**
*
*  BTICSDB DRIVER EXAMPLE 8  (10/23/2009)
*  Copyright (c) 2005-2009
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
*  compatible core, creates filters for receive messages
*  and builds a transmit schedule. It enables the Event
*  Log and polls for received messages.
*
**/

#include "BTICard.h"
#include "BTICSDB.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM          0
#define	MAX_CORES        4
#define	MAX_CHANNELS    32

#define BYTESPERBLOCK    6
#define BLOCKSPERFRAME  18
#define FRAMESPERSECOND 10

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(int argc, char* argv[])
{
	HCARD	hCard;
	HCORE	hCore;
	ERRVAL  errval;
	INT     corenum;
	INT     rcvchan = 0;
	INT	xmtchan = 0;
	INT	ch;
	BOOL	rcvfound,xmtfound;
	UINT16  Type;
	UINT32   Info;
	MSGADDR	xmtsync, xmtmsg1, xmtmsg2;
	MSGADDR msgaddr;
	MSGFIELDSCSDB msgfields;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP8  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2005-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTICSDB Example 8                                                 *");
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

	consoleprintf("\nUsing CSDB card #%d",CARDNUM);

/**
*
*  Find the first CSDB core with a receive and transmit channels.
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
*  Configure rcvchan for low-speed.
*  Configure xmtchan to transmit at low-speed.
*
**/

	BTICard_CardReset(hCore);

	errval = BTICSDB_ChConfig(CHCFGCSDB_SELFTEST,BITRATECSDB_LOWSPEED,BYTESPERBLOCK,FRAMESPERSECOND,BLOCKSPERFRAME,rcvchan,hCore); /*Configure rcvchan*/

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        channel #%d on core #%d.",rcvchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	errval = BTICSDB_ChConfig(CHCFGCSDB_SELFTEST,BITRATECSDB_LOWSPEED,BYTESPERBLOCK,FRAMESPERSECOND,BLOCKSPERFRAME,xmtchan,hCore); /*Configure xmtchan*/

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
*  Before any messages can be received, filters must be set
*  for the receive channel.  First, the default filter is
*  created.  Then, two filters are set for the two messages
*  of interest.
*
*  Each message is configured to create Event Log list entries.
*
**/

	BTICSDB_FilterDefault(MSGCRTCSDB_LOG | MSGCRTCSDB_HIT,rcvchan,hCore);
	BTICSDB_FilterSet(MSGCRTCSDB_LOG | MSGCRTCSDB_HIT,0x01,SIALL,rcvchan,hCore);
	BTICSDB_FilterSet(MSGCRTCSDB_LOG | MSGCRTCSDB_HIT,0x02,SIALL,rcvchan,hCore);

/**
*
*  Create a message for each of the addresses we are transmitting.
*
**/

	xmtsync	= BTICSDB_MsgCreate(MSGCRTCSDB_DEFAULT | MSGCRTCSDB_WIPESYNC,hCore);
	xmtmsg1	= BTICSDB_MsgCreate(MSGCRTCSDB_DEFAULT,hCore);
	xmtmsg2	= BTICSDB_MsgCreate(MSGCRTCSDB_DEFAULT,hCore);

/**
*
*  Next, a schedule is built using explicit message timing.
*
**/

	BTICSDB_SchedMsg(xmtsync,xmtchan,hCore);
	BTICSDB_SchedMsg(xmtmsg1,xmtchan,hCore);
	BTICSDB_SchedMsg(xmtmsg2,xmtchan,hCore);
	BTICSDB_SchedGap(1052,xmtchan,hCore);

/**
*
*  Initialize the Message Records.
*
**/

	BTICSDB_MsgDataByteWr(0x01,0,xmtmsg1,hCore);
	BTICSDB_MsgDataByteWr(0x02,0,xmtmsg2,hCore);

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

		if(BTICard_EventLogStatus(hCore) != STAT_EMPTY)
		{
			BTICard_EventLogRd(&Type,&Info,NULL,hCore);	//Read entry from Event Log list
			msgaddr = Info;

			BTICSDB_MsgBlockRd(&msgfields,msgaddr,hCore);

			consoleprintf("\n");
			consoleprintf("Msg Addr=%02X   " ,msgfields.data[0]);
			consoleprintf("Msg Status=%02X   ",msgfields.data[1]);
			consoleprintf("Hit Count=%d",msgfields.hitcount);
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
