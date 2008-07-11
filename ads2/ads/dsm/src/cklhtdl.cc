/* cklhtdl.cc
   Check the LhTdl  interface.

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
#include <LhTdl.h>

// Program functions.
void initInterrupts();
void lhtdl_isr();
void statusMsg (char *msg);

// Class declarations.
static LhTdl *lhtdl;

int cklhtdl ()
{
// Initialize the JPL TDL class.
  lhtdl = new LhTdl ((char*)(A24D16_BASE + ISIO1_BASE), LHTDL_PORT_1, statusMsg);

// Initialize interrupts.
  initInterrupts();
  lhtdl->initTdl();
  printf("At main loop.\n");
  while (TRUE) {
    taskDelay (sysClkRateGet()/5);
    lhtdl->checkStatus();
//    printf("In Main Loop \n");
  }
}
/*****************************************************************************/
 
void initInterrupts ()
 
// Initialize interrupts and handlers.
{
// Connect the LHTDL isr.
  if (intConnect ((VOIDFUNCPTR*)ISIO1_BIM2_ADR, (VOIDFUNCPTR)lhtdl_isr, 0)) {
    perror ("intConnect lhtdl_isr");
    exit(ERROR);
  }
  taskDelay(sysClkRateGet() / 10);
 
// Enable the interrupt handler on the mv162 board for level 1 interrupts.
  *VMECHIP2_LBIER = 0x01;
 
// Enable interrupts from the tans.
  lhtdl->enableInterrupt (ISIO1_BIM2_VCT, 1);
}
/*****************************************************************************/

void lhtdl_isr ()

// Interrupt service routine for the JPL TDL.
{
 logMsg ("in lhtdl_isr.\n", 0,0,0,0,0,0);
  lhtdl->isr ();
}
/*****************************************************************************/

void statusMsg (char *msg)

{
  printf (msg);
}
/*****************************************************************************/
