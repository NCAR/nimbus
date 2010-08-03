
/**
*
*  BTI429 DRIVER EXAMPLE 11  (10/23/2009)
*  Copyright (c) 2003-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP11.C -- Parametrics example
*
**/

/**
*
*  This program opens Card Number 0, and configures the
*  first transmit channel on the first 429 Core to build
*  a transmit schedule using transmit interval information.
*  The user can then control the amplitude using the UP and
*  DOWN arrow keys and the frequency using the LEFT and RIGHT
*  arrow keys.
*
**/

#include "BTICard.h"
#include "BTI429.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

#define DAC_HIGH	 0x0FFF
#define DAC_LOW		 0x0599
#define DIV_HIGH	 0x00FF
#define DIV_LOW		 0x0000

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
	MSGSTRUCT429 comp_airspeed, tot_air_temp, altitude_rate;
	MSGADDR msgaddr[3];
	INT	min_intrvls[3];
	INT	max_intrvls[3];
	UINT32 bnr_data[3];
	BOOL done;
	UINT16 dacval = 0x0FF0;		// Amplitude
	UINT16 divval = 0x0027;		// Frequency
	INT ch;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP11  (10/23/2009)                                             *");
	consoleprintf("\n    *  Copyright 2003-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTI429 Example 11                                                 *");
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

	consoleprintf("\nUsing ARINC 429 card #%d",CARDNUM);

/**
*
*  Find the first ARINC 429 core with a transmit channel that supports parametrics.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (xmtchan=0;xmtchan<MAX_CHANNELS;xmtchan++)
		{
			if (BTI429_ChIsXmt(xmtchan,hCore) && BTI429_ChGetInfo(INFO429_PARAM,xmtchan,hCore))	break;
		}

		if (xmtchan != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || xmtchan == MAX_CHANNELS)
	{
		consoleprintf("\nError:  No ARINC 429 parametric transmit channels");
		consoleprintf("\n        present in card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\n\tUsing ARINC 429 core #%d",corenum);
	consoleprintf("\n\tUsing parametric transmit channel #%d",xmtchan);
	consoleprintf("\n");

/**
*
*  Configure xmtchan to transmit at high speed.
*
**/

	BTICard_CardReset(hCore);

	errval = BTI429_ChConfig(CHCFG429_HIGHSPEED,xmtchan,hCore);

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
*  Configure Parametrics on xmtchan
*
**/

	dacval = (dacval>DAC_HIGH) ? DAC_HIGH:dacval;
	dacval = (dacval<DAC_LOW) ? DAC_LOW:dacval;

	errval = BTI429_ParamAmplitudeConfig(PARAMCFG429_DEFAULT,dacval,xmtchan,hCore);
	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        the amplitude on channel #%d on core #%d.",xmtchan,corenum);
		consoleprintf("\n        (%s)\n\n",BTICard_ErrDescStr(errval,hCard));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	divval = (divval>DIV_HIGH) ? DIV_HIGH:divval;
	divval = (divval<DIV_LOW)  ? DIV_LOW:divval;

	errval = BTI429_ParamBitRateConfig(PARAMCFG429_DEFAULT,divval,xmtchan,hCore);
	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        the bit rate on channel #%d on core #%d.",xmtchan,corenum);
		consoleprintf("\n        (%s)\n\n",BTICard_ErrDescStr(errval,hCard));
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

/**
*
*  Create a message for each of the labels we are transmitting.
*
**/

	comp_airspeed.addr = BTI429_MsgCreate(MSGCRT429_DEFAULT,hCore);
	tot_air_temp.addr  = BTI429_MsgCreate(MSGCRT429_DEFAULT,hCore);
	altitude_rate.addr = BTI429_MsgCreate(MSGCRT429_DEFAULT,hCore);

/**
*
*  Setup the arrays of message addresses, and intervals
*  needed to build the schedule.  Note that the nth element
*  in each array correspond to each other.
*
**/

	msgaddr[0] = comp_airspeed.addr;
	min_intrvls[0] = 250;
	max_intrvls[0] = 500;

	msgaddr[1] = tot_air_temp.addr;
	min_intrvls[1] = 250;
	max_intrvls[1] = 500;

	msgaddr[2] = altitude_rate.addr;
	min_intrvls[2] = 100;
	max_intrvls[2] = 200;

