
/**
*
*  BTI708 DRIVER EXAMPLE 10  (10/23/2009)
*  Copyright (c) 2003-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP10.C -- Parametrics example
*
**/

/**
*
*  This program opens Card Number 0, and configures the
*  first transmit channel on the first 708 Core to transmit
*  a sequence of 32 messages at a rate of 200Hz. The messages
*  are automatically assigned the numbers 0 through 31.
*
*  The user can then control the amplitude using the UP and
*  DOWN arrow keys.
*
*  The program continues in this manner until a key is
*  pressed.
*
**/

#include "BTICard.h"
#include "BTI708.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

#define	FRAMEFREQ	 200            /*Frame frequency of 200Hz*/
#define LOGFREQ		 16             /*Generate a Log Event after every 16th message*/
#define	FRAMECOUNT	 32             /*Transmit a total of 32 messages*/

#define DAC_HIGH	 0x0FFF
#define DAC_LOW		 0x0599

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(void)
{
	HCARD  hCard;
	HCORE  hCore;
	ERRVAL errval;
	INT    corenum;
	INT    xmtchan=0;
	INT    j,jj;
	UINT16 Data[100];
	UINT16 dacval = 0x0FF0;		/* Amplitude */
	INT    count  = 0;
	BOOL   done;
	INT ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP10  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2003-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI708 Example 10                                                 *");
	consoleprintf("\n    *  \"Parametrics example.\"                                            *");
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
*  Find the first ARINC 708 core with a transmit channel that supports parametrics.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (xmtchan=0;xmtchan<MAX_CHANNELS;xmtchan++)
		{
			if (BTI708_ChIsXmt(xmtchan,hCore) && BTI708_ChGetInfo(INFO708_PARAM,xmtchan,hCore))	break;
		}

		if (xmtchan != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || xmtchan == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No ARINC 708 parametric transmit channels");
		consoleprintf("\n        present in card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\n\tUsing ARINC 708 core #%d",corenum);
	consoleprintf("\n\tUsing parametric transmit channel #%d",xmtchan);
	consoleprintf("\n");

/**
*
*  Configure xmtchan
*
**/

	BTICard_CardReset(hCore);

	errval = BTI708_ChConfig(CHCFG708_DEFAULT,xmtchan,hCore);

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
*  Build the transmit channel messages.
*
**/

	errval = BTI708_XmtBuild(MSGCRT708_DEFAULT,FRAMEFREQ,LOGFREQ,FRAMECOUNT,xmtchan,hCore);

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
*  Create and write the data for the messages.
*
**/

	for (j=0;j<FRAMECOUNT;j++)					/*Loop through all the messages*/
	{
		for (jj=0;jj<100;jj++) Data[jj] = j + (count * FRAMECOUNT);
		BTI708_MsgDataWr(Data,j,xmtchan,hCore);	/*Write data to the Device*/
	}
	count++;

/**
*
*  Configure Parametrics on xmtchan.
*
**/

	dacval = (dacval>DAC_HIGH) ? DAC_HIGH:dacval;
	dacval = (dacval<DAC_LOW) ? DAC_LOW:dacval;

	errval = BTI708_ParamAmplitudeConfig(PARAMCFG708_DEFAULT,dacval,xmtchan,hCore);
	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        the amplitude on channel #%d on core #%d.",xmtchan,corenum);
		consoleprintf("\n        (%s)\n\n",BTICard_ErrDescStr(errval,hCard));
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
*  Change parametric values when arrow keys are pressed. Exit when ESC is pressed.
*
**/

	consoleprintf("\n\tOPTIONS:");
	consoleprintf("\n\tARROW UP    = Add 16 to dacval");
	consoleprintf("\n\tARROW DOWN  = Subtract 16 from dacval");
	consoleprintf("\n\tESC         = Exit program");
	consoleprintf("\n\n");

	for (done=0;!done;)
	{
		consoleprintf("\r\tDacval: %03X  ",dacval);

		ch = getch();
		if (ch != ERR)
		{
			switch (ch)
			{
				case 27:	/* ESC */
				{
					done=1;
					break;
				}
				case KEY_UP:	/* UP   */
				case KEY_DOWN:	/* DOWN */
				{
					if (ch==KEY_UP)	dacval = (dacval+16>DAC_HIGH) ? DAC_HIGH : dacval+16;
					else        dacval = (dacval-16<DAC_LOW)  ? DAC_LOW  : dacval-16;

					BTI708_ChStop(xmtchan,hCore);

					errval = BTI708_ParamAmplitudeConfig(PARAMCFG708_DEFAULT,dacval,xmtchan,hCore);
					if (errval < 0)
					{
						consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
						consoleprintf("\n        the amplitude on channel #%d on core #%d.",xmtchan,corenum);
						consoleprintf("\n        (%s)\n\n",BTICard_ErrDescStr(errval,hCard));
						done=1;
					}

					BTI708_ChStart(xmtchan,hCore);

					break;
				}
			}
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
