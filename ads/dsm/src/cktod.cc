/* cktod.cc
   Check the time of day clock.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include "mv162.h"         // mvme162 defines
#include <vxWorks.h>
#include <stdioLib.h>
#include <taskLib.h>

// Program include files.

// Class include files.
#include <TodClock.h>

// Program functions.

// Class declarations.
static TodClock *tod;

int cktod ()
{
// Initialize the TodClock class.
  tod = new TodClock ();

  printf ("Setting the time to 10/1/97 21:25:00\n");
  tod->setTime (97, 10, 1, 21, 25, 00);

  while (TRUE) {
    taskDelay (sysClkRateGet());
    tod->readTime ();
    printf ("   %02d/%02d/%02d  %02d:%02d:%02d, day = %2d\n", 
            tod->year(), tod->month(), tod->date(), 
            tod->hour(), tod->minute(), tod->second(), tod->day());
  }
}

