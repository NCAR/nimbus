#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h> //For INB, OUTB
#include <unistd.h>
#include "config_extern.h"

/* ************************************
This will set up configuration of DIO ports. */
int DIO_Config( WORD base, BYTE config)
{
	outb( config, base + 11 );
	return 0;
}

/* ************************************
Advances valve to the next position. The DIO port controlling valve motion is port B.
I am guessing that when HI is output, the logic on the board will only stop valve motion when position
sencor 1 or 3 activate, and when LOW is output, only position sensors 2 or 4 will stop the motor. This
would explain the fact that the valve always goes to the other position, even when it skips. */
int Valve_Fwd( WORD base, BYTE valve_position )
{
	BYTE cmd;
	//valve_position = Valve_GetPosition( base );
	cmd = ( valve_position % 2 ) ? 0 : 32; // If valve position is even, set pin 14 HI, else set it LOW.
	outb( cmd, base + 9 ); //Output HI or LO to port B, bit 5.
	return 0;
}

/* ************************************
Valve position quality check. If the valve did not move, or moved a wrong number of steps, detect it. */
int Valve_Check( BYTE valve_position, unsigned int * nSkips, unsigned int * nStops, unsigned int * prev_position )
{
	static BYTE isFirstRun = 1; // If initialization has been done once, don't do it again. Only changed by Valve_Check.
	short int nSteps;
	
	if ( isFirstRun )
	{
		*prev_position = valve_position + 3; // On the first run, there is no skip so don't trigger it.
		isFirstRun = 0;
	}
	
	nSteps = valve_position - *prev_position;
	*prev_position = valve_position;
	
	if (( nSteps == 1 ) || ( nSteps == -3)) // Normal operation.
		return 0;
	else if( nSteps == 0 ) // Valve did not move since last check.
		(*nStops)++;
	else if (( nSteps == -1 ) || ( nSteps == 3 )) // Valve moved more than 1 position.
		(*nSkips)++;
	else
		return -1; // Something unforeseen happened.
	
	return 1; // Valve stopped or skipped.
}

/* ************************************
Reads current valve position from the valve control board. The DIO port reading valve position is port C.
Valve position value is returned. On a typical error, zero will be returned because it means that the position
was read before one of the sensors activated (between valve positions, zero position value). */
BYTE Valve_GetPosition( WORD base )
{
	BYTE valve_position;
	
	valve_position = inb( base + 10 ); // Reading from port C.
	
	if ( valve_position == 7 )
		return 4;
	else if ( valve_position == 11 )
		return 3;
	else if ( valve_position == 13 )
		return 2;
	else if ( valve_position == 14 )
		return 1;
	else
		return valve_position; // If the value read doesn't make sense, return the actual incorrect value.
}
