
/**
*
*  BTI429 DRIVER EXAMPLE 10 (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP10.C -- Multiple Core Example
*
**/

/**
*
*  This program opens Card Number 0 and configures all
*  ARINC 429 cores to transmit, receive, and monitor.
*  All transmit channels and all receive channels are
*  used. It also enables the Sequential Monitor to record
*  transmitted and received messages.
*
**/

#include "BTICard.h"
#include "BTI429.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

#ifndef	TRUE
#define	TRUE 1
#endif

#ifndef	FALSE
#define	FALSE 0
#endif

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT value);

int main(void)
{
	HCARD   hCard;                        /*Handle to card*/
	HCORE   hCores[MAX_CORES] = {0};      /*Handle to cores*/
	BOOL    b429Core[MAX_CORES] = {0};    /*Flag indicating 717 core*/
	INT     corecount, corenum, chan;
	ERRVAL  errval;

	MSGSTRUCT429 xmt_airspeed;
	UINT16 seqbuf[2048];
	UINT32 seqcount;
	UINT32 blkcnt;
	LPSEQRECORD429 pRec429;
	SEQFINDINFO sfinfo;
	INT ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP10  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI429 Example 10                                                 *");
	consoleprintf("\n    *  \"Program for multiple core example.\"                              *");
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
*  Find all ARINC 429 cores on the board
*
**/

	corecount = 0;

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		b429Core[corenum] = FALSE;
		errval = BTICard_CoreOpen(&hCores[corenum],corenum,hCard);
		if (errval) break;

		for (chan=0;chan<MAX_CHANNELS;chan++)
		{
			if ( BTI429_ChIsRcv(chan,hCores[corenum]) ||
				 BTI429_ChIsXmt(chan,hCores[corenum]) )
			{
				b429Core[corenum] = TRUE;
				corecount++;
				break;
			}
		}
	}

	if (!corecount)
	{
		consoleprintf("\nError:  Unable to find any ARINC 429 cores on card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing %d ARINC 429 core(s) on card #%d",corecount,CARDNUM);
	consoleprintf("\n");

/**
*
*  Configure all transmit channels on all cores to transmit at low speed.
*  Configure all receive channels on all cores for automatic speed detection.
*  Configure the Sequential Monitor for each core.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		if (b429Core[corenum])
		{
			BTICard_CardReset(hCores[corenum]);			/* Reset the core. */

			for (chan=0;chan<MAX_CHANNELS;chan++)
			{
				if (BTI429_ChIsXmt(chan,hCores[corenum]))
				{
					errval = BTI429_ChConfig(CHCFG429_SEQALL,chan,hCores[corenum]);	/*Configure channel*/

					if (errval < 0)
					{
						consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
						consoleprintf("\n        channel #%d on core #%d.",chan,corenum);
						consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
						BTICard_CardClose(hCard);
						consoleexit(1);
					}

					/* Create a message for each of the synchronous labels we are transmitting. */
					xmt_airspeed.addr = BTI429_MsgCreate(MSGCRT429_DEFAULT,hCores[corenum]);

					/* Schedule the message */
					BTI429_SchedMsg(xmt_airspeed.addr,chan,hCores[corenum]);
					BTI429_SchedGap(1218,chan,hCores[corenum]);

					/* Initialize the Message Record. */
					xmt_airspeed.data = BTI429_FldPutLabel(0L,0206);   /*Init label field*/
					BTI429_MsgDataWr(xmt_airspeed.data,xmt_airspeed.addr,hCores[corenum]);
				}
				else if (BTI429_ChIsRcv(chan,hCores[corenum]))
				{
					errval = BTI429_ChConfig(CHCFG429_SEQALL | CHCFG429_AUTOSPEED,chan,hCores[corenum]);   /*Configure channel*/

					if (errval < 0)
					{
						consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
						consoleprintf("\n        channel #%d on core #%d.",chan,corenum);
						consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
						BTICard_CardClose(hCard);
						consoleexit(1);
					}

					/* Create a default filter */
					BTI429_FilterDefault(MSGCRT429_DEFAULT,chan,hCores[corenum]);
				}
			}

			/* Configure the Sequential Monitor on each core for continuous mode. */
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
	}

/**
*
*  Start operation of the card.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		if (b429Core[corenum])	BTICard_CardStart(hCores[corenum]);
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

			while(!BTICard_SeqFindNext429(&pRec429,&sfinfo))
			{
				consoleprintf("\nCore:%d Ch:%02u Data:%08lX Time Stamp:%08lX",
						corenum,
						((pRec429->activity & MSGACT429_CHMASK) >> MSGACT429_CHSHIFT),
						pRec429->data,
						pRec429->timestamp);
			}
		}
	}

/**
*
*  Stop the card.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
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
