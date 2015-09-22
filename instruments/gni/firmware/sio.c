/*------------------------------------------------------------------------------
SIO.C:  Serial Communication And ISR Routines.

Mike Spowart
NCAR/RAF
------------------------------------------------------------------------------*/

#include "REG952.H"
#include <stdio.h>
#include <string.h>
#include "sio.h"

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
#define TBUF_SIZE	72		/* DO NOT CHANGE */
#define RBUF_SIZE	16		/* DO NOT CHANGE */
#define ESC       (char)0x1B   //escape key 
#define CMD       (char)0x21   //! key 
#define NUM_SWITCHES 18
static idata unsigned char tbuf [TBUF_SIZE];
static idata unsigned char rbuf [RBUF_SIZE];

data unsigned char t_in = 0;
data unsigned char t_out = 0;
bit t_disabled = 0;

static data unsigned char r_in = 0;
static data unsigned char r_out = 0;
static data unsigned char nnn = 0;
static data unsigned char kk = 0;


//extern unsigned char data seconds;
extern unsigned char data count;
//extern unsigned char active_motor;
extern unsigned char idata drive[8];
extern bit step_wait;
extern bit escape;
extern bit set_switch;
extern bit write_flash(void);
extern void inhibits_on(char);
extern void inhibits_off(char);


