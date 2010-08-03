
/**
*
*  BTIDIO DRIVER EXAMPLE 4  (10/23/2009)
*  Copyright (c) 2005-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP4.C -- Event Log using Interrupts
*
**/

/**
*
*  This program opens Card Number 0 and configures the
*  first compatible core to toggle the value of Discrete 0
*  in Bank A when spacebar is pressed.
*
*  Discrete 0 of Bank A also configured to generate an
*  Event Log List entry.  An interrupt handler is installed
*  which enables the Event Log List to generate an interrupt
*  when an entry is made to the list.  When an entry is read
*  from the list, the associated data is read.
*
**/

#include "BTICard.h"
#include "BTIDIO.h"
#include <stdlib.h>
#include <ncurses.h>
#include <pthread.h>
#include <sys/poll.h>

#define	CARDNUM          0
#define	MAX_CORES        4
#define	MAX_BANKS        4

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

typedef struct					/*Variables used by the thread*/
{
	volatile BOOL fStayAlive;		/*Flag indicating the thread should stay alive*/
	volatile BOOL fThreadHasQuit;		/*Flag indicating the thread has quit*/
	volatile UINT32 Count;			/*Counting variable*/
	HCORE hCore;				/*Handle of core*/
	UINT16 type;				/*Type of Event (edge or fault)*/
	INT banknum;				/*Bank on which event occured*/
	UINT32 state;				/*State of Bank right after event*/
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
	UINT16 type;
	INT banknum;
	UINT32 state;
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
			BTICard_EventLogRd(&type,&state,&banknum,pMailbox->hCore);

			pMailbox->type = type;
			pMailbox->state = state;
			pMailbox->banknum = banknum;

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
	INT corenum;
	INT banknum = 0;
	INT ionum = 0;
	INT ch;
	BOOL value;
	BOOL done = 0;
	UINT32 bankcfg;
	UINT32 disccfg;

	pthread_t ThreadId;
	ThreadMailbox Mailbox;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP4  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2005-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTIDIO Example 4                                                  *");
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

	consoleprintf("\nUsing DIO card #%d",CARDNUM);

/**
*
*  Find the first core with a DIO bank.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (banknum=0;banknum<MAX_BANKS;banknum++)
		{
			if (BTIDIO_BankIsDIO(banknum,hCore))	break;
		}

		if (banknum != MAX_BANKS) break;
	}

	if (errval || corenum == MAX_CORES || banknum == MAX_BANKS)
	{
		consoleprintf("\nError:  No DIO banks present in card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing DIO core #%d",corenum);
	consoleprintf("\nUsing Bank %c",banknum + 'A');
	consoleprintf("\nUsing Discrete %d",ionum);
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
*  Configure banknum as a shunt, input bank with inverted input polarity.
*
**/

	BTICard_CardReset(hCore);

	/*
	*  Threshold:   1.4V, TTL levels
	*  Sample Rate: 1ms clock, no divide value --> 1ms sampling
	*  Do not enable Event Logging at the Bank Level
	*/
	bankcfg = BANKCFGDIO_SHUNT | BANKCFGDIO_INPUT | BANKCFGDIO_POLIN_INV;
	errval = BTIDIO_BankConfig(bankcfg,THRESHOLDDIO_10KOHM,SAMPLERATEDIO_MS | 0x0001,banknum,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        bank #%d on core #%d.",banknum,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_IntUninstall(hCard);
		pthread_cancel(ThreadId);
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Configure ionum as an output with event logging
*
**/

	disccfg = DISCCFGDIO_INOUT | DISCCFGDIO_POLIN_INV | DISCCFGDIO_LOGANYEDGE | DISCCFGDIO_LOGFAULT;
	errval = BTIDIO_DiscreteConfig(disccfg,ionum,banknum,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        bank #%d on core #%d.",banknum,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_IntUninstall(hCard);
		pthread_cancel(ThreadId);
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Configure the Event Log to handle up to 30 events
*
**/

	errval = BTICard_EventLogConfig(LOGCFG_ENABLE,1024,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        the Event Log list on core #%d.",corenum);
		consoleprintf("\n");
		BTICard_IntUninstall(hCard);
		pthread_cancel(ThreadId);
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

	consoleprintf("\nPress spacebar to toggle discrete %d.  Press escape to exit...\n\n",ionum);

	while (!done)
	{
		ch = getch();

		if (ch != ERR)
		{
			/* if escape is pressed, exit loop */
			if (ch == 0x1B)
			{
				done = 1;
			}

			/* if spacebar is pressed, toggle the discrete */
			else if (ch == ' ')
			{
				value = BTIDIO_DiscreteRd(ionum,banknum,hCore);
				BTIDIO_DiscreteWr(!value,ionum,banknum,hCore);
			}

			/* Clear faults on user command */
			else if (ch == 'c' || ch == 'C')
			{
				BTIDIO_BankFaultClr(0x01,banknum,hCore);
			}
		}


		/* Print the last recevied Event Log */
		if (Mailbox.type == EVENTTYPE_DIOEDGE)
			consoleprintf("\rBank %d last received an edge (0x%04X)   ",
					Mailbox.banknum,Mailbox.state);

		else if (Mailbox.type == EVENTTYPE_DIOFAULT)
		 	consoleprintf("\rBank %d last received a fault (0x%04X)   ",
					Mailbox.banknum,Mailbox.state);

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
