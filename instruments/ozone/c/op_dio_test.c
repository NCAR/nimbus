#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h> //For INB, OUTB
#include <unistd.h>
#include "config_base.h"
#include "config_extern.h"
#define TRUE 1
#define FALSE 0
#define CMD_OFFSET_READ 0x09
#define CMD_OFFSET_WRITE 0x01
#define CMD_GAIN_READ 0x0A
#define CMD_GAIN_WRITE 0x02
#define CMD_CONFIG_READ 0x0B
#define CMD_CONFIG_WRITE 0x03
#define CMD_CONVERT_SINGLE 0x80
#define CMD_CONVERT_CONT 0xC0
#define CMD_CALIB_OFFSET 0x85
#define CMD_CALIB_GAIN 0x86
#define SYNC1 0xFF //serial port re-initialize sequence, send 15x
#define SYNC0 0xFE //end of serial re-initialize 
#define cs_hi 0x03
#define cs_a_lo 0x02
#define cs_b_lo 0x01
#define reset1 0x20000000
#define PORT_A BASE_PROM_DAQ+8
#define base  BASE_PROM_DAQ


////////////////////pin assignment Port A: 	A0 = SDO serial data Master to slave
////////////////////				A1 = CS_A select chip A
////////////////////				A2 = CS_B select chip B
////////////////////				A3 = CLK enable  (no longer used)
////////////////////				A4 = SCLK Serial Clock
////////////////////	Port C (High - CH)	C4 = SDI serial data slave to master 

int DIO_Config( WORD base, BYTE config)
{
	outb(config, base + 11);
	return 0;
}

int  SPI_WRITE_8( BYTE  spi_cmd , char Chip ) //write 8 bits on SPI
{
	unsigned int cs_cond, icmd, s_byte;
	int sbit, n, wait=0 ;
	switch(Chip)			// select appropiate chip/s
	{
		case 'A':    cs_cond=0x0c; break;
		case 'B':    cs_cond=0x0a; break;
		case '2':    cs_cond=0x08; break;
		default :    cs_cond=0x06; break;
	}

	icmd=(spi_cmd * 0x0202020202ULL & 0x010884422010ULL)%1023; //reverse byte order to shift 	

	if(cs_cond != 0x06)		// do not write if no chip is selected
	{
		for(n=1; n< 9 ; n++)
		{
			sbit = icmd & 0x01;		// isolate serial bit
			s_byte=sbit | cs_cond;		// include cs condtion to byte to write to port
			outb(s_byte , PORT_A);		// write serial bit
			for(wait=0; wait<200;wait++);	// wait
			outb((s_byte | 0x10), PORT_A);	// write clock high 
			icmd=icmd >> 1;			// shift command for next bit
			for(wait=0;wait<250;wait++);	// wait	
			outb(s_byte , PORT_A);		// write clock low
			for(wait=0; wait<760  ;wait++);
		}

	outb(0x06, PORT_A);	// pull CS high and disable clock
	}
	return 0;
}



int SPI_WRITE_32( unsigned long  spi_cmd , char Chip )
{
	unsigned int cs_cond, icmd, icmd1, icmd2, icmd3, icmd4, s_byte;
	int sbit, n, wait=0;
	switch(Chip)
	{
		case 'A':    cs_cond=0x0c; break;
		case 'B':    cs_cond=0x0a; break;
		case '2':    cs_cond=0x08; break;
		default :     cs_cond=0x06; break;
	}

	icmd1 = ((spi_cmd & 0xff) * 0x0202020202ULL & 0x010884422010ULL) % 1023;
	icmd2 = (((spi_cmd & 0xff00 ) >> 8) * 0x0202020202ULL & 0x010884422010ULL) % 1023;
	icmd3 = (((spi_cmd &0xff0000)>>16) * 0x0202020202ULL & 0x010884422010ULL) % 1023;
	icmd4 = (((spi_cmd &0xff000000)>>24) * 0x0202020202ULL & 0x010884422010ULL) % 1023;
	icmd= icmd4 | (icmd3 << 8) | (icmd2 << 16) | (icmd1 << 24);

//	outb(cs_cond , PORT_A);
	
	if(cs_cond != 0x06)		// do not write if no chip is selected
	{
		for(n=1; n< 33 ; n++)
		{
			sbit = icmd & 0x01;	// isolate seral bit
			s_byte=sbit | cs_cond;	// include cs condtion to byte to write to port
			outb(s_byte , PORT_A);	// write serial bit
			for(wait=0;wait<200;wait++);
			outb((s_byte | 0x10), PORT_A);
			icmd=icmd >> 1;		// shift command for next bit
			for(wait=0;wait<250;wait++);
			outb(s_byte , PORT_A);
			for(wait=0; wait<760  ;wait++);
		}

	outb(0x06, PORT_A);	// pull CS high and disable clock
	}
	return 0;
}

 unsigned long long SPI_READ( int bit_num , char Chip ) // read serial , Port C - C4 - pin 21