/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
static void com_isr (void) interrupt 4 using 2
{
/*------------------------------------------------
Received data interrupt.
------------------------------------------------*/
  if (RI != 0)
  {
    RI = 0;

    if ((r_in + 1) != r_out){
      rbuf [r_in] = S0BUF;
		  escape = 0;
		  if(rbuf[r_in++] == ESC) escape = 1;
//		if(rbuf[r_in++] == CMD) escape = 0;
  	}
  }

/*------------------------------------------------
Transmitted data interrupt.
------------------------------------------------*/
  if (TI != 0)
  {
    TI = 0;
																																					 
    if (t_in != t_out)
      S0BUF = tbuf [t_out++];
    else
      t_disabled = 1;
  }
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
void com_initialize (void)
{
/*------------------------------------------------
Setup Baud rate generator for proper baud rate.
------------------------------------------------*/
  EA = 0;                         /* Disable Interrupts */
  WDCON = 0x00;
  com_baudrate ();

/*------------------------------------------------
Clear com buffer indexes.
------------------------------------------------*/
  EWDRT = 0;
  RTCCON = 0x60;     //clear flag  
  r_out = 0;
  TI = 0;				    /* clear transmit interrupt */
  t_in = 0;			    /* empty transmit buffer */
  t_out = 0;
  t_disabled = 1;	  /* disable transmitter */
//	escape = 0;
  TR1 = 0;			    /* stop timer 1 */
  ET1 = 0;			    /* disable timer 1 interrupt */
  TR0 = 0;			    /* stop timer 0 */
  ET0 = 0;			    /* disable timer 0 interrupt */


/*------------------------------------------------
Setup serial port registers.
------------------------------------------------*/
  TI = 0;				/* clear transmit interrupt */
  RI = 0;				/* clear receiver interrupt */
  ES = 1;				/* enable serial interrupts */
  PS = 1;				/* set serial interrupts to low priority */
  IP0H |= 0x10;
  PWDRT = 1;
  EX0 = 0;
  EX1 = 1;
  IT1 = 1;
  IE1 = 0;      /* Clear ext. #1 interrupt */
/*
  P0M1 = 0x00;  // Set to bi-directional (Input only = 0xFF) 
  P0M2 = 0xFF;
  P1M1 = 0x1C;  // set Port 1 pins to bi-directional except I2C
  P1M2 = 0x0C;
  P2M1 = 0x00;
  P2M2 = 0x3F;
  P3M1 = 0x00;
  P3M2 = 0x03;
  P4M1 = 0x00;
  P4M2 = 0x00;
  P5M1 = 0x00;
  P5M2 = 0x00;
*/
  P0M1 = 0x0F;  // Set to bi-directional (Input only = 0xFF) 
  P0M2 = 0xF0;
  P1M1 = 0x1C;  // set Port 1 pins to bi-directional except I2C
  P1M2 = 0xCC;
  P2M1 = 0x01;
  P2M2 = 0x3E;
  P3M1 = 0x00;
  P3M2 = 0x33;
  P4M1 = 0x00;
  P4M2 = 0xFF;
  P5M1 = 0x00;
  P5M2 = 0xFF;

  REN = 1;         //Enable RXD reception
  TRIM |= 0xC0;    // Select RC Oscillator
  DIVM = 0x00;     // CCLK = OSCCLK
  I2SCLL = 10;
  I2SCLH = 10;
  I2ADR = 0x26;
  I2CON = 0x44;

  AD0CON = 0x85;    //Conversion starts immediately, enable & boundary
//  AD0INS = 0xA7;   //Enable ADC0-ADC4	  Rev. 4
  AD0INS = 0xB6;   //Enable ADC0-ADC4	Rev. 5
  BURST0 = 1;       //Continuous conversion
  SCC0 = 0;
  SCAN0 = 0;
  AD0MODB = 0x62;   //Outside Boundary limits
  ADC0HBND = 0x9B;  //Upper boundary limit
  ADC0LBND = 0x00;  //Lower boundary limit
  ADC0HBND = 0x200;  //Upper boundary limit
  ADC0LBND = 0x00;  //Lower boundary limit
  IEN2 = 0x06;      //Enable ADC interrupt
  EA = 1;           /* Enable Interrupts */
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
void com_baudrate ()
{
// Baud Rate Generator
  BRGCON_0 = 0x02;
  BRGR0_0 = 0xF0;	    //9600 baud
  BRGR1_0 = 0x02;
//  BRGR0 = 0xB0;				 //38400 baud
//  BRGR1 = 0x00;
  PCON =0x0;
  SM0_0 = 0;
  SM1 = 1;
  SM2 = 0;
  BRGCON_0 = 0x03;   //Select and enable baud rater generator
//  EA = 1;          // Enable Interrupts 
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/

char com_putchar (unsigned char c)
{
//------------------------------------------------
//If the buffer is full, return an error value.
//------------------------------------------------
  if ((TBUF_SIZE - com_tbuflen ()) <= 2)
    return (-1);
		   
//------------------------------------------------
//Add the data to the transmit buffer.  If the
//transmit interrupt is disabled, then enable it.
//------------------------------------------------
  EA = 0;             // Disable Interrupts 

  tbuf [t_in++] = c;

  if (t_disabled)			// if transmitter is disabled 
  {
    t_disabled = 0;
    TI = 1;		      	// enable it 
  }

  EA = 1;             // Enable Interrupts 

  return (0);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int com_getchar (void)
{
  int c;

  if (com_rbuflen () == 0)
    return (-1);

//  EA = 0;                         // Disable Interrupts 
  c = rbuf [r_out++];
	if(r_in == r_out){
	  r_in = 0;
		r_out = 0;
	}
//  EA = 1;                         // Enable Interrupts 

  return (c);
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/

unsigned char com_rbuflen (void)
{
  return (r_in - r_out);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
unsigned char com_tbuflen (void)
{
  return (t_in - t_out);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


void exint_isr (void) interrupt 2 using 3
{
//------------------------------------------------
//Ext. int. #1 interrupt.
//------------------------------------------------
  EA = 0;
  IE1 = 0;
  set_switch = 1;
  EA = 1;

}


void RTC_isr (void) interrupt 10 using 1
{
/*------------------------------------------------
Real Time Clock interrupt.
------------------------------------------------*/
//  EA = 0;
  RTCCON = 0x62; //clear flag
  step_wait = 1;
  RTCCON = 0x63; 
}
void ADC_isr (void) interrupt 16 using 3
{
/*------------------------------------------------
A2D interrupt.
------------------------------------------------*/
  AD0CON = AD0CON & 0xF7;   //Clear interrupt bit 3

  if(XTAL1 == 1) XTAL1 = 0;
  else XTAL1 = 1;
/*
  if(nnn == 0) inhibits_on(active_motor);
  else {
    inhibits_off(active_motor);
  }
  nnn += 1;
  if(nnn == 3) nnn = 0;

*/
  if(kk == 0) {
    nnn += 1;
    if(nnn == 3) nnn = 0;
  }
  kk += 1; 
  if (kk == 4) kk = 0;
  if(nnn == 2) {

    P5 = 0x11;
    P4 = 0x11;
    P0 = P0 | 0x10;

  }
  else {

    P5 = 0x00;
    P4 = 0x00;
    P0 = P0 & 0x0F;

  }

}																 