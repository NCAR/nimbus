/* ckirs.cc
   Test program to read arinc data from the Honeywell Irs.

   Original Author: Jerry V. Pelk
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
#include <HwIrs.h>

#define PIRAD   (1.0 / ((0x01 << 20) / 180.0))

// Program functions. 
void displayIrs (Irs_blk *irs_blk);	// display the block
void tfpInit();				// init time-freq processor
void initInterrupts();			// initialize interrurpts
void hertz25_isr(int);			// 25 hertz sampling isr
void hwIrsIsr (int chan);		// irs isr wrapper

// Class declarations.
Bc635Vme tfp ((char*)(A24D16_BASE + TFP_BASE)); // time-freq processor
IP429 ip429 ((char*)IP_a_IO_BASE, IP429_VCT);   // arinc 429 interface
HwIrs hw_irs (&ip429, tfp, 1, 4);		// Honeywell irs class


// Global variables.
extern BOOT_PARAMS sysBootParams;
WDOG_ID wid;

ckirs ()
{
  Irs_blk *irs_blk;			// 1 second data pointer
  float tas = 80.0;
  float alt = 5000.0;

// Initialize the time generator class.
  tfpInit ();

// Initialize interrupts.
  initInterrupts ();

// Initialize the IP429 class.
  ip429.IP429 ((char*)IP_a_IO_BASE, IP429_VCT);

// Initialize the Honeywell irs class.
  hw_irs.HwIrs (&ip429, tfp, 1, 4);

// Enable the 429 interrupts.
  ip429.enableInterrupt (1);
//  ip429.enableInterrupt (4);		// generates a first write interrupt

// Create a watchdog timer.
  wid = wdCreate();

// Main loop.
  while (TRUE) {

// Check for a new second.
    if (tfp.newSecond()) {
      tfp.clearNewSecond ();
      irs_blk = (Irs_blk*)hw_irs.buffer();	// get the irs buffer
      displayIrs (irs_blk);
      hw_irs.setTasAlt (tas, alt);
    }
    taskDelay(3);
  }
}
/*****************************************************************************/

void displayIrs (Irs_blk *irs_blk)
{
  printf ("lag50 = %d, lag25 = %d, lag10 = %d, lag5 = %d\n",
          irs_blk->lag_50hz_frame, irs_blk->lag_25hz_frame, 
          irs_blk->lag_10hz_frame, irs_blk->lag_5hz_frame);
  printf ("lat = %7.3f, lon = %7.3f\n", (hw_irs.latitude() >> 11) * PIRAD,
          (hw_irs.longitude() >> 11) * PIRAD);
}
/*****************************************************************************/

void tfpInit ()

// Initialize the time-frequency processor.
{
  tfp.Bc635Vme ((char*)(A24D16_BASE + TFP_BASE));

// Get the time from the rtc.
  tfp.selectRealTimeClockMode ();
  taskDelay(180);                        // give tfp time for mode change

// Sync to the 1PPS. 
  tfp.select1PPSMode();

  taskDelay(180);                       // give tfp time for mode change

// Set the periodic output at 10 Khz, and a 50 usec pulse width.
  tfp.setPeriodicOutput (10000, 50);
}
/*****************************************************************************/
 
void initInterrupts ()
 
// Initialize interrupts and handlers.
{
// Connect the time-freq processor isr.
  if (intConnect ((VOIDFUNCPTR*)TFP_ADR,(VOIDFUNCPTR)hertz25_isr, TRUE)){
    perror ("intConnect tfp_isr");
    exit(ERROR);
  }

// Connect the IP429 isr.  The IP429 uses a base vector with the lower
// three bits of the vector specifying the interrupting channel.
  if (intConnect ((VOIDFUNCPTR*)IP429_ADR,(VOIDFUNCPTR)hwIrsIsr, 0)) {
    perror ("intConnect hwIrsIsr");
    exit(ERROR);
  }
  if (intConnect ((VOIDFUNCPTR*)(IP429_ADR+0x04),(VOIDFUNCPTR)hwIrsIsr, 1)){
    perror ("intConnect hwIrsIsr");
    exit(ERROR);
  }
  if (intConnect ((VOIDFUNCPTR*)(IP429_ADR+0x08),(VOIDFUNCPTR)hwIrsIsr, 2)){
    perror ("intConnect hwIrsIsr");
    exit(ERROR);
  }
  if (intConnect ((VOIDFUNCPTR*)(IP429_ADR+0x0C),(VOIDFUNCPTR)hwIrsIsr, 3)){
    perror ("intConnect hwIrsIsr");
    exit(ERROR);
  }
  if (intConnect((VOIDFUNCPTR*)(IP429_ADR+0x10),(VOIDFUNCPTR)hwIrsIsr, 4)){
    perror ("intConnect hwIrsIsr");
    exit(ERROR);
  }
  if (intConnect((VOIDFUNCPTR*)(IP429_ADR+0x14),(VOIDFUNCPTR)hwIrsIsr, 5)){
    perror ("intConnect hwIrsIsr");
    exit(ERROR);
  }
 
// Enable the transmit and receive interrupts in the IPIC chip at level 2 for
// receive, level 1 for transmit.
  *IPIC_IP_a_INT0_CTL = IPIC_INT_CTL_IEN | IPIC_INT_CTL_LEVEL_2;
  *IPIC_IP_a_INT1_CTL = IPIC_INT_CTL_IEN | IPIC_INT_CTL_LEVEL_1;

// Enable strobe interrupts from the tfp at 5 hertz and level 3.
  tfp.enableMinorStrobeInt (TFP_VCT, 3, 200);
 
// Enable the interrupt handler on the mv162 board.
  *VMECHIP2_LBIER = 0x04;
}
/*****************************************************************************/
 
void hertz25_isr (int hertz5_flag)
 
// 25 hertz isr.
{
  int j;
  static int hertz5_cnt = RATE_5;		// 5 hertz interrupt counter
  static int hertz25_cnt = RATE_25;		// 25 hertz interrupt counter
  static int ticks25 = sysClkRateGet() / RATE_25; // ticks per 25 hz interval

// Check if this is a 5 hertz interrupt from the tfp.
  if (hertz5_flag) {
    tfp.strobeIsr ();				// run the tfp isr

// Check for correct 5 hertz interrupt count.
    if (hertz5_cnt != RATE_5) {           
      logMsg ("Incorrect interrupt counter at 5 hz interval, = %d.\n",
              hertz5_cnt, 0,0,0,0,0);
    }
    hertz5_cnt = 0;                       // reset 5 hertz intr counter 

// Check for the start of a new second.
    if (tfp.newSecond()) {
      hw_irs.secondAlign ();		// align the irs data
      hertz25_cnt = 0;
    }
  }

// Start the watch dog timer if the next 25 hertz interval does not coincide 
// with a 5 hertz interval.
  if (++hertz5_cnt < RATE_5)
    wdStart ((WDOG_ID)wid, ticks25, (FUNCPTR)hertz25_isr, FALSE);

// Increment the 25 hertz counter.
  hertz25_cnt++;
}
/*****************************************************************************/
 
void hwIrsIsr (int chan)
 
// Honeywell irs isr wrapper.
{
//  logMsg("ISR1 \n",0,0,0,0,0,0);
  if (chan == 0)
    hw_irs.readIsr ();
  else {
//    hw_irs.sendTasAlt();
  logMsg ("chan = \n", chan,0,0,0,0,0);
  }
}
/*****************************************************************************/
