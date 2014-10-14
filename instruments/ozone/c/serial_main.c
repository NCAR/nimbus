#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h> //For INB, OUTB
#include <unistd.h>
#include <string.h>
#include <cstdlib>  // atoi()
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include "config_base.h"
#include "config_extern.h"
#include "UdpSocket.h"
//#include "rs232.h" //http://www.teuniz.net/RS-232/
//http://softexpert.wordpress.com/2007/10/18/how-to-connect-to-a-serial-port-in-linux-using-c/

#define COMPORT 1
#define CMD_OFFSET_RD 	0x09
#define CMD_OFFSET_WR	0x01
#define CMD_GAIN_RD 	0x0A
#define CMD_GAIN_WR 	0x02
#define CMD_CFG_WR 		0x03
#define CMD_CFG_RD 		0x0B
#define CMD_SGL_CVT 	0x80

BYTE GetStateParams ( float Result[], struct CHAN_CONFIG chan_Conf[], BYTE nChannels );
BYTE GetEngData( float Result[], struct CHAN_CONFIG chan_Conf[], BYTE nChannels );
BYTE ValveAdvance ( BYTE secA, BYTE secB, BYTE curr_position );
int PRM_DAC_Write(BYTE Channel, float volts);
void set_blocking (int fd, int should_block);
int set_interface_attribs (int fd, int speed, int parity);
unsigned long serial_fix_24( unsigned long serial_data);
unsigned long serial_fix_32( unsigned long serial_data);

