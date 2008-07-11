/* ck2dio.cc 

   This program checks operation of the I/O channel based 2D interface.
   mvme 162 version

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
#include <mv162.h>

#include <vxWorks.h>
#include <stdio.h>
#include <sysLib.h>
#include <taskLib.h>
#include <wdLib.h>

#include <header.h>
#include <dsmctl.h>
#include <dsmVmeDefs.h>

#include <Pms2IO.h>

void statusMsg (char *msg);
void p2d_wdisr ();

Pms2IO pms2 ((char*)(A24D16_BASE + G2CMD), (char*)(A24D16_BASE + G2TIME),
             (char*)(A24D16_BASE + G2BUF), statusMsg);

WDOG_ID wid;
int ticks25; 				// ticks per 25 hz interval

ck2dio ()
{
  char *buf;
  P2d_rec *rec;

  ticks25 = sysClkRateGet() / RATE_25;

// Get the operating parameters. 

  printf("\n MVME110/PMS-2D interface test program\n\n");

  pms2.Pms2IO ((char*)(A24D16_BASE + G2CMD), (char*)(A24D16_BASE + G2TIME),
               (char*)(A24D16_BASE + G2BUF), statusMsg);

// Initialize the probes.
  pms2.initProbe (P2D_P_STR);
  pms2.initProbe (P2D_C_STR);
  pms2.setRate (10);

// Start up a watchdog timer at a 25 hertz interval. 
  wid = wdCreate();              
  wdStart ((WDOG_ID)wid, ticks25, (FUNCPTR)p2d_wdisr, FALSE);

  printf ("Starting the main loop.\n");

  while (1) {
    if (pms2.bufFull()) {
      buf = pms2.buffer();
      rec = (P2d_rec*)buf;
      pms2.releaseBuf();
      printf ("%c%c %02d:%02d:%02d.%03d, tas = %d, overld = %d\n",
               buf[0], buf[1], rec->hour, rec->minute, rec->sec, rec->msec,
               rec->tas * 125 / 255, rec->overld);
    }
    taskDelay (sysClkRateGet() / 5);
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
  static int tas = 30;
  
// Restart the timer.
  wdStart ((WDOG_ID)wid, ticks25, (FUNCPTR)p2d_wdisr, FALSE);

// Every 25 interupts set the new second flag.
  if (!(hz25_cnt = ++hz25_cnt % RATE_25)) {
    if (!(second = ++second % 60)) 
      if (!(minute = ++minute % 60)) 
        hour = ++hour % 24;
    pms2.setTime (hour, minute, second);
    pms2.setTas (tas);
    if ((tas = tas + 5) > 90)
      tas = 30;
  }

// Call the 2d data collection routine.
  pms2.collect (!hz25_cnt);
}
/*****************************************************************************/
