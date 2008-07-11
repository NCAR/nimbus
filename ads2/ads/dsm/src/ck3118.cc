/* ck3118.cc
   Check the Vm3118.  Requires tfp to operating correctly.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

// Edit NUM_CHANs and FIRST_CHANs for desired rates and channels
#define NUM_25HZ_CHANS		16		// number of 25 hz channels
#define NUM_5HZ_CHANS		16		// number of 5 hz channels
#define FIRST_25HZ_CHAN		0		// first 25 hz channel to sample
#define FIRST_5HZ_CHAN		32		// first 5 hz channel to sample

#include "mv162.h" // mvme162 defines
#include <vxWorks.h>
#include <intLib.h>
#include <logLib.h>
#include <stdioLib.h>
#include <wdLib.h>

// Program include files.
#include <dsmVmeDefs.h>

// Class include files.
#include <AnaAuxTable.h>
#include <Bc635Vme.h>
#include <Vm3118.h>

// Program functions.
void initInterrupts ();				// initialize interrupts
void buildAnaAuxTable();			// build a test table
void hertz50_isr (int hertz5_flag);		// time-freq processor isr
void displayFirstSamples();			// display data
void tfpInit();					// init time-freq class

// Class declarations.
AnaAuxTable *table;				// analog aux sample table
Bc635Vme *tfp;					// time-freq processor
Vm3118 *vm3118; 

WDOG_ID wid;

int ck3118 ()
{
  int sec_cnt = 0;

// Initialize the AnaAuxTable class.
  table = new AnaAuxTable();
  if (table == NULL) 
    {
      perror ("Creating AnaAuxTable:");
      exit (ERROR);
    }

// Initialize the time-freq processor class.
  printf ("Initializing the time-frequency processor.\n");
  tfpInit();

// Initialize interrupts.
  initInterrupts ();

// Create a watchdog timer.
  wid = wdCreate();

// Build the analog table 
  buildAnaAuxTable();

// Create the Vm3118 interface class.
  vm3118 = new Vm3118 ((char*)(A24D16_BASE + VM3118_BASE), *table);
  if (vm3118 == NULL) {
    perror ("Creating Vm3118:");
    exit (ERROR);
  }

// Main loop.
  while (TRUE) {
    if (tfp->newSecond()) {
      tfp->clearNewSecond();
      if (!(sec_cnt++ % 2)) 
        displayFirstSamples();
    }
  }
}
/*****************************************************************************/

void initInterrupts ()

// Initialize interrupts and handlers.
{
// Connect the time-freq processor isr.
  if (intConnect ((VOIDFUNCPTR*)TFP_ADR,(VOIDFUNCPTR)hertz50_isr, TRUE)) {
    perror ("cktfp intConnect hertz50_isr");
    exit(ERROR);
  }

// Enable the interrupt handler on the mv162 board.
  *VMECHIP2_LBIER = 0x04;     

// Enable strobe interrupts from the tfp at 5 hertz.
  tfp->enableMinorStrobeInt (TFP_VCT, 3, 200);
}
/*****************************************************************************/

void buildAnaAuxTable ()

// Creates a test analog table
{
  int j;

  for (j = 0; j < NUM_25HZ_CHANS; j++) 
    table->enter (j + FIRST_25HZ_CHAN, RATE_25, 1, j*2, NUM_25HZ_CHANS * 2, 2);

  for (j = 0; j < NUM_5HZ_CHANS; j++) 
    table->enter (j + FIRST_5HZ_CHAN, RATE_5, 1, (NUM_25HZ_CHANS + j) * 2, 
                 NUM_5HZ_CHANS * 2, 2);

  table->displayTable();
}
/*****************************************************************************/

void hertz50_isr (int hertz5_flag)

// 50 hertz isr.  Starts conversions, and collects data from the Vm3118.
{
  static int hz50_cnt = RATE_50;                // 50 hertz interrupt counter
  static int ticks50 = sysClkRateGet() / RATE_50; // ticks per 50 hz interval

// Start the watch dog timer if the next 50 hertz interval does not coincide
// with a 5 hertz interval.
  if (((hz50_cnt + 1) % RATE_10))
    wdStart ((WDOG_ID)wid, ticks50, (FUNCPTR)hertz50_isr, FALSE);

  if (hertz5_flag) {
    tfp->strobeIsr ();

// Check for correct 5 hertz interrupt count.
    if (hz50_cnt % RATE_10)
      logMsg ("Incorrect interrupt counter at 5 hz interval, hz50_cnt = %d.\n",
              hz50_cnt, 0,0,0,0,0);
 
// Check for the start of a new second from the tfp or for a hertz50_cnt
// overflow. This happens if the timing interface gets a spike on its
// 1 hertz line.  This upsets the sync in the interface.
    if (tfp->newSecond() || (hz50_cnt >= RATE_50)) {
      vm3118->hertz1_isr();        // align the vm3118 class with the 1 hertz
      hz50_cnt = 0;
    }
  }

// Start conversions on the even interrupts, acquire the data on the odd.
  if (!(hz50_cnt % 2))
    vm3118->startConversion (hz50_cnt);
  else if (!vm3118->getAdcData())
    logMsg ("Failure acquiring data from the Vm3118.\n", 0,0,0,0,0,0);

  hz50_cnt++;			// increment interrupt counter
}
/*****************************************************************************/

void displayFirstSamples()

// Displays the first sample of each channel.
{
  int j;

  printf ("25 Hertz Channels\n");
  for (j = 0; j < NUM_25HZ_CHANS; j++) 
    printf ("%2.4  ", (*((short*)vm3118->buf25() + j))*305.176E-6);
  if (NUM_25HZ_CHANS % 8)
  putchar ('\n');

  printf ("5 Hertz Channels\n");
  for (j = 0; j < NUM_5HZ_CHANS; j++) 
    printf ("%2.4f  ", (*((short*)vm3118->buf5() + j))*305.176E-6);
  putchar ('\n');
}
/*****************************************************************************/
 
void tfpInit ()
 
// Initialize the time-frequency processor.
{
  tfp = new Bc635Vme ((char*)(A24D16_BASE + TFP_BASE));
  if (tfp == NULL)
    {
      perror ("Creating Bc635Vme:");
      exit (ERROR);
    }
 
  tfp->setPath();                                // set default path packet
  tfp->selectModulatedInput();                   // select modulated irig input
  tfp->selectModulatedOutput();                  // select modulated irig output
 
// Sync to the 1PPS if operating as the time master.
  tfp->select1PPSMode();
 
// Set the periodic output at 10 Khz, and a 50 usec pulse width.
  tfp->setPeriodicOutput (10000, 50);
}
/*****************************************************************************/
