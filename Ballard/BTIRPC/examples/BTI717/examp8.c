
/**
*
*  BTI717 DRIVER EXAMPLE 8  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP8.C -- IRIG timer example
*
**/

/**
*
*  This program opens Card Number 0, and configures the
*  first BiPhase channel as a receiver.  The second
*  BiPhase channel is configured as a transmitter.  Both
*  channels operate on the self-test bus to electrically connect
*  them. The transmit channel is loaded with incrementing data,
*  and various words are configured for entry into the
*  Sequential Monitor.  The program will read out of the
*  Sequential Record and display the data to the screen using
*  the IRIG timer.
*
**/

#include "BTICard.h"
#include "BTI717.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

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
	INT wps = 256;
	BOOL timetagIsBCD;
	SUPERFRMADDR superfrmaddr;
	UINT16 sfblock[8200];
	INT j,jj,ch;

	UINT16 seqbuf[2048];
	UINT32 seqcount;
	UINT32 blkcnt;
	LPSEQRECORD717 pRec717;
	SEQFINDINFO sfinfo;
	BTIIRIGTIME  irigtime;
	struct tm    *curtime;
	time_t       long_time;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP8  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI717 Example 8                                                  *");
	consoleprintf("\n    *  \"Program for IRIG timer example.\"                                 *");
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

	consoleprintf("\nUsing ARINC 717 card #%d",CARDNUM);

/**
*
*  Find the first ARINC 717 core with both BiPhase receive and transmit channels.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		// Search for receive
		for (rcvchan=0;rcvchan<MAX_CHANNELS;rcvchan++)
		{
			if (BTI717_ChIsType(CHCFG717_BIPHASERCV, rcvchan, hCore))	break;
		}

		// Search for transmit
		for (xmtchan=0;xmtchan<MAX_CHANNELS;xmtchan++)
		{
			if ( xmtchan == rcvchan ) continue;

			if (BTI717_ChIsType(CHCFG717_BIPHASEXMT, xmtchan, hCore))	break;
		}

		if ( (rcvchan != MAX_CHANNELS) && (xmtchan != MAX_CHANNELS) ) break;
	}

	if (errval || corenum == MAX_CORES || rcvchan == MAX_CHANNELS || xmtchan == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No ARINC 717 BiPhase receive and transmit channel pair present in cardnum %d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing ARINC 717 core #%d",corenum);
	consoleprintf("\nUsing BiPhase receive channel #%d",rcvchan);
	consoleprintf("\nUsing BiPhase transmit channel #%d",xmtchan);
	consoleprintf("\n");

/**
*
*  Configure receiver channel for auto speed detection.
*  Use the internal wrap around.
*
**/

	BTICard_CardReset(hCore);

	errval = BTI717_ChConfig(CHCFG717_BIPHASERCV | CHCFG717_AUTOSPEED | CHCFG717_SELFTEST,rcvchan,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring channel %d.",errval,rcvchan);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Configure transmitter channel at specified words per second.
*  Use the internal wrap around.
*
**/

	if      (wps == 64)   errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_SELFTEST | CHCFG717_64WPS,  xmtchan,hCore);
	else if (wps == 128)  errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_SELFTEST | CHCFG717_128WPS, xmtchan,hCore);
	else if (wps == 256)  errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_SELFTEST | CHCFG717_256WPS, xmtchan,hCore);
	else if (wps == 512)  errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_SELFTEST | CHCFG717_512WPS, xmtchan,hCore);
	else if (wps == 1024) errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_SELFTEST | CHCFG717_1024WPS,xmtchan,hCore);
	else if (wps == 2048) errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_SELFTEST | CHCFG717_2048WPS,xmtchan,hCore);
	else if (wps == 4096) errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_SELFTEST | CHCFG717_4096WPS,xmtchan,hCore);
	else if (wps == 8192) errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_SELFTEST | CHCFG717_8192WPS,xmtchan,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring channel %d.",errval,xmtchan);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Init subframe data.
*
**/

	for (j=1;j<=4;j++)
	{
		BTI717_SubFrmBlkRd(sfblock,j,xmtchan,hCore);
		for (jj=2;jj<=wps;jj++) sfblock[jj+7] = ((0x0100*j)+jj) & 0x0FFF;
		BTI717_SubFrmBlkWr(sfblock,j,xmtchan,hCore);
	}

/**
*
*  Create some SuperFrame structures.
*
**/

	BTI717_SuperFrmConfig(10,16,xmtchan,hCore);

/**
*
*  Define SF2, W10 to be SuperFrame Data.
*  Init superframe data.
*
**/

	superfrmaddr = BTI717_SuperFrmDefine(2,10,xmtchan,hCore);
	for (j=1;j<=16;j++) BTI717_SuperFrmWordWr((UINT16)(0x0500+j),j,superfrmaddr,xmtchan,hCore);

