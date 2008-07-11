/* ckophir.cc
   Check the Ophir 3 radiometer interface.

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
#include <Ophir3.h>

// Program functions.
void initInterrupts();
void ophir_isr();
void statusMsg (char *msg);

// Class declarations.
Ophir3 *ophir3;

int ckophir ()
{
// Initialize the Ophir3 class.
  ophir3 = new Ophir3((char*)(A24D16_BASE + ISIO1_BASE), OPHIR_PORT_1, statusMsg);

// Initialize interrupts.
  initInterrupts();

  while (TRUE) {
    taskDelay (sysClkRateGet());
    ophir3->checkStatus();
    ophir3->checkZero();
    printf ("Raw buffer:\n");
    ophir3->displayRaw();
    printf ("Processed buffer:\n");
    ophir3->display();
  }
}
/*****************************************************************************/
 
void initInterrupts ()
 
// Initialize interrupts and handlers.
{
// Connect the Tans GPS isr.
  if (intConnect ((VOIDFUNCPTR*)ISIO1_BIM4_ADR, (VOIDFUNCPTR)ophir_isr, 0)) {
    perror ("intConnect ophir_isr");
    exit(ERROR);
  }
  taskDelay(sysClkRateGet() / 10);
 
// Enable the interrupt handler on the mv162 board for level 1 interrupts.
  *VMECHIP2_LBIER = 0x01;
 
// Enable interrupts from the tans.
  ophir3->enableInterrupt (ISIO1_BIM4_VCT, 1);
}
/*****************************************************************************/

void ophir_isr ()

// Interrupt service routine for the Ophir 3.
{
  ophir3->isr ();
//logMsg ("in tans_isr.\n", 0,0,0,0,0,0);
}
/*****************************************************************************/

void statusMsg (char *msg)

{
  printf (msg);
}
/*****************************************************************************/