/* ************************************
Main OP instrument control and DAQ program.
Call: op_main 10 15 60
where 10 is the number of seconds to sample channel A, 15 is the number of seconds to sample channel B, 60 is the number of minutes to aquire data.
Must be executed as root or sudo. */
int main(int argc, const char* argv[])
{
	unsigned int n_iter=99;
	unsigned int i, ii, iWaitLoop=0, iSleep=20000, iRemCnts, secA=10, secB=10;
	float fCntRatio, timeStamp;
	BYTE CurrPos, ExpectedPos=0; // Define valve position variables. ExpectedPos set to 0 to trap possible valve failure at init time.
	
	FILE *dataFile;
	time_t t; // Time stamp data storage.
	char dataFName[50];
	
	// Definitions for counters and AD data.
	unsigned int nChanPRM=8, nChanDMM=16; // Define number of channels.
	
	unsigned int nAuxVars=5; // Definitions of non-AD variables.
	unsigned int nDependentVars = nChanPRM + nChanDMM + nAuxVars - 1; // Number of dependent variables.
	char *auxVars[] = {"Tsecs","CountA","CountB","iSleep","ValvePos"};
	char *auxVarsLong[] = {"Elapsed time since 00:00:00 GMT, s","Counts channel A","Counts channel B","Idle time in the DAQ loop, usec","Stream selection valve position"};
	
	unsigned int data[2]; // Storage for QMM counts.
	float stateParams[nChanPRM], engData[nChanDMM]; // State parameters from Prometheus and AD housekeeping data from DMM.
	struct CHAN_CONFIG dmmChanConf[nChanDMM], prmChanConf[nChanPRM];
	
	int fd1;   // serial communication variables
	int wr,rd;
	char send_buff_a[16],send_buff_b[16];
	unsigned long serial_buff_a[1], serial_buff_b[1];
	int bits_a=0,bits_b=0;
	float volts_a=0, volts_b=0;	

	float val=0;// for Flow control DAC
	float flow_buffer[32];
	float median_flow, ctr_volt;
			
	char udp_buffer[32]; //UDP broadcast variables
	
	
	
	
		if (ioperm( BASE_QMM, 6, 1 )) //Set hardware port access for QMM board if possible.
		{
			printf("Access to QMM AD ports denied, exiting. Are you root or sudo?\n");
			return -1;
		}
		if (ioperm( BASE_PROM_DAQ, 15, 1 )) //Set hardware port access for Prometheus if possible.
		{
			printf("Access to Prometheus AD ports denied, exiting. Are you root or sudo?\n");
			return -1;
		}
		if (ioperm( BASE_DMM, 15, 1 )) //Set hardware port access for DMM board if possible.
		{
			printf("Access to Prometheus DIO ports denied, exiting. Are you root or sudo?\n");
			return -1;
		}

	
	if (argc > 1) // Command line parameters are used to set the flow times for each channel and total run time.
	{
		secA = atoi(argv[1]); // Seconds to sample in CellA. Defaults to 10.
		secB = atoi(argv[2]); // Seconds to sample in CellB. Defaults to 10.
		n_iter = atoi(argv[3]) * 60 * ( MAIN_FREQ / DOWN_COUNTS ); // Number of iterations. Defaults to 99, or 10s, above.
	}
	
	QMM_SetAddresses( BASE_QMM ); // Set control register addresses.
	DMM_Chan_Config( dmmChanConf, nChanDMM ); // Fill in DMM channel information.
	PRM_Chan_Config( prmChanConf, nChanPRM ); // Fill in PRM channel information.
	
	timeStamp = getTimeStamp ( &t ); // Get the time stamp to create file names.
	
	/* Determine which data system this code is running on. OP-2 has QMM5 with 1 counter chip; OP-1 has QMM10 with 2 counter chips.
	Depending on the instrument, create output file names. */
	BYTE isOP1 = QMM_CheckVersion(); // Returns 1 for QMM10.
	if ( isOP1 )
	{
		fprintf(stderr, "Instrument is OP1\n");
		sprintf( dataFName, getDate( &t, "/mnt/hda1/data/OP1_%Y%m%d_%H%M%S.dat") );
	}
	else
	{
		fprintf(stderr, "Instrument is OP2\n");
		sprintf( dataFName, getDate( &t, "/mnt/hda1/data/OP2_%Y%m%d_%H%M%S.dat") );
	}

	
	// Prepare output file.
	dataFile = fopen( dataFName, "w");
	
	fprintf( dataFile, "%d 1001\n", nDependentVars + 16 ); // First line; Add the number of non-var name lines in the header, subtract indep. var.
	fprintf( dataFile, "Romashkin, Pavel; pavel@ucar.edu\n");
	fprintf( dataFile, "National Center for Atmospheric Research, Research Aviation Facility\n");
	fprintf( dataFile, "Airborne Ozone Photometer %s\n", isOP1 ? "OP1" : "OP2");
	fprintf( dataFile, "Flow Control data file\n"); // Project name.
	fprintf( dataFile, "1 1\n");
	fprintf( dataFile, "%s   %s\n", getDate( &t, "%Y %m %d"), getDate( &t, "%Y %m %d") ); // Date of acquisition and processing is the same in this case.
	fprintf( dataFile, "0\n%s\n", auxVarsLong[0]); // Independent variable name.
	fprintf( dataFile, "%d\n", nDependentVars ); // Number of dependent variables. Less one for independent variable.
	for (i=0; i<nDependentVars; i++ )
		fprintf( dataFile, "1.0 "); // Scale factors.
	fprintf( dataFile, "\n");
	for (i=0; i<nDependentVars; i++ )
		fprintf( dataFile, "%d ", 999); // Missing value.
	fprintf( dataFile, "\n");
	for (i=1; i<nAuxVars; i++ )
		fprintf( dataFile, "%s\n", auxVarsLong[i]); // Aux var names. Skip the first name - it is the indfependent variable.
	for (i=0; i<nChanPRM; i++ )
		fprintf( dataFile, "%s\n", prmChanConf[i].varNameLong); // PRM var names.
	for (i=0; i<nChanDMM; i++ )
		fprintf( dataFile, "%s\n", dmmChanConf[i].varNameLong); // DMM var names.
	fprintf( dataFile, "1\nThis file contains raw data from the RAF %s instrument.\n1\n", isOP1 ? "OP1" : "OP2"); // Comment line

	
	
	// Write column names into data file.
	for (i=0; i<nAuxVars; i++ )
		fprintf( dataFile, "%s	", auxVars[i] ); // Aux var names.
	for (i=0; i<nChanPRM; i++) // State parameters will go in first.
		fprintf( dataFile, "%s	", prmChanConf[i].varName );
	for (i=0; i<nChanDMM; i++) // Eng data will go in next.
		fprintf( dataFile, "%s	", dmmChanConf[i].varName );
	fprintf( dataFile, "\n" );
	
	QMM_Setup( BASE_QMM, isOP1, DOWN_COUNTS ); //Set up the counter board and individual counter configurations.
	QMM_Restart_Counters( isOP1 ); //Set up the counters for the first time.
	
	/* Record starting valve position. If this returns 0, valve failed on first run, and the ExpectedPos will be zero.
	In this case, ValveAdvance will try to move the valve manually to re-activate it. */
	CurrPos = Valve_GetPosition( BASE_PROM_DAQ );
	
	/* ---- Initialize and prepare for UDP broadcast --- */
 	UdpSocket _socket(31222, "192.168.184.1");
	_socket.openSock(UDP_UNBOUND);
			
	// Write data to UDP
	sprintf( udp_buffer, "OP1,%sT%s ", getDate( &t, "%Y%m%d"), getDate( &t, "%H%M%S"));
	_socket.writeSock(udp_buffer,21);		
	
	
	/* ---- Initialize and Prepare COM1 port for prometheus ---- */ 
	fd1=open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NONBLOCK);
	if(fd1==1){
		perror("open_port: Unable to open /dev/ttyS0 -");
	}
	else{
		fcntl(fd1, F_SETFL ,O_ASYNC);
		printf("Port 1 has been sucessfully opened and %d is the file description\n",fd1);
	}

	set_interface_attribs (fd1, B115200, 0);  // set speed to 115200 bps, 8n1 (no parity)
	set_blocking (fd1, 0);                // set no blocking
	tcflush(fd1, TCIFLUSH);
	
	/* ---- initialize detector ADC through COM1 ---- */
	// serial converter data format:
	// byte 0 = 'r' , 'w', 'c', or 's' read/write/conversion read/ serial adc initialize 
	// byte 1 = # of return or write bytes
	// byte 2 = 8 bit command
	// byte 3-n = write data, null for r,c, or s 
	
	//set configuration register: 240Sps, frs=0, bipolar
	//sprintf(send_buff_a,"s1%c%c%c%c",0x00, 0x00, 0x0C, 0x00);
	//wr=write(fd1,&send_buff_a,strlen(send_buff_a));
	
	
	sprintf(send_buff_a,"s%c%c%c%c",0x00, 0x00, 0x0C, 0x00);
	wr=write(fd1,&send_buff_a,5);
		
	usleep (100000); 
		
	//sprintf(send_buff_b,"s2%c%c%c%c",0x00, 0x00, 0x0C, 0x00);
	//wr=write(fd1,&send_buff_b,strlen(send_buff_b));
	
						
	sprintf(send_buff_a,"r%c%c",4,CMD_CFG_RD);		
	wr=write(fd1, send_buff_a,3);//dummy
	rd=read(fd1, serial_buff_a,4);	
	wr=write(fd1, send_buff_a,3);
	rd=read(fd1, serial_buff_a,4);	
	printf("configuration value A = 0x%lx\n",serial_buff_a[0]);
	

	sprintf(send_buff_a,"r%c%c",4,CMD_OFFSET_RD);	
	wr=write(fd1, send_buff_a,3);
	rd=read(fd1, serial_buff_a,4);
	wr=write(fd1, send_buff_a,3);
	rd=read(fd1, serial_buff_a,4);
	printf("offset value A = 0x%lx\n",serial_buff_a[0]);
	
	
	sprintf(send_buff_a,"r%c%c",4,CMD_GAIN_RD);
	wr=write(fd1, send_buff_a,3);
	rd=read(fd1, serial_buff_a,4);
	wr=write(fd1, send_buff_a,3);
	rd=read(fd1, serial_buff_a,4);	
	printf("Gain value A = 0x%lx\n",serial_buff_a[0]);
	
	
	// set buffer to do a single conversion on each loop
	// will return a value for ADC_A and ADC_2 for one conversion command
	//sprintf(send_buff_a,"c1%c%c",8,CMD_SGL_CVT);
	sprintf(send_buff_a,"c%c%c",3,CMD_SGL_CVT);
	wr=write(fd1, send_buff_a,3);//first read is a dummy to start conversions
	usleep ((3 + 3) * 70 + 35500); // ~70uS/Byte, 4 byts out, 4 bytes in + ADC conversion time =~35mS
	rd=read(fd1, serial_buff_a,3);
	
	sprintf(send_buff_b,"C%c%c",3,CMD_SGL_CVT);
	wr=write(fd1, send_buff_b,3);//first read is a dummy to start conversions
	usleep ((3 + 3) * 70 + 35500); // ~70uS/Byte, 4 byts out, 4 bytes in + ADC conversion time =~35mS
	rd=read(fd1, serial_buff_a,3);
	
	
		// =============== Main DAQ loop start. Should be infinite in the actual application. ==================
	for ( i=0; i < n_iter; i++ )
	{
		iWaitLoop = QMM_WaitForTC1(); // Wait for the hardware loop counter to reach zero. iWaitLoop records how many interations the loop took.
		QMM_Restart_Counters( isOP1 ); // Store count data, restart counters.
		timeStamp = getTimeStamp ( &t ); // Get the time stamp for the moment the data are stored.
		
		/* Valve operation. It is physically slow, so we start it immediately once the counters were latched into Hold regirters.
		There are a few things that can happen. If CurrPos has not reached a good value, we use the ExpectedPos to provide ValveAdvance
		with expected position. If the valve is stuck between positions at startup, ExpectedPos will be zero and ValveAdvance will try to move
		the valve by issuing a series of movement commands. */
		if ( CurrPos )
			ExpectedPos = CurrPos;
		ExpectedPos = ValveAdvance ( secA, secB, ExpectedPos ); // Advance the valve if needed.
		
		QMM_Read( data, isOP1 ); //Read the counter data. data[i] is the 1d array of floats.
		GetStateParams( stateParams, prmChanConf, nChanPRM ); // Get state parameters: cell pressure and temperature, and others.
		GetEngData( engData, dmmChanConf, nChanDMM ); // Get engineering data.
		
		/*---------------- Flow Control -------------*/
		//printf("engData # 7=%f \n", engData[7]); // engData[7] is most recent MAF value in SLPM

		//Write MAF data do UPB buffer
	//	sprintf(udp_buffer , " %f ,", engData[7]);
		
		//calculate control voltage
		/* save flow values */
	//	flow_buffer[i%32]= engData[7];
		
		/* sort flow values */ 
		// sort_low_to_high(flow_buffer);
	//	median_flow = flow_buffer[16]; // only after sorting
		 
		/* some_look_up_table_function*/
		/* write desired voltage out to ctr_volt */
	//	ctr_volt = median_flow;// look_up_flow(median_flow)
		
		//write control voltage to DAQ
	//	PRM_DAC_Write( 0 , val ); /*replace arg2 with ctr_volt */

		/*---------------- End Flow Control ---------*/
		
		/* ---- Serial Comm ---- */
		if(i%2)
		{
			wr=write(fd1, send_buff_a,3);
			usleep ((3 + 3) * 70 + 35000); // ~70uS/Byte, 4 byts out, 4 bytes in + ADC conversion time =~35mS
			rd=read(fd1, serial_buff_a,3);
				
			if(rd <= 0)// ADC read fail
			{	
				perror("serial ADC read fail :");
				exit(1);	
			}
					
			if(rd>0)
			{
				bits_a = (int) serial_fix_24(serial_buff_a[0]);								
				volts_a= (float) bits_a * 5 / 16777215 / 128;
			}
		printf("A=%7.7f \t",volts_a);
		}
		
		else
		{
			wr=write(fd1, send_buff_b,3);
			usleep ((3 + 3) * 69.44 + 35000); // ~70uS/Byte, 4 byts out, 4 bytes in + ADC conversion time =~35mS
			rd=read(fd1, serial_buff_b ,3);
					
			if(rd <= 0)// ADC read fail
			{	
				perror("serial ADC read fail :");
				exit(1);	
			}
					
			if(rd>0)
			{
				bits_b = (int) serial_fix_24(serial_buff_b[0]);								
				volts_b= (float) bits_b * 5 / 16777215 / 128;
			}	
		printf("B=%7.7f\n",volts_b);
		}
		/* ---- End Serial Comm ---- */
		

		// Write data to UDP
		sprintf( udp_buffer, "O3,%d,%d,",bits_a,bits_b);
		_socket.writeSock(udp_buffer, strlen(udp_buffer)+1);		
				
		// Write data into file.
		fprintf( dataFile, "%.3f	", timeStamp );
		
		
		//for ( ii=0; ii<2; ii++ ) // Write counter data.
		//printf("A=%7.7f \t B=%7.7f\n",volts_a,volts_b);
		fprintf( dataFile, "%d	", bits_a);
		fprintf( dataFile, "%d	", bits_b );
		
		
		fprintf( dataFile, "%d	", iSleep ); // Record iSleep for now, see how that changes.
		fprintf( dataFile, "%d	", CurrPos ); // Record valve position. CurrPos at this point in code applies to the data taken above.
			
		for ( ii=0; ii<nChanPRM; ii++ ) // Write state parameters.
			fprintf( dataFile, "%.2f	", stateParams[ii] );
			
		for ( ii=0; ii<nChanDMM; ii++ ) // Write eng data.
			fprintf( dataFile, "%.2f	", engData[ii] );
		fprintf( dataFile, "\n" );		
		
		usleep(iSleep); // Sleep for most of the time, rather than loop.
		
		// Check valve position after sleeping. This value may be 0 if valve is still moving at this time.
		CurrPos = Valve_GetPosition( BASE_PROM_DAQ );
		
		iRemCnts = QMM_ReadTimer(); // Get current value of the loop timer, which counts down from DOWN_COUNTS to 0.
		
		//Optimize sleep time by watching the remaining counts in the #1 counter and adjusting iSleep accordingly.
		fCntRatio = (float)iRemCnts / (float)DOWN_COUNTS;
		if ( fCntRatio > 0.8 ) //This probably means that the loop exceeded the TC of #1. Reset iSleep.
			iSleep = 20000; //Set sleep to some small number allowing plenty of time in the loop.
		else if ( fCntRatio > 0.18 ) //Increase sleep time quickly
			iSleep = iSleep + 1000;
		else if ( fCntRatio > 0.10 ) //Increase sleep time slowly
			iSleep = iSleep + 100;
		else if ( fCntRatio < 0.05 ) //Decrease sleep time slowly
			iSleep = iSleep - 100;						
		
	} // =============== Main DAQ loop end. Should be infinite in the actual application. ==================
	
	//RS232_CloseComport(COMPORT); // close port
	close(fd1);
	exit(0);
}

