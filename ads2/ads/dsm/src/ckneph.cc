/* ckneph.cc
   Check the Neph  interface.

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
#include <Neph.h>

// Program functions.
void initInterrupts();
void neph_isr();
void statusMsg (char *msg);

// Class declarations.
static Neph *neph;

int ckneph ()
{
  printf("Begin\n");
// Initialize the Neph class.
  neph = new Neph ((char*)(A24D16_BASE + ISIO1_BASE), NEPH_PORT_1, statusMsg);

// Initialize interrupts.
  initInterrupts();
  taskDelay (sysClkRateGet()/2);
  neph->initNeph();
  printf("At main loop.\n");
  while (TRUE) {
//    printf("At main loop.\n");
    taskDelay (sysClkRateGet());
    neph->checkStatus();
    neph->secondAlign();
  }
}
/*****************************************************************************/
 
void initInterrupts ()
 
// Initialize interrupts and handlers.
{
// Connect the NEPH isr.
  if (intConnect ((VOIDFUNCPTR*)ISIO1_BIM2_ADR, (VOIDFUNCPTR)neph_isr, 0)) {
    perror ("intConnect neph_isr");
    exit(ERROR);
  }
  taskDelay(sysClkRateGet() / 10);
 
// Enable the interrupt handler on the mv162 board for level 1 interrupts.
  *VMECHIP2_LBIER = 0x01;
 
// Enable interrupts from the rdma.
  neph->enableInterrupt (ISIO1_BIM2_VCT, 1);
}
/*****************************************************************************/

void neph_isr ()

// Interrupt service routine for the Neph.
{
// logMsg ("in neph_isr.\n", 0,0,0,0,0,0);
  neph->isr ();
}
/*****************************************************************************/

void statusMsg (char *msg)

{
  printf (msg);
}
/*****************************************************************************/
