
/**
*
*  BTICSDB DRIVER EXAMPLE 4  (10/23/2009)
*  Copyright (c) 2005-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP4.C -- Explicit message timing
*
**/

/**
*
*  This program opens Card Number 0, finds the first
*  compatible core, and builds a transmit schedule
*  using explicit message timing. The self-test
*  wraparound is enabled and filters are set to
*  receive the messages.  The minimum and maximum elapsed
*  time for each message is displayed.
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
#define BLOCKSPERFRAME  10
#define FRAMESPERSECOND 52

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(int argc, char* argv[])
{
	HCARD hCard;
	HCORE hCore;
	ERRVAL errval;
	INT corenum;
	INT ch;
	INT rcvchan = 0;
	INT xmtchan = 0;
	BOOL rcvfound, xmtfound;
	MSGADDR xmtsync, xmtmsg1, xmtmsg2;
	MSGADDR rcvmsg1, rcvmsg2;
	MSGFIELDSCSDB msg1fields, msg2fields;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP4  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2005-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTICSDB Example 4                                                 *");
	consoleprintf("\n    *  \"Program for explicit message timing.\"                            *");
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
*  Configure rcvchan to receive at high speed with the
*  Min/Max Elapse Time option also enabled.
*  Note that the self-test wraparound is also enabled.
*
*  Configure xmtchan to transmit at high speed.
*  Note that the self-test wraparound is also enabled.
*  The external line drivers are disabled, and xmtchan is
*  connected internally to all the receive channels.
*
**/

	BTICard_CardReset(hCore);                   /*Reset the card*/

	errval = BTICSDB_ChConfig(CHCFGCSDB_MAXMIN|CHCFGCSDB_SELFTEST,BITRATECSDB_HIGHSPEED,BYTESPERBLOCK,FRAMESPERSECOND,BLOCKSPERFRAME,rcvchan,hCore); /*Configure rcvchan*/

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        channel #%d on core #%d.",rcvchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	errval = BTICSDB_ChConfig(CHCFGCSDB_SELFTEST,BITRATECSDB_HIGHSPEED,BYTESPERBLOCK,FRAMESPERSECOND,BLOCKSPERFRAME,xmtchan,hCore); /*Configure xmtchan*/

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
*  Create a message for each of the labels we are transmitting.
*
**/

	xmtsync = BTICSDB_MsgCreate(MSGCRTCSDB_DEFAULT | MSGCRTCSDB_WIPESYNC,hCore);
	xmtmsg1 = BTICSDB_MsgCreate(MSGCRTCSDB_DEFAULT,hCore);
	xmtmsg2 = BTICSDB_MsgCreate(MSGCRTCSDB_DEFAULT,hCore);

/**
*
*  Next, a schedule is built using explicit message timing.
*		xmtsync --> every frame
*		xmtmsg1 --> every frame
*		xmtmsg2 --> every other frame
*
*       50kbps / (52 frames/second) ~= 961 bit-times / frame
*		961 bit-times/frame - (2 blocks/frame * 6 bytes/block * 11 bit-times/byte) = 829 bit-times of gap / frame
*		961 bit-times/frame - (3 blocks/frame * 6 bytes/block * 11 bit-times/byte) = 763 bit-times of gap / frame
*
**/

	BTICSDB_SchedMsg(xmtsync,xmtchan,hCore);
	BTICSDB_SchedMsg(xmtmsg1,xmtchan,hCore);
	BTICSDB_SchedGap(829,xmtchan,hCore);

	BTICSDB_SchedMsg(xmtsync,xmtchan,hCore);
	BTICSDB_SchedMsg(xmtmsg1,xmtchan,hCore);
	BTICSDB_SchedMsg(xmtmsg2,xmtchan,hCore);
	BTICSDB_SchedGap(763,xmtchan,hCore);

/**
*
*  Those messages that are to be transmitted are initialized.
*
**/

	BTICSDB_MsgDataByteWr(0x01,0,xmtmsg1,hCore);
	BTICSDB_MsgDataByteWr(0x02,0,xmtmsg2,hCore);

/**
*
*  Before any messages can be received, filters must be set
*  for the receive channel.  Two filters are set for the
*  two messages of interest.
*
**/

	rcvmsg1	= BTICSDB_FilterSet(MSGCRTCSDB_SEQ,0x01,SIALL,rcvchan,hCore);
	rcvmsg2	= BTICSDB_FilterSet(MSGCRTCSDB_SEQ,0x02,SIALL,rcvchan,hCore);

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

		BTICSDB_MsgBlockRd(&msg1fields,rcvmsg1,hCore);
		BTICSDB_MsgBlockRd(&msg2fields,rcvmsg2,hCore);

		consoleprintf("\r");

		consoleprintf("Message 1: min/max=%2ums/%2ums ",
		(msg1fields.mintime / 1000),
		(msg1fields.maxtime / 1000));

		consoleprintf("Message 2: min/max=%2ums/%2ums ",
		(msg2fields.mintime / 1000),
		(msg2fields.maxtime / 1000));

		consoleprintf("        ");
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
