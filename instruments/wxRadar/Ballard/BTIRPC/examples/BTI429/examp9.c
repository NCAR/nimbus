
/**
*
*  BTI429 DRIVER EXAMPLE 9  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP9.C -- Event Log using Interrupts
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
*  Event Log List entry.  An interrupt handler is installed
*  which enables the Event Log List to generate an interrupt
*  when an entry is made to the list.  When an entry is read
*  from the list, the associated data is read.
*
**/

#include "BTICard.h"
#include "BTI429.h"
#include <stdlib.h>
#include <ncurses.h>
#include <pthread.h>
#include <sys/poll.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

typedef struct                                  /*Variables used by the thread*/
{
	volatile BOOL fStayAlive;               /*Flag indicating the thread should stay alive*/
	volatile BOOL fThreadHasQuit;           /*Flag indicating the thread has quit*/
	volatile UINT32 Count;                   /*Counting variable*/
	HCORE	 hCore;                         /*Handle of core*/
	MSGSTRUCT429 msgdefault;                /*Default   Message Record*/
	MSGSTRUCT429 latitude;                  /*Latitude  Message Record*/
	MSGSTRUCT429 longitude;                 /*Longitude Message Record*/
} ThreadMailbox;

/**
*
*  The following thread is spun off by the main application.
*  It waits for the specified thread to be released when the
*  core generates an interrupt.  It then resets the event to
*  allow us to receive more events, reads an entry from the
*  Event Log list, and waits for the next event.
*
**/

LPVOID EventLogInterruptThread(LPVOID lpParam)
{
	UINT16  Type;
	UINT32   Info;
	MSGADDR msgaddr;
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
			BTICard_EventLogRd(&Type,&Info,NULL,pMailbox->hCore);

			msgaddr = Info;								//Info value is message address

			if (msgaddr == pMailbox->msgdefault.addr)
			{
				pMailbox->msgdefault.data = BTI429_MsgDataRd(msgaddr,pMailbox->hCore);
			}
			else if (msgaddr == pMailbox->latitude.addr)
			{
				pMailbox->latitude.data = BTI429_MsgDataRd(msgaddr,pMailbox->hCore);
			}
			else if (msgaddr == pMailbox->longitude.addr)
			{
				pMailbox->longitude.data = BTI429_MsgDataRd(msgaddr,pMailbox->hCore);
			}

			BTICard_IntClear(pMailbox->hCore);				//Clear interrupt from core
		}
	}

	pMailbox->fThreadHasQuit = TRUE;

	return(0);
}

int main(void)
{
	HCARD hCard;
	HCORE hCore;
	ERRVAL errval;
	INT    corenum;
	INT    rcvchan=0;
	INT    ch;

	pthread_t ThreadId;
	ThreadMailbox Mailbox;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP9  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI429 Example 9                                                  *");
	consoleprintf("\n    *  \"Program for Event Log using Interrupts.\"                         *");
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
*  Create a thread.
*
**/

	Mailbox.hCore  = hCore;

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
		BTICard_IntUninstall(hCard);
		pthread_cancel(ThreadId);
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

	Mailbox.msgdefault.addr = BTI429_FilterDefault(MSGCRT429_LOG,rcvchan,hCore);
	Mailbox.latitude.addr   = BTI429_FilterSet(MSGCRT429_LOG,0310,SDIALL,rcvchan,hCore);
	Mailbox.longitude.addr  = BTI429_FilterSet(MSGCRT429_LOG,0311,SDIALL,rcvchan,hCore);

/**
*
*  Initialize data values to zero.
*
**/

	Mailbox.msgdefault.data = 0;
	Mailbox.latitude.data   = 0;
	Mailbox.longitude.data  = 0;

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
*  Start the card to begin transmissions.
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

		consoleprintf("\r");
		consoleprintf("Default=%08lX " ,Mailbox.msgdefault.data);
		consoleprintf("Latitude=%08lX ",Mailbox.latitude.data);
		consoleprintf("Longitude=%08lX",Mailbox.longitude.data);
	}

/**
*
*  Clear the flag keeping the thread alive and wait for the
*  thread to quit.
*
**/

	consoleprintf("\nWaiting for the thread to quit.");

	Mailbox.fStayAlive = FALSE;		//Clear the fStayAlive flag so the thread commits suicide after the next interrupt

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
