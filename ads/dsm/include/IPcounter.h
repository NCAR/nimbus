/* IPcounter.h
 
   IP counter card (Industry Pack) driver.

   Original Author: Chris Webster
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef IPCOUNTER_H
#define IPCOUNTER_H

#include <vxWorks.h>
#include <logLib.h>
#include <sysLib.h>
#include <taskLib.h>

#include <dsmctl.h>

#define IP_CTL_REG_OFFSET	0x7F	// byte offset to the MACH control reg.

#define IP_MASTER_DATA_OFFSET	0x1F	// byte offset to master counter data
#define IP_MASTER_CTL_OFFSET	0x5F	// byte offset to master's control reg.

#define IP_SLAVES_DATA_OFFSET	0x00	// byte offset to slave counters' data
#define IP_SLAVES_CTL_OFFSET	0x40	// byte offset to slave's control reg.

class IPcounter {
public:
  IPcounter(char *base_adr);
  unsigned long counter(int port)	{return(data[port]);}

  void	readCounters(int time);
  void	adjustMaster();
  void  readProm(char *base_addr);


private:
  unsigned volatile char  *ip_ctl;		// IP control register
  unsigned volatile char  *ip_master_data_u;	// master counter data register
  unsigned volatile char  *ip_master_ctl;	// master counter control registers
  unsigned volatile short *ip_slaves_data;	// slave counters' data registers
  unsigned volatile short *ip_slaves_ctl;	// slave counters' control registers

  unsigned long data[6];

  int time;                                     // index of sample, 25 / sec.
};

#endif
