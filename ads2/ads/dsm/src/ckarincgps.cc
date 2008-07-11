/* ckarincgps.cc
   Test program to read arinc data from the Honeywell Gps.

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
#include <HwGps.h>

#define PIRAD   (1.0 / ((0x01 << 20) / 180.0))

// Program functions. 
void displayGps (HwGps_blk *hgps_blk);	// display the block
void tfpInit();				// init time-freq processor
void initInterrupts();			// initialize interrurpts
void hertz5_isr();			// 5 hertz sampling isr
void hwGpsIsr (int chan);		// irs isr wrapper

// Class declarations.
static Bc635Vme *tfp;                   // time-freq processor
static IP429 *ip429;                    // arinc 429 interface
static HwGps *hw_gps;  		        // Honeywell gps class

// Global variables.
extern BOOT_PARAMS sysBootParams;

int ckarincgps ()
{
  HwGps_blk *hgps_blk;			// 1 second data pointer
  unsigned int temp;
// Initialize the time generator class.
  tfpInit ();
// Initialize interrupts.
  initInterrupts ();

// Initialize the IP429 class.
  ip429 = new IP429 ((char*)IP_a_IO_BASE, IP429_VCT);

// Initialize the Honeywell gps class.
  hw_gps = new HwGps (ip429,0);

// Enable the 429 interrupts.
  ip429->enableInterrupt (0);
//  printf("0 \n");
//  ip429->enableInterrupt (3);		// generates a first write interrupt

// Main loop.
  while (TRUE) {

// Check for a new second.
    if (tfp->newSecond()) {
      hw_gps->secAlign ();
      temp = ip429->servrqst();
      tfp->clearNewSecond ();
      hgps_blk = (HwGps_blk*)hw_gps->buffer();	// get the gps buffer
      displayGps (hgps_blk);
    }
    taskDelay(3);
  }
}
/*****************************************************************************/

void displayGps (HwGps_blk *hgps_blk)
{
/*  printf ("lag50 = %d, lag25 = %d, lag10 = %d, lag5 = %d\n",
          irs_blk->lag_50hz_frame, irs_blk->lag_25hz_frame, 
          irs_blk->lag_10hz_frame, irs_blk->lag_5hz_frame);
  printf ("lat = %7.3f, lon = %7.3f\n", (hw_irs.latitude() >> 11) * PIRAD,
          (hw_irs.longitude() >> 11) * PIRAD);
*/
  printf(" ms=0x%x, time=0x%x, alt=0x%x, hdop=0x%x, vdop=0x%x, lat=ox%x, lon=0x%x, gspd=0x%x\n", 
	hgps_blk->meas_status,
	hgps_blk->utc_time,
	hgps_blk->altitude,
	hgps_blk->hdop,
	hgps_blk->vdop,
	hgps_blk->latitude,
	hgps_blk->longitude,
	hgps_blk->ground_speed);
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
  if (intConnect ((VOIDFUNCPTR*)TFP_ADR,(VOIDFUNCPTR)hertz5_isr, TRUE)){
    perror ("intConnect tfp_isr");
    exit(ERROR);
  }

// Connect the IP429 isr.  The IP429 uses a base vector with the lower
// three bits of the vector specifying the interrupting channel.
  if (intConnect ((VOIDFUNCPTR*)IP429_ADR,(VOIDFUNCPTR)hwGpsIsr, 0)) {
    perror ("intConnect hwGpsIsr");
    exit(ERROR);
  }
  if (intConnect ((VOIDFUNCPTR*)(IP429_ADR+0x04),(VOIDFUNCPTR)hwGpsIsr, 1)){
    perror ("intConnect hwGpsIsr");
    exit(ERROR);
  }
  if (intConnect ((VOIDFUNCPTR*)(IP429_ADR+0x08),(VOIDFUNCPTR)hwGpsIsr, 2)){
    perror ("intConnect hwGpsIsr");
    exit(ERROR);
  }
  if (intConnect ((VOIDFUNCPTR*)(IP429_ADR+0x0C),(VOIDFUNCPTR)hwGpsIsr, 3)){
    perror ("intConnect hwGpsIsr");
    exit(ERROR);
  }
  if (intConnect((VOIDFUNCPTR*)(IP429_ADR+0x10),(VOIDFUNCPTR)hwGpsIsr, 4)){
    perror ("intConnect hwGpsIsr");
    exit(ERROR);
  }
  if (intConnect((VOIDFUNCPTR*)(IP429_ADR+0x14),(VOIDFUNCPTR)hwGpsIsr, 5)){
    perror ("intConnect hwGpsIsr");
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
 
void hertz5_isr ()
 
// 5 hertz isr.
{
  int j;
  static int hertz5_cnt = RATE_5;               // 5 hertz interrupt counter
//  tfp->strobeIsr ();                           // run the tfp isr
 
  hertz5_cnt += 1;
 
// Check for 1 hertz interrupt count.
  if (hertz5_cnt  >= RATE_5) {
 
// Build the ArincGps arinc block.
//    arincgps->buildArincGps();
 
    hertz5_cnt = 0;                       // reset 5 hertz intr counter
  }
}
/*****************************************************************************/
 
void hwGpsIsr (int chan)
 
// Honeywell gps isr wrapper.
{
  if (chan == 0){
    hw_gps->readgpsIsr ();
  }
  else {
    logMsg ("chan error\n", 0,0,0,0,0,0);
  }
}