/**
*
*  Set superframe counter position.
*
**/

	BTI717_SuperFrmCounterPos(1,wps,rcvchan,hCore);
	BTI717_SuperFrmCounterPos(1,wps,xmtchan,hCore);

/**
*
*  Configure the Sequential Monitor.
*
**/

	errval = BTICard_SeqConfig(SEQCFG_DEFAULT,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring the Sequential Monitor.",errval);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Configure word options.
*
**/

	//Mark word 1 of SFs 1, 2, 3, and 4 for the Sequential Monitor

	BTI717_SubFrmWordConfig(WRDCFG717_SEQ,1,1,rcvchan,hCore);
	BTI717_SubFrmWordConfig(WRDCFG717_SEQ,2,1,rcvchan,hCore);
	BTI717_SubFrmWordConfig(WRDCFG717_SEQ,3,1,rcvchan,hCore);
	BTI717_SubFrmWordConfig(WRDCFG717_SEQ,4,1,rcvchan,hCore);

	BTI717_SubFrmWordConfig(WRDCFG717_SEQ,1,1,xmtchan,hCore);
	BTI717_SubFrmWordConfig(WRDCFG717_SEQ,2,1,xmtchan,hCore);
	BTI717_SubFrmWordConfig(WRDCFG717_SEQ,3,1,xmtchan,hCore);
	BTI717_SubFrmWordConfig(WRDCFG717_SEQ,4,1,xmtchan,hCore);

	//Mark the word at SPF 4 of our superframe word (word 10, SF 2)
	//for the Sequential Monitor

	BTI717_SuperFrmWordConfig(WRDCFG717_SEQ,4,superfrmaddr,xmtchan,hCore);

/**
*
*  Configure the IRIG timer. (DEFAULT -> bit rate = IRIGB, internal, slave/receiver)
*  Sequential monitor will store IRIG time in sequential records
*
**/

	errval = BTICard_IRIGConfig(IRIGCFG_DEFAULT,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        the IRIG timer on core #%d.",corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	timetagIsBCD = (TIMETAG_FORMAT_BCD == BTICard_TimerStatus(hCore)) ? TRUE : FALSE;

/**
*
*  Synchronize the on core IRIG timer with the computer local time
*
**/

	time( &long_time );								/* Get time as long integer. */
	curtime = localtime( &long_time );				/* Convert to local time. */

	irigtime.days  = (UINT16)curtime->tm_yday;		/* Day of year (0 - 365; January 1 = 0) */
	irigtime.hours = (UINT16)curtime->tm_hour;		/* Hours after midnight (0 - 23) */
	irigtime.min   = (UINT16)curtime->tm_min;		/* Minutes after hour (0 - 59) */
	irigtime.sec   = (UINT16)curtime->tm_sec;		/* Seconds after minute (0 - 59) */
	irigtime.msec  = 0;								/* The tm structure does not include milliseconds. */
	irigtime.usec  = 0;								/* The tm structure does not include microseconds. */

	errval = BTICard_IRIGWr(&irigtime,hCore);
	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while writting",errval);
		consoleprintf("\n        the IRIG time on core #%d.",corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Start operation of the card.
*
**/

	BTICard_CardStart(hCore);

/**
*
*  Read the Sequential Record and dsiplay to screen.
*
**/

	consoleprintf("\nPress any key to exit....\n\n");

	while(1)
	{
		ch = getch();
		if (ch != ERR) break;

		seqcount = BTICard_SeqBlkRd(seqbuf,sizeof(seqbuf)/sizeof(seqbuf[0]),&blkcnt,hCore);

		BTICard_SeqFindInit(seqbuf,seqcount,&sfinfo);

		while(!BTICard_SeqFindNext717(&pRec717,&sfinfo))
		{

			if (!timetagIsBCD)
			{
				//Convert binary timetag to BCD
				BTICard_IRIGTimeBinToBCD(&(pRec717->timestamph),
										 &(pRec717->timestamp),
										 pRec717->timestamph,
										 pRec717->timestamp);
			}

			consoleprintf("\nCh:%-2d  Word:%-2d  SF:%-2d  SPF:%-2d  Value:%04o (Octal)",
				(pRec717->activity & MSGACT717_CHMASK) >> MSGACT717_CHSHIFT,
				pRec717->wordnum,
				pRec717->subframe,
				(pRec717->superframe % 16) + 1,
				pRec717->data);

			consoleprintf("  TimeStamp:%02u:%02u %02u.%03u.%03u",
				BTICard_IRIGFieldGetHours(pRec717->timestamph,pRec717->timestamp),
				BTICard_IRIGFieldGetMin(pRec717->timestamph,pRec717->timestamp),
				BTICard_IRIGFieldGetSec(pRec717->timestamph,pRec717->timestamp),
				BTICard_IRIGFieldGetMillisec(pRec717->timestamph,pRec717->timestamp),
				BTICard_IRIGFieldGetMicrosec(pRec717->timestamph,pRec717->timestamp));
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
