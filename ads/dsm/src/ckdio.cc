/* ckmio.cc 

   This program checks operation of the OR vmio12 interface.
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
#include <Vmio14In.h>
#include <Vmio14Out.h>

static Vmio12 *vmio12;
static Vmio14In *din0;
static Vmio14In *din8;
static Vmio14Out *dout;

int ckdio ()
{
  int j;

// Init the classes.
  vmio12 = new Vmio12 ((char*)(A24D16_BASE + VMIO12_BASE_0));
  din0 = new Vmio14In (vmio12, 0);
  dout = new Vmio14Out (vmio12, 2);
  din8 = new Vmio14In (vmio12, 8);

  taskDelay(sysClkRateGet() / 3);

  while (TRUE) {
//    dout->writePort (din0->readPort());
    dout->writePort (0x01);
    printf("chan 0 = 0x%X, chan 8 = 0x%X.\n", din0->readPort(), din8->readPort());
    taskDelay(sysClkRateGet());
  }
}
