/* ck2d.cc 

   This program checks operation of the VME 2D interface.
   mvme 162 version

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

#include <header.h>
#include <dsmctl.h>
#include <dsmVmeDefs.h>
#include <Bc635Vme.h>
#include <Pms2dHouse.h>
#include <PmsVme2d.h>

void statusMsg (char *msg);
void p2d_wdisr ();
void tfp_isr ();
void tfpInit();
void initInterrupts();
void init_isr ();
void isr_wrap ();

static PmsVme2d *pms2;

WDOG_ID wid;
int ticks50; 				// ticks per 50 hz interval
int ticks25; 				// ticks per 25 hz interval
int ticks1; 				// ticks per 1 hz interval
int hour;
int minute;
int second;
int interrupt_enabled;          // interrupt enabled flag

//Class declarations.

static Pms2dHouse *pms2dhouse;
static Bc635Vme *tfp;
 
int ck2d ()
{
  char *buf;
  P2d_rec *rec;
  int tas = 160;
  int hour = 8;
  int minute = 30;
  int second = 59;
  int new_second;
  interrupt_enabled = FALSE;

// Init the classes.

// Initialize the time-freq processor class.
  printf ("Initializing the time-frequency processor.\n");
  tfpInit();
 
  pms2dhouse = new Pms2dHouse((char*)(A24D16_BASE + PMSVME2D_BASE_0));

  ticks50 = sysClkRateGet() / RATE_50;
  ticks25 = sysClkRateGet() / RATE_25;
  ticks1 = sysClkRateGet();

// Get the operating parameters. 

  printf("\n MVME162/PMS-2D interface test program\n\n");

  printf("In check program\n");
  pms2 = new PmsVme2d ((unsigned char*)(A24D16_BASE + PMSVME2D_BASE_0), statusMsg);
// Initialize the probes.

  pms2->initProbe (0,"C1",1,25);
//  pms2->initProbe (1,"P1",2,100);

  pms2->setRate (10);
  pms2->setTas (tas);
  pms2->startSampling();

// Start up a watchdog timer at a 25 hertz interval. 
  wid = wdCreate();              
  wdStart ((WDOG_ID)wid, ticks1, (FUNCPTR)p2d_wdisr, FALSE);

// Uncomment this line to run with interrupts.
//  init_isr();
  pms2->clearInterrupt(); 
  printf ("Starting the main loop.\n");
  printf ("2D channel 0, Res = 25.\n");

  while (1) {
//    printf("Checking for VME buffer full\n");

  pms2->initProbe (0,"C1",1,50);
//  pms2->setRate (10);
//  pms2->setTas (tas);
//  pms2->startSampling();
    if (pms2->bufFull()) {
      printf("Found VME buffer full\n");
      buf = pms2->buffer();
      rec = (P2d_rec*)buf;
      pms2->releaseBuf();
      printf ("%c%c %02d:%02d:%02d.%03d, tas = %d, overld = %d\n",
               buf[0], buf[1], rec->hour, rec->minute, rec->second, rec->msec,
               rec->tas * 125 / 255, rec->overld);
    }
    if (new_second = tfp->newSecond()) {
      tfp->clearNewSecond();
      pms2dhouse->secondAlign ();
    }
    taskDelay (sysClkRateGet());
  }
}
/*****************************************************************************/
 
void statusMsg (char *msg)
 
// Class wrapper for sending status messages.
{
  fprintf (stderr, msg);
}
/*****************************************************************************/

void p2d_wdisr()
{
  static int hz25_cnt = 0;
  static int hour = 0;
  static int minute = 0;
  static int second = 0;
  int chan;
  chan = 0;
// Restart the timer.
  wdStart ((WDOG_ID)wid, ticks1, (FUNCPTR)p2d_wdisr, FALSE);
// Every 25 interupts set the new second flag.
//  if (!(hz25_cnt = ++hz25_cnt % RATE_25)) {

  if (!(second = ++second % 60)) {
    if (!(minute = ++minute % 60)) 
      hour = ++hour % 24;
    pms2->setTime (hour, minute, second);
  }
// Call the 2d data collection routine.
  pms2->collect (!hz25_cnt);
  pms2dhouse->getData(chan);
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
 
void tfp_isr ()
 
// Interrupt service routine for the time-freq processor.
{
  tfp->strobeIsr ();
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
 
void init_isr ()
 
// Sets up the isr routine.
{
  int stat;
 
// Connect the isr for both probes.
  if (stat=intConnect ((VOIDFUNCPTR*)PMS2D_ADR0, (VOIDFUNCPTR)isr_wrap, 0)) {
    perror ("intConnect: probe 0");
    exit(1);
  }
 
// Enable the vmebus interrupt handler on the mv162 board for level 1.
  *VMECHIP2_LBIER = 0x01;
 
// Enable the interface BIM for a level 1 interrupt. 
    taskDelay (sysClkRateGet());
  pms2->enableInterrupt (PMS2D_VCT0, 1);
    taskDelay (sysClkRateGet());
  interrupt_enabled = TRUE;
}
/*****************************************************************************/
 
void isr_wrap ()
 
// C++ isr wrapper.
{
  pms2->clearInterrupt();
  logMsg ("Probe interrupt.\n",0,0,0,0,0,0);
}

