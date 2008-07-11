/* IP429.h
 
   IP429-1 Arinc 429 Industry Pack driver.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef IP429_H
#define IP429_H

#include <vxWorks.h>
#include <logLib.h>
#include <sysLib.h>
#include <taskLib.h>

#include <dsmctl.h>

#define IP429_FIFO_SIZE		8		// fifo size in arinc words
#define IP429_DATA_REG_OFFSET	0x40		// byte offset to the data regs

// Define control register bits.
#define IP429_CTL_RX_LOW_SPEED	0x4000		// rx at low speed (12.5 khz)
#define IP429_CTL_TX_LOW_SPEED	0x2000		// tx at low speed (12.5 khz)
#define IP429_CTL_NO_LOOPBACK   0x0020		// disable loopback test

#define IP429_TX_ENTX		0x80		// tx enabled status bit
#define IP429_TX_GAP_SCALE_LOW	0x00		// set .1 ms gap time resolution
#define IP429_TX_GAP_SCALE_HIGH	0x10		// set 1 ms gap time resolution
#define IP429_TX_ENABLE		0x01		// master transmitter enable

#define IP429_IE_TEIN		0x02		// master tx interrupt enable
#define IP429_IE_REIN		0x01		// master rx interrupt enable
#define IP429_IE_EIN_BASE	0x04		// channel intr enable base bit
 
// Define the control registers.
 
typedef struct {
  char unused0;
  char svc_rqst;			// service request
  char unused1;
  char misc_status;			// miscellaneous status
  char unused2;
  char int_enable;			// interrupt enable
  char unused3;
  char int_vect;			// interrupt acknowledge
  char unused4;
  char tx4;				// transmit channel 4 control
  char unused5;
  char tx5;				// transmit channel 5 control
  char unused6;
  char go4;				// gap time channel 4
  char unused7;
  char go5;				// gap time channel 5
  char unused8;
  char sync;				// sync output control
  char unused9;
  char reset;				// write resets the IP429
} Ip429Ctl;
 
// Define the individual channel data registers.
typedef struct {
  short data1;				// chan data word 1
  short data2;				// chan data word 2
} Ip429Chan;

// Define the board data registers.
typedef struct {
  Ip429Chan chan[IP429_NUM_CHANS];	// rx (0-3) and tx (4-5) data regs
  short unused[4];
  short ctl_a;				// control word A
  short ctl_b;				// control word B
} Ip429Data;
 
class IP429 {
public:
  IP429 (char *base_adr, char vector);	// constructor
  short data1 (int chan)	{return ip_data->chan[chan].data1;}
  short data2 (int chan)	{return ip_data->chan[chan].data2;}
  int write429 (int chan, int *data, int len, int go); // write to a channel
  void setSpeedLow (int chan);		// set channel speed low (12.5 Khz)
  void setSpeedHigh (int chan);		// set channel speed high (100 Khz)
  void enableLoopback (int chan);	// enable loopback test
  void disableLoopback (int chan);	// disable loopback test

  void enableInterrupt (int chan)
		{ip_ctl->int_enable |= IP429_IE_EIN_BASE << chan;}
  void disableInterrupt (int chan) 
         	{ip_ctl->int_enable &= ~(IP429_IE_EIN_BASE << chan);}
  short servicerqst () {return (ip_ctl->svc_rqst & 0x80);}
  short servrqst () {return (ip_ctl->svc_rqst);}

private:
  volatile Ip429Ctl *ip_ctl;		// control registers
  volatile Ip429Data *ip_data;		// data registers
  short ctl_a;				// current value of ip_data->ctl_a
  short ctl_b;				// current value of ip_data->ctl_b
};

#endif
