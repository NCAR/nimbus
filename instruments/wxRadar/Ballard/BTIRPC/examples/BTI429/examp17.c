
/**
*
*  BTI429 DRIVER EXAMPLE 17  (10/23/2009)
*  Copyright (c) 2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP17.C -- Sequential log frequency example
*
**/

/**
*
*  This program opens Card Number 0, finds the first
*  compatible core, builds a transmit schedule using
*  transmit interval information, and creates filters
*  for two receive messages.  It also enables the
*  Sequential Monitor to record transmitted and
*  received messages. (See Example #3)
*
*  The sequential monitor is configured to add an
*  entry in the event log list for each 32 records.
*  When the progam detects an entry in the event log
*  list it reads the 32 records in a single buffer.
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

#define SEQLOGFREQ	  32

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(void)
{
	HCARD hCard;
	HCORE hCore;
	ERRVAL errval;
	INT corenum;

	INT rcvchan=0;
	INT xmtchan=0;
	BOOL rcvfound, xmtfound;

	MSGSTRUCT429 msgdefault, latitude, longitude;
	MSGSTRUCT429 comp_airspeed, tot_air_temp, altitude_rate;
	MSGADDR msgaddr[3];
	INT	min_intrvls[3];
	INT	max_intrvls[3];
	UINT16  seqbuf[2048];
	UINT32  seqcount;
	UINT32  blkcnt;
	UINT16  type;
	UINT32  info;
	INT     ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP17  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2009  Ballard Technology, Inc.  Everett, WA, USA.       *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI429 Example 17                                                 *");
	consoleprintf("\n    *  \"Sequential log frequency example\"                                *");
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
*  Find the first ARINC 429 core with both a receive and a transmit channel.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		rcvfound = 0;
		xmtfound = 0;

		errval = BTICard_CoreOpen(&hCore,corenum,hCard);	/* Open a handle to the core. */
		if (errval) break;

		for (rcvchan=0;rcvchan<MAX_CHANNELS;rcvchan++)		/* Search for receive channel. */
		{
			rcvfound = BTI429_ChIsRcv(rcvchan,hCore);
			if (rcvfound) break;
		}

		if (rcvfound)
		{
			for (xmtchan=0;xmtchan<MAX_CHANNELS;xmtchan++)	/* Search for transmit channel. */
			{
				xmtfound = BTI429_ChIsXmt(xmtchan,hCore);
				if (xmtfound) break;
			}
		}

		if (rcvfound && xmtfound)	break;
	}

	if (errval || (corenum == MAX_CORES) || !rcvfound || !xmtfound)
	{
		consoleprintf("\nError:  No compatible ARINC 429 core present in card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing ARINC 429 core #%d",corenum);
	consoleprintf("\nUsing receive channel #%d",rcvchan);
	consoleprintf("\nUsing transmit channel #%d",xmtchan);
	consoleprintf("\n");

/**
*
*  Configure rcvchan for automatic speed detection with
*  the self-test wraparound enabled.
*  Configure xmtchan to transmit at low speed with
*  the self-test wraparound enabled.
*
**/

	BTICard_CardReset(hCore);

	errval = BTI429_ChConfig(CHCFG429_AUTOSPEED,rcvchan,hCore);   /*Configure channel*/

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        channel #%d on core #%d.",rcvchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	errval = BTI429_ChConfig(CHCFG429_SEQALL,xmtchan,hCore);      /*Configure channel*/

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        channel #%d on core #%d.",xmtchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
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
*  in each array correspond to eacho ther.
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

	comp_airspeed.data = BTI429_FldPutLabel(0L,0206);   /*Init label field*/
	tot_air_temp.data  = BTI429_FldPutLabel(0L,0211);
	altitude_rate.data = BTI429_FldPutLabel(0L,0212);

	comp_airspeed.data = BTI429_BNRPutMant(comp_airspeed.data,0L,14,0);   /*Init BNR data*/
	tot_air_temp.data  = BTI429_BNRPutMant(tot_air_temp.data,0L,11,0);
	altitude_rate.data = BTI429_BNRPutMant(altitude_rate.data,0L,16,0);

	BTI429_MsgDataWr(comp_airspeed.data,comp_airspeed.addr,hCore);
	BTI429_MsgDataWr(tot_air_temp.data,tot_air_temp.addr,hCore);
	BTI429_MsgDataWr(altitude_rate.data,altitude_rate.addr,hCore);

/**
*
*  Before any messages can be received, filters must be set
*  for the receive channel.  First, the default filter is
*  created.  Then, two filters are set for the two messages
*  of interest.
*
*  Record the two filtered messages.
*
**/

	msgdefault.addr = BTI429_FilterDefault(MSGCRT429_DEFAULT,rcvchan,hCore);
	latitude.addr   = BTI429_FilterSet(MSGCRT429_SEQ,0310,SDIALL,rcvchan,hCore);
	longitude.addr  = BTI429_FilterSet(MSGCRT429_SEQ,0311,SDIALL,rcvchan,hCore);

/**
*
*  Configure the Event Log list.
*
**/

	errval = BTICard_EventLogConfig(LOGCFG_ENABLE,1024,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        event log list on core #%d.",corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Configure the Sequential Monitor.
*
**/

	errval = BTICard_SeqConfig(SEQCFG_LOGFREQ,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring the sequential record.",errval);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	BTICard_SeqLogFrequency(SEQLOGFREQ,hCore);

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

		if (BTICard_EventLogRd(&type,&info,NULL,hCore))
		{
			seqcount = BTICard_SeqBlkRd(seqbuf,sizeof(seqbuf)/sizeof(seqbuf[0]),&blkcnt,hCore);

			consoleprintf("\nSeqBlkRd :: blkcnt=%u seqcount=%u",blkcnt,seqcount);
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
