
/**
*
*  BTI429 DRIVER EXAMPLE 16  (10/23/2009)
*  Copyright (c) 2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP16.C -- Transmit message skipping
*
**/

/**
*
*  This program opens Card Number 0 and configures the
*  first compatible core to build a transmit schedule
*  using transmit interval information, and then transmit
*  the messages with changing data.
*
*  The skip state of the comp_airspeed label is controled
*  by keyboard input.
*
**/

#include <BTICard.h>
#include <BTI429.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define	CARDNUM		  0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(void)
{
	HCARD hCard;
	HCORE hCore;
	ERRVAL errval;
	INT corenum;
	INT xmtchan=0;
	MSGSTRUCT429 comp_airspeed, tot_air_temp, altitude_rate;
	MSGADDR msgaddr[3];
	INT	min_intrvls[3];
	INT	max_intrvls[3];
	UINT32 bnr_data[3];

	BOOL pause=0;
	BOOL done;
	char ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP16  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2009  Ballard Technology, Inc.  Everett, WA, USA.       *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI429 Example 16                                                 *");
	consoleprintf("\n    *  \"Transmit message skipping\"                                       *");
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
	consoleprintf("\nUsing transmit channel #%d",xmtchan);
	consoleprintf("\n");

/**
*
*  Configure xmtchan to transmit at high speed.
*
**/

	BTICard_CardReset(hCore);

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
*  Create a message for each of the labels we are transmitting.
*
**/

	comp_airspeed.addr = BTI429_MsgCreate(MSGCRT429_DEFAULT,hCore);
	tot_air_temp.addr  = BTI429_MsgCreate(MSGCRT429_DEFAULT,hCore);
	altitude_rate.addr = BTI429_MsgCreate(MSGCRT429_DEFAULT,hCore);

/**
*
*  Set up the arrays of message addresses; and intervals
*  needed to build the schedule.  Note that the nth element
*  in each array correspond to each other.
*
**/

	msgaddr[0] = comp_airspeed.addr;
	min_intrvls[0] = 250;
	max_intrvls[0] = 500;

	msgaddr[1] = tot_air_temp.addr;
	min_intrvls[1] = 250;
	max_intrvls[1] = 500;

	msgaddr[2] = altitude_rate.addr;
	min_intrvls[2] = 100;
	max_intrvls[2] = 200;

/**
*
*  Next, build a schedule using the minimum and maximum
*  transmit intervals.
*
**/

	errval = BTI429_SchedBuild(3,msgaddr,min_intrvls,max_intrvls,xmtchan,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while building",errval);
		consoleprintf("\n        a schedule on channel #%d of core #%d.",xmtchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Initialize the three Message Records.
*
**/

	comp_airspeed.data = BTI429_FldPutLabel(0L,0206);     /*Init label field*/
	tot_air_temp.data  = BTI429_FldPutLabel(0L,0211);
	altitude_rate.data = BTI429_FldPutLabel(0L,0212);

	comp_airspeed.data = BTI429_BNRPutMant(comp_airspeed.data,0L,14,0);     /*Init BNR data*/
	tot_air_temp.data  = BTI429_BNRPutMant(tot_air_temp.data,0L,11,0);
	altitude_rate.data = BTI429_BNRPutMant(altitude_rate.data,0L,16,0);

	BTI429_MsgDataWr(comp_airspeed.data,comp_airspeed.addr,hCore);
	BTI429_MsgDataWr(tot_air_temp.data,tot_air_temp.addr,hCore);
	BTI429_MsgDataWr(altitude_rate.data,altitude_rate.addr,hCore);

/**
*
*  Start the card to begin transmissions.
*
**/

	BTICard_CardStart(hCore);

/**
*
*  Continualy change data.
*  Change the state when keyboard keys are pressed. Exit when ESC is pressed.
*
**/

	consoleprintf("\n\tOPTIONS:");
	consoleprintf("\n\tP   = Pause (skip)");
	consoleprintf("\n\tU   = Unpause (unskip)");
	consoleprintf("\n\tESC = Exit program");
	consoleprintf("\n\n");

	for (done=0;!done;)
	{
		comp_airspeed.data = BTI429_MsgDataRd(comp_airspeed.addr,hCore);   /*Read messages back*/
		tot_air_temp.data  = BTI429_MsgDataRd(tot_air_temp.addr,hCore);
		altitude_rate.data = BTI429_MsgDataRd(altitude_rate.addr,hCore);

		bnr_data[0] = BTI429_BNRGetMant(comp_airspeed.data,14);   /*Extract BNR data*/
		bnr_data[1] = BTI429_BNRGetMant(tot_air_temp.data,11);
		bnr_data[2] = BTI429_BNRGetMant(altitude_rate.data,16);

		++bnr_data[0];   /*Change BNR data*/
		++bnr_data[1];
		++bnr_data[2];

		comp_airspeed.data = BTI429_BNRPutMant(comp_airspeed.data,bnr_data[0],14,0);   /*Put BNR data*/
		tot_air_temp.data  = BTI429_BNRPutMant(tot_air_temp.data,bnr_data[1],11,0);    /*back into*/
		altitude_rate.data = BTI429_BNRPutMant(altitude_rate.data,bnr_data[2],16,0);   /*messages*/

		BTI429_MsgDataWr(comp_airspeed.data,comp_airspeed.addr,hCore);   /*Write new message values*/
		BTI429_MsgDataWr(tot_air_temp.data,tot_air_temp.addr,hCore);
		BTI429_MsgDataWr(altitude_rate.data,altitude_rate.addr,hCore);

		ch = getch();
		switch (ch)
		{
			case 27:	/* ESC */
			{
				done=1;
				break;
			}
			case  80:	/* P */
			case 112:	/* p */
			{
				pause = 1;
				break;
			}
			case  85:	/* U */
			case 117:	/* u */
			{
				pause = 0;
				break;
			}
			default:
			{
				continue;
			}
		}

		BTI429_MsgSkipWr(pause,comp_airspeed.addr,hCore);

		consoleprintf("\r\tState: %s \t\t",(pause) ? "Pause (skip enabled)" : "Unpause (skip disabled)");
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
