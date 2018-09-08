#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <netinet/in.h>
#include "rd_udp.h"
#include <string>
//#include <cstdlib> strtol(s.c_str(),0,10);

static const int port = 41002;
uint32_t beam0Count = 0;
uint32_t beam1Count = 0;
static uint32_t beam2Count = 0;
static uint32_t beam3Count = 0;
uint32_t  totalDropped = 0;

long  previousSeqCount[4]={[0]=0, [1]=0,[2]=0,[3]=0};

uint32_t  expectedSeqNum=0;
uint32_t  totalLongRecieved=0;
uint32_t  totalShortRecieved=0;


/* -------------------------------------------------------------------- */
RdUDP::RdUDP() : pts(0)
{
  for (int i = 0; i < 100; ++i)
    delay[i] = 0;

  udp = new QUdpSocket(this);
  QHostAddress	host;

  host.setAddress(INADDR_ANY);
  printf("conn = %d\n", udp->bind(host, port, QUdpSocket::ShareAddress));

  connect(udp, SIGNAL(readyRead()), this, SLOT(newData()));
}

/* -------------------------------------------------------------------- */

void RdUDP::newData()
{
  int dataByteLength = udp->readDatagram(buffer, 65000); 
  const uint32_t *lp = (uint32_t *)buffer;
  uint32_t sequenceNumber = lp[1];//was long, lp[2] for test program, lp[1] for lams
  
  if ( dataByteLength >= 1440)
  {
  	//uint32_t packet
//printf("Key : %X : %u ; %u: %u, %u, %u \n",lp[0], lp[0], lp[1], lp[2], lp[3], lp[4]);
 	totalLongRecieved++;
	uint32_t key = lp[0];
	if (key == 0x11111111)
	{
	   _beam =0;
	   beam0Count++;		  
        }
        else if (key == 0x33333333)
	{
	    _beam =1;
	    beam1Count++;
	}
	else if (key == 0x77777777)
	{
	   _beam =2;
	   beam2Count++;
	}
	else if (key == 0x55555555)
	{
	    _beam =3;
	    beam3Count++;
	}
	else 
	{
	     time(&rawtime);
	     printf("\n %s :5: Invalid beam identifier = %d.\n",ctime(&rawtime), key);
	     _beam =0;
 	}
        expectedSeqNum=previousSeqCount[_beam]+1;
// 	printf(" expectedSeqNum %ld : seqNum: %ld\n",  expectedSeqNum, sequenceNumber);
//	printf("totalLong: %ld totalShort: %ld", totalLongRecieved, totalShortRecieved);	 
	if (totalLongRecieved>totalShortRecieved+1)//lost a short
	{
		uint32_t temp = totalLongRecieved-totalShortRecieved-1;
		printf("lost short: have %u more long packets than short. long short  == %u : %u\n",temp, totalLongRecieved, totalShortRecieved );
		totalShortRecieved+=temp;
		totalDropped+=temp;
     	}
	/*else if( totalLongRecieved<totalShortRecieved+1) //lost a uint32_t. I think this, and shorts lost a short are redundant
	{
        	uint32_t temp = totalShortRecieved-totalLongRecieved;
		printf("have %ld more short packets than uint32_t:uint32_tlost\n", temp);
		totalDropped+=temp;
		totalLongRecieved += temp; 
	}*/

	        if ( expectedSeqNum==sequenceNumber)//everything is ok
 	        {
			time(&rawtime); 
 			uint32_t totalPacketCount = totalLongRecieved+totalShortRecieved;
			printf("\rBeam0:%u, Beam1:%u, Beam2:%u, Beam3:%u, dropped %u / %u ", beam0Count, beam1Count, beam2Count, beam3Count, totalDropped, totalPacketCount);
			fflush(stdout);
			//expectedSeqNum++;
		}
	/*	else if (expectedSeqNum ==1)//first packet recieved
		{
			expectedSeqNum=sequenceNumber+1;
			printf("FIRST!\n");
	
		}
	*/	else if ( expectedSeqNum <sequenceNumber)//missed more than one 
		{
			uint32_t temp = sequenceNumber -  expectedSeqNum;
			time(&rawtime);
			printf("\nOut of sequence:  %s    %u sequential packets missing for beam: %lu\n",ctime(&rawtime) ,temp, _beam );
			totalDropped+=temp;
			// expectedSeqNum+=temp;

		}else printf("packet count error\n");

        previousSeqCount[_beam]= sequenceNumber;

  }
  else if (dataByteLength>10 && dataByteLength <1400 )
  {
  //short packet
	totalShortRecieved++;
 	//printf("Hey we got a short totS:%ld totL:%ld\n", totalShortRecieved, totalLongRecieved);
 
	/*if (totalLongRecieved>totalShortRecieved)
	{
		uint32_t temp = totalLongRecieved-totalShortRecieved;
		printf("lost short:have %ld more uint32_t packets than short\n",temp );
		totalShortRecieved+=temp-1;//more totalShortSent, totalLongSent with this correction. 
		totalDropped+=temp;
     	}*/
	if( totalLongRecieved<totalShortRecieved)
	{
        	uint32_t temp = totalShortRecieved-totalLongRecieved;
		printf("lost long: have %u more short packets than long\n", temp);
		totalDropped+=temp;
		totalLongRecieved += temp; 
	}

	
  }
  else 
  {
  //neither long nor short
	  time(&rawtime);
	  printf("\n %s :5: Invalid Packet Size\n", ctime(&rawtime));
  }

//printf("beams:%u:%u:%u: %u :totdropped: %u :prevseqcount array: %ld :: %ld :: %ld :: %ld :expectedSeqNum: %u :totlong: %u :short: %u \n",beam0Count, beam1Count, beam2Count, beam3Count, totalDropped, previousSeqCount[0], previousSeqCount[1], previousSeqCount[2], previousSeqCount[3], expectedSeqNum, totalLongRecieved, totalShortRecieved);
}
