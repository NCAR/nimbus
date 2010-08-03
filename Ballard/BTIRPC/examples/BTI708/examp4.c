
/**
*
*  BTI708 DRIVER EXAMPLE 4  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP4.C -- Record to file example
*
**/

/**
*
*  This program opens Card Number 0, and configures the
*  first compatible core to configure the first receive
*  channel to record it's data to the Sequential Monitor.
*  The program then writes the data to a file (EXAMP4.DAT).
*
*  An Event Log frequency of 0 is specifed, which will
*  not generate any Event Log List entries for thess channels.
*
*  The program will continue to record data to the file
*  until a key is pressed.
*
*  Note: Running this program for a long time could cause the file
*  being written to become very large.  Please be cautious while
*  running this example.
*
**/

#include "BTICard.h"
#include "BTI708.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

char filename[256] = "examp4.dat";

/**
*
*  This helper function checks for the help option on the
*  command line.
*
**/

int isarghelp(int argc,char *argv[])
{
	int j;

	for (j=0;j<argc;++j)
	{
		if (!strcmp(argv[j],"?") || !strcmp(argv[j],"-?") || !strcmp(argv[j],"/?"))
		{
			return(1);
		}
	}
	return(0);
}

/**
*
*  This helper function checks for options on the
*  command line.
*
**/

char *isargopt(int argc,char *argv[],const char *opts)
{
	UINT16 j;

	for (j=0;j<argc;++j)
	{
		if ((*argv[j]=='-' || *argv[j]=='/') && !strncmp(opts,argv[j]+1,strlen(opts)))
		{
			return(argv[j]+strlen(opts)+1);
		}
	}
	return(NULL);
}

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

#define consoleprintf printf

int main(int argc,char *argv[])
{
	HCARD  hCard;
	HCORE  hCore;
	ERRVAL errval;
	INT    corenum;
	INT    rcvchan=0;
	FILE   *fh;
	char   *opts;
	INT    cursor = 1;

	UINT16 seqbuf[2048];
	UINT32 seqcount;
	UINT32 blkcnt;
	LPSEQRECORD708 pRec708;
	SEQFINDINFO sfinfo;
	INT		ch;

//	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP4  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI708 Example 4                                                  *");
	consoleprintf("\n    *  \"Record to file example.\"                                         *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n");

/**
*
*  Check if we should display the help information.
*
**/

	if (isarghelp(argc,argv))
	{
		consoleprintf("\nUsage:  EXAMP4 [options]");
		consoleprintf("\n");

		consoleprintf("\n-?                    Display help information");
		consoleprintf("\n-FILENAME:<filename>  File to write data to.  Default EXAMP4.DAT.");
		consoleprintf("\n");
		consoleexit(0);
	}

	consoleprintf("\nFor help:  EXAMP4 ?");
	consoleprintf("\n");

/**
*
*  Check for the -FILENAME:<filename> option.
*
**/

	if ((opts = isargopt(argc,argv,"FILENAME")) != NULL)
	{
		if (*opts++ != ':')
		{
			consoleprintf("\nError:  Semicolon is missing after option."
							"\n        Use the form -FILENAME:<filename>.");
			consoleprintf( "\n");
			consoleexit(1);
		}

		if (!strlen(opts))
		{
			consoleprintf("\nError:  Filename is missing after option."
							"\n        Use the form -FILENAME:<filename>.");
			consoleprintf("\n");
			consoleexit(1);
		}

		strcpy(filename,opts);

		consoleprintf("\nUsing the filename %s.",filename);
	}

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
*  Find the first ARINC 708 core with a receive channel.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (rcvchan=0;rcvchan<MAX_CHANNELS;rcvchan++)	if (BTI708_ChIsRcv(rcvchan,hCore)) break;

		if (rcvchan != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || rcvchan == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No ARINC 708 receive channels present in cardnum %d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing ARINC 708 core #%d",corenum);
	consoleprintf("\nUsing receive channel #%d",rcvchan);
	consoleprintf("\n");

/**
*
*  Configure receive channel.
*
**/

	BTICard_CardReset(hCore);

//	errval = BTI708_ChConfig(CHCFG708_SEQALL | CHCFG708_TERMON | CHCFG708_VARBIT  ,rcvchan,hCore);
	errval = BTI708_ChConfig(CHCFG708_SEQALL | CHCFG708_TERMOFF | CHCFG708_VARBIT | CHCFG708_BITSYNC,rcvchan,hCore);

	if (errval)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        channel #%d on core #%d.",rcvchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Build the receive channel messages.
*
**/

	errval = BTI708_RcvBuild(MSGCRT708_DEFAULT,0,1,rcvchan,hCore);

	if (errval)
	{
		consoleprintf("\nError:  An error was encountered (%i) while building",errval);
		consoleprintf("\n        channel #%d on core #%d.",rcvchan,corenum);
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

	errval = BTICard_SeqConfig(SEQCFG_DEFAULT,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configureing",errval);
		consoleprintf("\n        the sequential monitor on core #%d.",corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Open the data file for writing.
*
**/

	fh = fopen(filename,"wb");

	if (fh==NULL)
	{
		consoleprintf("\nError:  Unable to open the data file %s.",filename);
		consoleprintf("\n");
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
*  Loop until a key is hit.
*
**/

	consoleprintf("\nPress any key to exit...");
	consoleprintf("\n");
	consoleprintf("\nWriting...  ");

	while (1)
	{
		ch = getch();
		if (ch != ERR) break;

		seqcount = BTICard_SeqBlkRd(seqbuf,sizeof(seqbuf)/sizeof(seqbuf[0]),&blkcnt,hCore);

		BTICard_SeqFindInit(seqbuf,seqcount,&sfinfo);

		while(!BTICard_SeqFindNext708(&pRec708,&sfinfo))
		{
			if      (cursor==1) consoleprintf("\b|");
			else if (cursor==2) consoleprintf("\b/");
			else if (cursor==3) consoleprintf("\b-");
			else
			{
				consoleprintf("\b\\");
				cursor = 0;
			}
			cursor++;

			fwrite(pRec708->data,1,sizeof(pRec708->data),fh);
		}
	}

	consoleprintf("\bDone.\n\n");

/**
*
*  Close the data file.
*
**/

	fclose(fh);

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
/*
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
*/
void consoleexit(INT value)
{
//	consoleprintf("\n");
//	endwin();
	exit(value);
}
/*
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
*/
