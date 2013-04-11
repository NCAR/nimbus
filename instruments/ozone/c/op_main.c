#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h> //For INB, OUTB
#include <unistd.h>
#include "config_base.h"
#include "config_extern.h"

BYTE GetStateParams ( float Result[] );
BYTE GetEngData( float Result[] );
BYTE ValveAdvance ( BYTE secA, BYTE secB );
static int prev_position; // Define persistent valve operation flags.

/* ************************************
Main OP instrument control and DAQ program.
Call: op_main 10, 15
where 10 is the number of seconds to sample channel A, 15 is the number of seconds to sample channel B.
Must be executed as root or sudo. */
int main(int argc, const char* argv[])
{
	unsigned int n_iter=5000;
	unsigned int i, iWaitLoop=0, iSleep=50000, iRemCnts, secA=10, secB=10;
	float fCntRatio;
	unsigned int data[2];
	float stateParams[8], engData[16]; // State parameters from Prometheus and AD housekeeping data from DMM.
	BYTE nStateParams, nEngParams, CurrPosition;
	
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
	
	if (argc > 1) // Command line parameters.
	{
		secA = atoi(argv[1]);
		secB = atoi(argv[2]);
	}
	
	QMM_SetAddresses( BASE_QMM ); // Set control register addresses.
	
	//Determine which data system this code is running on. OP-2 has QMM5 with 1 counter chip; OP-1 has QMM10 with 2 counter chips.
	BYTE isOP1 = QMM_CheckVersion(); // Returns 1 for QMM10.
	if ( isOP1 )
		printf("Instrument is OP1\n");
	else
		printf("Instrument is OP2\n");
	
	QMM_Setup( BASE_QMM, isOP1, DOWN_COUNTS ); //Set up the counter board and individual counter configurations.
	QMM_Restart_Counters( isOP1 ); //Set up the counters for the first time.
	
	prev_position = Valve_GetPosition( BASE_PROM_DAQ ); // Record starting valve position.
	
	for ( i=0; i < n_iter; i++ ) // Main DAQ loop. Should be infinite in the actual application.
	{
		iWaitLoop = QMM_WaitForTC1(); // Wait for the hardware loop counter to reach zero.
		QMM_Restart_Counters( isOP1 ); // Store count data, restart counters.
		
		ValveAdvance ( secA, secB ); // Advance the valve if needed.
		
		QMM_Read( data, isOP1 ); //Read the counter data. data[i] is the 1d array of floats.
		nStateParams = GetStateParams( stateParams ); // Get state parameters: cell pressure and temperature.
		nEngParams = GetEngData( engData );
		
		usleep(iSleep); // Sleep for most of the time, rather than loop.
		
		// Check valve position after sleeping. Hopefully valve has finished moving and its position is now accurate.
		CurrPosition = Valve_GetPosition( BASE_PROM_DAQ );
		if ( CurrPosition != 15 )
			prev_position = CurrPosition; // If read before valve stops, position will be 15.
		//printf("Prev. pos.: %d; Curr. pos: %d; ", prev_position, CurrPosition);
		
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
	static short isFirstRun=1;
	
	//if ( isFirstRun )
		isFirstRun = PRM_Chan_Config( chan_Conf, nChannels ); // Fill in channel information once.
	
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
	static short isFirstRun=1;
	
	//if ( isFirstRun )
		isFirstRun = DMM_Chan_Config( chan_Conf, nChannels ); // Fill in channel information.
	
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

/* ************************************
Valve operation wrapper. Implements valve advancing and status read. Note: valve position read-out can be inaccurate
if it occurs immediately after the valve moved. It takes some time for valve to rotate and for the sensor to output
the position. If a read is performed in the meantime a value with several position bits set may result.
Returns: expected valve position. May be the same as previous or advanced by one.
Parameters:
secA: number of seconds to flow ambient sample through cell A (valve pos. odd);
secB: number of seconds to flow ambient sample through cell B (valve pos. even);
cycle_number: cycle number of the main DAQ loop. */
BYTE ValveAdvance ( BYTE secA, BYTE secB )
{
	BYTE dio_conf = 1; // B is the output port; DIRCL is the input port; others are not used.
	BYTE expectedPos, currChannel;
	static short isFirstRun=1, cntr=0;
	
	if ( isFirstRun )
		isFirstRun = DIO_Config( BASE_PROM_DAQ, dio_conf ); //First time, set up DIO ports: A,B,CH for output, CL for input.
	
	currChannel = prev_position % 2 ? secA : secB; // Flowing ambient air, odd position, A; even position, B.
	cntr++;
	
	//Move valve if cycle number is a multiple of channel wait time and timer loop period.
	if ( (cntr % (MAIN_FREQ / DOWN_COUNTS * currChannel)) == 0 )
	{
		Valve_Fwd( BASE_PROM_DAQ, prev_position );
		expectedPos = (prev_position % 4) + 1;
		cntr = 0;
	}
	else
		expectedPos = prev_position;
	
	return expectedPos;
}
