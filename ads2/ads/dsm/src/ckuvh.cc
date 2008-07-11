/* ckuvh.cc
   Check the UV hygrometer interface.

   Original Author: Jerry V. Pelk
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
#include <UvHyg.h>

// Program functions.
void initInterrupts();
void uvh_isr();
void statusMsg (char *msg);

// Class declarations.
static UvHyg *uvh;

int ckuvh ()
{

// Initialize the Ophir3 class.
  uvh = new UvHyg ((char*)(A24D16_BASE + ISIO1_BASE), UVHYG_PORT_1, statusMsg);

// Initialize interrupts.
  initInterrupts();

  while (TRUE) {
    taskDelay (sysClkRateGet());
    uvh->secondAlign();
    printf ("index = %d\n", uvh->index());
    uvh->checkStatus();
    uvh->display();
  }
}
/*****************************************************************************/
 
void initInterrupts ()
 
// Initialize interrupts and handlers.
{
// Connect the Tans GPS isr.
  if (intConnect ((VOIDFUNCPTR*)ISIO1_BIM2_ADR, (VOIDFUNCPTR)uvh_isr, 0)) {
    perror ("intConnect uvh_isr");
    exit(ERROR);
  }
  taskDelay(sysClkRateGet() / 10);
 
// Enable the interrupt handler on the mv162 board for level 1 interrupts.
  *VMECHIP2_LBIER = 0x01;
 
// Enable interrupts from the tans.
  uvh->enableInterrupt (ISIO1_BIM2_VCT, 1);
}
/*****************************************************************************/

void uvh_isr ()

// Interrupt service routine for the Ophir 3.
{
  uvh->isr ();
//logMsg ("in uvh_isr.\n", 0,0,0,0,0,0);
}
/*****************************************************************************/

void statusMsg (char *msg)

{
  printf (msg);
}
/*****************************************************************************/
