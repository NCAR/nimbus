
/**
*
*  BTI422 DRIVER EXAMPLE 3  (10/23/2009)
*  Copyright (c) 2004-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP3.C -- Event Log using Interrupts
*
**/

/**
*
*  This program opens Card Number 0, and configures the
*  first serial port at 115,200 bps with 8 bit data,
*  no parity, 1 stop bit.
*
*  An Event Log List is configured and the receive FIFO
*  is enabled to generate an Event Log List entry when
*  there are greater than 64 bytes in the FIFO.  An
*  interrupt handler is installed which enables the Event
*  Log List to generate an interrupt when an entry is
*  made to the list.  When an entry is read from the
*  list, the 64 bytes of data are read.
*
**/

#include "BTICard.h"
#include "BTI422.h"
#include <stdlib.h>
#include <ncurses.h>
#include <pthread.h>
#include <sys/poll.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_PORTS     16
#define	BUFSIZE       64

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

typedef struct                                          /*Variables used by the thread*/
{
	volatile BOOL fStayAlive;                       /*Flag indicating the thread should stay alive*/
	volatile BOOL fThreadHasQuit;                   /*Flag indicating the thread has quit*/
	HCORE   hCore;                                  /*Handle to core*/
	UINT32   Count;                                  /*Counting variable*/
} ThreadMailbox;

/**
*
*  The following thread is spun off by the main application.
*  It waits for the specified thread to be released when the
*  card generates an interrupt.  It then resets the event to
*  allow us to receive more events, reads an entry from the
*  Event Log list, and waits for the next event.
*
**/

LPVOID EventLogInterruptThread(LPVOID lpParam)
{
	UINT16  Type;
	INT     Port;
	BYTE    databuf[BUFSIZE];
	struct pollfd eventlog_poll;

	ThreadMailbox *pMailbox = (ThreadMailbox*)lpParam;

	pMailbox->fStayAlive     = TRUE;
	pMailbox->fThreadHasQuit = FALSE;
	pMailbox->Count          = 0;

	eventlog_poll.fd         = 0;
	eventlog_poll.events     = POLLIN;
	eventlog_poll.revents    = 0;

	while(pMailbox->fStayAlive)
	{
		if (!eventlog_poll.fd)	//Wait for valid fd (after int install)
		{
			eventlog_poll.fd = (INT)BTICard_IntGet(pMailbox->hCore);
			continue;
		}

		if (poll(&eventlog_poll,1,200) > 0)
		{

			pMailbox->Count++;							//Increment counter

			//Read entry from Event Log list
			BTICard_EventLogRd(&Type,NULL,&Port,pMailbox->hCore);

			if (Type == EVENTTYPE_422RXTHRESHOLD)
			{
				BTI422_FIFODataRd(databuf,BUFSIZE,NULL,Port,pMailbox->hCore);

				// Process received data
			}

			BTICard_IntClear(pMailbox->hCore);			//Clear interrupt from card
		}
	}

	pMailbox->fThreadHasQuit = TRUE;

	return(0);
}

int main(int argc, char* argv[])
{
	HCARD hCard;
	HCORE hCore;
	ERRVAL errval;
	INT corenum;
	INT port=0;
	INT ch;

	pthread_t ThreadId;
	ThreadMailbox Mailbox;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP3  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2004-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI422 Example 3                                                  *");
	consoleprintf("\n    *  \"Event Log using Interrupts.\"                                     *");
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
*  Create a thread.
*
**/

	Mailbox.hCore    = hCore;

	errval = pthread_create(
		&ThreadId,                      //ID of Thread being created
		NULL,                           //Default attributes
		EventLogInterruptThread,        //Pointer to thread function
		(LPVOID)&Mailbox);              //Argument for new thread

	if (errval)
	{
		consoleprintf("\nError:  Thread creation failed (error = %d)",errval);
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Install the interrupt handler.
*
**/

	errval = BTICard_IntInstall(NULL,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while installing",errval);
		consoleprintf("\n        interrupt handler on core #%d.",corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		pthread_cancel(ThreadId);
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

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
		BTICard_IntUninstall(hCard);
		pthread_cancel(ThreadId);
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Configure the Event Log list.
*
**/

	errval = BTICard_EventLogConfig(LOGCFG_ENABLE,1024,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        the Event Log list on core #%d.",corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		BTICard_IntUninstall(hCard);
		pthread_cancel(ThreadId);
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Configure FIFO Threshold.
*
**/

	//Enable Threshold Event Logging
	BTI422_FIFOConfigRx(FIFOCFG422_RXTHRESHOLD,BUFSIZE,port,hCore);

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

		consoleprintf("\rInterrupt count = %d",Mailbox.Count);
	}

/**
*
*  Clear the flag keeping the thread alive and wait for the
*  thread to quit.
*
**/

	consoleprintf("\nWaiting for the thread to quit.");

	Mailbox.fStayAlive = FALSE;	//Clear the fStayAlive flag so the thread commits suicide after the next interrupt

	pthread_join(ThreadId,NULL);

/**
*
*  Uninstall the interrupt handler.
*
**/

	errval = BTICard_IntUninstall(hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while uninstalling",errval);
		consoleprintf("\n        interrupt handler on core #%d.",corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		BTICard_CardClose(hCard);
		consoleexit(1);
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