/* ************************************ 
Prometheus AD wrapper. Core functionality is in op_func_dmm.c and op_func_ad.c but there is still too much details of the AD
setup and processing to put them directly in the main. */
BYTE GetStateParams ( float Result[], struct CHAN_CONFIG chan_Conf[], BYTE nChannels )
{
	unsigned int AD_DELAY = 1000; //AD conversion delay. Hardware controlled 10us is not long enough when doing rapid repetitive conversions.
	static BYTE startChan = 0, endChan = 7; //Channel range to scan, flag
	static BYTE isBipolar = 0; // Prometheus AD is unipolar.
	static float vRef = 5.0;
	short status;
	unsigned int i;
	static unsigned short nAverages = 10;
	
	if ( nChannels != endChan - startChan + 1) // Check if the number of channels is the same as scan range.
	{
		fprintf(stderr, "PROMETHEUS ANALOG CHANNEL NUMBER MISMATCH! QUITTING.");
		exit(0);
	}
	
	for ( i=0; i<nChannels; i++ ) // Reset the output array.
		Result[i] = 0;
	
	status = PRM_AD_Scan( BASE_PROM_DAQ, startChan, endChan, nAverages, AD_DELAY, Result ); //Obtain AD values from the board.
	if ( status < 0)
	{
		printf("Prometheus AD Scan runtime problem detected, code %d\n", status);
		return 0;
	}
	
	PRM_AD_Voltage( Result, nChannels, vRef, isBipolar ); //Calculate AD voltages.
	
	for ( i=0; i<nChannels; i++ ) //Calculate actual values for each channel according to its data type.
		Result[i] = (*chan_Conf[i].Func) ( Result[i], chan_Conf[i].R1, chan_Conf[i].R2, vRef );
		
	return nChannels;
}

