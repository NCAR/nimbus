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
/* ************************************
Channel configuration is defined here. */
int DMM_Chan_Config(struct CHAN_CONFIG chan_Conf[], BYTE nChannels)
{
	unsigned int i; // General counter
	
	/* Enter AD channel configuration: R1 and R2 are set up so that voltages from all sources are withing +/- 5V.
	R1 and R2 are 3-wire configuration dividers, with R2 being the one sampled.
	Func is the processing function used to convert the final AD voltage to engineering units. */
	for ( i=0; i < nChannels; i++ ) //Set default values to calculate voltages.
	{
		chan_Conf[i].R1 = 0; //Not all channels have dividers.
		chan_Conf[i].R2 = 1;
		chan_Conf[i].Func = VoltageCalc; //Initialize all processing functions to calc voltage. We will define the P, T below.
	}
	
	for ( i=0; i < 7; i++ ) //The first 8 channels are temperatures. Assign TemperatureCalc to process these.
	{
		chan_Conf[i].R1 = 20; //R1 is resistance between thermistor and VREF.
		chan_Conf[i].R2 = 0; //R2 is ressitance between thermistor and ground (zero in case of single ended AD.
		chan_Conf[i].Func = TemperatureCalc;
	}
	
	chan_Conf[7].Func = PressureCalc; //Channel 7 is is Cell A Absolute Pressure. R1 and R2 are not used and don't matter.
	
	chan_Conf[8].R1 = chan_Conf[15].R1 = 10; //5V power supply, PC104 5VDC
	chan_Conf[8].R2 = chan_Conf[15].R2 = 20;
	chan_Conf[9].R1 = chan_Conf[10].R1 = chan_Conf[11].R1 = 10; //+28, +24, -24VDC
	chan_Conf[9].R2 = chan_Conf[10].R2 = chan_Conf[11].R2 = 2;
	chan_Conf[12].R1 = chan_Conf[13].R1 = chan_Conf[14].R1 = 20; //+15, -15, +12VDC
	chan_Conf[12].R2 = chan_Conf[13].R2 = chan_Conf[14].R2 = 10;

	return 0;
}
