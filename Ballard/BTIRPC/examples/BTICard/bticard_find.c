/**
*
*  BTICard FIND EXAMPLE  (10/23/2009)
*  Copyright (c) 2004-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   bticard_find.c -- Finds RPC cards
*
**/

/**
*
*  This example will perform a broadcast on the network and then
*  display information about each BTIDriver-complient RPC card that
*  responds.  If an error is encountered, a message is displayed
*  indicating the type of error that occurred.
*
**/

#include <BTICard.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

#define consoleprintf printf

extern ERRVAL BTIRPC_DiscoverEx(BOOL (*callback)(LPBTIIDENTIFY info,char * ipaddr));

BOOL 	headerdone = FALSE;
BOOL 	shortform  = FALSE;
INT	count      = 0;

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

static BOOL discover_callback(LPBTIIDENTIFY info,char * ipaddr)
{
	if (!headerdone && !shortform)
	{
		consoleprintf(" %-12s %-20s %-6s %-15s %-17s\n",
			"Card Type",
			"Model",
			"Ser. #",
			"IP Address",
			"MAC Address"
			);
		consoleprintf("|%-12s|%-20s|%-6s|%-15s|%-17s|\n",
			"------------",
			"--------------------",
			"------",
			"---------------",
			"-----------------"
			);
		headerdone = TRUE;
	}

	if (!shortform)
	{
		consoleprintf(" %-12s %-20s %-6s %-15s %-17s\n",
			info->type_string,
			info->card_string,
			info->serial_number,
			ipaddr,
			info->mac_address
			);
	}
	else
	{
		consoleprintf("%d,%s\n",
			count++,
			info->mac_address
			);
	}

	return(FALSE);
}

int main(INT argc,LPSTR *argv)
{
	ERRVAL errval;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTICard_FIND (10/23/2009)                                         *");
	consoleprintf("\n    *  Copyright 2004-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n");
	consoleprintf("\n");
	consoleprintf("\n");

	if (isarghelp(argc,argv))
	{
		consoleprintf("\nUsage:  bticard_find [options]");
		consoleprintf("\n");

		consoleprintf("\n-?                      Display help information");
		consoleprintf("\n-SHORT                  Display results in short form.");
		consoleprintf("\n");

		consoleexit(0);
	}

	if(isargopt(argc,argv,"SHORT"))
	{
		shortform = TRUE;
	}

/**
*
*  Send discover broadcast
*
**/

	errval = BTIRPC_DiscoverEx(discover_callback);

	if (errval)
	{
		consoleprintf("\nError:  Discover failed.");
		consoleprintf("\n");
		consoleexit(1);
	}

	consoleexit(0);
	return(0);
}

void consoleentry(void)
{
return;
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
return;
	consoleprintf("\n");
	endwin();
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
