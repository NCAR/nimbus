
/**
*
*  BTI1553 DRIVER EXAMPLE 27  (10/23/2009)
*  Copyright (c) 2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP27.C -- BC retries
*
**/

/**
*
*  This example requires a Ballard 1553 C version Device or higher.
*  It configures Card Number 0 as a BC, and a Bus Monitor.
*  The BC transmits two RT->BC messages (command words 0C43H &
*  0C23H). A retry is scheduled following the 0C43H message on a
*  no response condition. A retry is scheduled following the 0C23H
*  message on a message error condition. Use this example with
*  example 20.
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

#define CMD0		0x0C43			/*Command word for first message*/
#define CMD1		0x0C23			/*Command word for second message*/

#define	FRAMETIME	1000000			/*Frame time in microseconds*/

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

	MSGADDR BCMsgs[2];

	UINT16 seqbuf[2048];
	UINT32  seqcount;
	UINT32  blkcnt;
	LPSEQRECORD1553 pRec1553;
	SEQFINDINFO sfinfo;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP27  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2009  Ballard Technology, Inc.  Everett, WA, USA.       *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI1553 Example 27                                                *");
	consoleprintf("\n    *  \"BC retries\"                                                      *");
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
*  Configure the channel for bus controller mode.
*
**/

	BTICard_CardReset(hCore);									/*Reset the core*/

	errval = BTI1553_BCConfig(BCCFG1553_DEFAULT,channum,hCore);	/*Configure the core*/

	if (errval)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring bus",errval);
		consoleprintf("\n        controller mode on channel #%d on core #%d.",channum,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Create the message records for each of the three messages.
*
**/

	BCMsgs[0] = BTI1553_BCCreateMsg(MSGCRT1553_DEFAULT,CMD0,0,NULL,hCore);
	BCMsgs[1] = BTI1553_BCCreateMsg(MSGCRT1553_DEFAULT,CMD1,0,NULL,hCore);

/**
*
*  Schedule the messages to create the desired bus controller
*  schedule. Add a retry after the 0C43H message to retry one
*  time on the alternate bus when no status word response is
*  received. Add the retry after the 0C23H message to retry
*  two times when the message error bit is set in the status
*  word.
*
**/

	BTI1553_BCSchedFrame(FRAMETIME,    channum,hCore);
	BTI1553_BCSchedMsg(BCMsgs[0],      channum,hCore);
	BTI1553_BCSchedRetry(COND1553_NORESP|COND1553_ALTBUS,1,channum,hCore);

	BTI1553_BCSchedFrame(FRAMETIME,    channum,hCore);
	BTI1553_BCSchedMsg(BCMsgs[1],      channum,hCore);
	BTI1553_BCSchedRetry(COND1553_ME,2,channum,hCore);

/**
*
*  Configure the channel for monitor mode.
*
**/

	errval = BTI1553_MonConfig(MONCFG1553_DEFAULT,channum,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring for",errval);
		consoleprintf("\n        monitor mode on channel #%d on core #%d.",channum,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Configure the sequential record.
*
**/

	errval = BTICard_SeqConfig(SEQCFG_DEFAULT,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        the sequential record on core #%d.",corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
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
*  Read the entire contents of the monitor and display the results.
*
**/

	consoleprintf("\nPress any key to exit....\n\n");

	while(1)
	{
		ch = getch();
		if (ch != ERR) break;

		seqcount = BTICard_SeqBlkRd(seqbuf,sizeof(seqbuf)/sizeof(seqbuf[0]),&blkcnt,hCore);

		BTICard_SeqFindInit(seqbuf,seqcount,&sfinfo);

		while(!BTICard_SeqFindNext1553(&pRec1553,&sfinfo))
		{
			consoleprintf("\n");

			consoleprintf(" Ch:%u",(pRec1553->activity & MSGACT1553_CHMASK) >> MSGACT1553_CHSHIFT);
			consoleprintf(" Time:%lu",pRec1553->timestamp);

			if (pRec1553->activity & MSGACT1553_RCVCWD1) consoleprintf(" Cwd1:%04X",pRec1553->cwd1);
			if (pRec1553->activity & MSGACT1553_RCVCWD2) consoleprintf(" Cwd2:%04X",pRec1553->cwd2);
			if (pRec1553->activity & MSGACT1553_RCVSWD1) consoleprintf(" Swd1:%04X",pRec1553->swd1);
			if (pRec1553->activity & MSGACT1553_RCVSWD2) consoleprintf(" Swd2:%04X",pRec1553->swd2);

			consoleprintf(" BUS:%s", (pRec1553->activity & MSGACT1553_BUS) ? "B" : "A");

			if      (pRec1553->error & MSGERR1553_NORESP) consoleprintf(" No response!");
			else if (pRec1553->error > 1) consoleprintf(" Error! (%04XH)",pRec1553->error);
		}
	}

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
