#include <stdio.h>
#include <sys/io.h> //For INB, OUTB
#include <stdlib.h>
#include <unistd.h>
#include "config_base.h"
#include "config_extern.h"

unsigned int AD_DELAY = 1000; //AD conversion delay. Hardware controlled 10us is not long enough when doing rapid repetitive conversions.

/* ************************************
This program is for checking the Prometheus AD functionality separately.
Calling sequence:
op_check_prom 10 2000
where 10 is the number of averages for the AD conversion, 2000 is the AD cirquit extra settling time in arbitrary loop iterations.
Main AD T and P data acquisition program. */
int main (int argc, const char* argv[])
{
	BYTE startChan = 0, endChan = 7; //Channel range to scan
	BYTE nChannels = endChan - startChan + 1;
	BYTE isBipolar = 0; // Prometheus AD is unipolar.
	float Result[nChannels], vRef = 5.0;
	short status;
	unsigned int i, ii;
	struct CHAN_CONFIG chan_Conf[nChannels];
	unsigned int doPrint;
	
	if (argc == 1)
		doPrint = 0; //If user supplied no argument, don't print.
	else doPrint = atoi(argv[1]); // Print? Also, this is the nAverages, if not a zero.
	
	if (argc == 3 ) // If we specified a loop delay on command line, use it.
		AD_DELAY = atoi(argv[2]);
	
	unsigned int nAverages = doPrint ? doPrint : 10;
	
	PRM_Chan_Config( chan_Conf, nChannels ); // Fill in channel information.
	if (ioperm( BASE_PROM_DAQ, 15, 1 )) //Set hardware port access if possible.
	{
		printf("Access to hardware ports denied, exiting.\n");
		return -1;
	}
	
	for ( i=startChan; i<=endChan; i++ )
		Result[i] = 0;
	
	printf ("Starting AD conversion\n");
	
	for ( ii=0; ii<nChannels; ii++ ) // Print var names.
		printf ("%s ", chan_Conf[ii].varName);
	printf("\n");
	
	for ( i=0; i < 10000; i++ )
	{
		status = PRM_AD_Scan( BASE_PROM_DAQ, startChan, endChan, nAverages, AD_DELAY, Result ); //Obtain AD values from the board.
		if ( status < 0)
		{
			printf("AD Scan runtime problem detected, code %d", status);
			return -1;
		}
		
		PRM_AD_Voltage(Result, nChannels, vRef, isBipolar); //Calculate AD voltages.
		
		for ( ii=0; ii<nChannels; ii++ ) //Calculate actual values for each channel according to its data type.
			Result[ii] = (*chan_Conf[ii].Func) ( Result[ii], chan_Conf[ii].R1, chan_Conf[ii].R2, vRef );
		
		//Result[5] *= 300; PressureBaratron was added, this calculation is now performed uniformly with the rest of them.
		
		if ( !(i % 5) && doPrint ) // Print data one per second.
		{
			i = 0;
			for ( ii=0; ii<nChannels; ii++ )
				printf ("%.2f ", Result[ii]);
			printf("\n");
		}
		usleep(100000);
	}
	
	return 0;
}
