/* cktans3.cc
   Check the Trimble TANS receiver.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include "mv162.h"         // mvme162 defines
#include <vxWorks.h>
#include <intLib.h>
#include <logLib.h>
#include <stdioLib.h>
#include <sysLib.h>
#include <taskLib.h>

// Program include files.
#include <dsmctl.h>
#include <dsmVmeDefs.h>
#include <header.h>

// Class include files.
#include <GpsTans3.h>

// Program functions.
void initInterrupts ();				// initialize interrupts
void statusMsg (char *msg);			// status message handler
void setTime (int year, int month, int day, int hour, int minute, int second);
void tans_isr ();				// time-freq processor isr

// Class declarations.
static GpsTans3 *tans3; 

int cktans3 ()
{
  Gps_blk *gps_blk;

// Initialize the GpsTans3 class.
  tans3 = new GpsTans3 ((char*)(A24D16_BASE + ISIO1_BASE), TANS_PORT_1, 
                statusMsg, setTime);

// Initialize interrupts.
  initInterrupts ();

  while (TRUE) {
    taskDelay (sysClkRateGet());
    tans3->checkStatus ();
    tans3->setAltitude (2000.0);

    gps_blk = (Gps_blk*)tans3->buffer();

    printf ("week = %d, time = %f, utc_offset = %f\n",
            gps_blk->gpsweek, gps_blk->gpstime, gps_blk->gpsutc);

    printf ("ghealth = %d, gpsmode = %d, addstat = %d\n",
             gps_blk->ghealth, gps_blk->gpsmode, gps_blk->addstat);

    printf ("glat = %f, glon = %f, galt = %f\n",
            gps_blk->glat, gps_blk->glon, gps_blk->galt);
  }
}
/*****************************************************************************/

void initInterrupts ()

// Initialize interrupts and handlers.
{
// Connect the Tans GPS isr.
  if (intConnect ((VOIDFUNCPTR*)ISIO1_BIM3_ADR,(VOIDFUNCPTR)tans_isr, 0)) {
    perror ("cktfp: intConnect tans_isr");
    exit(ERROR);
  }
  taskDelay(20);

// Enable the interrupt handler on the mv162 board for level 1 interrupts.
  *VMECHIP2_LBIER = 0x01;     

// Enable interrupts from the tans. 
  tans3->enableInterrupt (ISIO1_BIM3_VCT, 1);
}
/*****************************************************************************/
 
void statusMsg (char *msg)
 
// Handles status messages from the gps.
{
  printf (msg);
}
/*****************************************************************************/

void setTime (int year, int month, int day, int hour, int minute, int second)

// Handles time setting from the gps.
{
  logMsg ("setTime: %02d/%02d/%02d %02d:%02d:%02d\n",
          year, month, day, hour, minute, second); 
}
/*****************************************************************************/

void tans_isr ()

// Interrupt service routine for the Trimble TANS receiver.
{
  tans3->isr ();
//logMsg ("in tans_isr.\n", 0,0,0,0,0,0);
}
/*****************************************************************************/

