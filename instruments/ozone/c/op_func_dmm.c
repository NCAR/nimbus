#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h> //For INB, OUTB
#include <unistd.h>
#include "config_extern.h"

/* ************************************
The DMM AD scan routine performs the following steps: sets up channel scan range; initiate conversion; wait to finish; read data; initiate conversion
again, the AD channel is automatically incremented by the DMM board from ChannelLow and looped around to ChannelLow when ChannelHigh is reached.
Time required for the scan equals 10us + nAverages * nChannels * 10us. In this case, the time is 10us + 160us * nAverages. For 10 samples, time is 1.7 ms.
Average the AD as requested by nAverages. ChannelLow: lowest channel scanned. Range 0-15. ChannelHigh: highest channel scanned. Range 0-15. */
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
