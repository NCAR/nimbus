
/**
*
*  BTICSDB DRIVER EXAMPLE 1  (10/23/2009)
*  Copyright (c) 2005-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP1.C -- Transmitter example
*
**/

/**
*
*  This program opens Card Number 0; and configures the
*  first compatible core to build a transmit schedule
*  using transmit frame rate information, and then transmit
*  the messages. The user triggers burst messages.
*
**/

#include "BTICard.h"
#include "BTICSDB.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

#define BLOCKSPERFRAME  18
#define BYTESPERBLOCK    6
#define FRAMESPERSECOND 10

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(int argc, char* argv[])
{
	HCARD hCard;
	HCORE hCore;
	ERRVAL errval;
	INT corenum;
	INT xmtchan = 0;
	MSGADDR msgaddr[3];
	INT freq[3];
	BYTE bufcont[BYTESPERBLOCK];
	BOOL done = 0;
	INT input;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP1  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2005-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTICSDB Example 1                                                 *");
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

	consoleprintf("\nUsing CSDB card #%d",CARDNUM);

/**
*
*  Find the first CSDB core with a transmit channel.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (xmtchan=0;xmtchan<MAX_CHANNELS;xmtchan++)	if (BTICSDB_ChIsXmt(xmtchan,hCore))	break;

		if (xmtchan != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || xmtchan == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No CSDB transmit channels present in card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing CSDB core #%d",corenum);
	consoleprintf("\nUsing transmit channel #%d",xmtchan);
	consoleprintf("\n");

/**
*
*  Configure xmtchan to transmit at low speed.
*
**/

	BTICard_CardReset(hCore);

	errval = BTICSDB_ChConfig(CHCFGCSDB_DEFAULT,BITRATECSDB_LOWSPEED,BYTESPERBLOCK,FRAMESPERSECOND,BLOCKSPERFRAME,xmtchan,hCore);

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
*  Create a message for each of the addresses we are transmitting.
*
**/

	msgaddr[0] = BTICSDB_MsgCreate(MSGCRTCSDB_DEFAULT | MSGCRTCSDB_WIPESYNC,hCore);      /*create sync message*/
	msgaddr[1] = BTICSDB_MsgCreate(MSGCRTCSDB_DEFAULT,hCore);                            /*create continuous message*/
	msgaddr[2] = BTICSDB_MsgCreate(MSGCRTCSDB_DEFAULT | MSGCRTCSDB_BURST,hCore);         /*create burst message*/

/**
*
*  Set up the frame rates needed to build the schedule.
*  freq = (Bus Frames/Second) / (Message Updates/Second)
*
**/

	freq[0] = 1;	/*sync message is every frame*/
	freq[1] = 2;	/*every other frame*/
	freq[2] = 1;	/*every frame*/

/**
*
*  Next, build a schedule using the messages and frequencies
*  specified.
*
**/

	errval = BTICSDB_SchedBuild(3,msgaddr,freq,0,0,xmtchan,hCore);

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
*  Initialize the Message Records.
*
**/

	//continuous message
	bufcont[0] = 0x12;	/*address byte*/
	bufcont[1] = 0x80;	/*status byte*/
	bufcont[2] = 0x00;	/*data = 121.1MHz*/
	bufcont[3] = 0x10;	/* "" */
	bufcont[4] = 0x21;	/* "" */
	bufcont[5] = 0x00;	/*pad*/
	BTICSDB_MsgDataWr(bufcont,BYTESPERBLOCK,msgaddr[1],hCore);


	//burst message
	BTICSDB_MsgDataByteWr(0x31,0,msgaddr[2],hCore);	/*address byte*/
	BTICSDB_MsgDataByteWr(0xB1,1,msgaddr[2],hCore);	/*status byte*/
	BTICSDB_MsgDataByteWr(0x90,3,msgaddr[2],hCore); /*tune = 118.9MHz*/
	BTICSDB_MsgDataByteWr(0x18,4,msgaddr[2],hCore);	/* "" */

/**
*
*  Start operation of the card.
*
**/

	BTICard_CardStart(hCore);

/**
*
*  Trigger burst message when a key is pressed.
*
**/

	consoleprintf("\nPress any key to trigger burst, escape to exit....\n\n");

	while(!done)
	{
		input = getch();
		if (input != ERR)
		{
			if (input == 0x1B)
			{
				done = 1;
			}
			else
			{
				if (BTICSDB_MsgDataByteRd(1,msgaddr[2],hCore) & 0x80)
				{
					BTICSDB_MsgDataByteWr(0x31,1,msgaddr[2],hCore); /*clear valid bit*/
				}
				else
				{
					BTICSDB_MsgDataByteWr(0xB1,1,msgaddr[2],hCore); /*set valid bit*/
				}

				BTICSDB_MsgValidSet(msgaddr[2],hCore);                  /*set message as valid for transmit*/
			}
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
