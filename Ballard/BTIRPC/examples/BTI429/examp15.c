
/**
*
*  BTI429 DRIVER EXAMPLE 15  (10/23/2009)
*  Copyright (c) 2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP15.C -- Print a transmit schedule
*
**/

/**
*
*  This program opens Card Number 0 and configures the
*  first compatible core to build a transmit schedule
*  using transmit interval information, and then prints
*  the schedule to the screen.
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

/**
*
*  This helper function decodes a command block and
*  prints the details.
*
**/

VOID BTI429_SchedDump(UINT16 cmdblk[],HCARD handleval)
{
	MSGADDR      msgaddr;
	MSGFIELDS429 msgfields;

	enum
	{
		OPCODE429_NOP,					//Halt
		OPCODE429_MSG,					//Message
		OPCODE429_GAP,					//Gap
		OPCODE429_LIST,					//Conditional Gap - do list buffer
		OPCODE429_HALT,					//Halt
		OPCODE429_USER,					//User code
		OPCODE429_PULSE0,				//Clear DOUT
		OPCODE429_PULSE1,				//Set DOUT
		OPCODE429_PAUSE,				//Pause
		OPCODE429_RESTART,				//Restart
		OPCODE429_LOGCOND,				//Log event on condition
		OPCODE429_CALLCOND,				//Call conditional
		OPCODE429_RETURN,				//Return from call
		OPCODE429_BRANCHCOND			//Branch on condition
	};

	if ((cmdblk[0] & 0xFF) > OPCODE429_BRANCHCOND) return;

	switch(cmdblk[0] & 0xFF)
	{
		case OPCODE429_NOP:            consoleprintf("%-14s ","NOP");         break;	//Nothing
		case OPCODE429_MSG:            consoleprintf("%-14s ","MSG");         break;	//Message
		case OPCODE429_GAP:            consoleprintf("%-14s ","GAP");         break;	//Gap
		case OPCODE429_LIST:           consoleprintf("%-14s ","LIST");        break;	//List
		case OPCODE429_HALT:           consoleprintf("%-14s ","HALT");        break;	//Halt
		case OPCODE429_USER:           consoleprintf("%-14s ","USER");        break;	//Pause
		case OPCODE429_PULSE0:         consoleprintf("%-14s ","PULSE0");      break;	//Clear DIO
		case OPCODE429_PULSE1:         consoleprintf("%-14s ","PULSE1");      break;	//Set DIO
		case OPCODE429_PAUSE:          consoleprintf("%-14s ","PAUSE");       break;	//Pause
		case OPCODE429_RESTART:        consoleprintf("%-14s ","RESTART");     break;	//Restart
		case OPCODE429_LOGCOND:        consoleprintf("%-14s ","LOGCOND");     break;	//Event log conditional
		case OPCODE429_CALLCOND:       consoleprintf("%-14s ","CALLCOND");    break;	//Call conditional
		case OPCODE429_RETURN:         consoleprintf("%-14s ","RETURN");      break;	//Return from call
		case OPCODE429_BRANCHCOND:     consoleprintf("%-14s ","BRANCHCOND");  break;	//Branch conditional
			break;
	}

	if (cmdblk[0] & 0x4000) consoleprintf("			 ");
	if (cmdblk[0] & 0x2000) consoleprintf("SHOT ");

	switch(cmdblk[0] & 0xFF)
	{
		case OPCODE429_MSG:
		{
			consoleprintf("msgaddr=%04X%04X gap=%04X",cmdblk[1],cmdblk[2],cmdblk[3]);
			msgaddr = BTICard_MakeLong(cmdblk[1],cmdblk[2]);
			BTI429_MsgBlockRd(&msgfields,msgaddr,handleval);
			consoleprintf(" data=%08X opt=%04X",msgfields.msgdata,msgfields.msgopt);
			break;
		}
		case OPCODE429_GAP:        consoleprintf("gap=%04X",cmdblk[1]); break;
		case OPCODE429_LIST:       consoleprintf("gap=%04X listaddr=%04X%04X ",cmdblk[1],cmdblk[6],cmdblk[7]); break;
		case OPCODE429_CALLCOND:   consoleprintf("dest=%04X ",cmdblk[7]); break;
		case OPCODE429_BRANCHCOND: consoleprintf("dest=%04X ",cmdblk[7]); break;
	}

	consoleprintf("\n");
}

/**
*
*  This helper function loops through all command blocks
*  in a channels schedule and sends them to the
*  BTI429_SchedDump(...) function to be decoded
*
**/

VOID BTI429_SchedDumpAll(INT channum,HCARD handleval)
{
	enum {CMDSIZE=8};			//Size of a command block

	INT j;
	UINT16 cmdblk[CMDSIZE];

	INT cmdcount = BTI429_CmdCountRd(channum,handleval);

	consoleprintf("\n");

	for (j=0;j<=cmdcount;++j)
	{
		BTI429_CmdRd(cmdblk,BTI429_CmdAddr(j,channum,handleval),handleval);
		BTI429_SchedDump(cmdblk,handleval);
	}
}

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

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP15  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2009  Ballard Technology, Inc.  Everett, WA, USA.       *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI429 Example 15                                                 *");
	consoleprintf("\n    *  \"Print a transmit schedule\"                                       *");
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
*  Output the results of the Schedule Build operation
*
**/

	consoleprintf("\nARINC 429 schedule for channel #%d",xmtchan);
	consoleprintf("\n");
	consoleprintf("\nBEGIN");

	BTI429_SchedDumpAll(xmtchan,hCore);

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
