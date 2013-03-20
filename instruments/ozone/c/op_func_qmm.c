#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h> //For INB, OUTB
#include <unistd.h>
#include "config_extern.h"
#include "config_qmm.h"

/* Define QMM registers. We will write a pointer to Control to point to a register, and read/write to Data to control the chip
or get the counts. The values here are added to the QMM base address. */
static WORD QMM_CONTROL_CHIP1 = 1; //Status/Control register, chip 1
static WORD QMM_CONTROL_CHIP2 = 5; //Status/Control register, chip 2
static WORD QMM_DATA_CHIP1 = 0; //Data register, chip 1.
static WORD QMM_DATA_CHIP2 = 4; //Data register, chip 2
//static WORD QMM_digout = 2; //Digital I/O port; +3 is the same register. This is not used in OP.
//static WORD QMM_interrupt = 6; //Interrupt control register; +7 is the same register. This is not used in OP.

/* ************************************
Sets the control register addresses. These are base address dependent and can not be established at compile time. */
void QMM_SetAddresses( WORD base )
{
	static BYTE isFirstTime = 1;
	if ( isFirstTime )
	{
		QMM_CONTROL_CHIP1 += base; // Set register values to actual addresses.
		QMM_CONTROL_CHIP2 += base;
		QMM_DATA_CHIP1 += base;
		QMM_DATA_CHIP2 += base;
		//QMM_digout += base;
		//QMM_interrupt += base;
	}
}
	
/* ************************************
QMM counter board data aquisition program. The value is 32 bits long. */
int QMM_Read(unsigned int data[])
{
	unsigned int temp;
	
	temp = QMM_Get_Register8( QMM_CONTROL_CHIP1, QMM_DATA_CHIP1, QMM_REG_HOLD2 ); //Get lower 2 bytes, chan A
	temp = (QMM_Get_Register8( QMM_CONTROL_CHIP1, QMM_DATA_CHIP1, QMM_REG_HOLD3 ) << 16) | temp; //Get upper two bytes chan A and shift.
	data[0] = temp;
	
	temp = QMM_Get_Register8( QMM_CONTROL_CHIP1, QMM_DATA_CHIP1, QMM_REG_HOLD4 ); //Get lower 2 bytes, chan B
	temp = (QMM_Get_Register8( QMM_CONTROL_CHIP1, QMM_DATA_CHIP1, QMM_REG_HOLD5 ) << 16) | temp; //Get upper two bytes chan B and shift.
	data[1] = temp;
	return 0;
}

/* ************************************
QMM counter board initialization program. Check if the instrument is OP1 and load the counters accordingly. */
int QMM_Setup(WORD base, BYTE isOP1, unsigned int DOWN_COUNTS )
{	
	//Reset the counters.
	outb( QMM_MASTER_RESET, QMM_CONTROL_CHIP1 );
	if ( isOP1 )
		outb( QMM_MASTER_RESET, QMM_CONTROL_CHIP2 );
	
	//Set up master mode registers. The chip is in 8 bit mode at start-up, so write 1 byte at a time.
	QMM_Set_Register8( QMM_CONTROL_CHIP1, QMM_DATA_CHIP1, QMM_REG_MASTERMODE, QMM_MASTER_CONFIG);
	if ( isOP1 )
		QMM_Set_Register8( QMM_CONTROL_CHIP2, QMM_DATA_CHIP2, QMM_REG_MASTERMODE, QMM_MASTER_CONFIG); //Both chips are the same, if present.
	
	//Set up the counters. See mode descriptions in the header file.
	QMM_Set_Register8( QMM_CONTROL_CHIP1, QMM_DATA_CHIP1, QMM_REG_MODE1, QMM_MODE_LOOP_CONFIG); // #1 is the hardware loop timer
	QMM_Set_Register8( QMM_CONTROL_CHIP1, QMM_DATA_CHIP1, QMM_REG_MODE2, QMM_MODE_A1_CONFIG); // #2 chip 1 is Chan. A LSB
	QMM_Set_Register8( QMM_CONTROL_CHIP1, QMM_DATA_CHIP1, QMM_REG_MODE3, QMM_MODE_A2_CONFIG); // #3 chip 1 is Chan. A MSB
	QMM_Set_Register8( QMM_CONTROL_CHIP1, QMM_DATA_CHIP1, QMM_REG_MODE4, QMM_MODE_B1_OP2_CONFIG); // #4 chip 1 is Chan. B LSB, OP-2
	QMM_Set_Register8( QMM_CONTROL_CHIP1, QMM_DATA_CHIP1, QMM_REG_MODE5, QMM_MODE_B2_OP2_CONFIG); // #5 chip 1 is Chan. B MSB, OP-2
	if ( isOP1 )
	{
		QMM_Set_Register8( QMM_CONTROL_CHIP2, QMM_DATA_CHIP2, QMM_REG_MODE2, QMM_MODE_B1_OP1_CONFIG); // #2 chip 2 is Chan. B LSB, OP-1
		QMM_Set_Register8( QMM_CONTROL_CHIP2, QMM_DATA_CHIP2, QMM_REG_MODE3, QMM_MODE_B2_OP1_CONFIG); // #3 chip 2 is Chan. B MSB, OP-1
	}

	//Prepare the Load registers for all counters. Load register data are what counters are reset to when they are re-started.
	QMM_Set_Register8( QMM_CONTROL_CHIP1, QMM_DATA_CHIP1, QMM_REG_LOAD1, DOWN_COUNTS); // Load the duration of timing loop here.
	QMM_Set_Register8( QMM_CONTROL_CHIP1, QMM_DATA_CHIP1, QMM_REG_LOAD2, 0);
	QMM_Set_Register8( QMM_CONTROL_CHIP1, QMM_DATA_CHIP1, QMM_REG_LOAD3, 0);
	QMM_Set_Register8( QMM_CONTROL_CHIP1, QMM_DATA_CHIP1, QMM_REG_LOAD4, 0); //OP-2
	QMM_Set_Register8( QMM_CONTROL_CHIP1, QMM_DATA_CHIP1, QMM_REG_LOAD5, 0); //OP-2
	if ( isOP1 )
	{
		QMM_Set_Register8( QMM_CONTROL_CHIP2, QMM_DATA_CHIP2, QMM_REG_LOAD2, 0); //OP-1
		QMM_Set_Register8( QMM_CONTROL_CHIP2, QMM_DATA_CHIP2, QMM_REG_LOAD3, 0); //OP-1
	}
	
	return 0;
}

