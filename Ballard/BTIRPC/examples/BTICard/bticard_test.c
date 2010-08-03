
/**
*
*  BTICard TEST EXAMPLE  (10/23/2009)
*  Copyright (c) 2004-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   bticard_test.c -- Tests installed cards
*
**/

/**
*
*  This example will open the specified card number, print useful
*  information about the card's configuration, and run a series
*  of diagnostic tests on the card.  If an error is encountered,
*  a message is displayed indicating the type of error that occurred.
*
**/

#include <BTICard.h>
#include <BTI1553.h>
#include <BTI429.h>
#include <BTI708.h>
#include <BTI717.h>
#include <BTI422.h>
#include <BTICSDB.h>
#include <BTIDIO.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define MAXCORES 4
#define MAXCHANS 64
#define CISSIZE  64

void consoleprintf(const char *, ...);
void consoleentry(void);
void consoleexit(INT);

#define consoleprintf printf

ERRVAL TEST_CardInfo(INT cardnum,HCARD hCard);
ERRVAL TEST_CoreInfo(INT corenum,HCORE hCore);

BOOL skipio   = FALSE;
BOOL skipmem  = FALSE;
BOOL skipcomm = FALSE;
BOOL skipbus  = FALSE;

UINT16 crc8(UINT16 crc,UINT16 value)
{
	UINT16 j;
	UINT16 temp;
	const UINT16 key = 0x008C;

	value &= 0x00FF;
	crc   &= 0x00FF;

	for (j=0;j<8;++j,value>>=1)
	{
		temp = crc & 1;
		crc  >>= 1;
		if (temp ^ (value & 1)) crc ^= key;
	}

	return(crc);
}

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

