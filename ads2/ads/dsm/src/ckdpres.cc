/* ckdpres.cc
   Check the digital pressure interface.

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
#include <Dpres.h>
#include <Bc635Vme.h>

// Program functions.
void initInterrupts();
void dpres_isr();
void statusMsg (char *msg);
void tfp_isr ();
void tfpInit();

// Class declarations.
static Dpres *dpres;
static Bc635Vme *tfp;

int ckdpres ()
{
  int new_second;
// Initialize the Dpres class.
  printf("Initialize Dpres Class\n");
  dpres = new Dpres((char*)(A24D16_BASE + ISIO1_BASE), DPRES_PORT_1, statusMsg);

// Initialize the time-freq processor class.
  printf ("Initializing the time-frequency processor.\n");
  tfpInit();

// Initialize interrupts.
  initInterrupts();

printf("At main loop.\n");

  while (TRUE) {
    taskDelay (sysClkRateGet()/5);

    if (new_second = tfp->newSecond()) { 
      tfp->clearNewSecond();
      dpres->secondAlign ();
      dpres->checkStatus();
    }
  }
}
/*****************************************************************************/
 
void initInterrupts ()
 
// Initialize interrupts and handlers.
{
// Connect the time-freq processor isr.
  if (intConnect ((VOIDFUNCPTR*)TFP_ADR,(VOIDFUNCPTR)tfp_isr, 0)) {
    perror ("cktfp: intConnect tfp_isr");
    exit(ERROR);
  }

// Connect the DPRES isr.
  if (intConnect ((VOIDFUNCPTR*)ISIO1_BIM3_ADR, (VOIDFUNCPTR)dpres_isr, 0)) {
    perror ("intConnect dpres_isr");
    exit(ERROR);
  }

  taskDelay(sysClkRateGet() / 10);
 
// Enable the interrupt handler on the mv162 board.
  *VMECHIP2_LBIER = 0x05;
 
// Enable interrupts.
  dpres->enableInterrupt (ISIO1_BIM3_VCT, 1);

// Enable strobe interrupts from the tfp at 5 hertz.
  tfp->enableMinorStrobeInt (TFP_VCT, 3, 200);
}
/*****************************************************************************/

void dpres_isr ()

// Interrupt service routine.
{
// logMsg ("in dpres_isr.\n", 0,0,0,0,0,0);
  dpres->isr ();
}
/*****************************************************************************/
 
void tfp_isr ()
 
// Interrupt service routine for the time-freq processor.
{
  tfp->strobeIsr ();
}
/*****************************************************************************/
 
void tfpInit ()
 
// Initialize the time-frequency processor.
{
  tfp = new Bc635Vme ((char*)(A24D16_BASE + TFP_BASE));
 
  tfp->setPath();                       // set default path packet
  tfp->selectModulatedInput();          // select modulated irig input
  tfp->selectModulatedOutput();         // select modulated irig output
 
// Sync to the 1PPS if operating as the time master.
  tfp->select1PPSMode();
 
  taskDelay(sysClkRateGet() * 2);

// Set the periodic output at 10 Khz, and a 50 usec pulse width.
  tfp->setPeriodicOutput (10000, 50);

}
/*****************************************************************************/

void statusMsg (char *msg)
{
  printf (msg);
}
