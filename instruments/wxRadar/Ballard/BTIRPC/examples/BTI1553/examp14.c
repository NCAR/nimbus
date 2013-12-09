
/**
*
*  BTI1553 DRIVER EXAMPLE 14  (10/23/2009)
*  Copyright (c) 2003-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP14.C -- Multiple Cores - Monitor all channels on all cores
*
**/

/**
*
*  This example configures all 1553 channels on all cores of Card Number 0
*  as Bus Monitor terminals. It prints the channel number, time-tag, command
*  words (if any), and status words (if any) for each monitor record.
*
**/

#include "BTICard.h"
#include "BTI1553.h"
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
void consoleexit(INT);

int main(void)
{
	HCARD  hCard;                          /*Handle to card*/
	HCORE  hCores[MAX_CORES] = {0};        /*Handle to cores*/
	BOOL   b1553Core[MAX_CORES] = {0};     /*Flag indicating 1553 core*/
	INT    corecount;                      /*Number of cores found*/
	ERRVAL errval;                         /*Error return value*/
	INT    channum=0;                      /*Channel number*/
	INT    ch;

	UINT16 seqbuf[2048];
	UINT32  seqcount;
	UINT32  blkcnt;
	LPSEQRECORD1553 pRec1553;
	SEQFINDINFO sfinfo;
	INT    j;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP14  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2003-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI1553 Example 14                                                *");
	consoleprintf("\n    *  \"Multi Core - Monitor all channels on all cores\"                  *");
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
*  Find all MIL-STD-1553 cores.
*
**/

	corecount = 0;

	for (j=0; j<MAX_CORES; j++)
	{
		b1553Core[j] = FALSE;
		errval = BTICard_CoreOpen(&hCores[j],j,hCard);
		if (errval) break;

		for (channum=0;channum<MAX_CHANNELS;channum++)
		{
			if (BTI1553_ChIs1553(channum,hCores[j]))
			{
				b1553Core[j] = TRUE;
				corecount++;
				break;
			}
		}
	}

	if (0==corecount)
	{
		consoleprintf("\nError:  No MIL-STD-1553 cores present in card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing %d MIL-STD-1553 core(s)",corecount);
	consoleprintf("\n");

/**
*
*  Configure the sequential record on each 1553 core.
*  Configure all 1553 channels for monitor mode.
*
**/

	for (j=0; j<MAX_CORES; j++)
	{
		if (b1553Core[j])
		{
			BTICard_CardReset(hCores[j]);	/*First reset the core*/

			errval = BTICard_SeqConfig(SEQCFG_DEFAULT,hCores[j]);
			if (errval < 0)
			{
				consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
				consoleprintf("\n        the sequential record on core #%d.",j);
				consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
				BTICard_CardClose(hCard);
				consoleexit(1);
			}

			for (channum=0;channum<MAX_CHANNELS;channum++)
			{
				if (BTI1553_ChIs1553(channum,hCores[j]))
				{
					errval = BTI1553_MonConfig(MONCFG1553_DEFAULT,channum,hCores[j]);
					if (errval < 0)
					{
						consoleprintf("\nError:  An error was encountered (%i) while configuring for",errval);
						consoleprintf("\n        monitor mode on channel #%d on core #%d.",channum,j);
						consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
						consoleprintf("\n");
						BTICard_CardClose(hCard);
						consoleexit(1);
					}
				}
			}
		}
	}

/**
*
*  Start each 1553 core to begin
*
**/

	for (j=0; j<MAX_CORES; j++)
	{
		if (b1553Core[j])	BTICard_CardStart(hCores[j]);
	}

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

		for (j=0; j<MAX_CORES; j++)
		{
			if (b1553Core[j])
			{
				seqcount = BTICard_SeqBlkRd(seqbuf,sizeof(seqbuf)/sizeof(seqbuf[0]),&blkcnt,hCores[j]);

				BTICard_SeqFindInit(seqbuf,seqcount,&sfinfo);

				while(!BTICard_SeqFindNext1553(&pRec1553,&sfinfo))
				{
					consoleprintf("\n");

					consoleprintf(" Core:%02d",j);
					consoleprintf(" Ch:%02u",(pRec1553->activity & MSGACT1553_CHMASK) >> MSGACT1553_CHSHIFT);
					consoleprintf(" Time:%lu",pRec1553->timestamp);

					if (pRec1553->activity & MSGACT1553_RCVCWD1) consoleprintf(" Cwd1:%04X",pRec1553->cwd1);
					if (pRec1553->activity & MSGACT1553_RCVCWD2) consoleprintf(" Cwd2:%04X",pRec1553->cwd2);
					if (pRec1553->activity & MSGACT1553_RCVSWD1) consoleprintf(" Swd1:%04X",pRec1553->swd1);
					if (pRec1553->activity & MSGACT1553_RCVSWD2) consoleprintf(" Swd2:%04X",pRec1553->swd2);

					if (pRec1553->error & MSGERR1553_NORESP) consoleprintf(" No response!");
					if (pRec1553->error & MSGERR1553_ANYERR) consoleprintf(" Error!");
				}
			}
		}
	}

/**
*
*  Stop all cores.
*
**/

	for (j=0; j<MAX_CORES; j++)
	{
		BTICard_CardStop(hCores[j]);
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
