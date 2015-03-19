#include <stdio.h>
#include <sys/io.h> //For INB, OUTB
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "config_base.h"
#include "config_extern.h"

#define Base BASE_PROM_DAQ

/* ************************************
comments
*/
int PRM_DAC_Write(BYTE Channel, float volts)
{
	
	unsigned write_code=0; //MSB+LSB for 12 Bit DAC + channel select
	unsigned short int iAlert; //Hardware problem counter; it reaches just 3-4 in normal operation on 100MHz CPU.
	BYTE LSB, MSB; /* LSB contains lowest 8 bits for DAC. MSB contains 2 bit channel select, followed by 2 don't care bits, followed by the MSb's for the DAC */
	BYTE mask=0x0F; // used for channel selection
	
	
	//calculate write value for 0-10V operation and 2^12-1 = 4095 bit values. must be rounded. 
	write_code= (unsigned) round(volts/10 * 4095.0);
	
	//derive the LSB from the write code by isolaing the lower 8 bits 
	LSB=write_code & 0xFF;
	
	//find the channel number to append to the MSB, default is channel 0
	switch (Channel) 
	{	
		case 0 : mask = 0x00; break;
		case 1 : mask = 0x40; break;
		case 2 : mask = 0x80; break;
		case 3 : mask = 0xC0; break;
		//default: mask = 0x0F; break; 
	}
	
	MSB = ((write_code >> 8) & 0x0F) | mask;
		
	//wait while DAC is busy. DAC busy bit is bit 4 in register 3 when read. 
	for( iAlert=0; inb( Base + 3) & 0x10; iAlert++ ) // wait for DAC busy bit (bit 7) = 0
			if ( iAlert > 10000 ) return -20; //In case DAC conversion hangs up.
	
	
	// write the LSB to register 6
	outb( LSB, Base + 6 );

	// write MSB to register 7. Initiates output
	outb( MSB, Base + 7);

	return 0;
}
