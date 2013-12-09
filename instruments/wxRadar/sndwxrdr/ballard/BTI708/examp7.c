
/**
*
*  BTI708 DRIVER EXAMPLE 7  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP7.C -- Event Log using Interrupts
*
**/

/**
*
*  This program opens Card Number 0, and configures the
*  first compatible core to configure the first transmit
*  channel to transmit a sequence of 32 messages at a
*  rate of 200Hz.  The messages are automatically assigned
*  the numbers 0 through 31.
*
*  An Event Log frequency of 16 is specifed, which will
*  cause the channel to issue an Event Log List entry
*  every 16th message (message numbers 15, and 31).
*  The program uses these events to update he data for
*  the half of messages that were just transmitted.
*
*  An interrupt handler is installed which enables the
*  Event Log List to generate an interrupt when an entry
*  is made to the list.  The interrupt service thread
*  will update the data when the interrupt is issued from
*  the device.
*
*  The program continues in this manner until a key is
*  pressed.
*
**/

#include "BTICard.h"
#include "BTI708.h"
#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/poll.h>

#define	CARDNUM		  0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

#define	FRAMEFREQ	  200          /*Frame frequency of 200Hz*/
#define LOGFREQ		  16           /*Generate a Log Event after every 16th message*/
#define	FRAMECOUNT	  32           /*Transmit a total of 32 messages*/

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

typedef struct						/*Variables used by the thread*/
{
	volatile BOOL fStayAlive;		/*Flag indicating the thread should stay alive*/
	volatile BOOL fThreadHasQuit;	/*Flag indicating the thread has quit*/
	UINT32   Count;					/*Counting variable*/
	INT     xmtchan;				/*Transmit channel number*/
	HCORE   hCore;					/*Handle to core*/
} ThreadMailbox;

//typedef struct						/*Variables used by the thread*/
//{
//	BOOL    fWinNT40;				/*Flags whether we are running under Windows NT 4.0*/
//	BOOL    fStayAlive;				/*Flag indicating the thread should stay alive*/
//	HCORE   hCore;					/*Handle to core*/
//	INT     xmtchan;				/*Transmit channel number*/
//	UINT32   Count;					/*Counting variable*/
//	HANDLE  hEvent;					/*Handle of event object*/
//	HANDLE  hThread;				/*Handle of thread object*/
//} ThreadMailbox;

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
	UINT16 framenum;
	INT    j,jj;
	UINT16 Data[100];

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

			framenum = (UINT16)Info;					//Info value is frame number

			if (framenum == ((FRAMECOUNT/2)-1))			//First half transmitted
			{
				for (j=0;j<=framenum;j++)
				{
					for (jj=0;jj<100;jj++) Data[jj] = j + (pMailbox->Count * FRAMECOUNT);
					BTI708_MsgDataWr(Data,j,pMailbox->xmtchan,pMailbox->hCore);
				}
			}
			else if (framenum == (FRAMECOUNT-1))		//Second half transmitted
			{
				for (j=(FRAMECOUNT/2);j<=framenum;j++)
				{
					for (jj=0;jj<100;jj++) Data[jj] = j + (pMailbox->Count * FRAMECOUNT);
					BTI708_MsgDataWr(Data,j,pMailbox->xmtchan,pMailbox->hCore);
				}
			}

			BTICard_IntClear(pMailbox->hCore);				//Clear interrupt from card
		}
	}

	pMailbox->fThreadHasQuit = TRUE;

	return(0);
}

int main(void)
{
	HCARD  hCard;
	HCORE  hCore;
	ERRVAL errval;
	INT    corenum;
	INT    xmtchan=0;
	INT    j,jj;
	INT    ch;
	UINT16 Data[100];

	pthread_t ThreadId;
	ThreadMailbox Mailbox;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP7  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI708 Example 7                                                  *");
	consoleprintf("\n    *  \"Event Log using Interrupts example.\"                             *");
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
*  Find the first ARINC 708 core with a transmit channel.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (xmtchan=0;xmtchan<MAX_CHANNELS;xmtchan++)	if (BTI708_ChIsXmt(xmtchan,hCore)) break;

		if (xmtchan != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || xmtchan == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No ARINC 708 transmit channels present in cardnum %d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing ARINC 708 core #%d",corenum);
	consoleprintf("\nUsing transmit channel #%d",xmtchan);
	consoleprintf("\n");

/**
*
*  Create a thread.
*
**/

	Mailbox.hCore  = hCore;
	Mailbox.xmtchan  = xmtchan;

	errval = pthread_create(
		&ThreadId,				//ID of Thread being created
		NULL,					//Default attributes
		EventLogInterruptThread,//Pointer to thread function
		(LPVOID)&Mailbox);		//Argument for new thread

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

	errval = BTICard_IntInstall(NULL, hCore);

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
*  Configure transmit channel.
*
**/

	BTICard_CardReset(hCore);

	errval = BTI708_ChConfig(CHCFG708_DEFAULT,xmtchan,hCore);

	if (errval)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        channel #%d on core #%d.",xmtchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_IntUninstall(hCard);
		pthread_cancel(ThreadId);
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Build the transmit channel messages.
*
**/

	errval = BTI708_XmtBuild(MSGCRT708_DEFAULT,FRAMEFREQ,LOGFREQ,FRAMECOUNT,xmtchan,hCore);

	if (errval)
	{
		consoleprintf("\nError:  An error was encountered (%i) while building",errval);
		consoleprintf("\n        channel #%d on core #%d.",xmtchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_IntUninstall(hCard);
		pthread_cancel(ThreadId);
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Create and write the data for the messages.
*
**/

	for (j=0;j<FRAMECOUNT;j++)					/*Loop through all the messages*/
	{
		for (jj=0;jj<100;jj++) Data[jj] = j;
		BTI708_MsgDataWr(Data,j,xmtchan,hCore);	/*Write data to the Device*/
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
*  Start operation of the card.
*
**/

	BTICard_CardStart(hCore);

/**
*
*  Loop until a key is hit.
*
**/

	consoleprintf("\nPress any key to exit...");
	consoleprintf("\n\n");

	while (1)
	{
		ch = getch();
		if (ch != ERR) break;

		consoleprintf("\rNumber of interrupts %d",Mailbox.Count);
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
