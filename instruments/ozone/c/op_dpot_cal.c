#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h> //For INB, OUTB
#include <unistd.h>
#include <string.h>
#include <cstdlib>  // atoi()
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>
#include "config_base.h"
#include "config_extern.h"

#define SLEEP_TIME 1000000

//#include "rs232.h" //http://www.teuniz.net/RS-232/
//http://softexpert.wordpress.com/2007/10/18/how-to-connect-to-a-serial-port-in-linux-using-c/

#define COMPORT 1

void set_blocking (int fd, int should_block);
int set_interface_attribs (int fd, int speed, int parity);
unsigned int serial_fix_24( unsigned char*);
unsigned long serial_fix_32( unsigned long serial_data);
void print_menu(void);
void reset_ARM(void);

	
/* ************************************
Digital Potentiometer User Calibration Program
************************************ */
int main(int argc, const char* argv[])
{

	unsigned int i=1,j=0;
	unsigned short bias;
	float fl_bias;
	int fd1;   // serial communication variables
	int wr,rd,fwr;
	char buf[16];
	int cal=0;
	int first=1;
	float timeStamp;
	int outflag=0;
	
	FILE *dataFile;
	time_t t; // Time stamp data storage.
	char dataFName[50];
	char *auxVars[] = {"Tsecs","Bias"};
	
	
	
	if (ioperm( BASE_PROM_DAQ, 15, 1 )) //Set hardware port access for Prometheus if possible.
	{
		printf("Access to Prometheus AD ports denied, exiting. Are you root or sudo?\n");
		return -1;
	}

	
	BYTE dio_conf = 1; // B is the output port; DIRCL is the input port; others are not used.
	DIO_Config( BASE_PROM_DAQ, dio_conf ); 
	
	/* ---- Initialize and Prepare COM1 port for prometheus ---- */ 
	fd1=open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NONBLOCK);
	if(fd1<0){
		perror("open_port: Unable to open /dev/ttyS0 -");
	}
	else{
		fcntl(fd1, F_SETFL ,O_ASYNC);
		printf("Port 1 has been sucessfully opened and %d is the file description\n",fd1);
        }

	set_interface_attribs (fd1, B115200, 0);  // set speed to 115200 bps, 8n1 (no parity)
	set_blocking (fd1, 1);                // set blocking
	tcflush(fd1, TCIFLUSH);			
	
	printf("This program is for user calibration of the digital potentiometer IC on the OP-1 Detector boards.\n");
	print_menu();	
	