/* ************************************ 
engineering data acquisition wrapper. Core functionality is in op_func_dmm.c and op_func_ad.c but there is still too much details of
setup and processing to put them directly in the main. */ 
BYTE GetEngData( float Result[], struct CHAN_CONFIG chan_Conf[], BYTE nChannels )
{
	unsigned int AD_DELAY = 2000; //AD conversion delay. Hardware controlled 10us is not long enough when doing rapid repetitive conversions.
	static BYTE startChan = 0, endChan = 15; //Channel range to scan, flag
	static BYTE isBipolar = 1;
	static float vRef = 5.0;
	short status;
	unsigned int i;
	static unsigned short nAverages = 10;
	
	if ( nChannels != endChan - startChan + 1) // Check if the number of channels is the same as scan range.
	{
		fprintf(stderr, "DMM ANALOG CHANNEL NUMBER MISMATCH! QUITTING.");
		exit(0);
	}

	for ( i=0; i<nChannels; i++ ) // Reset the output array.
		Result[i] = 0;
	
	status = DMM_AD_Scan( BASE_DMM, startChan, endChan, nAverages, AD_DELAY, Result ); //Obtain AD values from the board.
	if ( status < 0)
	{
		printf("DMM AD Scan runtime problem detected, code %d", status);
		return 0;
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
prev_position: this is the previous valve position fron the main level, used to decide how long to stay in this position. */

BYTE ValveAdvance ( BYTE secA, BYTE secB, BYTE curr_position )
{
	BYTE dio_conf = 1; // B is the output port; DIRCL is the input port; others are not used.
	BYTE expectedPos, currDelay;
	static short isFirstRun=1, cntr=0; // Persistent variables to keep track of valve timing.
	
	if ( isFirstRun )
		isFirstRun = DIO_Config( BASE_PROM_DAQ, dio_conf ); //First time, set up DIO ports: A,B,CH for output, CL for input.
	
	if ( curr_position == 0 ) // This should never happen. If we are here, the valve did not move properly on startup.
	{
		sleep( 2 ); // Wait for possible very slow hardware motion.
		Valve_Fwd( BASE_PROM_DAQ, 1); // Will try to move it "manually".
		sleep( 2 ); //Wait again for possible very slow hardware response.
		Valve_Fwd( BASE_PROM_DAQ, 2); // Try to move it from another position "manually".
		sleep( 2 ); //Sleep again, then read valve position again.
		curr_position = Valve_GetPosition( BASE_PROM_DAQ ); // Read position. If valve moved, it will be a valid number. Proceed.
		if ( curr_position == 0 )
			return 0; // Valve does not reposnd. It is up to the main level function to decide what to do.
	}
	
	currDelay = curr_position % 2 ? secA : secB; // Flowing ambient air, odd position, A; even position, B.
	cntr++;
	
	//Move valve if cycle number is a multiple of channel wait time and timer loop period.
	if ( (cntr % (MAIN_FREQ / DOWN_COUNTS * currDelay)) == 0 )
	{
		Valve_Fwd( BASE_PROM_DAQ, curr_position );
		expectedPos = (curr_position % 4) + 1;
		cntr = 0;
	}
	else
		expectedPos = curr_position;
	
	return expectedPos;
}


void
set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf("error %d from tggetattr", errno);
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                printf("error %d setting term attributes", errno);
}


int
set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf("error %d from tcgetattr", errno);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
                printf("error %d from tcsetattr", errno);
                return -1;
        }
        return 0;
} 

unsigned long
serial_fix_24( unsigned long serial_data)
{	
	unsigned long bits=0,msb,mb,lsb;
	
	serial_data<<=8;
	serial_data>>=8;
			
	msb=serial_data<<24;
	msb>>=8;
		
	lsb=serial_data<<8;
	lsb>>=24;
		
	mb=serial_data<<16;
	mb>>=24;
	mb<<=8;
		
	bits=msb|mb|lsb;		
	
	return bits;
}


unsigned long
serial_fix_32( unsigned long serial_data)
{	
	unsigned long bits=0,msb,mb,lb,lsb;
				
	msb=serial_data<<24;	
			
	lsb=serial_data>>24;
	
	mb=serial_data>>8;
	mb<<=24;
	mb>>=8;
	
	lb=serial_data<<8;
	lb>>=24;
	lb<<=8;
		
	bits= msb | mb | lb | lsb;			
	return bits;
}

