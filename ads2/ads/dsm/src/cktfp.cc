/* cktfp.cc
   Check the time and frequency processor.

   Original Author: Jerry V. Pelk
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
#include <dsmVmeDefs.h>

// Class include files.
#include <Bc635Vme.h>

// Program functions.
void tfpInit(), initInterrupts();
void tfp_isr();

// Class declarations.
static Bc635Vme *tfp;                            // time-freq processor


/* -------------------------------------------------------------------- */
int cktfp ()
{
  int	pt, tt;
  int	this_year, this_month, this_day, this_hour, this_minute, this_second;
  int	last_year, last_month, last_day, last_hour, last_minute, last_second;
  FILE	*fp;

  sysClkRateSet(25);    // 25Hz.

  tfpInit();
  initInterrupts();

  tfp->clearNewSecond();

  taskDelay(sysClkRateGet());


  if ((fp = fopen("DOSC:tfp->log", "a")) == NULL)
    printf("Can't open log file.\n");

  printf("\n\n-------- System reset %02d/%02d/%02d %02d:%02d:%02d -------\n\n",
	tfp->month(), tfp->day(), tfp->year(),
	tfp->hour(), tfp->minute(), tfp->second()); 

  fprintf(fp, "\n\n----- System reset %02d/%02d/%02d %02d:%02d:%02d -----\n\n",
	tfp->month(), tfp->day(), tfp->year(),
	tfp->hour(), tfp->minute(), tfp->second()); 

  fclose(fp);


  while (1)
    {
    this_year	= tfp->year();
    this_month	= tfp->month();
    this_day	= tfp->day();
    this_hour	= tfp->hour();
    this_minute	= tfp->minute();
    this_second	= tfp->second();

    if (tfp->newSecond())
      {
      tfp->clearNewSecond();
 
/*      printf("dec %02d/%02d/%02d %02d:%02d:%02d.%03d\n",
        this_month, this_day, this_year,
        this_hour, this_minute, this_second, tfp->msec());
*/
      // Check for dropped time.  This happens if the clocks are too far off.
      tt = this_hour * 3600 + this_minute * 60 + this_second;
 
      if (tt != pt + 1)
        {
        fp = fopen("DOSC:tfp->log", "a");

        fprintf(fp, ">> bad time %02d/%02d/%02d %02d:%02d:%02d\n",
                this_month, this_day, this_year,
                this_hour, this_minute, this_second);
 
        fprintf(fp, "  prev time %02d/%02d/%02d %02d:%02d:%02d\n",
                last_month, last_day, last_year,
                last_hour, last_minute, last_second);
 
        fclose(fp);

        printf("last_sec = %d, this_sec = %d\n", pt, tt);
        }
 
      pt = tt;
      }

    last_year	= this_year;
    last_month	= this_month;
    last_day	= this_day;
    last_hour	= this_hour;
    last_minute	= this_minute;
    last_second	= this_second;

    taskDelay(sysClkRateGet() / 5);
    }

}

/* -------------------------------------------------------------------- */
void tfpInit()
{
// If mode == 0, operate as the master, else a slave.
  int	mode = 1;

// Initialize the time-freq processor class.
  tfp = new Bc635Vme((char*)(A24D16_BASE + TFP_BASE));

  tfp->disableJamSync();
  tfp->setPath();				// send default path packet
  tfp->selectModulatedInput();			// select modulated irig input
  tfp->selectModulatedOutput();			// select modulated irig input

  // Sync to the 1PPS if operating as the master.
  if (mode == 0) 
    tfp->select1PPSMode();
  // Sync to irig time code if operating as a slave.
  else 
    tfp->selectTimeCodeMode();

  // Set the periodic output at 10 Khz, and a 50 usec pulse width.
  tfp->setPeriodicOutput(10000, 50);

  tfp->setMajorTime(98,12,7,12,0,0);

}

/* -------------------------------------------------------------------- */
void initInterrupts()
{
  if (intConnect ((VOIDFUNCPTR*)TFP_ADR,(VOIDFUNCPTR)tfp_isr, 0))
    {
    perror ("cktfp: intConnect tfp_isr");
    exit(ERROR);
    }

  // Enable the interrupt handler on the mv162 board.
  *VMECHIP2_LBIER = 0x04;     

  // Enable strobe interrupts from the tfp at 5 hertz.
  tfp->enableMinorStrobeInt(TFP_VCT, 3, 200);
}

/* -------------------------------------------------------------------- */
void tfp_isr()
{
  tfp->strobeIsr();

}
