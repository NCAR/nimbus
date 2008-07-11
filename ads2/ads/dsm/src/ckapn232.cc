/* ckapn232.cc 

   This program checks operation of the APN232 radar altimeter interface.
   mvme 162 version

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
#include <mv162.h>

#include <vxWorks.h>
#include <stdioLib.h>
#include <taskLib.h>

#include <dsmVmeDefs.h>

#include <APN232.h>
#include <Vmio12.h>

Vmio12 *vmio12;
APN232 *apn232;

int ckapn232 ()
{
  int j;

// Init the classes.
  vmio12 = new Vmio12 ((char*)(A24D16_BASE + VMIO12_BASE_0));

  apn232 = new APN232 (vmio12, 3);

  while (TRUE) {
    taskDelay(sysClkRateGet() / 10);
    printf("0x%X.\n", apn232->readPort());
  }
}
/*****************************************************************************/
