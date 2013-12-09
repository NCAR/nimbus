
/**
*
*  BTI1553 DRIVER EXAMPLE 28  (10/23/2009)
*  Copyright (c) 2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP28.C -- Discovering illegal subaddresses and word counts on an RT
*
**/

/**
*
*  This example configures Card Number 0 as a bus controller
*  to transmit a series of asyncronous commands to every
*  subaddress and word count combination for a given remote
*  terminal. The program will report successful results as
*  they occur and accumulate a sum of total messages, total
*  no response messages, and total mesages with an error.
*
**/

#include <BTICard.h>
#include <BTI1553.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#define	CARDNUM		  0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

#define RTNUM		  1

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

int main(int argc,char *argv[])
{
	HCARD  hCard;				/*Handle to card*/
	HCORE  hCore;				/*Handle to core*/
	ERRVAL errval;				/*Error return value*/
	INT    cardnum;				/*Card number*/
	INT    corenum;				/*Core number*/
	INT    channum=0;			/*Channel number*/

	XMITFIELDS1553 Msg;			/*Message structure*/
	INT    rtnum;
	char   *opts;

	INT i;
	INT sa, tr, wc;

	INT no_resp, msg_count, msg_error;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP28  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2009  Ballard Technology, Inc.  Everett, WA, USA.       *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI1553 Example 28                                                *");
	consoleprintf("\n    *  \"Discovering illegal subaddresses and word counts on an RT\"       *");
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
		consoleprintf("\nUsage:  EXAMP28 [options]");
		consoleprintf("\n");

		consoleprintf("\n-?                    Display help information");
		consoleprintf("\n-CARDNUM:dd           Set the card number. Default 0.");
		consoleprintf("\n-RTNUM:dd	            Set the RT number.   Default 1.");
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
*  Check for the -RTNUM:dd option.
*
**/

	rtnum = RTNUM;

	if ((opts = isargopt(argc,argv,"RTNUM")) != NULL)
	{
		if (*opts++ != ':')
		{
			consoleprintf("\nError:  Semicolon is missing after option."
				   "\n        Use the form -RTNUM:<number>.");
			consoleprintf("\n");
			consoleexit(1);
		}

		if (!strlen(opts))
		{
			consoleprintf("\nError:  The RT number is missing after the option."
				   "\n        Use the form -RTNUM:<number>.");
			consoleprintf("\n");
			consoleexit(1);
		}

		if (sscanf(opts,"%u",&cardnum)!=1)
		{
			consoleprintf("\nError:  The RT number is not a decimal number."
				   "\n        Use the form -RTNUM:dd.");
			consoleprintf("\n");
			consoleexit(1);
		}
	}

	if (rtnum < 0 || rtnum > 31)
	{
		consoleprintf("\nError:  Invalid remote terminal number.");
		consoleprintf("\n");
		consoleexit(1);
	}
	consoleprintf("\nUsing remote terminal number #%d",rtnum);

/**
*
*  Open the card with the specified card number.
*  An error value is returned which is tested
*  to determine if an error occurred.
*
**/

	errval = BTICard_CardOpen(&hCard,cardnum);

	if (errval)
	{
		consoleprintf("\nError:  Either card number %u is not present, or",cardnum);
		consoleprintf("\n        an error occurred (%i) opening the card.",errval);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		consoleexit(1);
	}

/**
*
*  Find the first MIL-STD-1553 core and channel.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (channum=0;channum<MAX_CHANNELS;channum++)
			if (BTI1553_ChIs1553(channum,hCore)) break;

		if (channum != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || channum == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No MIL-STD-1553 channels present in card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing MIL-STD-1553 core #%d",corenum);
	consoleprintf("\nUsing MIL-STD-1553 channel #%d",channum);
	consoleprintf("\n");

/**
*
*  Configure the channel for bus controller mode.
*
**/

	BTICard_CardReset(hCore);									/*Reset the core*/

	errval = BTI1553_BCConfig(BCCFG1553_DEFAULT,channum,hCore);	/*Configure the core*/

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring bus",errval);
		consoleprintf("\n        controller mode on channel #%d on core #%d.",channum,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Create an empty frame in the BC Schedule.
*
*  BTI1553_BCTransmitMsg uses an asynchronous Message Record
*  that is transmitted at the end of every frame.  Therefore,
*  even though we aren't scheduling messages, we must still
*  schedule a frame.
*
**/

	BTI1553_BCSchedFrame(1000,channum,hCore);

/**
*
*  Initialize the data words, and select a BC-RT transfer.
*
**/

	Msg.ctrlflags = MSGCRT1553_BCRT;	/*Selects BC-RT transfer*/

	/*Fill the data buffer*/
	for (i=0; i<32; i++)
		Msg.data[i] = 0xAB00 + i;

/**
*
*  Start operation of the card.
*
**/

	BTICard_CardStart(hCore);

/**
*
*  Cycle through all commands, SA 0-31, Tx and Rx, WC 1-32.
*  This will include all mode code commands, including terminals
*  that are not legal for 1553B.
*
**/

	msg_count = 0;
	no_resp   = 0;
	msg_error = 0;

	for (sa=0; sa<32; sa++)
	{
		for (tr=0; tr<2; tr++)
		{
			for (wc=1; wc<33; wc++)
			{
				Msg.cwd1 = BTI1553_ValPackCWD(rtnum,tr,sa,wc);		/*Create command word*/

				errval = BTI1553_BCTransmitMsg(&Msg,channum,hCore);	/*Transmit the message*/

				msg_count++;	/*Increment the message count*/

				/**
				*
				*  Test the message results.
				*
				*  Note the distinction between card errors and bus errors.
				*  The returned value of BTI1553_BCTransmitMsg is a card error
				*  (if any occurred).  Errors related to the RT's reponse are
				*  returned in the (errflags) member of the message
				*  structure.
				*
				**/

				if (errval < 0)								/*Card error*/
				{
					consoleprintf("\nError:  An error was encountered (%i) while transmitting.",errval);
					consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCore));
					BTICard_CardClose(hCard);
					consoleexit(1);
				}

				if (Msg.errflags & MSGERR1553_NORESP)		/*Was there a response?*/
				{
					no_resp++;
				}
				else if (Msg.errflags & MSGERR1553_ANYERR)	/*Was there an error?*/
				{
					msg_error++;
				}
				else										/*There were no errors*/
				{
					consoleprintf("\nSuccess!  Command = %04X { RT:%d TR:%d SA%d WC%d }",
						   Msg.cwd1,rtnum,tr,sa,wc);
				}
			}
		}
	}

/**
*
*  Report the final counts
*
**/

	consoleprintf("\n");
	consoleprintf("\nMessage count       = %d",msg_count);
	consoleprintf("\nNo response count   = %d",no_resp);
	consoleprintf("\nMessage error count = %d",msg_error);

/**
*
*  The card MUST be closed before continuing
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
