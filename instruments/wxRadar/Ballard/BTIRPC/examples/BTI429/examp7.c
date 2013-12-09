
/**
*
*  BTI429 DRIVER EXAMPLE 7  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP7.C -- File transfer example
*
**/

/**
*
*  This program opens Card Number 0, finds the first
*  compatible core, and configures an asynchronous transmit
*  list buffer.  The program reads a data file and transmits
*  that data with a user-specified label.  If no data file
*  or label is specified by the user, the default values are
*  examp7.dat, and label 0172.
*
**/

#include "BTICard.h"
#include "BTI429.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

char filename[256] = "EXAMP7.DAT";
UINT32 label = 0172;

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

int main(int argc,char *argv[])
{
	HCARD hCard;
	HCORE hCore;
	ERRVAL errval;
	INT corenum;
	INT xmtchan=0;
	FILE *fh;
	char *opts;

	LISTADDR asynclistaddr;
	UINT32 Data;
	INT ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP7  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI429 Example 7                                                  *");
	consoleprintf("\n    *  \"Program for file transfer.\"                                      *");
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
		consoleprintf("\nUsage:  EXAMP7 [options]");
		consoleprintf("\n");

		consoleprintf("\n-?                    Display help information");
		consoleprintf("\n-LABEL:ooo            Set the label (octal).  Default 0172.");
		consoleprintf("\n-FILENAME:<filename>  File to read data from. Default EXAMP7.DAT.");
		consoleprintf("\n");
		consoleexit(0);
	}

	consoleprintf("\nFor help:  EXAMP7 ?");
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
*  Check for the -FILENAME:<filename> option.
*
**/

	if ((opts = isargopt(argc,argv,"FILENAME")) != NULL)
	{
		if (*opts++ != ':')
		{
			consoleprintf(	"\nError:  Semicolon is missing after option."
							"\n        Use the form -FILENAME:<filename>.");
			consoleprintf( "\n");
			consoleexit(1);
		}

		if (!strlen(opts))
		{
			consoleprintf( "\nError:  Filename is missing after option."
							"\n        Use the form -FILENAME:<filename>.");
			consoleprintf( "\n");
			consoleexit(1);
		}

		strcpy(filename,opts);

		consoleprintf("\nUsing the filename %s.",filename);
	}

/**
*
*  Check for the -LABEL:ooo option.
*
**/

	if ((opts = isargopt(argc,argv,"LABEL")) != NULL)
	{
		if (*opts++ != ':')
		{
			consoleprintf(	"\nError:  Semicolon is missing after option."
							"\n        Use the form -LABEL:<label#>.");
			consoleprintf( "\n");
			consoleexit(1);
		}

		if (!strlen(opts))
		{
			consoleprintf(	"\nError:  The label number is missing after the option."
							"\n        Use the form -LABEL:<label#>.");
			consoleprintf( "\n");
			consoleexit(1);
		}

		if (sscanf(opts,"%o",&label)!=1)
		{
			consoleprintf(	"\nError:  The label number is not an octal number."
							"\n        Use the form -LABEL:ooo.");
			consoleprintf( "\n");
			consoleexit(1);
		}

		consoleprintf("\nUsing label %03o.",label);
	}

/**
*
*  Find the first ARINC 429 core with a transmit channel.
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
*  Configure xmtchan to transmit at low speed.
*
**/

	BTICard_CardReset(hCore);                          /*Reset the card*/

	errval = BTI429_ChConfig(CHCFG429_DEFAULT,xmtchan,hCore);

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
*  Create the asynchronous list buffer.
*
**/

	asynclistaddr = BTI429_ListAsyncCreate(LISTCRT429_FIFO,8192,xmtchan,hCore);

	if (!asynclistaddr)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        the list buffer on core #%d.",corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Next, a schedule is built by creating explicit message timing.
*  While creating a schedule we can still use the SchedGap() function
*  to create our gap times.  This function will determine that an
*  asynchronous list buffer has been created for the channel, and will
*  internally call the SchedGapList() function with appropriate parameters.
*  Alternatively, we could use the SchedGapList() function directly
*  and specify the address value ourselves. This method is useful if
*  more than one asynchronous list buffer is desired per channel.
*
**/

	BTI429_SchedGap(1024,xmtchan,hCore);                            //This line, and
//	BTI429_SchedGapList(1024,asynclistaddr,xmtchan,hCore);          //this line, do the same thing.

/**
*
*  Start operation of the card.
*
**/

	BTICard_CardStart(hCore);

/**
*
*  Open the data file for reading.
*
**/

	fh = fopen(filename,"rb");

	if (fh==NULL)
	{
		consoleprintf("\nError:  Unable to open the data file %s.",filename);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Transmit data until EOF or until a key is hit.
*
**/

	consoleprintf("\nPress any key to exit....\n\n");

	while (1)
	{
		ch = getch();
		if (ch != ERR) break;
		if (fread(&Data,1,sizeof(Data),fh) == 0)
		{
			if (ferror(fh) != 0) consoleprintf("fread encountered an error...\n");
			break;
		}

		Data = (Data & 0xFFFFFF00L) | (label & 0x000000FFL);
		while(!BTI429_ListDataWr(Data,asynclistaddr,hCore));
	}

	consoleprintf("\nFinished loading data on to the card.");
	consoleprintf("\nCard may still be transmitting.");
	consoleprintf("\n");
	consoleprintf("\n");

/**
*
*  Close the data file.
*
**/

	fclose(fh);

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
