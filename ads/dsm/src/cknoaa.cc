/* cknoaa.cc
   Check the NOAA interface.

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
#include <Noaa.h>

// Program functions.
void initInterrupts();
void noaa_isr();
void statusMsg (char *msg);

// Class declarations.
Noaa *noaa;

cknoaa ()
{

// Initialize the NOAA class.
  noaa = (Noaa*)malloc (sizeof (Noaa));
  noaa->Noaa ((char*)(A24D16_BASE + ISIO1_BASE), NOAA_PORT_1, statusMsg);

// Initialize interrupts.
  initInterrupts();

printf("At main loop.\n");
  while (TRUE) {
    taskDelay (sysClkRateGet());
    noaa->checkStatus();
//    printf("In Main Loop \n");
  }
}
/*****************************************************************************/
 
void initInterrupts ()
 
// Initialize interrupts and handlers.
{
// Connect the NOAA isr.
  if (intConnect ((VOIDFUNCPTR*)ISIO1_BIM3_ADR, (VOIDFUNCPTR)noaa_isr, 0)) {
    perror ("intConnect noaa_isr");
    exit(ERROR);
  }
  taskDelay(sysClkRateGet() / 10);
 
// Enable the interrupt handler on the mv162 board for level 1 interrupts.
  *VMECHIP2_LBIER = 0x01;
 
// Enable interrupts.
  noaa->enableInterrupt (ISIO1_BIM3_VCT, 1);
}
/*****************************************************************************/

void noaa_isr ()

// Interrupt service routine.
{
  noaa->isr ();
// logMsg ("in noaa_isr.\n", 0,0,0,0,0,0);
}
/*****************************************************************************/

void statusMsg (char *msg)

{
  printf (msg);
}
/*****************************************************************************/
