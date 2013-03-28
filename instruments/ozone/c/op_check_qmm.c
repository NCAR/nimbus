#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h> //For INB, OUTB
#include <unistd.h>
#include "config_base.h"
#include "config_extern.h"

/* ************************************
Usage:
op_check_counters [70000]
The program aquires data for n_iter iterations, then prints out the counts and some timing data.
Optional iSleep argument can be used to test various usleep times, however they will be automatically optimized by code.
Main OP V->F counter data acquisition program. */
int main(int argc, const char* argv[])
{
	unsigned int n_iter=50;
	unsigned int i=0, iWaitLoop=0, iSleep, iRemCnts, iRC[n_iter], iSL[n_iter], iWL[n_iter];
	float fCntRatio, fCR[n_iter];
	unsigned int data[n_iter][2];
	
	if (ioperm(BASE_QMM, 6, 1)) //Set hardware port access if possible.
	{
		printf("Access to hardware ports denied, exiting.\n");
		return -1;
	}
	
	if (argc == 1)
		iSleep = 50000; //If user supplied no argument, sleep for 50 ms initially.
	else iSleep = atoi(argv[1]); // Is the number of wait mic secs provided?
	printf("Starting counter read\n");
	
	QMM_SetAddresses( BASE_QMM ); // Set control register addresses.
	
	//Determine which data system this code is running on. OP-1 has QMM5 with 1 counter chip; OP-2 has QMM10 with 2 counter chips.
	BYTE isOP1 = QMM_CheckVersion(); // Returns 1 for QMM10.
	if ( isOP1 )
		printf("Instrument is OP1\n");
	else
		printf("Instrument is OP2\n");
	
	QMM_Setup( BASE_QMM, isOP1, DOWN_COUNTS ); //Set up the counter board and individual counter configurations.
	printf("Set up counters\n");
	QMM_Restart_Counters( isOP1 ); //Set up the counters for the first time.
	printf("Started counters\n");
	
	for ( i=0; i < n_iter; i++ ) // Main DAQ loop. Should be infinite in the actual application.
	{
		iWaitLoop = QMM_WaitForTC1(); // Wait for the hardware loop counter to reach zero.
		
		QMM_Restart_Counters( isOP1 ); // Store count data, restart counters.
		
		QMM_Read( data[i], isOP1 ); //Read the counter data.
		
		if ( iSleep )
			usleep(iSleep); // Sleep for most of the time, rather than loop.
		
		iRemCnts = QMM_ReadTimer(); // Get current value of the loop timer, which counts down from DOWN_COUNTS to 0.
		
		//Optimize sleep time by watching the remaining counts in the #1 counter and adjusting iSleep accordingly.
		fCntRatio = (float)iRemCnts / (float)DOWN_COUNTS;
		if ( fCntRatio > 0.8 ) //This probably means that the loop exceeded the TC of #1, reset sleep timer
			iSleep = 50000; //Set sleep to some reasonable number allowing plenty of time in the loop.
		else if ( fCntRatio > 0.18 ) //Increase sleep time quickly
			iSleep = iSleep + 1000;
		else if ( fCntRatio > 0.10 ) //Increase sleep time slowly
			iSleep = iSleep + 100;
		else if ( fCntRatio < 0.05 ) //Decrease sleep time slowly
			iSleep = iSleep - 100;
		//Record individual values to print them out outside the DAQ loop.
		iWL[i] = iWaitLoop;
		fCR[i] = fCntRatio;
		iSL[i] = iSleep;
		iRC[i] = iRemCnts;
	}
	for ( i=0; i < n_iter; i++ ) //Get the printing loop outside of the DAQ loop, where it can mess up things.
		printf("Counts A: %d, B: %d. Waited: %d ; RemCnts: %d ; Ratio: %.3f ; set sleep to: %d\n", data[i][0], data[i][1], iWL[i], iRC[i], fCR[i], iSL[i]);
	return 0;
}
