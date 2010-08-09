
/**
*
*  BTI708 DRIVER EXAMPLE 5  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP5.C -- Transmitter file playback example
*
**/

/**
*
*  This program opens Card Number 0, and configures the
*  first compatible core to configure the first transmit
*  channel to transmit a sequence of 64 messages at a
*  rate of 200Hz.  The messages are automatically assigned
*  the numbers 0 through 63.
*
*  A data file is used to source the data to transmit
*  on the bus.
*
*  An Event Log frequency of 32 is specifed, which will
*  cause the channel to issue an Event Log List entry
*  every 32nd message (message numbers 31, and 63).
*  The program uses these events to update he data for
*  the half of messages that were just transmitted.
*
*  Data from the data file are read until the end of the file
*  is reached.  Once reached, the program will seek to the
*  start of the file and transmit the file again.
*
*  The program continues in this manner until a key is
*  pressed.
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

UINT16 framefreq   = 200;           /*Frame frequency of 200Hz*/
UINT16 blocksize   = 32;            /*Generate an Event Log List entry on every 32nd frame*/
UINT16 framecount  = 64;            /*Transmit a total of 64 frames*/

char filename[256] = "EXAMP5.DAT";  /*Name of file containing data to transmit*/

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

int main(int argc,char *argv[])
{
	HCARD  hCard;
	HCORE  hCore;
	ERRVAL errval;
	INT    corenum;
	INT    xmtchan=0;
	INT    j;
	UINT16 Data[100];
	UINT16 Type;
	UINT32  Info;
	UINT16 framenum;
	FILE *fh;
	char *opts;
	INT    ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP5  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2001-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI708 Example 5                                                  *");
	consoleprintf("\n    *  \"Transmitter file playback example.\"                              *");
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
		consoleprintf("\nUsage:  EXAMP5 [options]");
		consoleprintf("\n");

		consoleprintf("\n-?                    Display help information");
		consoleprintf("\n-FREQ:dd              Set the frequency.  Default 200Hz.");
		consoleprintf("\n-BLOCKSIZE:dd         Set the block size.  Default 32 frames.");
		consoleprintf("\n-FILENAME:<filename>  File to read data from.  Default EXAMP5.DAT.");
		consoleprintf("\n-AUTO                 Loads two blocks of data from file, starts card,");
		consoleprintf("\n                      and exits the program, allowing the card to");
		consoleprintf("\n                      continuously transmit the data.");
		consoleprintf("\n");
		consoleexit(0);
	}

	consoleprintf("\nFor help:  EXAMP5 ?");
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
*  Check for the -FILENAME:<filename> option.
*
**/

	if ((opts = isargopt(argc,argv,"FILENAME")) != NULL)
	{
		if (*opts++ != ':')
		{
			consoleprintf("\nError:  Semicolon is missing after option."
							"\n        Use the form -FILENAME:<filename>.");
			consoleprintf("\n");
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
*  Check for the -BLOCKSIZE:dd option.
*
**/

	if ((opts = isargopt(argc,argv,"BLOCKSIZE")) != NULL)
	{
		if (*opts++ != ':')
		{
			consoleprintf("\nError:  Semicolon is missing after option."
							"\n        Use the form -BLOCKSIZE:<blocksize>.");
			consoleprintf("\n");
			consoleexit(1);
		}

		if (!strlen(opts))
		{
			consoleprintf("\nError:  The block size is missing after the option."
							"\n        Use the form -BLOCKSIZE:<blocksize>.");
			consoleprintf("\n");
			consoleexit(1);
		}

		if (sscanf(opts,"%hu",&blocksize)!=1)
		{
			consoleprintf("\nError:  The block size is not a decimal number."
							"\n        Use the form -BLOCKSIZE:dd.");
			consoleprintf("\n");
			consoleexit(1);
		}

		framecount = blocksize * 2;

		consoleprintf("\nUsing a block size of %u frames and a total of %u frames.",blocksize,framecount);
	}

/**
*
*  Check for the -FREQ:dd option.
*
**/

	if ((opts = isargopt(argc,argv,"FREQ")) != NULL)
	{
		if (*opts++ != ':')
		{
			consoleprintf("\nError:  Semicolon is missing after option."
							"\n        Use the form -FREQ:<blocksize>.");
			consoleprintf("\n");
			consoleexit(1);
		}

		if (!strlen(opts))
		{
			consoleprintf("\nError:  The frequency is missing after the option."
							"\n        Use the form -FREQ:<frequency>.");
			consoleprintf("\n");
			consoleexit(1);
		}

		if (sscanf(opts,"%hu",&framefreq)!=1)
		{
			consoleprintf("\nError:  The frequency is not a decimal number."
							"\n        Use the form -FREQ:dd.");
			consoleprintf("\n");
			consoleexit(1);
		}

		consoleprintf("\nUsing a frequency of %uHz.",framefreq);
	}

/**
*
*  Find the first ARINC 708 core with a transmit channel.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (xmtchan=0;xmtchan<MAX_CHANNELS;xmtchan++)	if (BTI708_ChIsXmt(xmtchan,hCore)) break;

		if (xmtchan != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || xmtchan == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No ARINC 708 transmit channels present in cardnum %d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing ARINC 708 core #%d",corenum);
	consoleprintf("\nUsing transmit channel #%d",xmtchan);
	consoleprintf("\n");

/**
*
*  Configure transmit channel.
*
**/

	BTICard_CardReset(hCore);

	errval = BTI708_ChConfig(CHCFG708_DEFAULT,xmtchan,hCore);

	if (errval)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        channel #%d on core #%d.",xmtchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Build the transmit channel messages.
*
**/

	errval = BTI708_XmtBuild(MSGCRT708_DEFAULT,framefreq,blocksize,framecount,xmtchan,hCore);

	if (errval)
	{
		consoleprintf("\nError:  An error was encountered (%i) while building",errval);
		consoleprintf("\n        channel #%d on core #%d.",xmtchan,corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

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
*  Preload two blocks of data from the file to the card.
*
**/

	for (j=0;j<framecount;++j)						//Loop through each of the frames
	{
		if (fread(Data,1,sizeof(Data),fh) == 0)		//If at end of file
		{
			consoleprintf("\nResetting data file %s",filename);
			fseek(fh,0,0);							//Seek to beginning of file
			fread(Data,1,sizeof(Data),fh);
			BTI708_MsgDataWr(Data,j,xmtchan,hCore);	//Write the data for frame
		}
		else
		{
			BTI708_MsgDataWr(Data,j,xmtchan,hCore);	//Write the data for frame
		}
	}

/**
*
*  Configure the Event Log list.
*
**/

	errval = BTICard_EventLogConfig(LOGCFG_ENABLE,1024,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        the event log list on core #%d.",corenum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
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
*  Check for the -AUTO option.
*
**/

	if ((opts = isargopt(argc,argv,"AUTO")) != NULL)
	{
		consoleprintf("\nExiting program.");
		consoleprintf("\nDevice is now transmitting static data.");

		fclose(fh);                             //Close the file
		BTICard_CardClose(hCard);               //Close the card
		consoleexit(0);
	}

/**
*
*  Loop until a key is hit.
*
**/

	consoleprintf("\nPress any key to exit...");
	consoleprintf("\n");

	while (1)
	{
		ch = getch();
		if (ch != ERR) break;

		if (BTICard_EventLogRd(&Type,&Info,NULL,hCore))	//Read entry from Event Log list
		{
			framenum = (UINT16)Info;

			if (framenum == ((framecount/2)-1))			//First half transmitted
			{
				for (j=0;j<=framenum;j++)
				{
					if (fread(Data,1,sizeof(Data),fh) == 0)		//If at end of file
					{
						consoleprintf("\nResetting data file %s",filename);
						fseek(fh,0,0);							//Seek to beginning of file
						fread(Data,1,sizeof(Data),fh);
						BTI708_MsgDataWr(Data,j,xmtchan,hCore);	//Write the data for frame
					}
					else
					{
						BTI708_MsgDataWr(Data,j,xmtchan,hCore);	//Write the data for frame
					}
				}

				consoleprintf("\nUpdated frames %u to %u.",0,framenum);
			}
			else if (framenum == (framecount-1))		//Second half transmitted
			{
				for (j=(framecount/2);j<=framenum;j++)
				{
					if (fread(Data,1,sizeof(Data),fh) == 0)		//If at end of file
					{
						consoleprintf("\nResetting data file %s",filename);
						fseek(fh,0,0);							//Seek to beginning of file
						fread(Data,1,sizeof(Data),fh);
						BTI708_MsgDataWr(Data,j,xmtchan,hCore);	//Write the data for frame
					}
					else
					{
						BTI708_MsgDataWr(Data,j,xmtchan,hCore);	//Write the data for frame
					}
				}

				consoleprintf("\nUpdated frames %u to %u.",framecount/2,framenum);
			}
		}
	}

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
