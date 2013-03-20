#include <stdio.h>
#include <sys/io.h> //For INB, OUTB
#include <stdlib.h>
#include <unistd.h>
#include "config_extern.h"

/* ************************************
Prometheus AD scan routine. Requires hardware level IO permission to be established by the calling program.
Unlike the DMM AD scan, the Prometheus has a FIFO that is used to store the entire series of channel conversions.
There is no need to trigger every conversion. nAverages allows to average several AD conversions.
Procedure (based on code from DiamondSystems):
	Fifo reset
	Set A/D low and high channel
	Set A/D mode and scan enable bit
	Check wait bit, wait until wait bit settles
	Start A/D conversion
	Check conversion status bit, wait until conversion completes
	For low channel to high channel
		Read LSB, MSB, and combine
*/
int PRM_AD_Scan ( WORD base, BYTE ChannelLow, BYTE ChannelHigh, WORD nAverages, WORD nDelay, float Result[])
{
	static BYTE isFirstRun = 1; // If initialization has been done once, don't do it again.
	BYTE nChannels = ChannelHigh - ChannelLow + 1; // Incrementing channel counter.
	BYTE LSB, MSB; //Read-in bytes for the two 8-bit wide FIFO reads.
	short int Data = 0; // Signed. The AD is bi-polar on this board.
	BYTE gain = 2; //Bit 1, set gain to 4, which is 0-5V.
	unsigned int i, ii, j; //General counter.
	unsigned int iAlert; //Hardware problem counter; it reaches just 3-4 in normal operation on 100MHz CPU.
	
	for ( i=0; i < nChannels; i++ ) //Wipe out Result to make sure old data are removed.
		Result[i] = 0;
	
	outb( 0x10, base ); // FIFO reset. Write 1 to the RSTFIFO bit (bit 4). Does not affect any of the other bits
	
	if ( isFirstRun ) // Only initialize the AD on the first run. Otherwise, don't waste time.
	{
		outb( (ChannelHigh << 4) | ChannelLow, base + 2 );	/* set A/D channel. Low <= High */
		outb( (gain | 0x04), base + 3 ); // A/D mode: gain and scan enable bit (bit 2)
		for( iAlert=0; inb( base + 3) & 0x20; iAlert++ ) // wait for the WAIT bit (bit 5) to go to 0
			if ( iAlert > 10000 ) return -10; //In case AD channel setting hangs up, or Actel chip revision is less than V6H.
		isFirstRun = 0;
	}
	
	// Data accumulation loop for averaging.
	for ( ii=0; ii<nAverages; ii++)
	{
		outb( 0x80, base + 0 ); // start A/D conversion; set STRTAD bit to high
		for( iAlert=0; inb( base + 3) & 0x80; iAlert++ ) // wait for A/D busy bit (bit 7) = 0
			if ( iAlert > 10000 ) return -20; //In case AD conversion hangs up.
		
		/* iterate through from ChannelLow to ChannelHigh */
		for ( i = 0; i < nChannels; i++ ) 
		{
			LSB = inb(base + 0); /* read data LSB */
			MSB = inb(base + 1); /* read data MSB */
			Data = (MSB << 8) | LSB; /* combine LSB + MSB */
			Result[i] += (float)Data; //Save DATA in the corresponding Result.
			
			/*The PRM AD circuit channels start to drift a little bit if many conversions are performed repetitively during averaging.
			The loop below with 1000-2000 iterations on the Prometheus CPU, appears to slow down the averaging loop
			just enough to have unaffected averages.*/
			if ( nDelay )
				for ( j=0; j < nDelay; j++)
					;
		}
	}
	
	for ( i=0; i<nChannels; i++ ) //Average the Result.
		Result[i] /= (float)nAverages;
	return 0;
}

/* ************************************
Channel configuration is defined here. */
int PRM_Chan_Config(struct CHAN_CONFIG chan_Conf[], BYTE nChannels)
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
	
	for ( i=0; i < 4; i++ ) //The first 4 channels are temperatures. Assign TemperatureCalc to process these.
	{
		chan_Conf[i].R1 = 10; //R1 is resistance between thermistor and VREF.
		chan_Conf[i].R2 = 10; //R2 is ressitance between thermistor and ground (zero in case of single ended AD.
		chan_Conf[i].Func = TemperatureCalc;
	}
	
	chan_Conf[4].R1 = 0.5; //Channel 4 is the 5V reference for the pressure transducers.
	chan_Conf[4].R2 = 2;
	
	//chan_Conf[5].Func = PressureCalc; // Channel 5 is Baratron. It uses simply 300 * ADvoltage for its Func. Do it in Main above.
	chan_Conf[6].Func = PressureCalc; // Chan. 6 is Cell B absolute pressure.
	
	chan_Conf[7].Func = CurrentCalc; // Chan. 7 is 28V current.
	chan_Conf[7].R2 = 0.2;

	return 0;
}
