
/**
*
*  BTI422 DRIVER EXAMPLE 2  (10/23/2009)
*  Copyright (c) 2004-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP2.C -- Polling the Event Log List
*
**/

/**
*
*  This program opens Card Number 0, and configures the
*  first serial port at 115,200 bps with 8 bit data,
*  no parity, 1 stop bit.  The port is loaded with
*  initial data.  An Event Log List is configured and
*  the transmit FIFO is enabled to generate an Event
*  Log List entry when there are 8 bytes left.  The
*  program polls for these events and writes more data
*  to the port that caused the event.
*
**/

#include "BTICard.h"
#include "BTI422.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_PORTS     16
#define	BUFSIZE       1024

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(int argc, char* argv[])
{
	HCARD hCard;
	HCORE hCore;
	ERRVAL errval;
	INT corenum;
	INT port=0;
	INT j;
	UINT16 type;
	INT count = 0;
	BYTE databuf[BUFSIZE];
	INT ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP2  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2004-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI422 Example 2                                                  *");
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

	consoleprintf("\nUsing ARINC 422 card #%d",CARDNUM);

/**
*
*  Find the first ARINC 422 core.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (port=0;port<MAX_PORTS;port++)
		{
			if (BTI422_PortIs422(port, hCore))	break;
		}

		if (port != MAX_PORTS) break;
	}

	if (errval || corenum == MAX_CORES || port == MAX_PORTS)
	{
		consoleprintf("\nError:  No ARINC 422 ports present in cardnum %d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing ARINC 422 core #%d",corenum);
	consoleprintf("\nUsing port #%d",port);
	consoleprintf("\n");

/**
*
*  Configure port.
*
**/

	BTICard_CardReset(hCore);

	errval = BTI422_PortConfig(PORTCFG422_PARITYNONE,BITRATE422_115200BPS,port,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring port %d.",errval,port);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Init data.
*
**/

	for (j=0;j<BUFSIZE;j++) databuf[j] = ((count)+j);
	BTI422_FIFODataWr(databuf,BUFSIZE,0,port,hCore);
	count++;

/**
*
*  Configure the Event Log list.
*
**/

	errval = BTICard_EventLogConfig(LOGCFG_ENABLE,256,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring the event log list.",errval);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Configure FIFO Threshold.
*
**/

	//Enable Threshold Event Logging
	BTI422_FIFOConfigTx(FIFOCFG422_TXTHRESHOLD,256,port,hCore);

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

		if (BTICard_EventLogRd(&type,NULL,&port,hCore))	//Read entry from Event Log list
		{
			if (type == EVENTTYPE_422TXTHRESHOLD)
			{
				for (j=0;j<=BUFSIZE;j++) databuf[j] = ((count)+j);
				BTI422_FIFODataWr(databuf,BUFSIZE,0,port,hCore);

				consoleprintf("\nUpdated data for port%1d. (count = %d)",port,count);

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
