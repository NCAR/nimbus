
/**
*
*  BTI1553 DRIVER EXAMPLE 30  (10/23/2009)
*  Copyright (c) 2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP30.C -- Changing BC or RT data when a message is serviced
*
**/

/**
*
*  This example configures Card Number 0 as an RT to transmit
*  to the BC.  The program watches the hit bit on transmit
*  subaddress 1 and updates the first data word.  The result
*  is an incrementing data word in the transmit subaddrss.
*
**/

#include <BTICard.h>
#include <BTI1553.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define	CARDNUM		  0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(void)
{
	HCARD  hCard;				/*Handle to card*/
	HCORE  hCore;				/*Handle to core*/
	ERRVAL errval;				/*Error return value*/
	INT    corenum;				/*Core number*/
	INT    channum=0;			/*Channel number*/
	INT    ch;

	MSGADDR RTMsg;
	UINT32   MsgErrors;
	UINT16  RTData[3] = {0,0,0};

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP30  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2009  Ballard Technology, Inc.  Everett, WA, USA.       *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI1553 Example 30                                                *");
	consoleprintf("\n    *  \"Changing BC or RT data when a message is serviced\"               *");
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
*  Find the first MIL-STD-1553 core and channel.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (channum=0;channum<MAX_CHANNELS;channum++)
			if (BTI1553_ChIs1553(channum,hCore)) break;

		if (channum != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || channum == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No MIL-STD-1553 channels present in card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing MIL-STD-1553 core #%d",corenum);
	consoleprintf("\nUsing MIL-STD-1553 channel #%d",channum);
	consoleprintf("\n");

/**
*
*  Configure the channel for remote terminal mode as shown in
*  Example 3.
*
**/

	errval = BTI1553_RTConfig(RTCFG1553_DEFAULT,2,channum,hCore);

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
*  Get the message address for a receive subaddress.
*
**/

	RTMsg = BTI1553_RTGetMsg(SUBADDRESS,2,XMT,1,channum,hCore);

	RTData[0] = 0;
	RTData[1] = 1;
	RTData[2] = 2;

	BTI1553_MsgDataWr(RTData,3,RTMsg,hCore);

/**
*
*  Loop until a key is hit.
*
**/

	BTICard_CardStart(hCore);

	consoleprintf("\nPress any key to exit....\n\n");

	consoleprintf("\nWaiting for first transmition");

	while(1)
	{
		ch = getch();
		if (ch != ERR) break;
/**
*
*  Watch the 'hit' bit of the subaddress to see if it has
*  been transmitted.  If it has, increment the first data word.
*
**/

		MsgErrors = BTI1553_MsgFieldRd(FIELD1553_ERROR,RTMsg,hCore);

		if (MsgErrors & MSGERR1553_HIT)
		{
			RTData[0] += 1;											/*Calculate the new data words*/

			BTI1553_MsgDataWr(RTData,1,RTMsg,hCore);				/*Write the new data words*/

			BTI1553_MsgFieldWr(0,FIELD1553_ERROR,RTMsg,hCore);		/*Clear the 'hit' bit*/

			consoleprintf("\rTransmitting data {%04XH, %04XH, %04XH}",RTData[0],RTData[1],RTData[2]);
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