{
	int n , wait;
	unsigned long long rbit, ibit, buff=0, cs_cond;

	switch(Chip)			//select appropiate chip
	{
		case 'A':    cs_cond=0x0c; break;
		case 'B':    cs_cond=0x0a; break;
		default:     cs_cond=0x06; break;
	}

	outb( cs_cond , PORT_A);	// select chip
	for(n=1; n < (bit_num+1) ; n++)
	{
		outb( cs_cond | 0x10 , PORT_A);		//write clock high
		rbit = inb(BASE_PROM_DAQ + 10);		//read in serial bit
		ibit = rbit & 0x10;			//isolate serial bit from port
		buff = buff <<1;			//shift buffer
		buff = buff | ibit;			//save bit to buffer
		for(wait=0;wait<200;wait++);
		outb(cs_cond , PORT_A);			//write clock low
//		for(wait=0;wait<950;wait++);
	}
	outb(0x06, PORT_A);	// pull CS high and disable clock
	buff=buff/0x10; 		// remove trailing 0 left-over from shift. 
	return buff;
} 


void SYS_RESET (void)	//SYSTEM RESET SEQUENCE
{
	unsigned long long read_out=0x10000000; 
	SPI_WRITE_8(CMD_CONFIG_WRITE, '2');
	SPI_WRITE_32(reset1, '2');
	sleep(0.5);
	SPI_WRITE_8(CMD_CONFIG_WRITE, '2');
	SPI_WRITE_32( 0x00 , '2');
	sleep(0.5);

	SPI_WRITE_8( CMD_CONFIG_READ, 'A');
	read_out=SPI_READ( 32 , 'A' );		//chip A read to clear, print if invalid	
	if((read_out & 0x10000000) == 0x10000000){
		printf("Valid Reset, Chip A\n");}
	else{printf("Invalid Reset, Chip A\n");}

	SPI_WRITE_8( CMD_CONFIG_READ, 'B');
	read_out=SPI_READ( 32 , 'B' );		//chip B read to clear, print if invalid
	if((read_out & 0x10000000) == 0x10000000){
		printf("Valid Reset, Chip B\n");}
	else{printf("Invalid Reset, Chip B\n");}

	printf("Reset Sequence Complete \n");
	sleep(0.5);
}

void SERIAL_INITIALIZE(void) 	// INITIALIZE SERIAL PORT FOR SYNCHRONOUS COMMUNICATION
{	int i;			// TO INITALIZE SEND SYNC1 atleast 15 TIMES, THEN SYNC0
	for(i=0; i<25 ; i++){
		SPI_WRITE_8( SYNC1 , '2');}
	SPI_WRITE_8(SYNC0,'2');
	printf("Serial port initialized \n");
	sleep(0.5);
}

void GAIN_CALIB ( void )
{
	int n, i=100;		// number of samples to average for gain
	unsigned long long  array_a[i], array_b[i], output_a=0, output_b=0;
	SPI_WRITE_8( CMD_CONFIG_WRITE , '2');
	SPI_WRITE_32( 0x882000 , '2' ); 		//Latches set for gain calibration, bipolar supply, 6.25sps

	for(n=0; n<i ; n++)
	{ 
		SPI_WRITE_8( CMD_CALIB_GAIN , '2' );
sleep(0.9);
		SPI_WRITE_8( CMD_GAIN_READ , 'A');
		array_a[n] = SPI_READ(32, 'A');

		SPI_WRITE_8( CMD_GAIN_READ , 'B');
		array_b[n] = SPI_READ(32, 'B');

		output_a = output_a + array_a[n];
		output_b = output_b + array_b[n];
sleep(0.1);
	}
	

	output_a=output_a/i;
	output_b=output_b/i;

	printf("Gain A = 0x%x \n", output_a);
	printf("Gain B = 0x%x \n", output_b);
}

void OFFSET_CALIB ( void )
{
	int i=100, n=0;
	unsigned long long  offset_a[i], offset_b[i], output_a, output_b;
	SPI_WRITE_8( CMD_CONFIG_WRITE , '2');
	SPI_WRITE_32( 0x1082000 ,'2');		//Latches set for offset calibration, bipolar supply, 6.25sps
	
	for(n=0 ; n<i ; n++)
	{
		SPI_WRITE_8( CMD_CALIB_OFFSET,'2');
	sleep(0.9);

		SPI_WRITE_8( CMD_OFFSET_READ , 'A');
		offset_a[n] = SPI_READ(32, 'A');
	
		SPI_WRITE_8( CMD_OFFSET_READ , 'B');
		offset_b[n] = SPI_READ(32, 'B');

		output_a=output_a + offset_a[n];
		output_b=output_b + offset_b[n];
	}

	printf("Offset A = 0x%x \n", output_a);
	printf("Offset B = 0x%x \n", output_b);
	
	sleep(0.1);
}

