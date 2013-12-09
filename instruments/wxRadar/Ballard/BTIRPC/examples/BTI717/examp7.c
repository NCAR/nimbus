
/**
*
*  BTI717 DRIVER EXAMPLE 7  (10/23/2009)
*  Copyright (c) 2003-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP7.C -- Parametrics example
*
**/

/**
*
*  The user can then control the amplitude using the UP and
*  DOWN arrow keys and the frequency using the LEFT and RIGHT
*  arrow keys.
*
**/

#include "BTICard.h"
#include "BTI717.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

#define DAC_HIGH	 0x0FF0
#define DAC_LOW		 0x0600

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(void)
{
	HCARD hCard;
	HCORE hCore;
	ERRVAL errval;
	INT corenum;
	INT xmtchan=0;

	INT wps = 256;
	SUPERFRMADDR superfrmaddr;
	UINT16 sfblock[8200];
	INT j, jj, ch;

	BOOL done;
	UINT16 dacval = 0x0FF0;		// Amplitude

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP7  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2003-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI717 Example 7                                                  *");
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

	consoleprintf("\nUsing ARINC 717 card #%d",CARDNUM);

/**
*
*  Find the first ARINC 717 core with a BiPhase transmit channel that supports parametrics.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (xmtchan=0;xmtchan<MAX_CHANNELS;xmtchan++)
		{
			if (BTI717_ChIsType(CHCFG717_BIPHASEXMT, xmtchan, hCore))
			{
				if (BTI717_ChGetInfo(INFO717_PARAM,xmtchan,hCore))	break;
			}
		}

		if (xmtchan != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || xmtchan == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No ARINC 717 BiPhase parametric transmit channels present in cardnum %d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing ARINC 717 core #%d",corenum);
	consoleprintf("\nUsing BiPhase parametric transmit channel #%d",xmtchan);
	consoleprintf("\n");

/**
*
*  Configure transmitter channel at specified words per second.
*  Use the internal wrap around.
*
**/

	if      (wps == 64)   errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_64WPS,  xmtchan,hCore);
	else if (wps == 128)  errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_128WPS, xmtchan,hCore);
	else if (wps == 256)  errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_256WPS, xmtchan,hCore);
	else if (wps == 512)  errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_512WPS, xmtchan,hCore);
	else if (wps == 1024) errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_1024WPS,xmtchan,hCore);
	else if (wps == 2048) errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_2048WPS,xmtchan,hCore);
	else if (wps == 4096) errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_4096WPS,xmtchan,hCore);
	else if (wps == 8192) errval = BTI717_ChConfig(CHCFG717_BIPHASEXMT | CHCFG717_8192WPS,xmtchan,hCore);

	if (errval < 0)
	{
		fprintf(stderr,"\nError:  An error was encountered (%i) while configuring channel %d.",errval,xmtchan);
		fprintf(stderr,"\n        (%s)",BTICard_ErrDescStr(errval,hCore));
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Init subframe data.
*
**/

	for (j=1;j<=4;j++)
	{
		BTI717_SubFrmBlkRd(sfblock,j,xmtchan,hCore);
		for (jj=2;jj<=wps;jj++) sfblock[jj+7] = ((0x0100*j)+jj) & 0x0FFF;
		BTI717_SubFrmBlkWr(sfblock,j,xmtchan,hCore);
	}

/**
*
*  Create some SuperFrame structures.
*
**/

	BTI717_SuperFrmConfig(10,16,xmtchan,hCore);

/**
*
*  Define SF2, W10 to be SuperFrame Data.
*  Init superframe data.
*
**/

	superfrmaddr = BTI717_SuperFrmDefine(2,10,xmtchan,hCore);
	for (j=1;j<=16;j++) BTI717_SuperFrmWordWr((UINT16)(0x0500+j),j,superfrmaddr,xmtchan,hCore);

/**
*
*  Set superframe counter position.
*
**/

	BTI717_SuperFrmCounterPos(1,wps,xmtchan,hCore);

/**
*
*  Configure Parametrics on xmtchan
*
**/

	dacval = (dacval>DAC_HIGH) ? DAC_HIGH:dacval;
	dacval = (dacval<DAC_LOW) ? DAC_LOW:dacval;

	errval = BTI717_ParamAmplitudeConfig(PARAMCFG717_DEFAULT,dacval,xmtchan,hCore);
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
	consoleprintf("\n\tESC         = exit program");
	consoleprintf("\n\n");

	done = 0;

	while (!done)
	{
		consoleprintf("\r\tDacval: %03X (Hex)",dacval);

		ch = getch();
		if (ch != ERR)
		{
			switch (ch)
			{
				case 27:	/* ESC */
				{
					done = 1;
					break;
				}
				case KEY_UP:	/* UP   */
				case KEY_DOWN:	/* DOWN */
				{
					if (ch==KEY_UP)	dacval = (dacval+16>DAC_HIGH) ? DAC_HIGH : dacval+16;
					else        dacval = (dacval-16<DAC_LOW)  ? DAC_LOW  : dacval-16;

					BTI717_ChStop(xmtchan,hCore);

					errval = BTI717_ParamAmplitudeConfig(PARAMCFG717_DEFAULT,dacval,xmtchan,hCore);
					if (errval < 0)
					{
						consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
						consoleprintf("\n        Amplitude on channel #%d on core #%d.",xmtchan,corenum);
						consoleprintf("\n        (%s)\n\n",BTICard_ErrDescStr(errval,hCard));
						done=1;
					}

					BTI717_ChStart(xmtchan,hCore);

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
