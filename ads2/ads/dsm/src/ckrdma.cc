/* ckrdma.cc
   Check the Rdma  interface.

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
#include <Rdma.h>

// Program functions.
void initInterrupts();
void rdma_isr();
void statusMsg (char *msg);

// Class declarations.
static Rdma *rdma;

int ckrdma ()
{
  printf("Begin\n");
// Initialize the Rdma class.
  rdma = new Rdma ((char*)(A24D16_BASE + ISIO1_BASE), RDMA_PORT_1, statusMsg);

// Initialize interrupts.
  initInterrupts();
  taskDelay (sysClkRateGet()/2);
  rdma->initRdma();
  printf("At main loop.\n");
  while (TRUE) {
//    printf("At main loop.\n");
    taskDelay (sysClkRateGet());
    rdma->checkStatus();
    rdma->secondAlign();
  }
}
/*****************************************************************************/
 
void initInterrupts ()
 
// Initialize interrupts and handlers.
{
// Connect the RDMA isr.
  if (intConnect ((VOIDFUNCPTR*)ISIO1_BIM2_ADR, (VOIDFUNCPTR)rdma_isr, 0)) {
    perror ("intConnect rdma_isr");
    exit(ERROR);
  }
  taskDelay(sysClkRateGet() / 10);
 
// Enable the interrupt handler on the mv162 board for level 1 interrupts.
  *VMECHIP2_LBIER = 0x01;
 
// Enable interrupts from the rdma.
  rdma->enableInterrupt (ISIO1_BIM2_VCT, 1);
}
/*****************************************************************************/

void rdma_isr ()

// Interrupt service routine for the Rdma.
{
// logMsg ("in rdma_isr.\n", 0,0,0,0,0,0);
  rdma->isr ();
}
/*****************************************************************************/

void statusMsg (char *msg)

{
  printf (msg);
}
/*****************************************************************************/
