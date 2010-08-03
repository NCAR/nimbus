
/**
*
*  BTI429 DRIVER EXAMPLE 14  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP14.C -- Transmitter example using local SchedBuildExx()
*
**/

/**
*
*  This program opens Card Number 0; and configures each
*  transmit channel on the first compatible core to build a
*  transmit schedule using the local version of SchedBuildExx().
*
**/

#include <BTICard.h>
#include <BTI429.h>
#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

#define BUFSIZE 16384

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

INT minmax[] =
{
	180,  220,	//Min/max times in ms
	180,  220,
	180,  220,
	180,  220,
	180,  220,
	180,  220,
	190,  210,
	800, 1200,
	190,  210,
	180,  220,
	180,  220,
	180,  220,
	180,  220,
	800, 1200,
	800, 1200,
	800, 1200,
	800, 1200,
	800, 1200,
	180,  220,
	 40,   60,
	 40,   60,
	180,  220,
	 90,  110,
	 90,  110,
	 75,  125,
	190,  210,
	800, 1200,
	800, 1200,
	800, 1200,
	 40,   60,
	 40,   60,
	180,  220
};

HCARD hCard;
HCORE hCore;
INT corenum;
MSGADDR msgaddr[64];
INT	min_intrvls[64];
INT	max_intrvls[64];
BOOL bGo=FALSE;

LPVOID buildschedule(LPVOID ptr)
{
	ERRVAL errval;
	LPVOID buf;

	ULONG_PTR xmtchan = (ULONG_PTR)ptr;

/**
*
*  Wait until we're signaled to go
*
**/

	consoleprintf("\nScheduling channel %u.",xmtchan);

	while(!bGo);

/**
*
*  Allocate a buffer used for calculating the schedule.  SchedBuild() defaults to 8K.
*
**/

	buf = malloc(BUFSIZE);

	if (!buf)
	{
		consoleprintf("\nError:  Unable to allocate temporary buffer.");
		return(NULL);
	}

/**
*
*  Next, build a schedule using the minimum and maximum transmit intervals.
*
*  Note that the speed parameter must match the speed option specified in
*  ChConfig() - non-zero for high speed, and zero for low speed.
*
**/

	errval = BTI429_SchedBuildExx(0,sizeof(minmax)/sizeof(minmax[0])/2,msgaddr,min_intrvls,max_intrvls,1,buf,BUFSIZE,xmtchan,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while building the schedule on",errval);
		consoleprintf("\n        channel #%d on core #%d.",xmtchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		return(NULL);
	}

	consoleprintf("\nThe schedule was successfully built on channel #%u.",xmtchan);

/**
*
*  Free the buffer.  It is no longer needed.
*
**/

	free(buf);
	return(NULL);
}

int main(void)
{
	ERRVAL errval;
	INT xmtchan=0;
	INT j;
	INT ret;
	pthread_t thread[MAX_CHANNELS];

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP14  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI429 Example 14                                                 *");
	consoleprintf("\n    *  \"Program for transmitter example.\"                                *");
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
*  Find the first 429 core with a transmit channel.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (xmtchan=0;xmtchan<MAX_CHANNELS;xmtchan++)	if (BTI429_ChIsXmt(xmtchan,hCore))	break;

		if (xmtchan != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || xmtchan == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No ARINC 429 transmit channels present in card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing ARINC 429 core #%d",corenum);
	consoleprintf("\n");

	BTICard_CardReset(hCore);

/**
*
*  Create a message for each of the labels we are transmitting and
*  set up the arrays of message addresses; and intervals
*  needed to build the schedule.  Note that the nth element
*  in each array correspond to each other.
*
**/

	for (j=0;j<sizeof(minmax)/sizeof(minmax[0])/2;++j)
	{
		msgaddr[j]     = BTI429_MsgCreate(MSGCRT429_DEFAULT,hCore);
		min_intrvls[j] = minmax[(j*2)+0];
		max_intrvls[j] = minmax[(j*2)+1];
	}

/**
*
*  Loop through all transmit channels
*
**/

	for (xmtchan=0;xmtchan<MAX_CHANNELS;xmtchan++)
	{
		ULONG_PTR xmtchanval = xmtchan;

		consoleprintf("\nChecking channel %u.",xmtchanval);

		if (!BTI429_ChIsXmt(xmtchan,hCore))	continue;

/**
*
*  Configure xmtchan to transmit at high speed.
*
**/

		errval = BTI429_ChConfig(CHCFG429_HIGHSPEED,xmtchan,hCore);

		if (errval < 0)
		{
			consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
			consoleprintf("\n        channel #%d on core #%d.",xmtchan,corenum);
			consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
			BTICard_CardClose(hCard);
			consoleexit(1);
		}

/**
*
*  Spawn thread to create schedules
*
**/

		ret = pthread_create(&thread[xmtchan],NULL,buildschedule,(LPVOID)xmtchanval);
	}

/**
*
*  Signal threads to start
*
**/

	bGo=TRUE;

/**
*
*  Loop through all transmit channels and wait for threads to complete
*
**/

	for (xmtchan=0;xmtchan<MAX_CHANNELS;xmtchan++)
	{
		if (!BTI429_ChIsXmt(xmtchan,hCore))	continue;

		ret = pthread_join(thread[xmtchan],NULL);
	}

/**
*
*  Start operation of the card.
*
**/

	errval = BTICard_CardStart(hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while starting",errval);
		consoleprintf("\n        card #%d / core #%d.",CARDNUM,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

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
