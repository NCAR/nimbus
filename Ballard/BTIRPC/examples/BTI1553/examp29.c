
/**
*
*  BTI1553 DRIVER EXAMPLE 29  (10/23/2009)
*  Copyright (c) 2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP29.C -- Conditional BC schedule branching
*
**/

/**
*
*  This example configures Card Number 0 as a bus controller
*  to transmit a schedule of two messages.  A third message
*  is insterted in the schedule inside a conditional block
*  directly after the CMD0 message. If the schedule sees the
*  SR bit set in the status word of this message, then the
*  BC branches to send the CMD2 message (VECTOR WORD mode
*  command) before continuing with the CMD1 message.
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

#define CMD0		0x0843			/*Command word for first message*/
#define CMD1		0x0821			/*Command word for second message*/
#define CMD2		0x0C10			/*VECTOR WORD mode command*/

#define	FRAMETIME	10000			/*Frame time in microseconds*/

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

	MSGADDR BCMsgs[3];
	UINT16  Data[3] = {0,0,0};
	SCHNDX  index, branch1, branch2;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP29  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2009  Ballard Technology, Inc.  Everett, WA, USA.       *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI1553 Example 29                                                *");
	consoleprintf("\n    *  \"Conditional BC schedule branching\"                               *");
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

	Data[0] = 0;
	Data[1] = 0;
	Data[2] = 0;

	BCMsgs[0] = BTI1553_BCCreateMsg(MSGCRT1553_DEFAULT,CMD0,0,Data,hCore);
	BCMsgs[1] = BTI1553_BCCreateMsg(MSGCRT1553_DEFAULT,CMD1,0,NULL,hCore);
	BCMsgs[2] = BTI1553_BCCreateMsg(MSGCRT1553_DEFAULT,CMD2,0,NULL,hCore);

/**
*
*  Schedule the messages to create the desired bus controller
*  schedule.
*
*  Two conditional branches are combined to create an IF ELSE
*  logical condition. The branches destination schedule index
*  values are updated after the destination is added to the
*  schedule.
*
**/

	BTI1553_BCSchedFrame(FRAMETIME,channum,hCore);
	BTI1553_BCSchedMsg(BCMsgs[0],channum,hCore);						/*Message always sent*/
	branch1 = BTI1553_BCSchedBranch(COND1553_SRQ,   0,channum,hCore);	/* IF (SR bit set)*/
	branch2 = BTI1553_BCSchedBranch(COND1553_ALWAYS,0,channum,hCore);	/* ELSE*/
	index = BTI1553_BCSchedMsg(BCMsgs[2],channum,hCore);				/*Message sent if SR bit set in BCMsg[0]*/
	BTI1553_BCSchedBranchUpdate(index,branch1,channum,hCore);			/* Update branch1 destination to previous index*/
	index = BTI1553_BCSchedMsg(BCMsgs[1],channum,hCore);				/*Message always sent*/
	BTI1553_BCSchedBranchUpdate(index,branch2,channum,hCore);			/* Update branch2 destination to previous index*/

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

	consoleprintf("\nPress any key to exit....\n\n");

	while (1)
	{
		ch = getch();
		if (ch != ERR) break;
	}

/**
*
*  Stop the card.
*
**/

	BTICard_CardStop(hCore);

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