/*	sprintf(buf, "d%c%c%c", 'a', 0x16, 0x1); // Use ADC control to release wiper protect
	wr=write(fd1, buf ,4);
	usleep(30000);
	sprintf(buf, "d%c%c%c", 'b', 0x16, 0x1); // Use ADC control to release wiper protect
	wr=write(fd1, buf ,4);
*/	
	while(i)
	{
		scanf("%c",buf);
		
		switch(buf[0])
		{
		
		case 'a':	//increment rdac 0 on board A by 1
					sprintf(buf, "d%c%c%c", 'a', 0x14, 0x0);
					wr=write(fd1, buf ,4);
					break;
			
		case 'b':	//increment rdac 0 on board B by 1
					sprintf(buf, "d%c%c%c", 'b', 0x14, 0x0);
					wr=write(fd1, buf ,4);
					break;	
					
		case 'c':	//increment rdac 1 on board A by 1
					sprintf(buf, "d%c%c%c", 'a', 0x14, 0x1);
					wr=write(fd1, buf ,4);
					break;
			
		case 'd':	//increment rdac 1 on board B by 1
					sprintf(buf, "d%c%c%c", 'b', 0x14, 0x1);
					wr=write(fd1, buf ,4);
					break;	
					
		case 'e':	//decrement rdac 0 on board A by 1
					sprintf(buf, "d%c%c%c", 'a', 0x6, 0x0);
					wr=write(fd1, buf ,4);
					break;
			
		case 'f':	//decrement rdac 0 on board B by 1
					sprintf(buf, "d%c%c%c", 'b', 0x6, 0x0);
					wr=write(fd1, buf ,4);
					break;						
			
		case 'g':	//decrement both rdacs on board A by 1
					sprintf(buf, "d%c%c%c", 'a', 0x7, 0x0);
					wr=write(fd1, buf ,4);
					break;
					
		case 'h':	//decrement both rdacs on board B by 1
					sprintf(buf, "d%c%c%c", 'b', 0x7, 0x0);
					wr=write(fd1, buf ,4);
					break;
					
		case 'i':	//decrement rdac 1 on board A by 1
					sprintf(buf, "d%c%c%c", 'a', 0x6, 0x1);
					wr=write(fd1, buf ,4);
					break;
			
		case 'j':	//decrement rdac 1 on board B by 1
					sprintf(buf, "d%c%c%c", 'b', 0x6, 0x1);
					wr=write(fd1, buf ,4);
					break;					
					
		case 'k': 	//increment both rdacs on board A by 1
					sprintf(buf, "d%c%c%c", 'a', 0x15, 0x0);
					wr=write(fd1, buf ,4);
					break;
					
		case 'l':	//increment both rdacs on board A by 1
					sprintf(buf, "d%c%c%c", 'b', 0x15, 0x0);
					wr=write(fd1, buf ,4);
					break;
					
		case 'n':	//increment both rdacs on board A by 6 dB
					sprintf(buf, "d%c%c%c", 'a', 0x13, 0x0);
					wr=write(fd1, buf ,4);	
					break;	
					
		case 'o':	//increment both rdacs on board B by 6 dB
					sprintf(buf, "d%c%c%c", 'b', 0x13, 0x0);
					wr=write(fd1, buf ,4);	
					break;			
					
		case 'm':	//print menu
					print_menu();
					break;
					
		case 'q':	//Quit program dpot_cal
					printf("Ending Program\n");
					i=0; 
					sprintf(buf, "d%c%c%c", 'a', 0x16, 0x0); // Use ADC control to enable wiper protect
					wr=write(fd1, buf ,4);
					usleep(30000);
					sprintf(buf, "d%c%c%c", 'b', 0x16, 0x0); // Use ADC control to enable wiper protect
					wr=write(fd1, buf ,4);
					break;			
					
		case 'p':	//decrement both rdacs on board A by 6dB (right shift bit value for half)
					sprintf(buf, "d%c%c%c", 'a', 0x5, 0x0);
					wr=write(fd1, buf ,4);	
					break;	
					
		case 'r':	//decrement both rdacs on board B by 6dB (right shift bit value for half)
					sprintf(buf, "d%c%c%c", 'b', 0x5, 0x0);
					wr=write(fd1, buf ,4);	
					break;	
					
		case 's':	// syncronize ADC Serial interface // does not initialize
					//sprintf(buf,"%c",'s');
					//wr=write(fd1, buf ,1);
					if(outflag==1)
					{
						outb( 0x0 ,BASE_PROM_DAQ+8);
						printf("outb low\n");
						outflag=0;
					}
					
					else if(outflag==0)
					{
						outb( 0xFF ,BASE_PROM_DAQ+8);
						printf("outb high\n");
						outflag=1;
					}
					
					break;			
					
		case 't':	//initialize SD-ADC A
					sprintf(buf,"i%c%c%c%c",0x01, 0x80, 0x24, 0x00);
					wr=write(fd1,buf,5);
					break;		
					
		case 'u':	//initialize SD-ADC B
					sprintf(buf,"j%c%c%c%c",0x01, 0x80, 0x24, 0x00);
					wr=write(fd1,buf,5);
					break;										
					
		case 'v':   // save both wiper positions on board A to NVM
					sprintf(buf, "d%c%c%c", 'a', 0x2, 0x0);
					wr=write(fd1, buf ,4);
					usleep(100);
					sprintf(buf, "d%c%c%c", 'a', 0x2, 0x1);
					wr=write(fd1, buf ,4);
					break;
					
		case 'w':   // save both wiper positions on board B to NVM
					sprintf(buf, "d%c%c%c", 'b', 0x2, 0x0);
					wr=write(fd1, buf ,4);
					usleep(100);
					sprintf(buf, "d%c%c%c", 'b', 0x2, 0x1);
					wr=write(fd1, buf ,4);
					break;
					
		case 'x':	//get bias ADC A value
					sprintf(buf, "%c", 'a');
					wr=write(fd1, buf ,1);
					usleep(100000);
					read(fd1,&bias,2);
					bias=ntohs(bias);
					printf("Bias A = %f\n",(float)bias*3.3/4095 );
					break;
		
		case 'y':	// get bias ADC B value
					sprintf(buf, "%c", 'b');
					wr=write(fd1, buf ,1);
					usleep(100000);
					read(fd1,&bias,2);
					bias=ntohs(bias);
					printf("Bias B = %f\n",(float)bias*3.3/4095 );
					break;
					
		case 'z':	usleep(1000000);	
					cal=1;
					timeStamp = getTimeStamp ( &t ); // Get the time stamp to create file names.
					sprintf( dataFName, getDate( &t, "/mnt/hda1/data/dpotcal%Y%m%d_%H%M%S.dat") );
					// Prepare output file.
					dataFile = fopen( dataFName, "w");
					fprintf( dataFile, "%s,%s", auxVars[0],auxVars[1] );
					fprintf( dataFile, "\n" );
					first=0;
					break;
		}//end switch

		if(cal)
		{	
			sprintf(buf,"%c",'s');
			wr=write(fd1, buf ,1);
			usleep(900);
			
			sprintf(buf,"j%c%c%c%c",0x01, 0x80, 0x24, 0x00);
			wr=write(fd1,buf,5);
			usleep(3600);
			
			for(i=0; i<6 ; i++)//decrement both Rdacs to 0
			{
				sprintf(buf, "d%c%c%c", 'b', 0x5, 0x0);
				wr=write(fd1, buf ,4);	
				usleep(100000);
			}
			
			printf("rdacs A = 0x0000000000\n");

			printf("Incrementing Rdac 0 A by 16...\n");
			for(i=0; i<1; i++)
			{
					//get bias ADC B value
					sprintf(buf, "%c", 'b');
					wr=write(fd1, buf ,1);
					timeStamp = getTimeStamp ( &t ); // Get the time stamp
					usleep(10000);
					//printf("pre-read passed\n");
					rd=read(fd1,&bias,2);
					if(rd!=2)
					{
						printf("read error\n");
						exit(0);	
					}
					
					bias=ntohs(bias);
					fl_bias=(float)bias*3.3/4095;
					printf("%5.4f\n",fl_bias);
					fwr = fprintf( dataFile, "%.6f	", timeStamp );
					if(fwr <= 0)
					{
						printf("file write error\n");
						exit(0);
					}

					fprintf( dataFile, "%5.4f	", fl_bias );
					fprintf( dataFile, "\n" );

					for(j=0; j<16; j++)
					{
						//increment rdac 0 on board B by 1
						sprintf(buf, "d%c%c%c", 'b', 0x14, 0x0);
						wr=write(fd1, buf ,4);
					}
					usleep(SLEEP_TIME);
				}
			

				printf("Decrementing Rdac 0 A by 16...\n");
				for(i=0; i<1; i++)
				{
					//get bias ADC B value
					sprintf(buf, "%c", 'b');
					wr=write(fd1, buf ,1);
					timeStamp = getTimeStamp ( &t ); // Get the time stamp
					usleep(1000);
					rd=read(fd1,&bias,2);
					bias=ntohs(bias);
					fl_bias=(float)bias*3.3/4095;
					printf("%5.4f\n",fl_bias);
					fprintf( dataFile, "%.6f	", timeStamp );
					fprintf( dataFile, "%5.4f	", fl_bias );
					fprintf( dataFile, "\n" );

					for(j=0; j<16; j++)
					{
						//decrement rdac 0 on board B by 1
						sprintf(buf, "d%c%c%c", 'b', 0x6, 0x0);
						wr=write(fd1, buf ,4);
					}
					usleep(SLEEP_TIME);
				}

				rd=0;
				printf("Incrementing Rdac 1 A by 16...\n");
				for(i=0; i<64; i++)
				{
					//get bias ADC B value
					sprintf(buf, "%c", 'b');
					wr=write(fd1, buf ,1);
					timeStamp = getTimeStamp ( &t ); // Get the time stamp
					usleep(1000);
					while(!(rd==2))
					{
					rd=read(fd1,&bias,2);
					}
					rd=0;
					bias=ntohs(bias);
					fl_bias=(float)bias*3.3/4095;
					printf("%5.4f\n",fl_bias);
					fprintf( dataFile, "%.6f	", timeStamp );
					fprintf( dataFile, "%5.4f	", fl_bias );
					fprintf( dataFile, "\n" );

					for(j=0; j<16; j++)
					{
						//increment rdac 1 on board B by 1
						sprintf(buf, "d%c%c%c", 'b', 0x14, 0x1);
						wr=write(fd1, buf ,4);
					}
					usleep(SLEEP_TIME);
				}


				printf("Decrementing Rdac 1 A by 16...\n");
				for(i=0; i<64; i++)
				{
					//get bias ADC B value
					sprintf(buf, "%c", 'b');
					wr=write(fd1, buf ,1);
					timeStamp = getTimeStamp ( &t ); // Get the time stamp
					usleep(1000);
					while(!(rd==2)){
					rd=read(fd1,&bias,2);}
					rd=0;
					bias=ntohs(bias);
					fl_bias=(float)bias*3.3/4095;
					printf("%5.4f\n",fl_bias);
					fprintf( dataFile, "%.6f	", timeStamp );
					fprintf( dataFile, "%5.4f	", fl_bias );
					fprintf( dataFile, "\n" );

					for(j=0; j<16; j++)
					{
						//decrement rdac 1 on board B by 1
						sprintf(buf, "d%c%c%c", 'b', 0x6, 0x1);
						wr=write(fd1, buf ,4);
					}
					usleep(SLEEP_TIME);
				}


				printf("Incrementing both Rdacs A by 16...\n");
				for(i=0; i<64; i++)
				{
					//get bias ADC B value
					sprintf(buf, "%c", 'b');
					wr=write(fd1, buf ,1);
					timeStamp = getTimeStamp ( &t ); // Get the time stamp
					usleep(1000);
					while(!(rd==2)){
					rd=read(fd1,&bias,2);}
					rd=0;
					bias=ntohs(bias);
					fl_bias=(float)bias*3.3/4095;
					printf("%5.4f\n",fl_bias);
					fprintf( dataFile, "%.6f	", timeStamp );
					fprintf( dataFile, "%5.4f	", fl_bias );
					fprintf( dataFile, "\n" );

					for(j=0; j<16; j++)
					{
						//increment both rdacs on board B by 1
						sprintf(buf, "d%c%c%c", 'b', 0x15, 0x0);
						wr=write(fd1, buf ,4);
					}
					usleep(SLEEP_TIME);
				}


				printf("Decrementing both Rdacs A by 16...\n");
				for(i=0; i<64; i++)
				{
					//get bias ADC B value
					sprintf(buf, "%c", 'b');
					wr=write(fd1, buf ,1);
					timeStamp = getTimeStamp ( &t ); // Get the time stamp
					usleep(1000);
					while(!(rd==2)){
					rd=read(fd1,&bias,2);}
					rd=0;
					bias=ntohs(bias);
					fl_bias=(float)bias*3.3/4095;
					printf("%5.4f\n",fl_bias);
					fprintf( dataFile, "%.6f	", timeStamp );
					fprintf( dataFile, "%5.4f	", fl_bias );
					fprintf( dataFile, "\n" );

					for(j=0; j<16; j++)
					{
						//decrement both rdacs on board B by 1
						sprintf(buf, "d%c%c%c", 'b', 0x7, 0x0);
						wr=write(fd1, buf ,4);
					}
					usleep(SLEEP_TIME);
				}
			

				printf("Incrementing both Rdacs A by 6dB...\n");
				for(i=0; i<10; i++)
				{
					//get bias ADC B value
					sprintf(buf, "%c", 'b');
					wr=write(fd1, buf ,1);
					timeStamp = getTimeStamp ( &t ); // Get the time stamp
					usleep(1000);
					while(!(rd==2)){
					rd=read(fd1,&bias,2);}
					rd=0;
					bias=ntohs(bias);
					fl_bias=(float)bias*3.3/4095;
					printf("%5.4f\n",fl_bias);
					fprintf( dataFile, "%.6f	", timeStamp );
					fprintf( dataFile, "%5.4f	", fl_bias );
					fprintf( dataFile, "\n" );

					//increment both rdacs on board B by 6 dB
					sprintf(buf, "d%c%c%c", 'b', 0x13, 0x0);
					wr=write(fd1, buf ,4);
					usleep(SLEEP_TIME);
				}


				printf("Decrementing both Rdacs A by 6dB...\n");
				for(i=0; i<10; i++)
				{
					//get bias ADC B value
					sprintf(buf, "%c", 'b');
					wr=write(fd1, buf ,1);
					timeStamp = getTimeStamp ( &t ); // Get the time stamp
					usleep(1000);
					while(!(rd==2)){
					rd=read(fd1,&bias,2);}
					rd=0;
					bias=ntohs(bias);
					fl_bias=(float)bias*3.3/4095;
					printf("%5.4f\n",fl_bias);
					fprintf( dataFile, "%.6f	", timeStamp );
					fprintf( dataFile, "%5.4f	", fl_bias );
					fprintf( dataFile, "\n" );

					//decrement both rdacs on board B by 6dB (right shift bit value for half)
					sprintf(buf, "d%c%c%c", 'b', 0x5, 0x0);
					wr=write(fd1, buf ,4);
					usleep(SLEEP_TIME);
				}

			cal=0;
			fclose(dataFile);
			printf("cal file complete\n");
			usleep(5000000);
			print_menu();
		
		}//end if(cal)*/

	} // =============== Main DAQ loop end. Should be infinite in the actual application. ==================
	
	close(fd1);
	exit(0);
}/* end main */

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
        tty.c_cc[VTIME] = 0;            // 0.5 seconds read timeout

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
        tty.c_cc[VMIN]  = 1;            // read doesn't block
        tty.c_cc[VTIME] = 0;            // 0.5 seconds read timeout

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

