/* cknonoy.cc
   Check the No_Noy  interface.

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
#include <NoNoy.h>

// Program functions.
void initInterrupts();
void nonoy_isr();
void statusMsg (char *msg);

// Class declarations.
NoNoy *nonoy;

cknonoy ()
{

// Initialize the NO_NOY class.
  nonoy = (NoNoy*)malloc (sizeof (NoNoy));
  nonoy->NoNoy ((char*)(A24D16_BASE + ISIO1_BASE), NONOY_PORT_1, statusMsg);

// Initialize interrupts.
  initInterrupts();

printf("At main loop.\n");
  while (TRUE) {
    taskDelay (sysClkRateGet());
    nonoy->checkStatus();
//    printf("In Main Loop \n");
  }
}
/*****************************************************************************/
 
void initInterrupts ()
 
// Initialize interrupts and handlers.
{
// Connect the NO_NOY isr.
  if (intConnect ((VOIDFUNCPTR*)ISIO1_BIM4_ADR, (VOIDFUNCPTR)nonoy_isr, 0)) {
    perror ("intConnect nonoy_isr");
    exit(ERROR);
  }
  taskDelay(sysClkRateGet() / 10);
 
// Enable the interrupt handler on the mv162 board for level 1 interrupts.
  *VMECHIP2_LBIER = 0x01;
 
// Enable interrupts from the No_Noy.
  nonoy->enableInterrupt (ISIO1_BIM4_VCT, 1);
}
/*****************************************************************************/

void nonoy_isr ()

// Interrupt service routine for the NO_NOY
{
  nonoy->isr ();
// logMsg ("in nonoy_isr.\n", 0,0,0,0,0,0);
}
/*****************************************************************************/

void statusMsg (char *msg)

{
  printf (msg);
}
/*****************************************************************************/
