/* ckstod.cc
   Send the serial time-of-day.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

                                                           
#include <mv162.h>
                                                                                
#include <vxWorks.h>
#include <stdio.h>
#include <sysLib.h>
#include <taskLib.h>
#include <wdLib.h>
#include <intLib.h>
#include <logLib.h>
#include <stdioLib.h>
                                                                                
#include <header.h>

// Program include files.
#include <dsmctl.h>
#include <dsmVmeDefs.h>

// Class include files.
#include <SerialTod.h>
#include <Bc635Vme.h>

// Program functions.
void initInterrupts();
void statusMsg (char *msg);
void tfp_isr ();
void tfpInit();

// Class declarations.
static SerialTod *serialtod;
static Bc635Vme *tfp;

int ckstod ()
{
  int new_second;
// Initialize the SerialTod class.
  printf("Initialize SerialTod Class\n");
  serialtod = new SerialTod((char*)(A24D16_BASE + ISIO1_BASE), 
              SERTOD_PORT, statusMsg, tfp);

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
      serialtod->sendtod ();
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

// Enable the interrupt handler on the mv162 board.
  *VMECHIP2_LBIER = 0x05;
 
// Enable strobe interrupts from the tfp at 5 hertz.
  tfp->enableMinorStrobeInt (TFP_VCT, 3, 200);
}
/*****************************************************************************/
 
void tfp_isr ()
 
// Interrupt service routine for the time-freq processor.
{
//  logMsg("tfp strobeIsr\n",0,0,0,0,0,0);
  tfp->strobeIsr ();
}
/*****************************************************************************/
 
void tfpInit ()
 
// Initialize the time-frequency processor.
{
  tfp = new Bc635Vme ((char*)(A24D16_BASE + TFP_BASE));

  tfp->disableJamSync(); 
  tfp->setPath();                       // set default path packet
  tfp->selectModulatedInput();          // select modulated irig input
  tfp->selectModulatedOutput();         // select modulated irig output
 
// Sync to the 1PPS if operating as the time master.
  tfp->select1PPSMode();
 
  taskDelay(sysClkRateGet() * 2);
  tfp->selectTimeCodeMode ();

  taskDelay(sysClkRateGet() * 2);       // give tfp time for mode change

// Set the periodic output at 10 Khz, and a 50 usec pulse width.
  tfp->setPeriodicOutput (10000, 50);
  tfp->setMajorTime(98,12,7,12,0,0);
  tfp->masterSyncUp();
}
/*****************************************************************************/

void statusMsg (char *msg)
{
  printf (msg);
}
