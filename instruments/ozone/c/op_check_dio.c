#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h> //For INB, OUTB
#include <unistd.h>
#include "config_base.h"
#include "config_extern.h"

BYTE ValveAdvance ( BYTE secA, BYTE secB );
static int nSkips=0, nStops=0, prev_position=1; // Define persistent valve operation flags.

/* ************************************
Usage:
op_check_dio
No arguments. Typing r reads the valve state; f advances the valve; q exits the program.
Main DIO program for OP valve control and monitoring. */
int main ()
{
	char action = 'r';
	BYTE dio_conf = 1; //B is the output port; DIRCL is the input port; others are not used.
	BYTE CurrPosition;
	int i;
	
	if (ioperm( BASE_PROM_DAQ, 15, 1 )) //Set hardware port access if possible.
	{
		printf("Access to hardware ports denied, exiting.\n");
		return -1;
	}
	
	prev_position = Valve_GetPosition( BASE_PROM_DAQ );
	
	for ( i=1; i<300; i++ )
	{
		printf("Expected valve pos.: %d; ", ValveAdvance ( 2, 3 )); // Advance the valve.
		usleep ( 100000 ); // Less than 150 ms does not give valve enough time to reach position.
		// Check valve position after sleeping. Hopefully valve has finished moving and its position is now accurate.
		BYTE CurrPosition = Valve_GetPosition( BASE_PROM_DAQ );
		printf("Prev. pos.: %d; Curr. pos: %d;\n", prev_position, CurrPosition);
		prev_position = CurrPosition;
		//Valve_Check( CurrPosition, &nSkips, &nStops, &prev_position ); // Valve_Check increments nStops every time it is called.
		//printf("N-stops: %d; N-skips: %d, iteration: %d\n", nStops, nSkips, i);
	}
	return 0;

	
	DIO_Config( BASE_PROM_DAQ, dio_conf ); //First time, set up DIO ports: A,B,CH for output, CL for input.
	// Action loop. Typing r reads the valve state; f advances the valve; q exits the program.
	for ( ; action != 'q'; )
	{
		if ( action == 'r' )
			printf( "Prev. pos.: %d; Curr. pos.: %d\n", prev_position, Valve_GetPosition( BASE_PROM_DAQ ));
		else if ( action == 'f' )
		{
			Valve_Fwd( BASE_PROM_DAQ, prev_position );
			usleep(200000);
			CurrPosition = Valve_GetPosition( BASE_PROM_DAQ );
			printf("Prev. pos.: %d; New pos.: %d; ", prev_position, CurrPosition);
			Valve_Check( CurrPosition, &nSkips, &nStops, &prev_position );
			printf("N-stops: %d; N-skips: %d\n", nStops, nSkips);
		}
		action = getchar();
	}
	return 0;
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
	static int isFirstRun=1, cntr=0;
	
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
