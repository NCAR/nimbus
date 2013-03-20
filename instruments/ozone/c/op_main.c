#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h> //For INB, OUTB
#include <unistd.h>
#include "config_base.h"
#include "config_extern.h"

BYTE GetStateParams ( float Result[] );
BYTE GetEngData( float Result[] );

/* ************************************
Main OP instrument control and DAQ program.
Must be executed as root or sudo. */
int main(int argc, const char* argv[])
{
	unsigned int n_iter=50;
	unsigned int i, ii, iWaitLoop=0, iSleep=50000, iRemCnts, iRC[n_iter], iSL[n_iter], iWL[n_iter];
	float fCntRatio, fCR[n_iter];
	unsigned int data[n_iter][2];
	float stateParams[n_iter][8], engData[n_iter][16]; // State parameters from Prometheus and AD housekeeping data from DMM.
	BYTE nStateParams, nEngParams;
	
		if (ioperm( BASE_QMM, 6, 1 )) //Set hardware port access for QMM board if possible.
		{
			printf("Access to QMM ports denied, exiting. Are you root or sudo?\n");
			return -1;
		}
		if (ioperm( BASE_PROM_DAQ, 15, 1 )) //Set hardware port access for Prometheus if possible.
		{
			printf("Access to Prometheus ports denied, exiting. Are you root or sudo?\n");
			return -1;
		}
		if (ioperm( BASE_DMM, 15, 1 )) //Set hardware port access for DMM board if possible.
		{
			printf("Access to hardware ports denied, exiting.\n");
			return -1;
		}
	
	if (argc == 1)
		;
	else iSleep = atoi(argv[1]); // Is the number of wait mic secs provided?
	
	QMM_SetAddresses( BASE_QMM ); // Set control register addresses.
	
	//Determine which data system this code is running on. OP-2 has QMM5 with 1 counter chip; OP-1 has QMM10 with 2 counter chips.
	BYTE isOP1 = QMM_CheckVersion(); // Returns 1 for QMM10.
	if ( isOP1 )
		printf("Instrument is OP1\n");
	else
		printf("Instrument is OP2\n");
	
	QMM_Setup( BASE_QMM, isOP1, DOWN_COUNTS ); //Set up the counter board and individual counter configurations.
	QMM_Restart_Counters( isOP1 ); //Set up the counters for the first time.
	
	for ( i=0; i < n_iter; i++ ) // Main DAQ loop. Should be infinite in the actual application.
	{
		iWaitLoop = QMM_WaitForTC1(); // Wait for the hardware loop counter to reach zero.
		QMM_Restart_Counters( isOP1 ); // Store count data, restart counters.
		QMM_Read( data[i] ); //Read the counter data. data[i] is the 1d array of floats.
		
		nStateParams = GetStateParams( stateParams[i] ); // Get state parameters: cell pressure and temperature.
		
		nEngParams = GetEngData( engData[i] );
		
		if ( iSleep )
			usleep(iSleep); // Sleep for most of the time, rather than loop.
		
		iRemCnts = QMM_ReadTimer(); // Get current value of the loop timer, which counts down from DOWN_COUNTS to 0.
		
		//Optimize sleep time by watching the remaining counts in the #1 counter and adjusting iSleep accordingly.
		fCntRatio = (float)iRemCnts / (float)DOWN_COUNTS;
		if ( fCntRatio > 0.8 ) //This probably means that the loop exceeded the TC of #1. Reset iSleep.
			iSleep = 50000; //Set sleep to some reasonable number allowing plenty of time in the loop.
		else if ( fCntRatio > 0.18 ) //Increase sleep time quickly
			iSleep = iSleep + 1000;
		else if ( fCntRatio > 0.10 ) //Increase sleep time slowly
			iSleep = iSleep + 100;
		else if ( fCntRatio < 0.05 ) //Decrease sleep time slowly
			iSleep = iSleep - 100;
		
		/* Record individual values to print them out outside the DAQ loop. */
		iWL[i] = iWaitLoop;
		fCR[i] = fCntRatio;
		iSL[i] = iSleep;
		iRC[i] = iRemCnts;
	}
	for ( i=0; i < n_iter; i++ ) //Get the printing loop outside of the DAQ loop, where it can mess up things.
	{
		//printf("Counts A: %d, B: %d. Waited: %d ; RemCnts: %d ; Ratio: %.3f ; set sleep to: %d\n", data[i][0], data[i][1], iWL[i], iRC[i], fCR[i], iSL[i]);
		printf("Ch.A: %d; Ch.B: %d;\n", data[i][0], data[i][1]);
		for ( ii=0; ii<=15; ii++ )
			printf("%.2f ", engData[i][ii]);
		printf("\n");
		for ( ii=0; ii<=7; ii++ )
			printf("%.2f ", stateParams[i][ii]);
		printf("\n");
	}
	return 0;
}

