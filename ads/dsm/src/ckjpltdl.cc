/* ckjpltdl.cc
   Check the JplTdl  interface.

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
#include <JplTdl.h>

// Program functions.
void initInterrupts();
void jpltdl_isr();
void statusMsg (char *msg);

// Class declarations.
static JplTdl *jpltdl;

int ckjpltdl ()
{
// Initialize the JPL TDL class.
  jpltdl = new JplTdl ((char*)(A24D16_BASE + ISIO1_BASE), JPLTDL_PORT_1, statusMsg);

// Initialize interrupts.
  initInterrupts();

printf("At main loop.\n");
  while (TRUE) {
    taskDelay (sysClkRateGet());
    jpltdl->checkStatus();
//    printf("In Main Loop \n");
  }
}
/*****************************************************************************/
 
void initInterrupts ()
 
// Initialize interrupts and handlers.
{
// Connect the JPL TDL isr.
  if (intConnect ((VOIDFUNCPTR*)ISIO1_BIM2_ADR, (VOIDFUNCPTR)jpltdl_isr, 0)) {
    perror ("intConnect jpltdl_isr");
    exit(ERROR);
  }
  taskDelay(sysClkRateGet() / 10);
 
// Enable the interrupt handler on the mv162 board for level 1 interrupts.
  *VMECHIP2_LBIER = 0x01;
 
// Enable interrupts from the tans.
  jpltdl->enableInterrupt (ISIO1_BIM2_VCT, 1);
}
/*****************************************************************************/

void jpltdl_isr ()

// Interrupt service routine for the JPL TDL.
{
  jpltdl->isr ();
// logMsg ("in jpltdl_isr.\n", 0,0,0,0,0,0);
}
/*****************************************************************************/

void statusMsg (char *msg)

{
  printf (msg);
}
/*****************************************************************************/
