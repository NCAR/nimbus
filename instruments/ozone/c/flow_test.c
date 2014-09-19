/* 	This code is to test the functionality of the flow control loop for the NCAR OP-1 Ozone instrument.
		-D. Baker																						*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h> //For INB, OUTB
#include <unistd.h>
#include <string.h>
#include "config_base.h"
#include "config_extern.h"


//Prototypes
int DMM_AD_Scan(WORD base, BYTE ChannelLow, BYTE ChannelHigh, WORD nAverages, WORD nDelay, float Result[]);


int main()
{	//start main

	//Variables
	short int j;
	short status;
	unsigned int AD_DELAY = 2000;
	float result=0;
	static unsigned short nAverages = 10;
	BYTE startChan=7;
	BYTE endChan=7;


	//PC104 HW Access
	if (ioperm( BASE_PROM_DAQ, 15, 1 )) //Set hardware port access for Prometheus if possible.
		{
			printf("Access to Prometheus AD ports denied, exiting. Are you root or sudo?\n");
			return -1;
		}
	if (ioperm( BASE_DMM, 15, 1 )) //Set hardware port access for DMM board if possible.
		{
			printf("Access to DMM AD ports denied, exiting. Are you root or sudo?\n");
			return -1;
		}


	//start AD for flow from DMM
	for(j=0; j<100; j++)
	{
		status = DMM_AD_Scan( BASE_DMM, startChan, endChan, nAverages, AD_DELAY, &result ); //Obtain AD values from the board.
		
		if ( status < 0)
		{
			printf("DMM AD Scan runtime problem detected, code %d \n", status);
			return 0;
		}

		printf("current result is %f \n", result);
	
	}


}
//--------------end main------------------------------//


/*--------------------Functions----------------------------------------------*/

int DMM_AD_Scan(WORD base, BYTE ChannelLow, BYTE ChannelHigh, WORD nAverages, WORD nDelay, float Result[])
{
	static BYTE isFirstRun = 1; // If initialization has been done once, don't do it again.
	BYTE nChannels = ChannelHigh - ChannelLow + 1; //Number iterating from ChannelLow to ChannelHigh. Increments by 1 per iteration.
	BYTE channelNum = 0;
	BYTE LSB = 0; //The 4 leftmost bits are AD sample LSB, the 4 rightmost bits are the channel number for this AD.
	BYTE MSB = 0; //Most Significant Byte. The 8 leftmost bits of AD sample.
	unsigned int DATA = 0; //Buffer to store the AD readings.
	unsigned int iAlert; //Hardware problem counter; it reaches just 3-4 in normal operation on 100MHz CPU.
	unsigned int i, ii; //General counter.
		
	for ( i=0; i<nChannels; i++ ) //Wipe out Result to make sure old data are removed.
		Result[i] = 0;
	
	if ( isFirstRun ) // Only initialize the AD on the first run. Otherwise, don't waste time.
	{
		outb((ChannelHigh << 4) | ChannelLow, base + 2); //Set A/D channel and wait for the AD to settle, takes 10us.
		
		for( iAlert=0; (channelNum = inb(base + 11)) & 0x10; iAlert++ ) //The WAIT bit should go to 0 in 10us.
			if ( iAlert > 10000 ) return -10; //In case AD channel setting hangs up, or Actel chip revision is less than V6H.
		isFirstRun = 0;
	}
	
	for ( i=0; i < nAverages; i++ ) //This is the average accumulation loop.
	{
		do
		{
			outb(0x01, base); //Start AD conversion. Value written does not matter.
			for( iAlert=0; inb(base + 8) & 0x80; iAlert++) //Wait for AD to finish. Bit 7 will go to zero when done.
				if ( iAlert > 10000 ) return -20; //In case AD conversion hangs up.
			
			LSB	= inb(base + 0); //Read data LSB + A/D channel tag
			MSB	= inb(base + 1); //Read data MSB
			channelNum = (int)LSB & 15; //The lower four bits are channel number. Record it.			
			DATA = (MSB << 4) | (LSB>> 4); //Combine LSB + MSB, delete channel tag.
			Result[channelNum] += (float)DATA; //Save DATA in the corresponding Result.
			
			/*The DMM board channels start to drift if many conversions are performed repetitively during averaging.
			The loop below with 2000-3000 iterations on the Prometheus CPU, appears to slow down the averaging loop
			just enough to have unaffected averages.*/
			if ( nDelay )
				for (ii=0; ii < nDelay; ii++)
					;
		} while (channelNum < ChannelHigh);
	}
	
	for ( i=0; i<nChannels; i++ ) //Average the Result.
		Result[i] /= (float)nAverages;

	return 0;
}