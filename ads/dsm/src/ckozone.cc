/* ckozone.cc
   Check the NOAA ozone interface.

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
#include <Ozone.h>

// Program functions.
void initInterrupts();
void ozone_isr();
void statusMsg (char *msg);

// Class declarations.
Ozone *ozone;

ckozone ()
{

// Initialize the NOAA class.
  ozone = (Ozone*)malloc (sizeof (Ozone));
  ozone->Ozone ((char*)(A24D16_BASE + ISIO1_BASE), OZONE_PORT_1, statusMsg);

// Initialize interrupts.
  initInterrupts();

printf("At main loop.\n");
  while (TRUE) {
    taskDelay (sysClkRateGet());
    ozone->checkStatus();
//    printf("In Main Loop \n");
  }
}
/*****************************************************************************/
 
void initInterrupts ()
 
// Initialize interrupts and handlers.
{
// Connect the Ozone isr.

  if (intConnect ((VOIDFUNCPTR*)ISIO1_BIM2_ADR, (VOIDFUNCPTR)ozone_isr, 0)) {
    perror ("intConnect ozone_isr");
    exit(ERROR);
  }
  taskDelay(sysClkRateGet() / 10);
 
// Enable the interrupt handler on the mv162 board for level 1 interrupts.
  *VMECHIP2_LBIER = 0x01;
 
// Enable interrupts.
  ozone->enableInterrupt (ISIO1_BIM2_VCT, 1);
}
/*****************************************************************************/

void ozone_isr ()

// Interrupt service routine.
{
  ozone->isr ();
// logMsg ("in ozone_isr.\n", 0,0,0,0,0,0);

}
/*****************************************************************************/

void statusMsg (char *msg)

{
  printf (msg);
}
/*****************************************************************************/
