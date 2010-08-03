
/**
*
*  BTICSDB DRIVER EXAMPLE 5  (10/23/2009)
*  Copyright (c) 2005-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP5.C -- Transmit and Receive List Buffer example
*
**/

/**
*
*  This program opens the Card Number 0, finds the first
*  compatible core, and configures transmit and receive
*  list buffers.  The program transmits one message filled
*  with ramping data in the associated list buffer. The
*  receive list buffer is read and the data is displayed.
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
#define LISTSIZE        32

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(int argc, char* argv[])
{
	HCARD hCard;
	HCORE hCore;
	ERRVAL errval;
	INT count;
	INT corenum;
	INT rcvchan = 0;
	INT xmtchan = 0;
	BOOL rcvfound, xmtfound;
	INT i,j,ch;
	MSGADDR xmtmsg, rcvmsg, syncmsg;
	LISTADDR xmtlistaddr, rcvlistaddr;
	BYTE xmtbuf[BYTESPERBLOCK];
	BYTE rcvbuf[BYTESPERBLOCK];

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP5  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2005-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTICSDB Example 5                                                 *");
	consoleprintf("\n    *  \"Program for transmit and receive list buffers.\"                  *");
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
*  Configure rcvchan with the self-test wraparound enabled.
*  Configure xmtchan to transmit at low speed with the
*  self-test wraparound enabled.
*
**/

	BTICard_CardReset(hCore);            /*Reset the card*/

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
*  for the receive channel. Filter for Address of 0x01 and
*  every combination of Source Identifier bits.
*
**/

	rcvmsg	= BTICSDB_FilterSet(MSGCRTCSDB_DEFAULT,0x01,SIALL,rcvchan,hCore);

/**
*
*  Create a message for each of the labels we are transmitting.
*
**/

	syncmsg	= BTICSDB_MsgCreate(MSGCRTCSDB_WIPESYNC,hCore);	/* Message is initialized to be a sync message */
	xmtmsg	= BTICSDB_MsgCreate(MSGCRTCSDB_DEFAULT,hCore);

/**
*
*  Next, a schedule is built by creating explicit message timing.
*  12.5kbps / 10 frames/second = 1250 bits/frame
*  1250 bits/frame - (2 messages * 6 bytes/block * 11 bits/byte) = 1118 bit-times
*
**/

	BTICSDB_SchedMsg(syncmsg,xmtchan,hCore);
	BTICSDB_SchedMsg(xmtmsg,xmtchan,hCore);
	BTICSDB_SchedGap(1118,xmtchan,hCore);

/**
*
*  Create the transmit and receive list buffers.
*
*  The transmit list buffer will be a circular buffer,
*  which means it should be preloaded with data, and
*  it will continuously transmit that data.
*
*  The receive list buffer will be a FIFO, which we will
*  read out of and display to the screen.
*
**/

	xmtlistaddr = BTICSDB_ListXmtCreate(LISTCRTCSDB_CIRCULAR,LISTSIZE,xmtmsg,hCore);
	rcvlistaddr = BTICSDB_ListRcvCreate(LISTCRTCSDB_FIFO,LISTSIZE,rcvmsg,hCore);

/**
*
*  Initialize the transmit list buffer.
*
**/

	for (i=0;i<LISTSIZE;i++)                //for each entry in the list
	{
		xmtbuf[0] = 0x01;               //set address byte
		for (j=1;j<BYTESPERBLOCK;j++)   //for each data byte in the entry
		{
			xmtbuf[j] = i;

		}

		BTICSDB_ListDataWr(xmtbuf,BYTESPERBLOCK,xmtlistaddr,hCore);
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

		if(BTICSDB_ListStatus(rcvlistaddr,hCore) != STAT_EMPTY)
		{
			count = BTICSDB_ListDataRd(rcvbuf,BYTESPERBLOCK,rcvlistaddr,hCore);
			consoleprintf("\nAddress=%02X  Circular Data=%02X  Bytes=%d",rcvbuf[0],rcvbuf[1],count);
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
