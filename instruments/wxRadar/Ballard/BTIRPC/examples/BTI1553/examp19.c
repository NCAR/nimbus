
/**
*
*  BTI1553 DRIVER EXAMPLE 19  (10/23/2009)
*  Copyright (c) 2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP19.C -- BC message skipping and single-shot
*
**/

/**
*
*  This example configures Card Number 0 as a bus controller
*  to transmit a schedule of three messages.  The schedule is
*  created in such a way that the messages are transmitted at
*  frequencies of 100Hz, 50Hz, and 25Hz (or every 10ms, 20ms,
*  and 40ms, respectively).
*
*  The program uses keyboard keys to control the skip setting
*  of the first message and the single-shot setting on a
*  schedule entry.
*
*  Message skipping requires a 4G+ device. See BTI1553_CmdSkipWr
*  documentation for controlling the skip setting on a schedule
*  entry. BTI1553_CmdSkipWr is available on all devices.
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
#define RCV_CMD2	0x0821			/*Receive command of an RT-RT transfer*/
#define XMT_CMD2	0x1C21			/*Transmit command of an RT-RT transfer*/

#define	FRAMETIME	10000			/*Frame time in microseconds*/

#ifndef	TRUE
#define	TRUE 1
#endif

#ifndef	FALSE
#define	FALSE 0
#endif

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

	MSGADDR BCMsgs[3];
	UINT16  Data[3] = {0,0,0};
	SCHNDX  index;
	BOOL    skip;
	BOOL    shot;
	BOOL    done;
	char    ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP19  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2009  Ballard Technology, Inc.  Everett, WA, USA.       *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI1553 Example 19                                                *");
	consoleprintf("\n    *  \"BC message skipping and single-shot\"                             *");
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

	BCMsgs[0] = BTI1553_BCCreateMsg(MSGCRT1553_DEFAULT,CMD0,    0,       Data,hCore);
	BCMsgs[1] = BTI1553_BCCreateMsg(MSGCRT1553_DEFAULT,CMD1,    0,       NULL,hCore);
	BCMsgs[2] = BTI1553_BCCreateMsg(MSGCRT1553_RTRT,   RCV_CMD2,XMT_CMD2,NULL,hCore);

/**
*
*  Schedule the messages to create the desired bus controller
*  schedule.
*
**/

	BTI1553_BCSchedFrame(FRAMETIME,channum,hCore);
	BTI1553_BCSchedMsg(BCMsgs[0],  channum,hCore);
	BTI1553_BCSchedMsg(BCMsgs[1],  channum,hCore);
	index = BTI1553_BCSchedMsg(BCMsgs[2],  channum,hCore);

	BTI1553_BCSchedFrame(FRAMETIME,channum,hCore);
	BTI1553_BCSchedMsg(BCMsgs[0],  channum,hCore);

	BTI1553_BCSchedFrame(FRAMETIME,channum,hCore);
	BTI1553_BCSchedMsg(BCMsgs[0],  channum,hCore);
	BTI1553_BCSchedMsg(BCMsgs[1],  channum,hCore);

	BTI1553_BCSchedFrame(FRAMETIME,channum,hCore);
	BTI1553_BCSchedMsg(BCMsgs[0],  channum,hCore);

/**
*
*  Initialize the state on the card
*
**/

	skip = FALSE;
	shot = FALSE;

	BTI1553_MsgSkipWr(skip, BCMsgs[0], hCore);		/* Applies to 4G+ Devices */
	BTI1553_CmdShotWr(shot, index, channum, hCore);

/**
*
*  Start the card to begin receiving messages.
*
**/

	BTICard_CardStart(hCore);

/**
*
*  Change the state when keyboard keys are pressed. Exit when ESC is pressed.
*
**/

	consoleprintf("\n\tOPTIONS:");
	consoleprintf("\n\tS   = Single-Shot");
	consoleprintf("\n\tK   = Skip");
	consoleprintf("\n\tN   = Normal");
	consoleprintf("\n\tESC = Exit program");
	consoleprintf("\n\n");

	for (done=0;!done;)
	{
		consoleprintf("\r\tState: %s \t\t",(!skip && !shot) ? "Normal" : ((!skip) ? "Single-shot" : "Skipped"));

		ch = getch();
		switch (ch)
		{
			case 27:	/* ESC */
			{
				done=1;
				break;
			}
			case  83:	/* S */
			case 115:	/* s */
			{
				skip = FALSE;
				shot = TRUE;
				break;
			}
			case  75:	/* K */
			case 107:	/* k */
			{
				skip = TRUE;
				shot = FALSE;
				break;
			}
			case  78:	/* N */
			case 110:	/* n */
			{
				skip = FALSE;
				shot = FALSE;
				break;
			}
			default:
			{
				continue;
			}
		}

		BTI1553_MsgSkipWr(skip, BCMsgs[0], hCore);		/* Applies to 4G+ Devices */
		BTI1553_CmdShotWr(shot, index, channum, hCore);

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
