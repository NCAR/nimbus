/* ckctr.cc 

   This program checks operation of the OR vmio12 and vmio14 counter interface.
   mvme 162 version

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
#include <mv162.h>

#include <vxWorks.h>
#include <logLib.h>
#include <stdioLib.h>
#include <taskLib.h>

#include <dsmVmeDefs.h>

#include <Vmio12.h>
#include <Vmio14Count.h>

static Vmio12 *vmio12;
static Vmio14Count *count;

int ckctr ()
{
  int j = 0;
  long cnt1, cnt2;

// Init the classes.
  vmio12 = new Vmio12 ((char*)(A24D16_BASE + VMIO12_BASE_0));
  count = new Vmio14Count (vmio12, 1);
  
  while (TRUE) {
    for (j = 0, cnt1 = 0, cnt2 = 0; j < 25; j++) {
      cnt1 += count->count1();
      cnt2 += count->count2();
      taskDelay (sysClkRateGet()/25);
    }
    printf ("count1 = %d, count2 = %d\n", cnt1, cnt2);
  }
}
/*****************************************************************************/
