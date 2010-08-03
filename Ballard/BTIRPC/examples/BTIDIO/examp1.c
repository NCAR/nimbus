
/**
*
*  BTIDIO DRIVER EXAMPLE 1  (10/23/2009)
*  Copyright (c) 2005-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   EXAMP1.C -- Input/Output example
*
**/

/**
*
*  This program opens Card Number 0 and configures the
*  first compatible core in shunt mode to query the current input state
*  of Bank A.  The corresponding outputs can be driven by the number keys (0-7).
*
**/

#include "BTICard.h"
#include "BTIDIO.h"
#include <ncurses.h>
#include <stdlib.h>

#define	CARDNUM          0
#define	MAX_CORES        4
#define	MAX_BANKS        4

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

int main(int argc, char* argv[])
{
	HCARD hCard;
	HCORE hCore;
	INT corenum;
	INT banknum = 0;
	ERRVAL errval;
	UINT16 bankcfg;
	BYTE input;
	BYTE output = 0;
	INT ch;
	BYTE fault_mask = 0x00;
	BOOL done;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  EXAMP1  (10/23/2009)                                              *");
	consoleprintf("\n    *  Copyright 2005-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTIDIO Example 1                                                  *");
	consoleprintf("\n    *  \"Program for input/output example.\"                               *");
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

	consoleprintf("\nUsing DIO card #%d",CARDNUM);

/**
*
*  Find the first core with a DIO bank.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (banknum=0;banknum<MAX_BANKS;banknum++)
		{
			if (BTIDIO_BankIsDIO(banknum,hCore))	break;
		}

		if (banknum != MAX_BANKS) break;
	}

	if (errval || corenum == MAX_CORES || banknum == MAX_BANKS)
	{
		consoleprintf("\nError:  No DIO banks present in card #%d.",CARDNUM);
		consoleprintf("\n");
		BTICard_CardClose(hCard);
		consoleexit(1);
	}

	consoleprintf("\nUsing DIO core #%d",corenum);
	consoleprintf("\nUsing Bank %c",banknum + 'A');
	consoleprintf("\n");
	consoleprintf("\n");

/**
*
*  Configure banknum as a shunt, input/output bank with logic low
*  on the input interpretted as 1.
*
**/

	BTICard_CardReset(hCore);

	bankcfg = BANKCFGDIO_SHUNT | BANKCFGDIO_INOUT | BANKCFGDIO_POLIN_INV;
	errval = BTIDIO_BankConfig(bankcfg,THRESHOLDDIO_2_5V,SAMPLERATEDIO_MS | 0x0001,0,hCore);

	if (errval < 0)
	{
		consoleprintf("\nError:  An error was encountered (%i) while configuring",errval);
		consoleprintf("\n        bank %c on core #%d.",banknum+'A',corenum);
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
*  Read and display the current value of the discrete inputs.
*
**/

	consoleprintf("\nPress keys 0-7 to toggle the outputs.  Press escape to exit...");
	consoleprintf("\n");
	consoleprintf("\n");

	done = 0;
	while (!done)  /* exit loop if Escape is pressed */
	{
		ch = getch();

		if (ch != ERR)
		{
			/* Check to see if user pressed Escape */
			if (ch == 0x1B)
				done = 1;

			/* Clear faults on user command */
			else if (ch == 'c' || ch == 'C')
				BTIDIO_BankFaultClr(fault_mask,banknum,hCore);

			/* Toggle Outputs based on user commands */
			if (ch >= '0' && ch <= '7')
			{
				output = output ^ (BYTE)(1 << (ch - '0'));
				BTIDIO_BankWr(output,banknum,hCore);
			}
		}


		/* Check for Faults on the Outputs */
		fault_mask = BTIDIO_BankFaultRd(banknum,hCore);

		/* Read Bank A and Display the State of its Discretes */
		input = BTIDIO_BankRd(banknum,hCore);

		consoleprintf("\rBank %c DOUT: ",(char)banknum + 'A');

		consoleprintf("%c",((fault_mask & 0x0080) ? 'F' : ((output & 0x0080) ? '1' : '0')));
		consoleprintf("%c",((fault_mask & 0x0040) ? 'F' : ((output & 0x0040) ? '1' : '0')));
		consoleprintf("%c",((fault_mask & 0x0020) ? 'F' : ((output & 0x0020) ? '1' : '0')));
		consoleprintf("%c",((fault_mask & 0x0010) ? 'F' : ((output & 0x0010) ? '1' : '0')));

		consoleprintf("%c",((fault_mask & 0x0008) ? 'F' : ((output & 0x0008) ? '1' : '0')));
		consoleprintf("%c",((fault_mask & 0x0004) ? 'F' : ((output & 0x0004) ? '1' : '0')));
		consoleprintf("%c",((fault_mask & 0x0002) ? 'F' : ((output & 0x0002) ? '1' : '0')));
		consoleprintf("%c",((fault_mask & 0x0001) ? 'F' : ((output & 0x0001) ? '1' : '0')));

		consoleprintf(", DIN: ");

		consoleprintf("%c",(input & 0x0080) ? '1' : '0');
		consoleprintf("%c",(input & 0x0040) ? '1' : '0');
		consoleprintf("%c",(input & 0x0020) ? '1' : '0');
		consoleprintf("%c",(input & 0x0010) ? '1' : '0');

		consoleprintf("%c",(input & 0x0008) ? '1' : '0');
		consoleprintf("%c",(input & 0x0004) ? '1' : '0');
		consoleprintf("%c",(input & 0x0002) ? '1' : '0');
		consoleprintf("%c",(input & 0x0001) ? '1' : '0');

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
