#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h> //For INB, OUTB
#include <unistd.h>
#include "config_base.h"
#include "config_extern.h"

/* ************************************
Usage:
op_check_dio
No arguments. Typing r reads the valve state; f advances the valve; q exits the program.
Main DIO program for OP valve control and monitoring. */
int main ()
{
	unsigned int nSkips=0, nStops=0, prev_position=0; // Define valve operation flags.
	char action = 'r';
	BYTE dio_conf = 1; //B is the output port; DIRCL is the input port; others are not used.
	BYTE CurrPosition;
	
	if (ioperm( BASE_PROM_DAQ, 15, 1 )) //Set hardware port access if possible.
	{
		printf("Access to hardware ports denied, exiting.\n");
		return -1;
	}
	
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
