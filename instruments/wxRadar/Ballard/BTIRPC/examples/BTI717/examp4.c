
/**
*
*  BTI717 DRIVER EXAMPLE 4  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP4.C -- Polling the Event Log List
*
**/

/**
*
*  This program opens Card Number 0, and configures the
*  first BiPhase channel as a transmitter.  The channel
*  is loaded with initial data.  An Event Log List is
*  configured and the last word in each SF is tagged to
*  generate an Event Log List entry.  The program polls
*  for these events and updates the data in the SF that
*  caused the event.
*
**/

#include "BTICard.h"
#include "BTI717.h"
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
	ERRVAL errval;
	INT corenum;
	INT xmtchan=0;
	INT wps = 256;
	UINT16 sfblock[8200];
	INT j,jj, ch;

	UINT16 Type;
	UINT32  Info;
	UINT16 sfnum;
	INT    count = 0;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP4  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI717 Example 4                                                  *");
	consoleprintf("\n    *  \"Polling the Event Log List example.\"                             *");
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

	consoleprintf("\nUsing ARINC 717 card #%d",CARDNUM);

/**
*
*  Find the first ARINC 717 core with a BiPhase receive channel.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (xmtchan=0;xmtchan<MAX_CHANNELS;xmtchan++)
		{
			if (BTI717_ChIsType(CHCFG717_BIPHASEXMT, xmtchan, hCore))	break;
		}

		if (xmtchan != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || xmtchan == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No ARINC 717 BiPhase transmit channels present in cardnum %d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing ARINC 717 core #%d",corenum);
	consoleprintf("\nUsing BiPhase transmit channel #%d",xmtchan);
	consoleprintf("\n");

/**
*
*  Configure transmitter channel at specified words per second.
*  Use the internal wrap around.
*
**/

	BTICard_CardReset(hCore);

	if      (wps == 64)   errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_64WPS,  xmtchan,hCore);
	else if (wps == 128)  errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_128WPS, xmtchan,hCore);
	else if (wps == 256)  errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_256WPS, xmtchan,hCore);
	else if (wps == 512)  errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_512WPS, xmtchan,hCore);
	else if (wps == 1024) errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_1024WPS,xmtchan,hCore);
	else if (wps == 2048) errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_2048WPS,xmtchan,hCore);
	else if (wps == 4096) errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_4096WPS,xmtchan,hCore);
	else if (wps == 8192) errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_8192WPS,xmtchan,hCore);

	if (errval < 0)
	{
		fprintf(stderr,"\nError:  An error was encountered (%i) while configuring channel %d.",errval,xmtchan);
		fprintf(stderr,"\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Init subframe data.
*
**/

	for (j=1;j<=4;j++)
	{
		BTI717_SubFrmBlkRd(sfblock,j,xmtchan,hCore);
		for (jj=2;jj<=wps;jj++) sfblock[jj+7] = ((count * wps)+jj) & 0x0FFF;
		BTI717_SubFrmBlkWr(sfblock,j,xmtchan,hCore);
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
		fprintf(stderr,"\nError:  An error was encountered (%i) while configuring the event log list.",errval);
		fprintf(stderr,"\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Configure word options.
*
**/

	//Mark last word of SFs 1, 2, 3, and 4 for Event Logging

	BTI717_SubFrmWordConfig(WRDCFG717_SFLOG,1,wps,xmtchan,hCore);
	BTI717_SubFrmWordConfig(WRDCFG717_SFLOG,2,wps,xmtchan,hCore);
	BTI717_SubFrmWordConfig(WRDCFG717_SFLOG,3,wps,xmtchan,hCore);
	BTI717_SubFrmWordConfig(WRDCFG717_SFLOG,4,wps,xmtchan,hCore);

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

	consoleprintf("\nPress any key to exit.");
	consoleprintf("\n");

	while (1)
	{
		ch = getch();
		if (ch != ERR) break;

		if (BTICard_EventLogRd(&Type,&Info,NULL,hCore))	//Read entry from Event Log list
		{
			sfnum = (UINT16)Info;						//Info is subframe number

			BTI717_SubFrmBlkRd(sfblock,sfnum,xmtchan,hCore);
			for (jj=2;jj<=wps;jj++)
			{
				sfblock[jj+7] &= 0xF000;				//Keep option bits, clear data
				sfblock[jj+7] |= ((count * wps)+jj) & 0x0FFF;	//Set new data
			}
			BTI717_SubFrmBlkWr(sfblock,sfnum,xmtchan,hCore);

			consoleprintf("\nUpdated data for SF%1d.",sfnum);

			if (sfnum == 4) count++;
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
