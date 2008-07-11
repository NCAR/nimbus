/* ckppsgps.cc
   Check the PPS GPS interface.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include "mv162.h"         // mvme162 defines
#include <vxWorks.h>
#include <intLib.h>
#include <stdioLib.h>
#include <taskLib.h>

// Program include files.
#include <dsmctl.h>
#include <dsmVmeDefs.h>

// Class include files.
#include <PpsGps.h>

// Program functions.
void initInterrupts();
void ppsgps_isr();
void statusMsg (char *msg), setTime(int, int, int, int, int, int);

// Class declarations.
static PpsGps *ppsgps;

int ckppsgps ()
{
Ppsgps_blk p;

// Initialize the PPS GPS class.
  ppsgps = new PpsGps ((char*)(A24D16_BASE + ISIO1_BASE), PPSGPS_PORT_1, statusMsg, setTime);

printf("sizeof = %d\n", sizeof(Ppsgps_blk));
printf("flags = %d\n", (int)(&(p.flags)) - (int)&p);
printf("cksum = %d\n", (int)(&(p.hdr_cksum)) - (int)&p);
printf("gpsTi = %d\n", (int)(&(p.gps_time)) - (int)&p);
printf("utcTi = %d\n", (int)(&(p.utc_time)) - (int)&p);
printf("lat = %d\n", (int)(&(p.lat)) - (int)&p);
printf("lon = %d\n", (int)(&(p.lon)) - (int)&p);

// Initialize interrupts.
  initInterrupts();
//  taskDelay (sysClkRateGet());
//  ppsgps->secondAlign();
//  ppsgps->checkStatus();

printf("At main loop.\n");
  while (TRUE) {
    taskDelay (sysClkRateGet());
    ppsgps->secondAlign();
//    printf ("index = %d\n", ppsgps->index());
    ppsgps->checkStatus();
//    ppsgps->display();
//    printf("In Main Loop \n");
  }
}
/*****************************************************************************/
 
void initInterrupts ()
 
// Initialize interrupts and handlers.
{
// Connect the PPS GPS isr.
  if (intConnect ((VOIDFUNCPTR*)ISIO1_BIM2_ADR, (VOIDFUNCPTR)ppsgps_isr, 0)) {
    perror ("intConnect ppsgps_isr");
    exit(ERROR);
  }
  taskDelay(sysClkRateGet() / 10);
 
// Enable the interrupt handler on the mv162 board for level 1 interrupts.
  *VMECHIP2_LBIER = 0x01;
 
// Enable interrupts from the tans.
  ppsgps->enableInterrupt (ISIO1_BIM2_VCT, 1);
}
/*****************************************************************************/

void ppsgps_isr ()

// Interrupt service routine for the PPS GPS.
{
  ppsgps->isr ();
// logMsg ("in ppsgps_isr.\n", 0,0,0,0,0,0);
}
/*****************************************************************************/

void statusMsg (char *msg)

{
  printf (msg);
}
/*****************************************************************************/

void setTime (int year, int month, int day, int hour, int minute, int second)

{

}