/* ************************************
Send a 16 bit long command to a QMM register, 8 bit at a time. 8 bit mode is default at startup; later it is possible to go to 16 bit mocde.
ControlReg is the chip's Control / Status register; DataReg is the Data Register; Pointer is the mode pointer; Data is the data
to send to the chip. IOPERM must be set in the caller function for this to work. */
int QMM_Set_Register8(WORD ControlReg, WORD DataReg, WORD Pointer, WORD Data)
{
	outb( Pointer, ControlReg ); //Point to the register
	outb( Data & 0xFF, DataReg ); // Write LSB
	outb( Data >> 8, DataReg ); // Write MSB
	return 0;
}

/* ************************************
Read the desired register in 8 bit increments. Counters are 16 bits wide, in 8 bit mode they will read 8 bit at a time and
automatically increment the internal point for the next 8 bit read for the same countetr. The returned value is up to 16 bits long. */
unsigned int QMM_Get_Register8(WORD ControlReg, WORD DataReg, WORD Pointer)
{
	WORD lsb, msb;
	outb( Pointer, ControlReg ); //Point to the register
	lsb = inb( DataReg ); // Read LSB
	msb = inb( DataReg ); // Read MSB
	return (msb << 8) | lsb; // Form the word and return it.
}

/* ************************************
This is counter restart code. The counters latch by hardware, so this program will store the data in Hold resisters, unlatch the gate and
re-start the counters. They will start counting, and in the meantime the Hold registers can be read.
Hardware level access must be provided by the calling program. */
int QMM_Restart_Counters( BYTE isOP1 )
{
	static BYTE isFirstTime = 1;
	if ( isFirstTime )
	{
		outb( 233, QMM_CONTROL_CHIP1 ); //Set Toggle Out to High on #1, Chip1. This enables hardware Gates to allow counting.
		outb( 127, QMM_CONTROL_CHIP1 ); //Load and arm all 5 counters on Chip1. All of them start counting at this time.
		if ( isOP1 ) // This is OP-1, where channel A is on #2, #3 chip 1 and B is on #2, #3 chip 2.
			outb( 127, QMM_CONTROL_CHIP2 ); //Load and arm all 5 counters on Chip2. All of them start counting at this time.
		isFirstTime = 0;
	}
	else
		/* This is counter control section, telling counters to stop, save and prepare for the next sampling of the channels.
		The commands here are carried out as direct hardware output because this allows to set up several counters in one chip command. */
		if ( isOP1 ) // This is OP-1, where channel A is on #2, #3 chip 1 and B is on #2, #3 chip 2.
		{
			outb( 134, QMM_CONTROL_CHIP1 ); //Disarm and save #2, #3, Chip1
			outb( 95, QMM_CONTROL_CHIP1 ); //Load all counters, Chip1
			outb( 233, QMM_CONTROL_CHIP1 ); //Set Toggle Out to High on #1. This enables hardware Gates to allow counting.
		
			outb( 134, QMM_CONTROL_CHIP2 ); //Disarm and save #2, #3, Chip2
			outb( 95, QMM_CONTROL_CHIP2 ); //Load all counters, Chip2
		
			outb( 63, QMM_CONTROL_CHIP1 ); //Arm all counters on Chip1
			outb( 63, QMM_CONTROL_CHIP2 ); //Arm all counters on Chip2
		}
		else //OP-2, where channels A and B go to the same chip on QMM.
		{

			outb( 158, QMM_CONTROL_CHIP1 ); //Disarm and save #2, #3, #4, #5 Chip1
			outb( 95, QMM_CONTROL_CHIP1 ); //Load all counters, Chip1
			outb( 233, QMM_CONTROL_CHIP1 ); //Set Toggle Out to High on #1. This enables hardware Gates to allow counting.
			outb( 63, QMM_CONTROL_CHIP1 ); //Arm all counters, Chip1
		}
	return 0;
}
/* ************************************
This routine will return the remaining timer value as number of counts at the time of request. */
int QMM_ReadTimer()
{
	outb ( 161, QMM_CONTROL_CHIP1 ); //Save #1 to Hold register
	return QMM_Get_Register8( QMM_CONTROL_CHIP1, QMM_DATA_CHIP1, QMM_REG_HOLD1 );
}

/* ************************************
This routine will return 1 for QMM10 and 0 for QMM5. */
BYTE QMM_CheckVersion()
{
	return inb( QMM_CONTROL_CHIP2 ) != 255 ; //If chip2 in not present, reading its status returns 255.
}

/* ************************************
This routine will wait for Counter 1 on Chip 1 to reach terminal count (TC) (zero in this case) and return the number
of iterations it took in the loop to wait for the TC to be reached. */
int QMM_WaitForTC1()
{
	unsigned int iWaitLoop;
	for ( iWaitLoop=0; inb(QMM_CONTROL_CHIP1) & 2; iWaitLoop++ ) //Bit 2 will go Low on TC of #1 and stay there since #1 counts once and disarms.
			;
	return iWaitLoop;
}
