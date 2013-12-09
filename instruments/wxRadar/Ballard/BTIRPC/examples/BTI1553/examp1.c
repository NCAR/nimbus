
/**
*
*  BTI1553 DRIVER EXAMPLE 1  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP1.C -- Simulating the BC - Unscheduled Messages
*
**/

/**
*
*  This example configures Card Number 0 as a BC to transmit
*  the receive command 0843H with data words 1234H, 5678H,
*  and 9ABCH.  If the message is completed successfully, it
*  displays the responding RT's status word.  Otherwise it
*  displays the value of any errors.
*
**/

#include "BTICard.h"
#include "BTI1553.h"
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
	HCARD  hCard;					/*Handle to card*/
	HCORE  hCore;					/*Handle to core*/
	ERRVAL errval;					/*Error return value*/
	INT    corenum;					/*Core number*/
	INT    channum=0;				/*Channel number*/

	XMITFIELDS1553 Msg;				/*Message structure*/

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP1  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI1553 Example 1                                                 *");
	consoleprintf("\n    *  \"Simulating the BC - Unscheduled Messages\"                        *");
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
		consoleprintf("\nError:  No MIL-STD-1553 channels present in cardnum %d.",CARDNUM);
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

	BTICard_CardReset(hCore);                                       /*Reset the core*/

	errval = BTI1553_BCConfig(BCCFG1553_DEFAULT,channum,hCore);     /*Configure the core*/

	if (errval < 0)
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
*  Create an empty frame in the BC Schedule.
*
*  BTI1553_BCTransmitMsg uses an asynchronous Message Record
*  that is transmitted at the end of every frame.  Therefore,
*  even though we aren't scheduling messages, we must still
*  schedule a frame.
*
**/

	BTI1553_BCSchedFrame(1000,channum,hCore);

/**
*
*  Start operation of the card.
*
**/

	BTICard_CardStart(hCore);

/**
*
*  Initialize the message command and data words, and select
*  a BC-RT transfer.  Then transmit the message.
*
**/

	Msg.ctrlflags = MSGCRT1553_BCRT;                        /*Selects BC-RT transfer*/
	Msg.cwd1      = BTI1553_ValPackCWD(1,0,2,3);            /*Receive command word*/
	Msg.data[0]   = 0x1234;                                 /*Data word 0*/
	Msg.data[1]   = 0x5678;                                 /*Data word 1*/
	Msg.data[2]   = 0x9ABC;                                 /*Data word 2*/

	errval = BTI1553_BCTransmitMsg(&Msg,channum,hCore);     /*Transmit the message*/

/**
*
*  Test for any error results.
*
*  Note the distinction between card errors and bus errors.
*  The returned value of BTI1553_BCTransmitMsg is a card error
*  (if any occurred).  Errors related to the RT's reponse are
*  returned in the (errflags) member of the message
*  structure.
*
**/

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while transmitting the message.",errval);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	if (Msg.errflags & MSGERR1553_NORESP)		/*Was there a response?*/
	{
		consoleprintf("\nWarning:  No response was received from the RT.");
		consoleprintf("\n          Refer to the documentation for a");
		consoleprintf("\n          description of the error flags (%04XH).",Msg.errflags);
		consoleprintf("\n");
	}
	else if (Msg.errflags & MSGERR1553_ANYERR)	/*Was there an error?*/
	{
		consoleprintf("\nWarning:  An error occurred while receiving the response from the RT.");
		consoleprintf("\n          Refer to the documentation for a");
		consoleprintf("\n          description of the error flags (%04XH).",Msg.errflags);
		consoleprintf("\n");
	}
	else										/*There were no errors*/
	{
		consoleprintf("\nSuccess!  The message was completed successfully.");
		consoleprintf("\n          The RT returned the status word %04XH.",Msg.swd1);
		consoleprintf("\n");
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
