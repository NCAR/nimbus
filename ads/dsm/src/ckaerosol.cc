/* ckaerosol.cc
   Check the NOAA aerosol interface.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include "mv162.h"         // mvme162 defines
#include <vxWorks.h>
#include <intLib.h>
#include <logLib.h>
#include <stdioLib.h>
#include <taskLib.h>

// Program include files.
#include <dsmctl.h>
#include <dsmVmeDefs.h>

// Class include files.
#include <Aerosol.h>

// Program functions.
void initInterrupts();
void aerosol_isr();
void statusMsg (char *msg);

// Class declarations.
Aerosol *aerosol;

ckaerosol ()
{

// Initialize the Aerosol class.
  aerosol = (Aerosol*)malloc (sizeof (Aerosol));
  aerosol->Aerosol ((char*)(A24D16_BASE + ISIO1_BASE),AEROSOL_PORT_1,statusMsg);

// Initialize interrupts.
  initInterrupts();

printf("At main loop.\n");
  while (TRUE) {
    taskDelay (sysClkRateGet());
    aerosol->checkStatus();
//    printf("In Main Loop \n");
  }
}
/*****************************************************************************/
 
void initInterrupts ()
 
// Initialize interrupts and handlers.
{
// Connect the Aerosol isr.
  if (intConnect ((VOIDFUNCPTR*)ISIO1_BIM1_ADR, (VOIDFUNCPTR)aerosol_isr, 0)) {
    perror ("intConnect aerosol_isr");
    exit(ERROR);
  }
  taskDelay(sysClkRateGet() / 10);
 
// Enable the interrupt handler on the mv162 board for level 1 interrupts.
  *VMECHIP2_LBIER = 0x05;
 
// Enable interrupts.
  aerosol->enableInterrupt (ISIO1_BIM1_VCT, 1);
}
/*****************************************************************************/

void aerosol_isr ()

// Interrupt service routine.
{
  aerosol->isr ();
// logMsg ("in aerosol_isr.\n", 0,0,0,0,0,0);
}
/*****************************************************************************/

void statusMsg (char *msg)

{
  printf (msg);
}
/*****************************************************************************/
