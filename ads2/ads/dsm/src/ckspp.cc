/* ckspp.cc
   Check the SPP probe interface.

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
#include <TapeHeader.h>

// Class include files.
#include <Spp.h>

// Program functions.
void initInterrupts();
void spp_isr();
void statusMsg (char *msg);

// Class declarations.
static Spp *spp;
static TapeHeader *dsm_hdr;             // sram tapeheader

int ckspp ()
{
  int range, index, which_spp, bins;
  range = 0;
  which_spp = 0;
  bins = 10;

// Initialize the Spp class.

  spp = new Spp ((char*)(A24D16_BASE + ISIO1_BASE), *dsm_hdr,
                 (int) which_spp, statusMsg);

// Initialize interrupts.
  initInterrupts();

  spp->initSpp(range);

  while (TRUE) {
//    spp->initSpp(range);
//    taskDelay (sysClkRateGet());

    index = spp->index();
    taskDelay (sysClkRateGet()/10);
    spp->getSppData();
    taskDelay (sysClkRateGet()/10);
    spp->getSppData();
    taskDelay (sysClkRateGet()/10);
    spp->getSppData();
    taskDelay (sysClkRateGet()/10);
    spp->getSppData();
    taskDelay (sysClkRateGet()/10);
    spp->getSppData();

    taskDelay (sysClkRateGet()/10);
    spp->getSppData();
    taskDelay (sysClkRateGet()/10);
    spp->getSppData();
    taskDelay (sysClkRateGet()/10);
    spp->getSppData();
    taskDelay (sysClkRateGet()/10);
    spp->getSppData();
    taskDelay (sysClkRateGet()/10);
    spp->getSppData();

    spp->secondAlign();
    spp->checkStatus();

  }
}
/*****************************************************************************/
 
void initInterrupts ()
 
// Initialize interrupts and handlers.
{
// Connect the Spp isr.
  if (intConnect ((VOIDFUNCPTR*)ISIO1_BIM4_ADR, (VOIDFUNCPTR)spp_isr, 0)) {
    perror ("intConnect spp_isr");
    exit(ERROR);
  }
  taskDelay(sysClkRateGet() / 10);
 
// Enable the interrupt handler on the mv162 board for level 1 interrupts.
  *VMECHIP2_LBIER = 0x01;
 
// Enable interrupts from the tans.
  spp->enableInterrupt (ISIO1_BIM4_VCT, 1);
}
/*****************************************************************************/

void spp_isr ()

// Interrupt service routine for the Ophir 3.
{
//  logMsg ("in spp_isr.\n", 0,0,0,0,0,0);
  spp->isr ();
}
/*****************************************************************************/

void statusMsg (char *msg)

{
  printf (msg);
}
/*****************************************************************************/