/* ************************************
Prometheus AD wrapper. Core functionality is in op_func_dmm.c and op_func_ad.c but there is still too much details of the AD
setup and processing to put them directly in the main. */
BYTE GetStateParams ( float Result[] )
{
	unsigned int AD_DELAY = 1000; //AD conversion delay. Hardware controlled 10us is not long enough when doing rapid repetitive conversions.
	static BYTE startChan = 0, endChan = 7; //Channel range to scan, flag
	static BYTE isBipolar = 0; // Prometheus AD is unipolar.
	BYTE nChannels = endChan - startChan + 1; // Set up nChannels. It will get returned to main.
	static float vRef = 5.0;
	short status;
	unsigned int i;
	struct CHAN_CONFIG chan_Conf[nChannels];
	static unsigned short nAverages = 10;
	
	PRM_Chan_Config( chan_Conf, nChannels ); // Fill in channel information once.
	
	for ( i=0; i<nChannels; i++ ) // Reset the output array.
		Result[i] = 0;
	
	status = PRM_AD_Scan( BASE_PROM_DAQ, startChan, endChan, nAverages, AD_DELAY, Result ); //Obtain AD values from the board.
	if ( status < 0)
	{
		printf("Prometheus AD Scan runtime problem detected, code %d\n", status);
		return -1;
	}
	
	PRM_AD_Voltage( Result, nChannels, vRef, isBipolar ); //Calculate AD voltages.
	
	for ( i=0; i<nChannels; i++ ) //Calculate actual values for each channel according to its data type.
		Result[i] = (*chan_Conf[i].Func) ( Result[i], chan_Conf[i].R1, chan_Conf[i].R2, vRef );
	
	// This channel is Baratron P. There is no plan to keep it in the future, so there is no transfer function for it in the op_func_ad.c
	Result[5] *= 300;
	
	return nChannels;
}

/* ************************************
engineering data acquisition wrapper. Core functionality is in op_func_dmm.c and op_func_ad.c but there is still too much details of
setup and processing to put them directly in the main. */
BYTE GetEngData( float Result[] )
{
	unsigned int AD_DELAY = 2000; //AD conversion delay. Hardware controlled 10us is not long enough when doing rapid repetitive conversions.
	static BYTE startChan = 0, endChan = 15; //Channel range to scan, flag
	BYTE nChannels = endChan - startChan + 1;
	static BYTE isBipolar = 1;
	static float vRef = 5.0;
	short status;
	unsigned int i;
	struct CHAN_CONFIG chan_Conf[nChannels];
	static unsigned short nAverages = 10;
	
	DMM_Chan_Config( chan_Conf, nChannels ); // Fill in channel information.
	
	for ( i=0; i<nChannels; i++ ) // Reset the output array.
		Result[i] = 0;
	
	status = DMM_AD_Scan( BASE_DMM, startChan, endChan, nAverages, AD_DELAY, Result ); //Obtain AD values from the board.
	if ( status < 0)
	{
		printf("DMM AD Scan runtime problem detected, code %d", status);
		return -1;
	}
	
	DMM_AD_Voltage( Result, nChannels, vRef, isBipolar ); //Calculate AD voltages.
	
	for ( i=0; i<nChannels; i++ ) //Calculate actual values for each channel according to its data type.
		Result[i] = (*chan_Conf[i].Func) ( Result[i], chan_Conf[i].R1, chan_Conf[i].R2, vRef );

	return nChannels;
}