void CS5530_CONFIG(void)
{
	SPI_WRITE_8( CMD_CONFIG_WRITE , '2');
//	SPI_WRITE_32( 0x01882000 , '2');		// Latches set for signal, bipolar supply, 6.25sps
	SPI_WRITE_32( 0X00082400 , 'A');		// LATCHES = 00, unipolar supply, 6.25sps
	sleep(1);
}


void CONV_READ_A( void )
{
	int ready_flag=1,n=0, i=15;
	unsigned long output_a;
	float volts_a=0, voltage[10]={};	
	for(n=0 ; n< i ; n++)
	{
		while( ready_flag != 0) 
		{ 
			ready_flag = SPI_READ(1, 'A');
		
		} 				// wait for ready flag

		ready_flag=1;
		sleep(0.5);

		SPI_WRITE_8( 0x00 , 'A');  // clear flag

		output_a = SPI_READ( 32 , 'A' );
//printf("volts 0x%x \n",(output_a >> 8));
		if( (output_a & 0x01) == 0x01)
		{ printf("OVER RANGE INPUT CHIP A \n"); }
		if( (output_a & 0x800) == 0){
//			voltage[n] = (output_a >> 8) * (715*10^-9);	// for +/-3V ref span
		}		
		else{
//			voltage[n] = -((output_a >> 8) & 0x7ff) * (715*10^-9);
		}

		voltage[n] = (output_a >> 8); // * (298*10^-9);
		volts_a=volts_a + voltage[n];
	}

	volts_a= (volts_a/10) * (0.000000289023);

	printf("Voltage A = %e \n",volts_a);	

}



void CONV_READ_B( void )
{
	int ready_flag=1,n=0, i=15;
	unsigned long output_b;
	float volts_b=0, voltage[10]={};	
	for(n=0 ; n< i ; n++)
	{
		while( ready_flag != 0) 
		{ 
			ready_flag = SPI_READ(1, 'A');
		
		} 				// wait for ready flag

		ready_flag=1;
		sleep(0.5);

		SPI_WRITE_8( 0x00 , 'A');  // clear flag

		output_b = SPI_READ( 32 , 'A' );
//printf("volts 0x%x \n",(output_b >> 8));
		if( (output_b & 0x01) == 0x01)
		{ printf("OVER RANGE INPUT CHIP B \n"); }
		if( (output_b & 0x800) == 0){
//			voltage[n] = (output_b >> 8) * (715*10^-9);	// for +/-3V ref span
		}		
		else{
//			voltage[n] = -((output_b >> 8) & 0x7ff) * (715*10^-9);
		}
	

		voltage[n] = (output_b >> 8); // * (298*10^-9);
	}

	for(n=0; n<i ; n++)
	{
		volts_b = volts_b + voltage[n];
	}
	volts_b=volts_b/10;

//	printf("Voltage B = %e \n",volts_b);	

}



//void SPI_CLK_CONFIG ( void )   // TIMER 0 INITIALIZATION
//{
//	outb(0x20, base + 4);	// timer control config
//	outb(0x28, base + 12);	// LSByte for frequency divisor
//	outb(0x00, base + 13);	
//	outb(0x00, base + 14);	// MSByte for freq. divisor
//	outb(0x02, base + 15);	// load counter
//	outb(0x10, base + 15);	// enable gate
//	outb(0x04, base + 15);	// enable counter
//}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

int main()
{
	unsigned long long  output_a, output_b; //, read_out=0x10000000;
	float volts_a, volts_b;

	if (ioperm( BASE_PROM_DAQ,16 , 1 )) //Set hardware port access if possible.
	{
		printf("Access to hardware ports denied, exiting.\n");
		return -1;
	}
  	BYTE  dio_conf=0x89;
	DIO_Config( BASE_PROM_DAQ, dio_conf ); //First time, set up DIO ports: A,B for output, CL,CH for input.

	printf("Running \n");
	

	SERIAL_INITIALIZE();
	SYS_RESET();
	GAIN_CALIB();
	OFFSET_CALIB();
	CS5530_CONFIG();	

	SPI_WRITE_32( CMD_CONVERT_CONT , '2');
	
	
	
while(1)
{					// conversion read loop

	CONV_READ_A();
//	CONV_READ_B();

/////////////////dont forget 1 osc_clk has to run both chips!!!!!!!!!!!!!!!!!!!!!!///////////////

// fprintf( dataFile, "%d \n", output ); 

	



}
}



