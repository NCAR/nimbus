/* ckcnt.cc 

   This program checks operation of the counter interface.
   mvme 162 version

   Original Author: Chris Webster
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
#include <mv162.h>

#include <vxWorks.h>
#include <intLib.h>
#include <stdioLib.h>
#include <taskLib.h>

#include <dsmVmeDefs.h>
#include <dsmctl.h>
#include <ipicDefs.h>

#include <IPcounter.h>
#include <Bc635Vme.h>

static IPcounter *IPcnt;
static Bc635Vme *tfp;                       // time-freq processor

void tfp_isr(), initInterrupts(), tfpInit();

/* -------------------------------------------------------------------- */
int ckcnt()
{
  int time;

// Initialize the time-freq processor class.
  printf ("Initializing the time-frequency processor.\n");
  tfpInit();
 
// Initialize interrupts.
  initInterrupts ();

// Init the classes.
  IPcnt = new IPcounter((char *)IP_c_IO_BASE);

  IPcnt->readProm((char *)IP_c_ID_BASE);

// Test counter card.
  time = -1;
  while (TRUE) {
    time++;
    IPcnt->readCounters(time);
// printf("%i-%i \n",j,IPcnt->counter(4));
    taskDelay (sysClkRateGet()/25);

    if (tfp->newSecond()) {
      tfp->clearNewSecond();
      time = -1;
    }
  }
}

/* -------------------------------------------------------------------- */
void tfp_isr ()
{
  tfp->strobeIsr ();
}

/* -------------------------------------------------------------------- */
void initInterrupts ()
{
// Connect the time-freq processor isr.
  if (intConnect ((VOIDFUNCPTR*)TFP_ADR,(VOIDFUNCPTR)tfp_isr, TRUE)) {
    perror ("cktfp: intConnect tfp_isr");
    exit(ERROR);
  }
 
// Enable the interrupt handler on the mv162 board.
  *VMECHIP2_LBIER = 0x04;
 
// Enable strobe interrupts from the tfp at 5 hertz.
  tfp->enableMinorStrobeInt (TFP_VCT, 3, 200);
}

/* -------------------------------------------------------------------- */
void tfpInit ()
 
// Initialize the time-frequency processor.
{
  tfp = new Bc635Vme ((char*)(A24D16_BASE + TFP_BASE));
 
//  tfp->setPath();                            // set default path packet
//  tfp->selectModulatedInput();               // select modulated irig input
//  tfp->selectModulatedOutput();              // select modulated irig output
 
  tfp->selectRealTimeClockMode ();
  taskDelay(sysClkRateGet() * 2);

// Sync to the 1PPS if operating as the time master.
  tfp->select1PPSMode();
 
  taskDelay(sysClkRateGet() * 2);

// Set the periodic output at 10 Khz, and a 50 usec pulse width.
  tfp->setPeriodicOutput (10000, 50);
}
/* END CKCNT.CC */