int main(INT argc,LPSTR *argv)
{
	CHAR *opts;
	INT cardnum = 0;
	HCARD hCard;
	HCORE hCore[MAXCORES];
	ERRVAL errval;
	ERRVAL core_results[MAXCORES];
	INT corecount;
	INT j;

	consoleentry();

	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTICARD_TEST  (10/23/2009)                                        *");
	consoleprintf("\n    *  Copyright 2004-2009  Ballard Technology, Inc.  Everett, WA, USA.  *");
	consoleprintf("\n    *  All rights reserved.                                              *");
	consoleprintf("\n    *  Go to www.ballardtech.com or email support@ballardtech.com        *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    *  BTICARD Example                                                   *");
	consoleprintf("\n    *  \"Tests installed cards.\"                                          *");
	consoleprintf("\n    *                                                                    *");
	consoleprintf("\n    **********************************************************************");
	consoleprintf("\n");

	if (isarghelp(argc,argv))
	{
		consoleprintf("\nUsage:  bticard_test [options]");
		consoleprintf("\n");

		consoleprintf("\n-?                      Display help information");
		consoleprintf("\n-CARDNUM:<cardnum>      Card number.  Default 0.");
		consoleprintf("\n-NOIO                   Do not run I/O tests.");
		consoleprintf("\n-NOMEM                  Do not run memory tests.");
		consoleprintf("\n-NOCOMM                 Do not run comm process tests.");
		consoleprintf("\n-NOBUS                  Do not run databus tests.");
		consoleprintf("\n");

		consoleexit(0);
	}

/**
*
*  Check for the -CARDNUM:<cardnum> option.
*
**/

	if ((opts = isargopt(argc,argv,"CARDNUM")) != NULL)
	{
		if (*opts++ != ':')
		{
			consoleprintf(	"\nError:  Semicolon is missing after option."
							"\n        Use the form -CARDNUM:<cardnum>.");
			consoleprintf( "\n");
			consoleexit(1);
		}

		if (!strlen(opts))
		{
			consoleprintf( "\nError:  Card number is missing after option."
							"\n        Use the form -CARDNUM:<cardnum>.");
			consoleprintf( "\n");
			consoleexit(1);
		}

		if (sscanf(opts,"%u",&cardnum)!=1)
		{
			consoleprintf("\nError:  Card number is not a decimal number.");
			consoleprintf("\n        Use the form -CARDNUM:dd.");
			consoleprintf("\n");
			consoleexit(1);
		}

		consoleprintf("\nUsing card number %u.",cardnum);
	}

/**
*
*  Check for other options.
*
**/

	if ((opts = isargopt(argc,argv,"NOIO"))    != NULL) skipio   = TRUE;
	if ((opts = isargopt(argc,argv,"NOMEM"))   != NULL) skipmem  = TRUE;
	if ((opts = isargopt(argc,argv,"NOCOMM"))  != NULL) skipcomm = TRUE;
	if ((opts = isargopt(argc,argv,"NOBUS"))   != NULL) skipbus  = TRUE;

/**
*
*  Open the card
*
**/

	errval = BTICard_CardOpen(&hCard,cardnum);

	if (errval)
	{
		consoleprintf("\nError:  Unable to request handle for card number %u.",cardnum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		consoleexit(1);
	}

/**
*
*  Get Core handles
*
**/

	for (corecount=0;corecount<MAXCORES;corecount++)
	{
		hCore[corecount] = 0;
		errval = BTICard_CoreOpen(&hCore[corecount],corecount,hCard);
		if (errval) break;
	}

/**
*
*  Get and print Main Board Production Info
*
**/

	errval = TEST_CardInfo(cardnum,hCard);

	if (errval)
	{
		consoleprintf("\nError:  Unable to get info for card number %u.",cardnum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
	}

/**
*
*  Get and print Core Production Info
*
**/

	for (j=0;j<corecount;j++)
	{
		errval = TEST_CoreInfo(j,hCore[j]);

		if (errval && (errval != ERR_SELFXMTFAIL))
		{
			consoleprintf("\nError:  An error occurred while testing Core %c.",'A'+j);
			consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
			consoleprintf("\n");
		}

		core_results[j] = errval;
	}

	consoleprintf("\n");

/**
*
*  Display the test results
*
**/

	consoleprintf("\n-------------------------------------------------------------------------------");
	consoleprintf("\nTest results for Card %d (%s):",cardnum,BTICard_CardTypeStr(hCard));

	for (j=0;j<corecount;j++)
	{
		consoleprintf("\n    Core %c: %s",'A'+j,(core_results[j]) ? "** FAILED **" : "** PASSED **");
	}

	consoleprintf("\n");

/**
*
*  Close the card
*
**/

	errval = BTICard_CardClose(hCard);

	if (errval)
	{
		consoleprintf("\nError:  Unable to release handle for card number %u.",cardnum);
		consoleprintf("\n        (%s)",BTICard_ErrDescStr(errval,hCard));
		consoleprintf("\n");
		consoleexit(1);
	}

	consoleexit(0);
	return(0);
}

ERRVAL TEST_CardInfo(INT cardnum,HCARD hCard)
{
	ERRVAL errval=ERR_NONE;
	CHAR name[32];
	UINT32 ulVersion;
	UINT32 ulPlat;
	UINT32 ulDate;
	UINT16 cisbuf[CISSIZE];
	UINT16 ciscrc = 0;
	char str1[8],str2[8];
	UINT32 boardnum = 0;
	INT j;


	sprintf(name,"Card %u (%s)",cardnum,BTICard_CardTypeStr(hCard));

	consoleprintf("\n");
	consoleprintf("\nInformation from %s",name);
	consoleprintf("\n-------------------------------------------------------------------------------");

	consoleprintf("\nModel: %s", BTICard_CardProductStr(hCard));

/**
*
*  See if co-processor is present
*
**/

	errval = BTICard_CoProcGetInfo(&ulPlat,COPROCINFO_PLAT,hCard);

	if (!errval)
	{
		consoleprintf("\n");
		consoleprintf("\nCo-Processor present");

		switch (ulPlat)
		{
			default:
				consoleprintf("\n");
				consoleprintf("\nError:  Unknown co-processor firmware platform");
				break;

			case 0x4734:		//4GBoot Platform '4G'
				consoleprintf("\n4GBoot Firmware");
				break;

			case 0x4C4F:		//Platfrom 'OL'
				consoleprintf("\nOmniLinux");
				break;

			case 0x3741:		//Platfrom 'A7'
				consoleprintf("\nBTIApp7 Firmware");
				break;
		}

		errval = BTICard_CoProcGetInfo(&ulVersion,COPROCINFO_VERSIONEX,hCard);

		if (errval)
		{
			consoleprintf("\n");
			consoleprintf("\nError:  Unable to read co-processor firmware version");
		}
		else if (!ulVersion)
		{
			errval = BTICard_CoProcGetInfo(&ulVersion,COPROCINFO_VERSION,hCard);

			if (errval)
			{
				consoleprintf("\n");
				consoleprintf("\nError:  Unable to read co-processor firmware version");
			}
			else
			{
				consoleprintf("\nVersion %u.%u",(ulVersion>>8) & 0xFF,(ulVersion>>0) & 0xFF);
			}
		}
		else
		{
			consoleprintf("\nVersion %u.%u.%u",(ulVersion>>16) & 0xFF,(ulVersion>>8) & 0xFF,(ulVersion>>0) & 0xFF);
		}

		errval = BTICard_CoProcGetInfo(&ulDate,COPROCINFO_DATE,hCard);

		if (errval)
		{
			consoleprintf("\n");
			consoleprintf("\nError:  Unable to read co-processor platform");
		}
		else
		{
			consoleprintf("\nDate %02u/%02u/%04u",(ulDate>>8) & 0xFF,(ulDate>>0) & 0xFF,(ulDate>>16) & 0xFFFF);
		}
	}

/**
*
*  Read CIS information
*
**/

	errval = BTICard_CISRd(cisbuf,CISSIZE,CISTYPE_CARD,hCard);

	if (!errval && cisbuf[0x00]!=0xFFFF)
	{
		consoleprintf("\n");
		consoleprintf("\nMain board production information on %s.",name);
		consoleprintf("\n-------------------------------------------------------------------------------");

		for (j=0;j<CISSIZE-1;j++)
		{
			ciscrc = crc8(ciscrc,cisbuf[j] >> 0);
			ciscrc = crc8(ciscrc,cisbuf[j] >> 8);
		}

		consoleprintf("\nPCB Assembly #%05X",((((UINT32)cisbuf[0x00])<<16) + (((UINT32)cisbuf[0x01])<<0)) >> 12),
		consoleprintf("\nRevision %c",'@' + ((cisbuf[0x01]>>8) & 0x0F)),
		consoleprintf("\nOption #%02u",(cisbuf[0x01]>>0) & 0xFF),
		consoleprintf("\nModel #%04X",	  cisbuf[0x02]);
		consoleprintf("\nSerial #%04X",  cisbuf[0x03]);
		consoleprintf("\nDate %02X/%02X/%04X",(cisbuf[0x3C]>>8) & 0xFF,(cisbuf[0x3C]>>0) & 0xFF,(cisbuf[0x3D]>>0) & 0xFFFF);
		consoleprintf("\nVersion %04X",cisbuf[0x3E]);
		consoleprintf("\nCRC %04X",cisbuf[0x3F]);
		consoleprintf("\nExpected CRC %04X",ciscrc);

		sprintf(str1,"%3X",cisbuf[2]);
		errval = BTICard_CoProcGetInfo(&boardnum,COPROCINFO_PROD,hCard);
		sprintf(str2,"%3d",(INT)boardnum);

		if (strcmp(str1,str2) && (!errval))
		{
			consoleprintf("\n");
			consoleprintf("\nWarning: Main Board CIS does not match Firmware model!");
		}

		errval = ERR_NONE;
	}

	return(errval);
}

ERRVAL TEST_CoreInfo(INT corenum,HCORE hCore)
{
	ERRVAL errval=ERR_NONE;
	CHAR name[32];
	UINT16 bufmodel[32];
	UINT16 cisbuf[CISSIZE];
	UINT16 ciscrc = 0;
	char str1[8],str2[8];
	BOOL module_match = FALSE;
	UINT32 count1553;
	UINT32 count429;
	UINT32 count717;
	UINT32 count708;
	UINT32 count422;
	UINT32 countCSDB;
	UINT32 countDIO;
	UINT32 chinfo;
	INT j;
	UINT32 modelnum = 0;

	BOOL discin,discout,discseries,discshunt;
	CHAR buf[256];

	modelnum = BTICard_CardGetInfo(INFOTYPE_PROD,0,hCore);

	sprintf(name,"Core %c",corenum + 'A');

/**
*
*  Read CIS information
*
**/

	errval = BTICard_CISRd(cisbuf,CISSIZE,CISTYPE_IOMODULE,hCore);

	if (!errval && cisbuf[0x00]!=0xFFFF)
	{
		consoleprintf("\n");
		consoleprintf("\nI/O module production information on %s.",name);
		consoleprintf("\n-------------------------------------------------------------------------------");

		for (j=0;j<CISSIZE-1;j++)
		{
			ciscrc = crc8(ciscrc,cisbuf[j] >> 0);
			ciscrc = crc8(ciscrc,cisbuf[j] >> 8);
		}

		consoleprintf("\nPCB Assembly #%05X",((((UINT32)cisbuf[0x00])<<16) + (((UINT32)cisbuf[0x01])<<0)) >> 12),
		consoleprintf("\nRevision %c",'@' + ((cisbuf[0x01]>>8) & 0x0F)),
		consoleprintf("\nOption #%02u",(cisbuf[0x01]>>0) & 0xFF),
		consoleprintf("\nModel #%04X",	  cisbuf[0x02]);
		consoleprintf("\nSerial #%04X",  cisbuf[0x03]);
		consoleprintf("\nDate %02X/%02X/%04X",(cisbuf[0x3C]>>8) & 0xFF,(cisbuf[0x3C]>>0) & 0xFF,(cisbuf[0x3D]>>0) & 0xFFFF);
		consoleprintf("\nVersion %04X",cisbuf[0x3E]);
		consoleprintf("\nCRC %04X",cisbuf[0x3F]);
		consoleprintf("\nExpected CRC %04X",ciscrc);

		sprintf(str1,"%3X",cisbuf[2]);
		sprintf(str2,"%3d",(INT)modelnum);

		if (strcmp(str1,str2))
		{
			consoleprintf("\n");
			consoleprintf("\nWarning: I/O Module CIS does not match Firmware model!");
		}
		else
		{
			module_match = TRUE;
		}
	}
	else if (modelnum)
	{
		consoleprintf("\n");
		consoleprintf("\nERROR: I/O Module CIS not programmed!!!");
	}

/**
*
*  Survey protocols
*
**/

	consoleprintf("\n");
	consoleprintf("\nProtocols supported on %s.",name);
	consoleprintf("\n-------------------------------------------------------------------------------");

	count1553 = BTICard_CardGetInfo(INFOTYPE_1553COUNT,0,hCore);
	count429  = BTICard_CardGetInfo(INFOTYPE_429COUNT, 0,hCore);
	count717  = BTICard_CardGetInfo(INFOTYPE_717COUNT, 0,hCore);
	count708  = BTICard_CardGetInfo(INFOTYPE_708COUNT, 0,hCore);
	count422  = BTICard_CardGetInfo(INFOTYPE_422COUNT, 0,hCore);
	countCSDB = BTICard_CardGetInfo(INFOTYPE_CSDBCOUNT,0,hCore);
	countDIO  = BTICard_CardGetInfo(INFOTYPE_DIOCOUNT, 0,hCore);

	if (count1553) consoleprintf("\nMIL-STD-1553 - %d channel(s).",count1553);
	if (count429)  consoleprintf("\nARINC 429    - %d channel(s).",count429);
	if (count717)  consoleprintf("\nARINC 717    - %d channel(s).",count717);
	if (count708)  consoleprintf("\nARINC 708    - %d channel(s).",count708);
	if (count422)  consoleprintf("\nRS-422       - %d port(s).   ",count422);
	if (countCSDB) consoleprintf("\nCSDB         - %d channel(s).",countCSDB);
	if (countDIO)  consoleprintf("\nDIO          - %d bank(s).",countDIO);

/**
*
*  Get channel configuration
*
**/

	consoleprintf("\n");
	consoleprintf("\nChannel configuration of %s.",name);
	consoleprintf("\n-------------------------------------------------------------------------------");

	for (j=0;j<MAXCHANS;j++)
	{
		if (BTI1553_ChIs1553(j,hCore))
		{
			if (BTI1553_ChIsA(j,hCore))        consoleprintf("\nChannel %2d is a MIL-STD-1553 channel (Model A).",j);
			else if (BTI1553_ChIsB4(j,hCore))  consoleprintf("\nChannel %2d is a MIL-STD-1553 channel (Model B4).",j);
			else if (BTI1553_ChIsB32(j,hCore)) consoleprintf("\nChannel %2d is a MIL-STD-1553 channel (Model B32).",j);
			else if (BTI1553_ChIsC(j,hCore))   consoleprintf("\nChannel %2d is a MIL-STD-1553 channel (Model C).",j);
			else if (BTI1553_ChIsD(j,hCore))   consoleprintf("\nChannel %2d is a MIL-STD-1553 channel (Model D).",j);
			else if (BTI1553_ChIsM(j,hCore))   consoleprintf("\nChannel %2d is a MIL-STD-1553 channel (Model M).",j);
			else if (BTI1553_ChIsS(j,hCore))   consoleprintf("\nChannel %2d is a MIL-STD-1553 channel (Model S).",j);
			else if (BTI1553_ChIsX(j,hCore))
			{
				consoleprintf("\nChannel %2d is a MIL-STD-1553 channel (Custom Model).",j);

				if (BTI1553_ChGetInfo(INFO1553_MULTIMODE,j,hCore))
				{
					chinfo = BTI1553_ChGetInfo(INFO1553_MAXCOUNT,j,hCore);
					if (!chinfo) chinfo = 32;
					consoleprintf("\n        Terminal Count: %d",chinfo);
				}
				else
				{
					consoleprintf("\n        Terminal Count: 1");
				}

				chinfo = BTI1553_ChGetInfo(INFO1553_ERRORGEN,j,hCore);
				consoleprintf("\n        Error Generation: %s",(chinfo) ? "Yes" : "No");

				chinfo = BTI1553_ChGetInfo(INFO1553_CONCURMON,j,hCore);
				consoleprintf("\n        Concurrent Monitor: %s",(chinfo) ? "Yes" : "No");

				chinfo = BTI1553_ChGetInfo(INFO1553_MONFILTER,j,hCore);
				consoleprintf("\n        Monitor Filtering: %s",(chinfo) ? "Yes" : "No");

				chinfo = BTI1553_ChGetInfo(INFO1553_PARAM,j,hCore);
				consoleprintf("\n        Parametric Control: %s",(chinfo) ? "Yes" : "No");
			}
		}

		if (BTI429_ChIsRcv(j,hCore)) consoleprintf("\nChannel %2d is an ARINC 429 Receive channel.",j);
		if (BTI429_ChIsXmt(j,hCore)) consoleprintf("\nChannel %2d is an ARINC 429 Transmit channel.",j);

		if (BTI717_ChIsSelEnc(j,hCore))
		{
			if (BTI717_ChIsRcv(j,hCore))      consoleprintf("\nChannel %2d is an ARINC 717 BiPhase/Bipolar selectable Receive channel.",j);
			if (BTI717_ChIsXmt(j,hCore))      consoleprintf("\nChannel %2d is an ARINC 717 BiPhase/Bipolar selectable Transmit channel.",j);
		}
		else if (BTI717_ChIsBiPhase(j,hCore)) consoleprintf("\nChannel %2d is an ARINC 717 Biphase (T/R configurable) channel.",j);
		else if (BTI717_ChIsRcv(j,hCore))     consoleprintf("\nChannel %2d is an ARINC 717 Bipolar Receive channel.",j);
		else if (BTI717_ChIsXmt(j,hCore))     consoleprintf("\nChannel %2d is an ARINC 717 Bipolar Transmit channel.",j);

		if (BTI708_ChIsRcv(j,hCore)) consoleprintf("\nChannel %2d is an ARINC 708 Receive channel.",j);
		if (BTI708_ChIsXmt(j,hCore)) consoleprintf("\nChannel %2d is an ARINC 708 Transmit channel.",j);

		if (BTI422_PortIs422(j,hCore)) consoleprintf("\nPort %2d is an RS-422 Transmit/Receive port.",j);

		if (BTICSDB_ChIsRcv(j,hCore)) consoleprintf("\nChannel %2d is a CSDB Receive channel.",j);
		if (BTICSDB_ChIsXmt(j,hCore)) consoleprintf("\nChannel %2d is a CSDB Transmit channel.",j);

		if (BTIDIO_BankIsDIO(j,hCore))
		{
			discin		= BTIDIO_BankGetInfo(INFODIO_INPUT,j,hCore);
			discout		= BTIDIO_BankGetInfo(INFODIO_OUTPUT,j,hCore);
			discseries	= BTIDIO_BankGetInfo(INFODIO_SERIES,j,hCore);
			discshunt	= BTIDIO_BankGetInfo(INFODIO_SHUNT,j,hCore);

			INT stroff = 0;

			sprintf(buf + stroff,"\nBank %2d is a Discrete bank. (%2d ",j,BTIDIO_DiscreteGetCount(j,hCore));
			stroff += 32;
			if (discin)
			{
				sprintf(buf + stroff,"I");
				stroff += 1;
			}
			if (discout)
			{
				sprintf(buf + stroff,"O");
				stroff += 1;
			}
			sprintf(buf + stroff,", ");
			stroff += 2;

			if (discseries && discshunt)
			{
				sprintf(buf + stroff,"Series/Shunt");
				stroff += 12;
			}
			else if (discseries)
			{
				sprintf(buf + stroff,"Series");
				stroff += 6;
			}
			else if (discshunt)
			{
				sprintf(buf + stroff,"Shunt");
				stroff += 5;
			}

			sprintf(buf + stroff,")");

			consoleprintf(buf);
		}

	}

/**
*
*  Other information
*
**/

	consoleprintf("\n");
	consoleprintf("\nAdditional information for %s.",name);
	consoleprintf("\n-------------------------------------------------------------------------------");
	if (BTICard_IDRegRd(0,hCore)) consoleprintf("\nIFGA   ID REG: %04X",BTICard_IDRegRd(0,hCore));
	if (BTICard_IDRegRd(1,hCore)) consoleprintf("\nEDGA 1 ID REG: %04X",BTICard_IDRegRd(1,hCore));
	if (BTICard_IDRegRd(2,hCore)) consoleprintf("\nEDGA 2 ID REG: %04X",BTICard_IDRegRd(2,hCore));

/**
*
*  Get FW version and date from core directly (for minor minor number)
*
**/

	BTICard_CardGetInfoEx(bufmodel,sizeof(bufmodel)/sizeof(bufmodel[0]),NULL,0,hCore);

	consoleprintf("\nFirmware Version: %1lu.%-1lu.%-1lu",
					  bufmodel[11] & 0xFF,
					  bufmodel[12] & 0xFF,
					  bufmodel[13] & 0xFF);

	consoleprintf("\nFirmware Date: %02lu/%02lu/%04lu",
					  bufmodel[15] & 0x00FF,
					  bufmodel[16] & 0x00FF,
					  bufmodel[14] & 0xFFFF);

/**
*
*  Run core tests
*
**/

	consoleprintf("\n");
	consoleprintf("\nRunning tests for %s.",name);
	consoleprintf("\n-------------------------------------------------------------------------------");

	if (!errval)
	{
		consoleprintf("\nRunning I/O tests: ");
		if (skipio) consoleprintf("SKIPPED");
		else
		{
			errval = BTICard_CardTest0(hCore);
			consoleprintf("%s",(errval) ? "FAILED" : "PASSED");
		}
	}

	if (!errval)
	{
		consoleprintf("\nRunning memory tests: ");
		if (skipmem) consoleprintf("SKIPPED");
		else
		{
			errval = BTICard_CardTest1(hCore);
			consoleprintf("%s",(errval) ? "FAILED" : "PASSED");
		}
	}

	if (!errval)
	{
		consoleprintf("\nRunning commproc tests: ");
		if (skipcomm) consoleprintf("SKIPPED");
		else
		{
			errval = BTICard_CardTest2(hCore);
			consoleprintf("%s",(errval) ? "FAILED" : "PASSED");
		}
	}

	if (!errval)
	{
		consoleprintf("\nRunning databus tests: ");
		if (skipbus) consoleprintf("SKIPPED");
		else if (!module_match)
		{
			consoleprintf("SKIPPED");
			consoleprintf("\n");
			consoleprintf("\nWarning:  I/O Module CIS does not match Firmware model!");
			consoleprintf("\nAs a result, the core databus tests are being skipped.");
		}
		else
		{
			errval = BTICard_CardTest3(hCore);

			if (errval == ERR_NOTRCV)
			{
				consoleprintf("SKIPPED");
				consoleprintf("\n");
				consoleprintf("\nWarning:  There are no receivers to perform a bus test.");
				consoleprintf("\nAs a result, the core databus tests are being skipped.");
				errval = ERR_NONE;
			}
			else if (errval == ERR_NOTXMT)
			{
				consoleprintf("SKIPPED");
				consoleprintf("\n");
				consoleprintf("\nWarning:  There are no transmitters to perform a bus test.");
				consoleprintf("\nAs a result, the core databus tests are being skipped.");
				errval = ERR_NONE;
			}
			else if (errval == ERR_SELFXMTFAIL)
			{
				consoleprintf("FAILED");
				consoleprintf("\n");
				consoleprintf("\nError:  The databus self-test failed.  Please verify that");
				consoleprintf("\nall channels are properly terminated and that no external.");
				consoleprintf("\ndevices are transmitting to them.");
			}
			else
			{
				consoleprintf("%s",(errval) ? "FAILED" : "PASSED");
			}
		}
	}

	return(errval);
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
