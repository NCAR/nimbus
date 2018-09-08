#include <cstdio>
#include <unistd.h>
#include <QDateTime>
#include "snd_udp.h"
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>

static const int port = 41002;	// Port we have chosen to use.

/* -------------------------------------------------------------------- */
SendUDP::SendUDP()
{
	int rc;
	udp = new QUdpSocket();
	unsigned long count =0x00000000;
        unsigned long beam = 0x00000000;
	long bigBuffer[1440]={0};
	int seqNum =0;
	int seqCounter=0;
	while (count<606)
	{
		//beam will have a value of 5, 1, 7, or 3
	    if(count%2==0)
	{
			if (seqCounter%4 ==0)
			{
				beam =0x11111111;
			//	printf("1\n");
				seqCounter++;
			}
			else if (seqCounter%4==1)
			{
				beam =0x33333333;	
			//	printf("3\n");
				seqCounter++;
    				//beam = 0x1FCA055;	
			}
			else if (seqCounter%4==2)
			{
				seqCounter++;
				beam =0x77777777;
			//	printf("7\n");
				//beam =0x4A2CB71 ;	
			}
			else if (seqCounter%4==3)
			{	
				seqCounter++;
				beam =0x55555555;		
			//	printf("5\n");
				//beam = 0x34FB5E3;
			}	
	
			if (count%8==0) //increases seqNum every 8 packets, or every 4long+4short
			{
				seqNum++;
			}

	//check 3+ missing in a row sls will be packets 3-4-5, lsl will be packets 8-9-10 :engage %4 here %3|| %5 below
	//check sequetial short long and  long short missing: count% 4==0 here,count %3 below 		
	//check only every other long drops: count %4==0
	//send a long packet if count is even: count%2 ==0 
			
			bigBuffer[0]=beam;
			bigBuffer[1] = seqNum;
			//sprintf(buffer);
			//printf(buffer);
		printf("countOnBigPacket: %ld seqNum: %d\n",count,seqNum);	
			const char *cptr = (char *)bigBuffer;
			if (32<=count||count<=6){

				if ((rc = udp->writeDatagram(cptr, 1440 , QHostAddress::Broadcast, port)) <= 0)
					printf("return= %d\n", rc);
			}
	}	
	else if(count%2==1 &&(31<=count||count<=5)) 
	{
	//check only every fourth(?) short drops: count %3 ==0		
	//send a short packet if count is odd: count %2 ==1
    			//sprintf(buffer, " 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0\r\n");
			printf("countOnLittlePacket: %ld\n",count);
			const char *cptr = (char *)buffer;
    			if ((rc = udp->writeDatagram(cptr, 512, QHostAddress::Broadcast, port)) <= 0)
      				printf("return= %d", rc);
	}	

		count +=0x00000001;
	//	sleep(1);
	}
}
