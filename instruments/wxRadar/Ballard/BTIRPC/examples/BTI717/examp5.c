
/**
*
*  BTI717 DRIVER EXAMPLE 5  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP5.C -- Event Log using Interrupts
*
**/

/**
*
*  This program opens Card Number 0, and configures the
*  first BiPhase channel as a receiver.
*
*  The first word of each SubFrame is configured to generate
*  an Event Log List entry.  An interrupt handler is installed
*  which enables the Event Log List to generate an interrupt
*  when an entry is made to the list.  When an entry is read
*  from the list, the associated data is read.
*
**/

#include "BTICard.h"
#include "BTI717.h"
#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/poll.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

typedef struct                          /*Variables used by the thread*/
{
	volatile BOOL fStayAlive;       /*Flag indicating the thread should stay alive*/
	volatile BOOL fThreadHasQuit;   /*Flag indicating the thread has quit*/
	volatile UINT32 Count;           /*Counting variable*/
	volatile UINT16 Data;           /*Data values transmitted by BCMsgs[0]*/
	HCORE	 hCore;                 /*Handle of core*/
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
	UINT16 Type;
	UINT32  Info;
	MSGADDR msgaddr;
	struct pollfd eventlog_poll;

	ThreadMailbox *pMailbox = (ThreadMailbox*)lpParam;

	pMailbox->fStayAlive     = TRUE;
	pMailbox->fThreadHasQuit = FALSE;
	pMailbox->Count          = 0;
	pMailbox->Data           = 0;

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

			msgaddr = BTICard_AddrHost(Info, pMailbox->hCore);	//Info value is message address (convert to Host address)

			pMailbox->Data = (BTICard_RAMRdW(msgaddr,pMailbox->hCore) & 0x0FFF);

			BTICard_IntClear(pMailbox->hCore);			//Clear interrupt from card
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
	INT corenum;
	INT rcvchan=0;
	INT ch;

	pthread_t ThreadId;
	ThreadMailbox Mailbox;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP5  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI717 Example 5                                                  *");
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

		for (rcvchan=0;rcvchan<MAX_CHANNELS;rcvchan++)
		{
			if (BTI717_ChIsType(CHCFG717_BIPHASERCV, rcvchan, hCore))	break;
		}

		if (rcvchan != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || rcvchan == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No ARINC 717 BiPhase receive channels present in cardnum %d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing ARINC 717 core #%d",corenum);
	consoleprintf("\nUsing BiPhase receive channel #%d",rcvchan);
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
*  Configure channel for auto speed detection.
*
**/

	BTICard_CardReset(hCore);

	errval = BTI717_ChConfig(CHCFG717_BIPHASERCV | CHCFG717_AUTOSPEED,rcvchan,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring channel %d.",errval,rcvchan);
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
		consoleprintf("\nError:  An error was encountered (%i) while configuring the",errval);
		consoleprintf("\n        Event Log list on on core #%d.",corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_IntUninstall(hCard);
		pthread_cancel(ThreadId);
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Configure word options.
*
**/

	//Mark first word of SFs 1, 2, 3, and 4 for Event Logging

	BTI717_SubFrmWordConfig(WRDCFG717_WRDLOG,1,1,rcvchan,hCore);
	BTI717_SubFrmWordConfig(WRDCFG717_WRDLOG,2,1,rcvchan,hCore);
	BTI717_SubFrmWordConfig(WRDCFG717_WRDLOG,3,1,rcvchan,hCore);
	BTI717_SubFrmWordConfig(WRDCFG717_WRDLOG,4,1,rcvchan,hCore);

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

		consoleprintf("\rData received = %04o (Octal)  Interrupt count = %d",Mailbox.Data,Mailbox.Count);
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
		consoleprintf("\nError:  An error was encountered (%i) while uninstalling interrupt handler.",errval);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
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
