
/**
*
*  BTICard DRIVER EXAMPLE 3  (10/23/2009)
*  Copyright (c) 2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP3.C -- Monitoring DIO
*
**/

/**
*
*  This example configures Card Number 0 to record DIO in
*  the sequential monitor.  This program monitors rising
*  edges on DIN lines 1,2,3,4,5,6 and the first DOUT line
*  on the core.  The DOUT signal is toggled each time the
*  user presses the spacebar.  The contents of the
*  sequential monitor is output to the screen.
*
*  The card number and DOUT signal line number can be
*  specified on the command line.
*
**/

#include <BTICard.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define	CARDNUM 0
#define CORENUM 0

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

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

/**
*
*  This helper function determines the first DOUT number by
*  looking at the board information.
*
**/

INT BTICard_GetFirstDOUTNum(HCARD hCard)
{
	UINT32 ulprod;

	ulprod = BTICard_CardGetInfo(INFOTYPE_PLAT, 0, hCard);

	switch (ulprod)
	{
		case 0x4242:	/*BB*/	return 3;
		case 0x4734:	/*G4*/	return 5;
		case 0x4735:	/*G5*/	return 9;
	}

	return -1;
}

int main(int argc,char *argv[])
{
	HCARD  hCard;					/*Handle to card*/
	HCORE  hCore;					/*Handle to core*/
	ERRVAL errval;					/*Error return value*/
	INT    cardnum;
	INT    doutnum;

	UINT16 rise;
	BOOL   done;
	INT    val=0;
	char   ch;
	char   *opts;

	UINT16 seqbuf[2048];
	UINT32 seqcount;
	UINT32 blkcnt;
	LPSEQRECORDDIO pRecDIO;
	SEQFINDINFO sfinfo;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP3  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2009  Ballard Technology, Inc.  Everett, WA, USA.       *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTICard Example 3                                                 *");
	consoleprintf("\n    *  \"Monitoring DIO\"                                                  *");
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
		consoleprintf("\nUsage:  EXAMP3 [options]");
		consoleprintf("\n");

		consoleprintf("\n-?                    Display help information");
		consoleprintf("\n-CARDNUM:dd           Set the card number.  Default 0.");
		consoleprintf("\n-DOUTNUM:dd	        Set the DOUT number.  Default 9.");
		consoleprintf("\n");
		exit(0);
	}

	consoleprintf("\nFor help:  EXAMP3 ?");
	consoleprintf("\n");

/**
*
*  Check for the -CARDNUM:dd option.
*
**/

	cardnum = CARDNUM;

	if ((opts = isargopt(argc,argv,"CARDNUM")) != NULL)
	{
		if (*opts++ != ':')
		{
			consoleprintf("\nError:  Semicolon is missing after option."
							"\n        Use the form -CARDNUM:<number>.");
			consoleprintf("\n");
			consoleexit(1);
		}

		if (!strlen(opts))
		{
			consoleprintf("\nError:  The card number is missing after the option."
							"\n        Use the form -CARDNUM:<number>.");
			consoleprintf("\n");
			consoleexit(1);
		}

		if (sscanf(opts,"%u",&cardnum)!=1)
		{
			consoleprintf("\nError:  The card number is not a decimal number."
							"\n        Use the form -CARDNUM:dd.");
			consoleprintf("\n");
			consoleexit(1);
		}
	}

	consoleprintf("\nUsing BTICard card #%d",cardnum);

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

	errval = BTICard_CoreOpen(&hCore, CORENUM, hCard);

	if (errval)
	{
		consoleprintf("\nError:  Either core number %u is not present, or",CORENUM);
		consoleprintf("\n        an error occurred (%i) opening the core.",errval);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		consoleexit(1);
	}
	consoleprintf("\nUsing Core #%d",CORENUM);

/**
*
*  Check for the -DOUTNUM:dd option.
*
**/

	doutnum = BTICard_GetFirstDOUTNum(hCard);

	if ((opts = isargopt(argc,argv,"DOUTNUM")) != NULL)
	{
		if (*opts++ != ':')
		{
			consoleprintf("\nError:  Semicolon is missing after option."
							"\n        Use the form -DOUTNUM:<number>.");
			consoleprintf("\n");
			consoleexit(1);
		}

		if (!strlen(opts))
		{
			consoleprintf("\nError:  The DOUT number is missing after the option."
							"\n        Use the form -DOUTNUM:<number>.");
			consoleprintf("\n");
			consoleexit(1);
		}

		if (sscanf(opts,"%u",&cardnum)!=1)
		{
			consoleprintf("\nError:  The DOUT number is not a decimal number."
							"\n        Use the form -DOUTNUM:dd.");
			consoleprintf("\n");
			consoleexit(1);
		}
	}

	if (doutnum <= 0 || doutnum > 16)
	{
		consoleprintf("\nError:  Unable to determine the DOUT number.");
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}
	consoleprintf("\nUsing DOUT number #%d",doutnum);

/**
*
*  Configure the DIO for Monitor.
*
**/

	//Monitor rising edges on DIN lines 1,2,3,4,5,6 and DOUT line doutnum
	rise = 0x0001 + 0x0002 + 0x0004 + 0x0008 + 0x0010 + 0x0020;
	rise = rise + (1 << (doutnum - 1));

	errval = BTICard_ExtDIOMonConfig(rise,0,0,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        the DIO for monitor on core #%d.",CORENUM);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
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
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        the sequential monitor on core #%d.",CORENUM);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
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

	consoleprintf("\nPress ESC to exit, <SPACE> to toggle DOUT and LED....\n\n");
	consoleprintf("\n\n");

	done = 0;

	while (!done)
	{
		//Read keyboard
		ch = getch();	/* get the last key pressed */
		switch(ch)
		{
		case 0x1B:	/* ESC */
			done = 1;	/* We are done */
			break;

/***
*
*	On command, toggle the value of the selected DOUT and of the board LED.
*	These are done through native API functions. The LED is lockstepped to
*	the DOUT line.
*
***/
		case ' ':	/* <SPACE> */
			val = val ? 0 : 1;											/* invert output value */
			BTICard_ExtDIOWr(doutnum,val,hCore);						/* write value to DOUT */
			BTICard_ExtLEDWr(val, hCore);								/* write value to LED */
			break;
		}

		//Read the sequential monitor buffer
		seqcount = BTICard_SeqBlkRd(seqbuf,sizeof(seqbuf)/sizeof(seqbuf[0]),&blkcnt,hCore);

		if (!seqcount) continue;

		BTICard_SeqFindInit(seqbuf,seqcount,&sfinfo);

		while(!BTICard_SeqFindNextDIO(&pRecDIO,&sfinfo))
		{
			consoleprintf("Time Stamp: %08lX\n",pRecDIO->timestamp);

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