unsigned int
serial_fix_24( unsigned char* buf)
{	
	union {
		unsigned char b[4];
		unsigned int i;
	} u;
 
        u.b[0] = buf[2];
        u.b[1] = buf[1];
        u.b[2] = buf[0];
        u.b[3] = 0;
        return u.i;
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

void
print_menu(void)
{
	printf("Menu:\n");

	printf("to increment\n");

	printf("\tboard A\n");
	printf("\t\tRdac 0 by 1 - a\n");
	printf("\t\tRdac 1 by 1 - b\n");
	printf("\t\tboth Rdacs by 1 - k\n");
	printf("\t\tboth Rdacs by 6dB - n\n");

	printf("\tboard B\n");
	printf("\t\tRdac 0 by 1 - c\n");
	printf("\t\tRdac 1 by 1 - d\n");
	printf("\t\tboth Rdacs by 1 - l\n");
	printf("\t\tboth Rdacs by 6dB - o\n");

	printf("to decrement\n");

	printf("\tboard A\n");
	printf("\t\tRdac 0 by 1 - e\n");
	printf("\t\tRdac 1 by 1 - i\n");
	printf("\t\tboth Rdacs by 1 - g\n");
	printf("\t\tboth Rdacs by 6dB - p\n");

	printf("\tboard B\n");
	printf("\t\tRdac 0 by 1 - f\n");
	printf("\t\tRdac 1 by 1 - j\n");
	printf("\t\tboth Rdacs by 1 - h\n");
	printf("\t\tboth Rdacs by 6dB - r\n");

	printf("other commands\n");

	printf("save both wiper positions on baord A to NVM - v\n");
	printf("save both wiper positions on baord B to NVM - w\n");
	printf("Sync ADCs - s\n");
	printf("initalize SD-ADC A - t\n");
	printf("initalize SD-ADC B - u\n");
	printf("Get bias ADC value A - x\n");
	printf("Get bias ADC value B - y\n");
	printf("print menu -m\n");
	printf("Rdac to ARM ADC Non-linearity calibration file procedure - z\n"); 
	printf("quit program-q\n\n\n");
	
}

void
reset_ARM(void) 
{
	outb( 0x0 ,BASE_PROM_DAQ+8);
	printf("outb low\n");
	usleep(5000000);
	outb( 0x8 ,BASE_PROM_DAQ+8);
	printf("outb high\n");
	usleep(3000000);	
}
