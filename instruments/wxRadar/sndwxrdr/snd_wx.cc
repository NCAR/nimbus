#include <iostream>
#include <fstream>

/**
*
*  Coppied from :
*  BTI708 DRIVER EXAMPLE 4  (10/23/2009)
*  Copyright (c) 2001-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
**/

/**
*
*  This program opens Card Number 0, and configures the
*  first compatible core to configure the first receive
*  channel to record it's data to the Sequential Monitor.
*  The program then writes the data to a file (EXAMP4.DAT).
*
*  An Event Log frequency of 0 is specifed, which will
*  not generate any Event Log List entries for thess channels.
*
*  The program will continue to record data to the file
*  until a key is pressed.
*
*  Note: Running this program for a long time could cause the file
*  being written to become very large.  Please be cautious while
*  running this example.
*
**/

#include "snd_wx.h" 
#include "BTICard.h"
#include "BTI708.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define	CARDNUM       0
#define	MAX_CORES     4
#define	MAX_CHANNELS  32

//static const int port = 25050;
static const int port = 1551;

SendUDP::SendUDP()
{

// Ballard variables
	HCARD  hCard;
	HCORE  hCore;
	ERRVAL errval;
	INT    corenum;
	INT    rcvchan=0;
	//FILE   *fh;
	//char   *opts;
	//INT    cursor = 1;

	UINT16 seqbuf[2048];
	UINT32 seqcount;
	UINT32 blkcnt;
	LPSEQRECORD708 pRec708;
	SEQFINDINFO sfinfo;
	//INT		ch;

// QT variables
        udp = new QUdpSocket(this);



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
		std::cerr << "\nError:  Either card number " << CARDNUM << " is not present, or";
		std::cerr << "\n        an error occurred (" << errval << ") opening the card.";
		std::cerr << "\n        (" << BTICard_ErrDescStr(errval,hCard) << "\n";
		std::cerr << "\n";
		exit(1);
	}

/**
*
*  Find the first ARINC 708 core with a receive channel.
*
**/

	for (corenum=0;corenum<MAX_CORES;corenum++)
	{
		errval = BTICard_CoreOpen(&hCore,corenum,hCard);
		if (errval) break;

		for (rcvchan=0;rcvchan<MAX_CHANNELS;rcvchan++)	if (BTI708_ChIsRcv(rcvchan,hCore)) break;

		if (rcvchan != MAX_CHANNELS) break;
	}

	if (errval || corenum == MAX_CORES || rcvchan == MAX_CHANNELS)
	{
		std::cerr << "\nError:  No ARINC 708 receive channels present in cardnum " << CARDNUM;
		std::cerr << "\n";
		BTICard_CardClose(hCard);
		exit(1);
	}

	std::cerr << "\nUsing ARINC 708 core #" << corenum;
	std::cerr << "\nUsing receive channel #" << rcvchan;
	std::cerr << "\n";

/**
*
*  Configure receive channel.
*
**/

	BTICard_CardReset(hCore);

//	errval = BTI708_ChConfig(CHCFG708_SEQALL | CHCFG708_TERMON | CHCFG708_VARBIT  ,rcvchan,hCore);
	errval = BTI708_ChConfig(CHCFG708_SEQALL | CHCFG708_TERMOFF | CHCFG708_VARBIT | CHCFG708_BITSYNC,rcvchan,hCore);

	if (errval)
	{
		std::cerr << "\nError:  An error was encountered (" << errval << ") while configuring";
		std::cerr << "\n        channel #" << rcvchan << "on core #" << corenum;
		std::cerr << "\n        (" << BTICard_ErrDescStr(errval,hCard) << ")";
		std::cerr << "\n";
		BTICard_CardClose(hCard);
		exit(1);
	}

/**
*
*  Build the receive channel messages.
*
**/

	errval = BTI708_RcvBuild(MSGCRT708_DEFAULT,0,1,rcvchan,hCore);

	if (errval)
	{
		std::cerr << "\nError:  An error was encountered (" << errval << ") while building";
		std::cerr << "\n        channel #" << rcvchan << "on core #" << corenum;
		std::cerr << "\n        (" << BTICard_ErrDescStr(errval,hCard) << ")";
		std::cerr << "\n";
		BTICard_CardClose(hCard);
		exit(1);
	}

/**
*
*  Configure the Sequential Monitor.
*
**/

	errval = BTICard_SeqConfig(SEQCFG_DEFAULT,hCore);

	if (errval < 0)
	{
		std::cerr << "\nError:  An error was encountered (" << errval << ") while configureing";
		std::cerr << "\n        the sequential monitor on core #" << corenum;
		std::cerr << "\n        (" <<BTICard_ErrDescStr(errval,hCard) << ")";
		std::cerr << "\n";
		BTICard_CardClose(hCard);
		exit(1);
	}

/**
*
*  Start operation of the card.
*
**/

	BTICard_CardStart(hCore);

/**
*
*  Loop until a ctrl-C is hit.
*
**/

        QByteArray QBData; 
        int rtcd = 0;
	std::string errstr;

	while (1)
	{
		seqcount = BTICard_SeqBlkRd(seqbuf,sizeof(seqbuf)/sizeof(seqbuf[0]),&blkcnt,hCore);

		BTICard_SeqFindInit(seqbuf,seqcount,&sfinfo);

                rtcd = udp->isOpen();
printf("isOpen gives return code %d - string = %s\n",rtcd, (udp->errorString().toStdString().c_str()));
                rtcd = udp->isWritable();
printf("isWritable gives return code %d\n",rtcd);
		while(!BTICard_SeqFindNext708(&pRec708,&sfinfo))
		{
			//fwrite(pRec708->data,1,sizeof(pRec708->data),fh);
			//if ((rc = udp->writeBlock(pRec708->data, sizeof(pRec708->data), host, port)) <= 0)
			//	std::cerr << "UDP write fail. Return = " << rc << "\n";
                        //udp->writeDatagram(pRec708->data, sizeof(pRec708->data),QHostAddress::Broadcast,port);
                        QBData.append((char*) pRec708->data, sizeof(pRec708->data));
                        rtcd = udp->writeDatagram(QBData, QHostAddress::Broadcast,port);
                        QBData.clear();
 
printf("Sending - Return code = %d - string = %s\n",rtcd, (udp->errorString().toStdString().c_str()));
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

}