/**
*
*  Next, build a schedule using the minimum and maximum
*  transmit intervals.
*
**/

	errval = BTI429_SchedBuild(3,msgaddr,min_intrvls,max_intrvls,xmtchan,hCore);

	if (errval < 0)
	{
		printf("\nError:  An error was encountered (%i) while building",errval);
		printf("\n        a schedule on channel #%d of core #%d.",xmtchan,corenum);
		printf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		BTICard_CardClose(hCard);
		exit(1);
	}

/**
*
*  Initialize the three Message Records.
*
**/

	comp_airspeed.data = BTI429_FldPutLabel(0L,0206);     /*Init label field*/
	tot_air_temp.data  = BTI429_FldPutLabel(0L,0211);
	altitude_rate.data = BTI429_FldPutLabel(0L,0212);

	comp_airspeed.data = BTI429_BNRPutMant(comp_airspeed.data,0L,14,0);     /*Init BNR data*/
	tot_air_temp.data  = BTI429_BNRPutMant(tot_air_temp.data,0L,11,0);
	altitude_rate.data = BTI429_BNRPutMant(altitude_rate.data,0L,16,0);

	BTI429_MsgDataWr(comp_airspeed.data,comp_airspeed.addr,hCore);
	BTI429_MsgDataWr(tot_air_temp.data,tot_air_temp.addr,hCore);
	BTI429_MsgDataWr(altitude_rate.data,altitude_rate.addr,hCore);

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
	consoleprintf("\n\tARROW RIGHT = Increment divval");
	consoleprintf("\n\tARROW LEFT  = Decrement divval");
	consoleprintf("\n\tESC         = exit program");
	consoleprintf("\n\n");

	for (done=0;!done;)
	{
		consoleprintf("\r\tDacval: %03X (Hex)   Divval: %03X (Hex)",dacval,divval);

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

					BTI429_ChStop(xmtchan,hCore);

					errval = BTI429_ParamAmplitudeConfig(PARAMCFG429_DEFAULT,dacval,xmtchan,hCore);
					if (errval < 0)
					{
						consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
						consoleprintf("\n        Amplitude on channel #%d on core #%d.",xmtchan,corenum);
						consoleprintf("\n        (%s)\n\n",BTICard_ErrDescStr(errval,hCard));
						done=1;
					}

					BTI429_ChStart(xmtchan,hCore);

					break;
				}
				case KEY_LEFT:	/* LEFT  */
				case KEY_RIGHT:	/* RIGHT */
				{
					if (ch==KEY_RIGHT)	divval = (divval+1>DIV_HIGH) ? DIV_HIGH : divval+1;
					else		divval = (divval-1<DIV_LOW)  ? DIV_LOW  : divval-1;

					BTI429_ChStop(xmtchan,hCore);

					errval = BTI429_ParamBitRateConfig(PARAMCFG429_DEFAULT,divval,xmtchan,hCore);
					if (errval < 0)
					{
						consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
						consoleprintf("\n        the bit rate on channel #%d on core #%d.",xmtchan,corenum);
						consoleprintf("\n        (%s)\n\n",BTICard_ErrDescStr(errval,hCard));
						done=1;
					}

					BTI429_ChStart(xmtchan,hCore);

					break;
				}
			}
		}

		comp_airspeed.data = BTI429_MsgDataRd(comp_airspeed.addr,hCore);   /*Read messages back*/
		tot_air_temp.data  = BTI429_MsgDataRd(tot_air_temp.addr,hCore);
		altitude_rate.data = BTI429_MsgDataRd(altitude_rate.addr,hCore);

		bnr_data[0] = BTI429_BNRGetMant(comp_airspeed.data,14);   /*Extract BNR data*/
		bnr_data[1] = BTI429_BNRGetMant(tot_air_temp.data,11);
		bnr_data[2] = BTI429_BNRGetMant(altitude_rate.data,16);

		++bnr_data[0];   /*Change BNR data*/
		++bnr_data[1];
		++bnr_data[2];

		comp_airspeed.data = BTI429_BNRPutMant(comp_airspeed.data,bnr_data[0],14,0);   /*Put BNR data*/
		tot_air_temp.data  = BTI429_BNRPutMant(tot_air_temp.data,bnr_data[1],11,0);    /*back into*/
		altitude_rate.data = BTI429_BNRPutMant(altitude_rate.data,bnr_data[2],16,0);   /*messages*/

		BTI429_MsgDataWr(comp_airspeed.data,comp_airspeed.addr,hCore);   /*Write new message values*/
		BTI429_MsgDataWr(tot_air_temp.data,tot_air_temp.addr,hCore);
		BTI429_MsgDataWr(altitude_rate.data,altitude_rate.addr,hCore);
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
