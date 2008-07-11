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
#include <sysLib.h>
#include <taskLib.h>

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
static void tfpInit();			// init time-freq processor
void initInterrupts();			// initialize interrurpts
void hertz50_isr(int);			// 25 hertz sampling isr
void hwIrsIsr(int chan);		// irs isr wrapper

// Class declarations.
static Bc635Vme *tfp;                           // time-freq processor
static IP429 *ip429;                            // arinc 429 interface

static HwIrs *hw_irs0;		                // Honeywell irs class
static HwIrs *hw_irs1;		                // Honeywell irs class

// Global variables.
extern BOOT_PARAMS sysBootParams;

int ckirs ()
{
  Irs_blk *irs_blk;			// 1 second data pointer
  float tas = 80.0;
  float alt = 5000.0;

  sysClkRateSet(25);    // 25Hz.
  sysAuxClkRateSet(50); // 50Hz.
  sysAuxClkConnect((FUNCPTR)hertz50_isr, FALSE);

// Initialize the time generator class.
  tfpInit ();
  
  printf("Init IP429 class\n");  
// Initialize the IP429 class.
  ip429 = new IP429 ((char*)IP_a_IO_BASE, IP429_VCT);

  printf("Init HwIrs class\n");
// Initialize the Honeywell irs class.
  hw_irs0 = new HwIrs (ip429, *tfp, 0, 4);
  hw_irs1 = new HwIrs (ip429, *tfp, 1, 0);

  ip429->enableInterrupt (0);
  taskDelay(500);

  printf("Init interrups\n");
// Initialize interrupts.
  initInterrupts ();

  printf("Enable 429 interrups\n");
// Enable the 429 interrupts.
//  ip429->enableInterrupt (1);
//  ip429->enableInterrupt (0);
//  ip429->enableInterrupt (4);		// generates a first write interrupt
//  ip429->disableInterrupt (0);


  printf("Enter Main Loop\n");
// Main loop.
  while (TRUE) {

// Check for a new second.
    if (tfp->newSecond()) {
      tfp->clearNewSecond ();
//     ip429->disableInterrupt (0);
//      printf("Disable Interrupt\n");

//      printf("Get Irs data\n");
      irs_blk = (Irs_blk*)hw_irs0->buffer();	// get the irs buffer
//      displayIrs (irs_blk);
      irs_blk = (Irs_blk*)hw_irs1->buffer();	// get the irs buffer
//      displayIrs (irs_blk);

      hw_irs0->setTasAlt (tas, alt);
    }
//    taskDelay(100);
//    ip429->enableInterrupt (0);
//    printf("Enable Interrupt\n");
//    taskDelay(20);
  }

}
/*****************************************************************************/

void displayIrs (Irs_blk *irs_blk)
{
  printf ("lag50 = %d, lag25 = %d, lag10 = %d, lag5 = %d\n",
          irs_blk->lag_50hz_frame, irs_blk->lag_25hz_frame, 
          irs_blk->lag_10hz_frame, irs_blk->lag_5hz_frame);

  printf ("lat = %7.3f, lon = %7.3f\n", (irs_blk->present_lat[0] >> 11) * PIRAD,
          (irs_blk->present_lon[0] >> 11) * PIRAD);
}
/*****************************************************************************/

static void tfpInit ()

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

  if (intConnect((VOIDFUNCPTR*)TFP_ADR, (VOIDFUNCPTR)hertz50_isr, TRUE)){
    perror("intConnect tfp_isr");
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
//                        | IPIC_INT_CTL_EDGE;
  *IPIC_IP_a_INT1_CTL = IPIC_INT_CTL_IEN | IPIC_INT_CTL_LEVEL_1; 
//                        | IPIC_INT_CTL_EDGE;

// Enable strobe interrupts from the tfp at 5 hertz and level 3.
  tfp->enableMinorStrobeInt (TFP_VCT, 3, 200);
 
// Enable the interrupt handler on the mv162 board.
  *VMECHIP2_LBIER = 0x04;

}

/*****************************************************************************/
 
static int hertz5_cnt = RATE_5;		// 5 hertz interrupt counter
static int hertz50_cnt = RATE_50;	// 50 hertz interrupt counter

void hertz50_isr(int hertz5_flag)
{
  int j;

// Check if this is a 5 hertz interrupt from the tfp.
  if (hertz5_flag) {
    sysAuxClkEnable();
    tfp->strobeIsr();				// run the tfp isr

// Check for correct 5 hertz interrupt count.
    if (hertz5_cnt != RATE_10) {           
      logMsg ("Incorrect interrupt counter at 5 hz interval, = %d.\n",
              hertz50_cnt, 0,0,0,0,0);
    }
    hertz5_cnt = 0;                       // reset 5 hertz intr counter 

// Check for the start of a new second.
    if (tfp->newSecond()) {
      hw_irs0->secondAlign ();		// align the irs data
      hw_irs1->secondAlign ();		// align the irs data
      hertz50_cnt = 0;
    }
  }
  else
    if (((hertz50_cnt + 1) % RATE_10) == 0)
      sysAuxClkDisable();


// Increment the 50 hertz counter.
  hertz50_cnt++;
}
/*****************************************************************************/
 
void hwIrsIsr (int chan)
 
// Honeywell irs isr wrapper.
{
//  logMsg("ISR1 \n",0,0,0,0,0,0);
//  *IPIC_IP_a_INT1_CTL |= IPIC_INT_CTL_ICLR;
  switch (chan)
    {
    case 0:
      hw_irs0->readIsr();
      break;

    case 1:
      hw_irs1->readIsr();
      break;

    default:
      hw_irs0->sendTasAlt();
    logMsg ("write\n", 0,0,0,0,0,0);
    }
}
/*****************************************************************************/
