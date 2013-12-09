
/**
*
*  BTI429 DRIVER EXAMPLE 9 (10/23/2009)
*  Copyright (c) 2003-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP9.C -- Multiple Core Example
*
**/

/**
*
*  This program opens Card Number 0 and configures all
*  ARINC 708 cores to transmit, receive, and monitor.
*  All transmit channels and all receive channels are
*  used. It also enables the sequential monitor to record
*  transmitted and received messages.
*
**/

#include "BTICard.h"
#include "BTI708.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM		  0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

#define	FRAMEFREQ	  200          /*Frame frequency of 200Hz*/
#define LOGFREQ		  16           /*Generate a Log Event after every 16th message*/
#define	FRAMECOUNT	  32           /*Transmit a total of 32 messages*/

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(void)
{
	HCARD hCard;
	HCORE hCores[MAX_CORES];
	INT corecount, corenum, chan;
	ERRVAL errval;

	INT    count = 0;
	UINT32  j,jj;
	UINT16 Data[100];
	UINT16 seqbuf[2048];
	UINT32 seqcount;
	UINT32 blkcnt;
	LPSEQRECORD708 pRec708;
	SEQFINDINFO sfinfo;
	INT ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP9  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2003-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI708 Example 9                                                  *");
	consoleprintf("\n    *  \"Program for multiple cores example.\"                             *");
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

/**
*
*  Find all ARINC 708 cores on the board
*
**/

	corecount = 0;

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		HCORE hCore;
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (chan=0;chan<MAX_CHANNELS;chan++)
		{
			if (BTI708_ChIs708(chan,hCore))	break;
		}

		if (chan!=MAX_CHANNELS)
		{
			hCores[corecount] = hCore;
			corecount++;
			continue;
		}
	}

	if (!corecount)
	{
		consoleprintf("\nError:  Unable to find any ARINC 708 cores on card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing %d ARINC 708 core(s) on card #%d",corecount,CARDNUM);
	consoleprintf("\n");

/**
*
*  Configure all transmit channels on all cores to send 32 messages at 200Hz
*  Configure all receive channels on all cores to receive messages in a single message record per core.
*  Configure each cores sequential record.
*
**/

	for (corenum=0;corenum<corecount;corenum++)
	{
		BTICard_CardReset(hCores[corenum]);			/* Reset the core. */

		for (chan=0;chan<MAX_CHANNELS;chan++)
		{
			if (BTI708_ChIsXmt(chan,hCores[corenum]))
			{
				errval = BTI708_ChConfig(CHCFG708_SEQALL,chan,hCores[corenum]);	/*Configure channel*/
				if (errval < 0)
				{
					consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
					consoleprintf("\n        transmit channel #%d on core #%d.",chan,corenum);
					consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
					BTICard_CardClose(hCard);
					consoleexit(1);
				}

				errval = BTI708_XmtBuild(MSGCRT708_DEFAULT,FRAMEFREQ,LOGFREQ,FRAMECOUNT,chan,hCores[corenum]);
				if (errval)
				{
					consoleprintf("\nError:  An error was encountered (%i) while building",errval);
					consoleprintf("\n        transmit channel #%d on core #%d.",chan,corenum);
					consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
					consoleprintf("\n");
					BTICard_CardClose(hCard);
					consoleexit(1);
				}

				for (j=0;j<FRAMECOUNT;j++)							/*Loop through all the messages*/
				{
					for (jj=0;jj<100;jj++) Data[jj] = j + (count * FRAMECOUNT);
					BTI708_MsgDataWr(Data,j,chan,hCores[corenum]);	/*Write data to the Device*/
				}
				count++;
			}
			else if (BTI708_ChIsRcv(chan,hCores[corenum]))
			{
				errval = BTI708_ChConfig(CHCFG708_DEFAULT,chan,hCores[corenum]);   /*Configure channel*/
				if (errval < 0)
				{
					consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
					consoleprintf("\n        receive channel #%d on core #%d.",chan,corenum);
					consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
					BTICard_CardClose(hCard);
					consoleexit(1);
				}

				errval = BTI708_RcvBuild(MSGCRT708_DEFAULT,0,1,chan,hCores[corenum]);
				if (errval)
				{
					consoleprintf("\nError:  An error was encountered (%i) while building",errval);
					consoleprintf("\n        receive channel #%d on core #.",chan,corenum);
					consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
					consoleprintf("\n");
					BTICard_CardClose(hCard);
					consoleexit(1);
				}
			}
		}

		/* Configure the sequential record on each core for continuous mode. */
		errval = BTICard_SeqConfig(SEQCFG_DEFAULT,hCores[corenum]);

		if (errval < 0)
		{
			consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
			consoleprintf("\n        the sequential record on core #%d.",corenum);
			consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
			BTICard_CardClose(hCard);
			consoleexit(1);
		}
	}

/**
*
*  Start operation of the card.
*
**/

	for (corenum=0;corenum<corecount;corenum++)
	{
		BTICard_CardStart(hCores[corenum]);
	}

/**
*
*  Display data on the screen.
*
**/

	consoleprintf("\nPress any key to exit....\n\n");

	while(1)
	{
		ch = getch();
		if (ch != ERR) break;

		for (corenum=0;corenum<corecount;corenum++)
		{
			seqcount = BTICard_SeqBlkRd(seqbuf,sizeof(seqbuf)/sizeof(seqbuf[0]),&blkcnt,hCores[corenum]);

			BTICard_SeqFindInit(seqbuf,seqcount,&sfinfo);

			while(!BTICard_SeqFindNext708(&pRec708,&sfinfo))
			{
				consoleprintf("\nCore:%d   Ch:%02u   Time Stamp:%08lX",
						corenum,
						((pRec708->activity & MSGACT708_CHMASK) >> MSGACT708_CHSHIFT),
						pRec708->timestamp);
			}
		}
	}

/**
*
*  Stop the card.
*
**/

	for (corenum=0;corenum<corecount;corenum++)
	{
		BTICard_CardStop(hCores[corenum]);
	}

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
