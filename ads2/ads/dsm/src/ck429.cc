/* ck429.cc
   Check the IP429-1 Aric 429 Industry Pack module.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include "mv162.h"     
#include <vxWorks.h>
#include <intLib.h>
#include <logLib.h>
#include <stdioLib.h>
#include <string.h>
#include <taskLib.h>

#define BLK	8				// # 32 bit words per write

// Program include files.
#include <dsmVmeDefs.h>
#include <ipicDefs.h>

// Class include files.
#include <IP429.h>

// Program functions.
void initInterrupts ();				// initialize interrupts
void ip429ReadIsr (int chan);			// IP429-1 read isr
void ip429WriteIsr (int chan);			// IP429-1 write isr

// Class declarations.
IP429 *ip429;	                                // arinc 429 interface

// Global variables.
int wr_buf[BLK];
int rd_buf[BLK];

int ck429 ()
{
  int j;

  for (j = 0; j < BLK; j++)
    wr_buf[j] = j + 1;

// Initialize the IP429 class.
  ip429 = new IP429 ((char*)IP_a_IO_BASE, IP429_VCT);

// Initialize interrupts.
  printf("begin init interrupts \n");
//  initInterrupts ();
  printf("end init interrupts \n");

//ip429.enableLoopback(4);
/*  ip429.setSpeedLow (0);
  ip429.setSpeedLow (4);

  printf ("Beginning testing for 10 seconds.\n");
  taskDelay(sysClkRateGet());

  ip429.enableInterrupt (0);
  ip429.enableInterrupt (4);		// this generates a first write intrpt
*/
  for (j = 1; j <= 10; j++) {
    printf ("second, %d\n", j);
    taskDelay(sysClkRateGet());
  }

//  ip429.disableInterrupt (4);
//  ip429.disableLoopback(4);
}
/*****************************************************************************/

void initInterrupts ()

// Initialize interrupts and handlers.
{
// Connect the IP429 isr.  The IP429 uses a base vector with the lower
// three bits of the vector specifying the interrupting channel.
  if (intConnect ((VOIDFUNCPTR*)IP429_ADR,(VOIDFUNCPTR)ip429ReadIsr, 0)) {
    perror ("intConnect ip429ReadIsr");
    exit(ERROR);
  }
  if (intConnect ((VOIDFUNCPTR*)(IP429_ADR+0x04),(VOIDFUNCPTR)ip429ReadIsr, 1)){
    perror ("intConnect ip429ReadIsr");
    exit(ERROR);
  }
  if (intConnect ((VOIDFUNCPTR*)(IP429_ADR+0x08),(VOIDFUNCPTR)ip429ReadIsr, 2)){
    perror ("intConnect ip429ReadIsr");
    exit(ERROR);
  }
  if (intConnect ((VOIDFUNCPTR*)(IP429_ADR+0x0C),(VOIDFUNCPTR)ip429ReadIsr, 3)){
    perror ("intConnect ip429ReadIsr");
    exit(ERROR);
  }
  if (intConnect((VOIDFUNCPTR*)(IP429_ADR+0x10),(VOIDFUNCPTR)ip429WriteIsr, 4)){
    perror ("intConnect ip429WriteIsr");
    exit(ERROR);
  }
  if (intConnect((VOIDFUNCPTR*)(IP429_ADR+0x14),(VOIDFUNCPTR)ip429WriteIsr, 5)){
    perror ("intConnect ip429WriteIsr");
    exit(ERROR);
  }
  taskDelay(sysClkRateGet() / 10);

// Enable the transmit and receive interrupts in the IPIC chip.
  *IPIC_IP_a_INT0_CTL = IPIC_INT_CTL_IEN | IPIC_INT_CTL_LEVEL_1;
  *IPIC_IP_a_INT1_CTL = IPIC_INT_CTL_IEN | IPIC_INT_CTL_LEVEL_1;
}
/*****************************************************************************/
 
void ip429ReadIsr (int chan)
 
// Read channel interrupt service routine for the IP429 arinc interface.
{
  static int ridx = 0;

  *((short*)(rd_buf + ridx) + 1) = ip429->data1 (chan);
  *(short*)(rd_buf + ridx) = ip429->data2 (chan);

  if (++ridx >= BLK) {
    for (ridx = 0; ridx < BLK; ridx++) {
      if (rd_buf[ridx] != wr_buf[ridx]) {
        logMsg ("Read error, rd_buf[%1d] = %d\n", ridx, rd_buf[ridx],0,0,0,0);
        break;
      }
    }
    if (ridx == BLK)
      logMsg ("Read\n", 0,0,0,0,0,0);
    memset ((char*)rd_buf, 0, sizeof (rd_buf));
    ridx = 0;
  }
}
/*****************************************************************************/

void ip429WriteIsr (int chan)

// Write channel interrupt service routine for the IP429 arinc interface.
{
// 10 hz at low speed, ~13 hz at high speed.
  ip429->write429 (chan, wr_buf, BLK, 77);
  logMsg ("Write\n", 0,0,0,0,0,0);
}
/*****************************************************************************/

