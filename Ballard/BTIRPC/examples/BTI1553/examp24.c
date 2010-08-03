
/**
*
*  BTI1553 DRIVER EXAMPLE 24  (10/23/2009)
*  Copyright (c) 2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP24.C -- Controlling RT status word bits
*
**/

/**
*
*  This example configures Card Number 0 as an RT.  The
*  user controls bits in the RT status word using the
*  keyboard.
*
**/

#include <BTICard.h>
#include <BTI1553.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define	CARDNUM		  0
#define TAVAL		  1
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

#define SW_BUSY		  0x0008
#define SW_SRQ        0x0100

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

	UINT16 swdval;				/*MIL-STD-1553 status word*/
	BOOL   busy;
	BOOL   srq;
	BOOL   done;
	char   ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP24  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2009  Ballard Technology, Inc.  Everett, WA, USA.       *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI1553 Example 24                                                *");
	consoleprintf("\n    *  \"Controlling RT status word bits\"                                 *");
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
*  Configure the channel for remote terminal mode.
*
**/

	BTICard_CardReset(hCore);

	errval = BTI1553_RTConfig(RTCFG1553_DEFAULT,TAVAL,channum,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring for",errval);
		consoleprintf("\n        remote terminal mode on channel #%d on core #%d.",channum,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Initialize the status word flags
*
**/

	busy = 0;
	srq = 0;

	swdval = BTI1553_RTSWDRd(TAVAL,channum,hCore);

	swdval = (busy) ? (swdval | SW_BUSY) : (swdval & (~SW_BUSY));
	swdval = (srq)  ? (swdval | SW_SRQ)  : (swdval & (~SW_SRQ));

	errval = BTI1553_RTSWDWr(swdval, TAVAL, channum, hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while writing the",errval);
		consoleprintf("\n        status word to remote terminal #%d on channel #%d.",TAVAL,channum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Start the card to begin receiving messages.
*
**/

	BTICard_CardStart(hCore);

/**
*
*  Change RT status word bits when keyboard keys are pressed. Exit when ESC is pressed.
*
**/

	consoleprintf("\n\tOPTIONS:");
	consoleprintf("\n\tB   = toggle the busy bit of the status word");
	consoleprintf("\n\tS   = toggle the service request bit of the status word");
	consoleprintf("\n\tESC = Exit program");
	consoleprintf("\n\n");

	for (done=0;!done;)
	{
		consoleprintf("\r\tSWD: %04X\tBusy: %s \tSRQ: %s \t\t",swdval, (busy) ? "Set":"Clear",(srq) ? "Set":"Clear");

		ch = getch();
		switch (ch)
		{
			case 27:	/* ESC */
			{
				done=1;
				break;
			}
			case  66:	/* B */
			case  98:	/* b */
			{
				/*BUSY*/
				busy = (busy) ? 0 : 1;
				break;
			}
			case  83:	/* S */
			case 115:	/* s */
			{
				/*SERVICE REQUEST*/
				srq = (srq) ? 0 : 1;
				break;
			}
			default:
			{
				continue;
			}
		}

		swdval = BTI1553_RTSWDRd(TAVAL,channum,hCore);

		swdval = (busy) ? (swdval | SW_BUSY) : (swdval & (~SW_BUSY));
		swdval = (srq)  ? (swdval | SW_SRQ)  : (swdval & (~SW_SRQ));

		errval = BTI1553_RTSWDWr(swdval, TAVAL, channum, hCore);

		if (errval < 0)
		{
			consoleprintf("\nError:  An error was encountered (%i) while writing the",errval);
			consoleprintf("\n        status word to remote terminal #%d on channel #%d.",TAVAL,channum);
			consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
			consoleprintf("\n");
			done=1;
			break;
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
