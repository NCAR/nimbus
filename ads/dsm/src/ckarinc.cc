/* ckarinc.cc
   Test program to simulate HwGps data.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

// System include files.
#include <vxWorks.h>
#include <mv162.h>
#include <bootLib.h>
#include <intLib.h>
#include <logLib.h>
#include <stdio.h>
#include <taskLib.h>
#include <wdLib.h>

// Program include files.
#include <header.h>
#include <dsmctl.h>
#include <dsmVmeDefs.h>
#include <ipicDefs.h>

// Class include files.
#include <Bc635Vme.h>
#include <DsmConfig.h>
#include <IP429.h>
#include <ArincGps.h>
#include <HwGps.h>

#define PIRAD   (1.0 / ((0x01 << 20) / 180.0))

// Program functions. 
void tfpInit();				// init time-freq processor
void initInterrupts();			// initialize interrurpts
void hertz50_isr();                      // 50 hertz sampling isr
static void arincgpsIsr (int chan);	// gps isr wrapper

// Class declarations.
static Bc635Vme *tfp;                           // time-freq processor
static IP429 *ip429;                            // arinc 429 interface
static ArincGps *arincgps;                      // ArincGps data class

// Global variables.
extern BOOT_PARAMS sysBootParams;
WDOG_ID wid;
int ticks50;                            // ticks per 50 hz interval

int ckarinc ()
{

//  ticks50 = sysClkRateGet() / RATE_50;
  ticks50 = sysClkRateGet() * 10 ;

//  HwGps_blk *gps_blk;			// 1 second data pointer
   printf("Begin ckarinc \n");
// Initialize the time generator class.
  tfpInit ();

// Initialize interrupts.
  initInterrupts ();

   printf("Init IP429 class \n");
// Initialize the IP429 class.
  ip429 = new IP429 ((char*)IP_a_IO_BASE, IP429_VCT);

   printf("Init the gps class \n"); 
// Initialize the Honeywell gps class.
//  arincgps = new ArincGps (ip429, *tfp, 0, 5);
  arincgps = new ArincGps (ip429, *tfp, 5);
 
// Enable the 429 interrupts.
  ip429->enableInterrupt (0);
  printf("enable int(0) \n");
//  ip429->enableInterrupt (5);		// generates a first write interrupt

// Start up a watchdog timer at a 50 hertz interval.
  wid = wdCreate();
  wdStart ((WDOG_ID)wid, ticks50, (FUNCPTR)hertz50_isr, FALSE);
  ticks50 = sysClkRateGet() * 10;

// Main loop.
  while (TRUE) {
// Check for a new second.
//   printf("mainloop \n");
    if (tfp->newSecond()) {
//      printf("buildarinc \n");
//      arincgps->buildArincGps();
//      ip429->enableInterrupt (5);	// generates a first write interrupt
      tfp->clearNewSecond ();
    }
    taskDelay(3);
  }
}
/*****************************************************************************/

void tfpInit ()

// Initialize the time-frequency processor.
{
  tfp = new Bc635Vme ((char*)(A24D16_BASE + TFP_BASE));

// Get the time from the rtc.
  tfp->selectRealTimeClockMode ();
  taskDelay(sysClkRateGet() * 2);	// give tfp time for mode change

// Sync to the 1PPS. 
  tfp->select1PPSMode();

  taskDelay(sysClkRateGet() * 2);	// give tfp time for mode change

// Set the periodic output at 10 Khz, and a 50 usec pulse width.
  tfp->setPeriodicOutput (10000, 50);
}
/*****************************************************************************/
 
void initInterrupts ()
 
// Initialize interrupts and handlers.
{
// Connect the time-freq processor isr.
  if (intConnect ((VOIDFUNCPTR*)TFP_ADR,(VOIDFUNCPTR)hertz50_isr, TRUE)){
    perror ("intConnect tfp_isr");
    exit(ERROR);
  }

// Connect the IP429 isr.  The IP429 uses a base vector with the lower
// three bits of the vector specifying the interrupting channel.
    if (intConnect ((VOIDFUNCPTR*)(IP429_ADR),(VOIDFUNCPTR)arincgpsIsr, 0)) {
      perror ("intConnect arincIsr");
      exit(ERROR);
    }
  if (intConnect ((VOIDFUNCPTR*)(IP429_ADR+0x04),(VOIDFUNCPTR)arincgpsIsr, 1)){
    perror ("intConnect arincIsr");
    exit(ERROR);
  }
  if (intConnect ((VOIDFUNCPTR*)(IP429_ADR+0x08),(VOIDFUNCPTR)arincgpsIsr, 2)){
    perror ("intConnect arincIsr");
    exit(ERROR);
  }
  if (intConnect ((VOIDFUNCPTR*)(IP429_ADR+0x0C),(VOIDFUNCPTR)arincgpsIsr, 3)){
    perror ("intConnect arincIsr");
    exit(ERROR);
  }
  if (intConnect((VOIDFUNCPTR*)(IP429_ADR+0x10),(VOIDFUNCPTR)arincgpsIsr, 4)){
    perror ("intConnect arincIsr");
    exit(ERROR);
  }
  if (intConnect((VOIDFUNCPTR*)(IP429_ADR+0x14),(VOIDFUNCPTR)arincgpsIsr, 5)){
    perror ("intConnect arincIsr");
    exit(ERROR);
  }
 // Enable the transmit and receive interrupts in the IPIC chip at level 2 for
 // receive, level 1 for transmit.
  *IPIC_IP_a_INT0_CTL = IPIC_INT_CTL_IEN | IPIC_INT_CTL_LEVEL_2;
  *IPIC_IP_a_INT1_CTL = IPIC_INT_CTL_IEN | IPIC_INT_CTL_LEVEL_1;

// Enable strobe interrupts from the tfp at 5 hertz and level 3.
  tfp->enableMinorStrobeInt (TFP_VCT, 3, 200);
 
// Enable the interrupt handler on the mv162 board.
  *VMECHIP2_LBIER = 0x04;
}
/*****************************************************************************/
 
 void hertz50_isr ()
 
// 50 hertz isr.
 {
  int j;
//  static int hertz50_cnt = RATE_50;		// 5 hertz interrupt counter
//  tfp->strobeIsr ();				// run the tfp isr
    wdStart ((WDOG_ID)wid, ticks50, (FUNCPTR)hertz50_isr, FALSE);

//  hertz50_cnt += 1;
  logMsg("ISR\n",0,0,0,0,0,0);
// Check for 1 hertz interrupt count.
//  if (hertz50_cnt  >= RATE_50) {           

// Build the ArincGps arinc block.
    arincgps->buildArincGps();
    ip429->enableInterrupt (5);            // generates a first write interr
//    hertz50_cnt = 0;                       // reset 5 hertz intr counter 
//  }
}

/*****************************************************************************/
 
static void arincgpsIsr (int intf)
 
// ArincGps  data isr wrapper.
{
  short chan;
  ip429->disableInterrupt (5);        
  arincgps->sendArincGps();
  chan = ip429->servicerqst();
  while(!chan) {
    chan = ip429->servicerqst();
  } 
  arincgps->sendArincGps();
}
