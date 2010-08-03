
/**
*
*  BTI708 DRIVER EXAMPLE 6  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP6.C -- Header extraction example
*
**/

/**
*
*  This program opens Card Number 0, and configures the
*  first compatible core to configure the first receive
*  channel to receive messages in a single message record.
*  This message is automatically assigned to message number
*  0.
*
*  An Event Log frequency of 0 is specifed, which will
*  not generate any Event Log List entries for this channel.
*
*  The program will display the ARINC 708 header information
*  from the message record, until a key is pressed.
*
**/

#include "BTICard.h"
#include "BTI708.h"
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
	HCARD  hCard;
	HCORE  hCore;
	ERRVAL errval;
	INT    corenum;
	INT    rcvchan=0;
	UINT16 Data[100];
	INT    ch;

/**
*
*  Create a display window.
*
**/

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP6  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI708 Example 6                                                  *");
	consoleprintf("\n    *  \"Header extraction example.\"                                      *");
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
*  Find the first ARINC 708 core with a receive channel.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (rcvchan=0;rcvchan<MAX_CHANNELS;rcvchan++)	if (BTI708_ChIsRcv(rcvchan,hCore)) break;

		if (rcvchan != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || rcvchan == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No ARINC 708 receive channels present in cardnum %d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing ARINC 708 core #%d",corenum);
	consoleprintf("\nUsing receive channel #%d",rcvchan);
	consoleprintf("\n");

/**
*
*  Configure receive channel.
*
**/

	BTICard_CardReset(hCore);

	errval = BTI708_ChConfig(CHCFG708_DEFAULT,rcvchan,hCore);

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
*  Build the receive channel messages.
*
**/

	errval = BTI708_RcvBuild(MSGCRT708_DEFAULT,0,1,rcvchan,hCore);

	if (errval)
	{
		consoleprintf("\nError:  An error was encountered (%i) while building",errval);
		consoleprintf("\n        channel #%d on core #.",rcvchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
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
*  Move to display window.
*
**/

	move(15,0);

/**
*
*  Loop until a key is hit.
*
**/

	consoleprintf("\nPress any key to exit...");
	consoleprintf("\n");

	while (1)
	{
		ch = getch();
		if (ch != ERR) break;

		BTI708_MsgDataRd(Data,0,rcvchan,hCore);

		consoleprintf("\nLabel:          %02X (Hex)  %o (Oct)",BTI708_FieldGetLabel(Data),BTI708_FieldGetLabel(Data));
		consoleprintf("\nControl Accept: %04X (Hex)",BTI708_FieldGetControlAccept(Data));
		consoleprintf("\nBits 16-11:     %02X (Hex)",(Data[0] >> 10) & 0x003F);
		consoleprintf("\nBits 26-17:     %03X (Hex)",Data[1] & 0x03FF);
		consoleprintf("\nMode:           %04X (Hex)",BTI708_FieldGetMode(Data));
		consoleprintf("\nTilt:           %04X (Hex)",BTI708_FieldGetTilt(Data));
		consoleprintf("\nGain:           %d (Dec)",BTI708_FieldGetGain(Data));
		consoleprintf("\nRange:          %d (Dec)",BTI708_FieldGetRange(Data));
		consoleprintf("\nBit 49:         %01X (Hex)",Data[4] & 0x0001);
		consoleprintf("\nData Accept:    %01X (Hex)",BTI708_FieldGetDataAccept(Data));
		consoleprintf("\nScan Angle:     %03X (Hex)",BTI708_FieldGetAngle(Data));
		consoleprintf("\nBit 64:         %01X (Hex)",(Data[4] >> 15) & 0x0001);

		move(18,0);
	}

/**
*
*  Move to bottom of screen.
*
**/

	move(31,0);

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
